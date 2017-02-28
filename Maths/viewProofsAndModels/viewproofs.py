#!/usr/bin/python

import sys
sys.path.append('lib/')
from os import system,uname 
from optparse import OptionParser
from which import programsToUse

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
                      help="Use this to only produce a dot file")

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

    paths=programsToUse(['gvizify','prooftrans','dot',viewer])
    v  = {'File' : file,
          'Myself':myselfexe,
          'Viewer' : paths[viewer]
    }
    for key in paths:
        v[key] = paths[key]
    if not (file.endswith(".xml")):
        preprocess = 'cat %(File)s | %(prooftrans)s xml' % v
    else:
        preprocess = 'cat %(File)s' % v
    #    
    v['Preprocess'] =preprocess
    command = '%(Preprocess)s | %(gvizify)s --rotate=0 | %(dot)s -Tpdf > %(File)s.pdf ; %(Viewer)s %(File)s.pdf' % v
    #print command
    system(command)
    exit(0)

def mainNoViewer():
    print "Option --noviewer not yet implemented"
    exit(0)
    
def main():
    global args, options
    process_command_line()
    if (options.viewer == True):
        mainViewer()
        exit(0)
    else:
        mainNoViewer()
        exit(0)


if __name__ == "__main__":
    main()
