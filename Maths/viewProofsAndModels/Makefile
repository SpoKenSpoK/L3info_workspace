THISDIR= $(notdir $(shell pwd))

all: zip install


zip: clean
	cd ../ ; zip -r ${THISDIR}.zip ${THISDIR}/*
	mv  ../${THISDIR}.zip ./

install:
	cp ${THISDIR}.zip ../html/TP/${THISDIR}.zip

clean:
	rm *.dot *.pdf *.zip
	cd examples; rm *.xml *.dot *.pdf 

all: zip install

test1:
	./viewmodels.py examples/ordres.models

test2:
	./viewproofs.py examples/proof.prf

