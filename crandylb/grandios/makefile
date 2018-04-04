# makefile -- for grandios directory, CRB, Mar 24, 2013
# 03/24/2013 CRB Added -fsigned-char for ARM

grandios.o:	grandios.c grandios.h
	gcc -c -m32 -DWITHMAIN -fsigned-char grandios.c -o grandios.o

fiotinc.o:	fiotinc.c grandios.h
	gcc -c -m32 fiotinc.c -o fiotinc.o

testfiotinc:	fiotinc.o grandios.o
	gcc -m32 grandios.o fiotinc.o -o testfiotinc

test:		testfiotinc
	echo "Press Ctrl-D to complete test (sends EOF to stdin)"
	./testfiotinc iotd.txt - fiot.ch3 fiot.ch4
# Press Ctrl-D to complete test (sends EOF to stdin)
