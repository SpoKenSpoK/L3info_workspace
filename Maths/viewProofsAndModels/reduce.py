#!/usr/bin/python

from xml.dom.minidom import *
import sys
from optparse import OptionParser

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

symbols=["d","s","p"]

def process_command_line():
    global options, args, datasource,symbols  # This function sets these.
    parser = OptionParser(
        usage = "usage: %prog [XML filename]",
        version="%prog 1.0, 2014-01-30",
        description=
        """This program by Luigi Santocanale, translates a Mace4 model (in xml format, obtained via the
command interpformat xml, into graphviz format.
It deals only with unary and binary relational symbols.
"""        )
    parser.add_option("--language", 
                    dest="language", default="", metavar="ANGLE",
                    help="list of symbols")

    optionparams = sys.argv[1:]
    (options, args) = parser.parse_args(args=optionparams)
    if len(args) == 0:
        datasource=sys.stdin
    elif len(args) == 1:
        datasource=args[0]
    else:
        print "Sorry, must provide at most 1 argument (the XML's filename)"
        exit(1)
    
    symbols=(options.language).split()


def generate_results():
  "Parse XML from datasource, keep only the symbols in the list symbols, print it as xml"

  global symbols  
  # Process XML file:
  doc = parse(datasource)  # [0] is the program name.
  # interplist = doc.getElementsByTagName("interp")
  # if (interplist.length == 0):
  #  print "Error! Must be at least 1 interpretation item in the XML file"
  #  exit(1)
  
  for n in range(0,3):
      oplist=doc.getElementsByTagName("op"+str(n))+doc.getElementsByTagName("opn")
      for op in oplist:
          symbol=extract_textlist(op.getElementsByTagName("sym"))[0]
          if not symbol in symbols:
              parent = op.parentNode
              parent.removeChild(op)
                  
  print  doc.toxml()

def main():
  process_command_line()
  generate_results()

if __name__ == "__main__":
    main()
