#!/usr/bin/python

from xml.dom.minidom import *
import sys
from os import system,uname 
from optparse import OptionParser

sys.path.append('lib/')
from which import programsToUse

def compute_node_labels(size,op1list):
    labels = [""]*size
    for op1 in op1list:
        type=op1.getAttribute("type")
        if(type == "relation"):
            symbol=extract_textlist(op1.getElementsByTagName("sym"))[0]
            list=extract_textlist(op1.getElementsByTagName("v"))
            nodeno=0
            for value in list:
                if value == "1":
                    if labels[nodeno] == "":
                        labels[nodeno] += symbol
                    else:
                        labels[nodeno] += ","+symbol
                nodeno+=1
    
    return labels

def compute_constants(size,op0list,node_labels):
    for op0 in op0list:
        type=op0.getAttribute("type")
        if(type == "function"):
            symbol=extract_textlist(op0.getElementsByTagName("sym"))[0]
            list=extract_textlist(op0.getElementsByTagName("v"))
            nodeno=int(list[0])
            if node_labels[nodeno] == "":
                node_labels[nodeno] += symbol
            else:
                node_labels[nodeno] += ","+symbol
    
    return node_labels

def compute_arrow_labels(size,op2list):
    labels = [""]*(size*size)
    for op2 in op2list:
        type=op2.getAttribute("type")
        if(type == "relation"):
            symbol=extract_textlist(op2.getElementsByTagName("sym"))[0]
            list=extract_textlist(op2.getElementsByTagName("v"))
            arrowno=0
            for value in list:
                if value == "1":
                    if labels[arrowno] == "":
                        labels[arrowno] += symbol
                    else:
                        labels[arrowno] +=(","+symbol)
                arrowno+=1

    return labels

def compute_functions(size,op1list):
    labels = [""]*(size*size)
    for op1 in op1list:
        type=op1.getAttribute("type")
        if(type == "function"):
            symbol=extract_textlist(op1.getElementsByTagName("sym"))[0]
            list=extract_textlist(op1.getElementsByTagName("v"))
            for source in range(0,size):
                target=int(list[source]) 
                if labels[(source * size) + target] == "":
                    labels[(source * size) + target] += symbol
                else:
                    labels[(source * size) + target] +=(","+symbol)

    return labels

def draw_nodes(number,size,labels):
    for  x in range(0,size):
        if labels[x] == "":
            sep=""
        else:
            sep=" : "
        print "\t\"%d_%d\" [label=\"%d%s%s\"];" %(number,x,x,sep,labels[x])

def draw_arrows(number,size,labels):
    for  source in range(0,size):
        for  target in range(0,size):
            label=labels[(source*size) + target]
            if not (label=="") :
                print "\t\"%d_%d\" -> \"%d_%d\" [label=\"%s\"];" %(number,source,number,target,label)

def draw_functions(number,size,labels):
    for source in range(0,size):
        for  target in range(0,size):
            label=labels[(source*size) + target]
            if not (label=="") :
                print "\t\"%d_%d\" -> \"%d_%d\" [label=\"%s\" style=\"dotted\"];" %(number,source,number,target,label)

def extract_textlist(elements):
  "Given list of elements e, return as list the text enclosed by each."
  resultfragments = []
  for element in elements:
    textfragment = []
    for child in element.childNodes:
      if child.nodeType in (child.TEXT_NODE, child.CDATA_SECTION_NODE):
        textfragment.append(child.data)
    resultfragments.append( "".join(textfragment) )
  return resultfragments

def process_command_line():
    global options, args, myselfexe, datasource  # This function sets these.
    parser = OptionParser(
        usage = "usage: %prog [XML filename]",
        version="%prog 1.0, 2014-01-30",
        description=
        """This program by Luigi Santocanale, translates a Mace4 model (in xml format, obtained via the
command interpformat xml, into graphviz format.
It deals only with unary and binary relational symbols.
"""        )
    parser.add_option("-v","--viewer",
                      action="store_true", dest="viewer", default=True, 
                      help="Use this to open directly the pdf in a viewer (evince on Linux, or Preview on MacOS")
    parser.add_option("-n","--noviewer",
                      action="store_false", dest="viewer", default=True, 
                      help="Use this to only produce the dot file")

    myselfexe=sys.argv[0]
    optionparams = sys.argv[1:]
    (options, args) = parser.parse_args(args=optionparams)
    if len(args) == 0:
        datasource=sys.stdin
    elif len(args) == 1:
        datasource=args[0]
    else:
        print "Sorry, must provide at most 1 argument (the XML's filename)"
        exit(1)


def generate_results():
  "Parse XML from datasource, and write to stdout the graphviz results"
  
  # Process XML file:
  doc = parse(datasource)  # [0] is the program name.
  interplist = doc.getElementsByTagName("interp")
  if (interplist.length == 0):
    print "Error! Must be at least 1 interpretation item in the XML file"
    exit(1)
  
  # Generate header:
  print "digraph {"

  for interp in interplist:
      size=int(interp.getAttribute("size"))
      number=int(interp.getAttribute("number"))

      op1list=interp.getElementsByTagName("op1")
      node_labels=compute_node_labels(size,op1list)    
      op0list=interp.getElementsByTagName("op0")
      node_labels=compute_constants(size,op0list,node_labels)    

      function_labels=compute_functions(size,op1list)    
      op2list=interp.getElementsByTagName("op2")
      arrow_labels=compute_arrow_labels(size,op2list)
      
      print "\tsubgraph cluster_%d {" %(number)
      print "\t\tlabel=\"Model %d\"" %(number)
      draw_nodes(number,size,node_labels)
      draw_arrows(number,size,arrow_labels)
      draw_functions(number,size,function_labels)
      print "\t}\n"
  
  print "}"

def mainViewer():
    global args, options
    if len(args) != 1 :
        print "The name of a file is needed\n"
        exit(-1)
    file=args[0]
    if uname()[0] == 'Darwin':
        viewer='open'
    else:
        viewer='evince'
    paths=programsToUse(['interpformat','dot',viewer])
    v  = {'File' : file,
          'Myself':myselfexe,
          'Viewer' : paths[viewer]
    }
    for key in paths:
        v[key] = paths[key]
    if not (file.endswith(".xml")):
        preprocess = 'cat %(File)s | %(interpformat)s xml' % v
    else:
        preprocess = 'cat %(File)s' % v
    v['Preprocess'] =preprocess
    command = '%(Preprocess)s | %(Myself)s --noviewer | %(dot)s -Tpdf > %(File)s.pdf ; %(Viewer)s %(File)s.pdf' % v
    #print command
    system(command)
    exit(0)

def main():
    global args, options
    process_command_line()
    if (options.viewer == True):
        mainViewer()
        exit(0)
    else:
        generate_results()

#if shutil.which('interpformat') != 'None' :
#command = 'interpformat xml < %(File)s | %(Myself)s --batch | dot -Tpdf > %(File)s.pdf ; %(Viewer)s %(File)s.pdf' % w
# else :

if __name__ == "__main__":
    main()
