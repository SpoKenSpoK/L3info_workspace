#!/bin/bash

inputfile=$1
MACE4="mace4 -f"
# choisir parmi : dot, neato, twopi, circo, fdp, sfdp, patchwork 
# meilleurs resuktats :  circo,  sfdp 
DOT=circo


$MACE4 $inputfile | interpformat xml | ./viewmodels.py | $DOT -Tpdf > $inputfile.pdf
open  $inputfile.pdf
