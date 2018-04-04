CFLAGS	= -g

TIDY	= core a.out *.o list code diffs scratch code.c \
	fiot iotest simcmp simtest fltest[12] st2test
CLEAN	= $(TIDY) stage2

all:	st2test

tidy:
	rm -rf $(TIDY)

clean:
	rm -rf $(CLEAN)

iotest:		fiot iotd iotlist iotcode
	./fiot < iotd 1> code 2> list
	diff list iotlist > diffs
	if [ -s diffs ]; then exit 1; fi
	diff code iotcode > diffs
	if [ -s diffs ]; then exit 1; fi
	touch iotest

simtest:	iotest simcmp simt simtcode
	./simcmp < simt 1> code 2> list
	if [ -s list ]; then exit 1; fi
	diff code simtcode > diffs
	if [ -s diffs ]; then exit 1; fi
	touch simtest

fltest1:	simtest simcmp flbm iwch.o flt1 fld1
	cat flbm flt1 | ./simcmp > code.c
	cc $(CFLAGS) -DFLT1 code.c iwch.o f4io.o
	./a.out < fld1 2> list
	diff list flt1list > diffs
	if [ -s diffs ]; then exit 1; fi
	touch fltest1

fltest2:	fltest1 simcmp flbm iwch.o flt2 fld2
	cat flbm flt2 | ./simcmp > code.c
	cc $(CFLAGS) -DFLT2 code.c iwch.o f4io.o
	./a.out < fld2 1> code 2> list
	diff list flt2list > diffs
	if [ -s diffs ]; then exit 1; fi
	diff code flt2code > diffs
	if [ -s diffs ]; then exit 1; fi
	touch fltest2

st2test:	fltest2 stage2 st2t
	./stage2 < st2t 1> code 2> list
	diff list st2list > diffs
	if [ -s diffs ]; then exit 1; fi
	diff code st2code > diffs
	if [ -s diffs ]; then exit 1; fi
	touch st2test

fiot:	f4io.h f4io.o fiot.o
	cc $(CFLAGS) -o fiot fiot.o f4io.o

simcmp:	simc.o f4io.h f4io.o
	cc $(CFLAGS) -o simcmp simc.o f4io.o

stage2:	simcmp flbm stg2 iwch.o f4io.h f4io.o
	cat flbm stg2 | ./simcmp > code.c
	cc $(CFLAGS) -DSTAGE2 -o stage2 code.c iwch.o f4io.o 

f4io.o:	f4io.h f4io.c
fiot.o:	fiot.c
simc.o:	simc.c
iwch.o: iwch.c
