#include <stdio.h>
#define MAXLIST 12000

	int list[MAXLIST];

progr()
/* THIS ROUTINE MAKES SEVERAL BASIC TESTS ON THE DISTRIBUTED I/O
   PACKAGE.  ALTHOUGH IT IS NOT EXHAUSTIVE BY ANY MEANS, IT DOES SEEM
   TO POINT UP THE MOST COMMON FAULTS.  YOU SHOULD CONSULT THE
   IMPLEMENTATION GUIDE FOR AN EXPLANATION OF THE OUTPUT TO BE EXPECTED
   FROM THE TEST, AND FOR THE MEANING OF VARIOUS ERRORS.

   THE ASSUMED CHANNEL CONFIGURATION FOR THIS PROGRAM IS-

          CHANNEL 1 - DATA INPUT (USE THE CARDS SERIALIZED IOTD)
          CHANNEL 3 - MACHINE READABLE OUTPUT
          CHANNEL 4 - PRINTER OUTPUT
*/
{
	int i, j, k;
/*
          READ A TITLE LINE AND WRITE IT ON BOTH OUTPUT CHANNELS
*/
	i = MAXLIST;
	if (ioop(-1,1,list,1,&i) != 0) fprintf(stderr,"STOP 11\n");
	if (ioop(1,3,list,1,&i) != 0) fprintf(stderr,"STOP 31\n");
/*
          THE TITLE SPECIFIES CHANNEL 3.  BEFORE WRITING IT TO THE
          PRINTER, CHANGE THE 3 TO A 4.  (REMEMBER THAT THE DIGIT
          CHARACTERS MAP INTO SUCCESSIVE INTEGERS.)
*/
	list[46]=list[46]+1;
	if (ioop(1,4,list,1,&i) != 0) fprintf(stderr,"STOP 41\n");
/*
         FOLLOW THE TITLE WITH A BLANK LINE ON CHANNEL 4
*/
	j = 1;
	if (ioop(1,4,list,1,&j) != 0) fprintf(stderr,"STOP 42\n");
/*
          READ AND PRINT THE FIRST LINE OF THE TEST.
*/
	i = MAXLIST;
	if (ioop(-1,1,list,1,&i) != 0) fprintf(stderr,"STOP 12\n");
	if (ioop(1,4,list,1,&i) != 0) fprintf(stderr,"STOP 43\n");
/*
          BUMP THE LINE NUMBER AND PRINT ONLY IT.
*/
	list[1]=list[1]+1; j = 3;
	if (ioop(1,4,list,1,&j) != 0) fprintf(stderr,"STOP 44\n");
/*
          READ TWO LINES INTO DIFFERENT STORE AREAS, TO SEE IF THEY
          INTERACT IN ANY WAY.
*/
	i = MAXLIST;
	if (ioop(-1,1,list,1,&i) != 0) fprintf(stderr,"STOP 13\n");
	j = MAXLIST;
	if (ioop(-1,1,list,i,&j) != 0) fprintf(stderr,"STOP 14\n");
/*
          PRINT FROM THE FIRST AREA, WHICH SHOULD BE THE FIRST LINE.
*/
	if (ioop(1,4,list,1,&i) != 0) fprintf(stderr,"STOP 45\n");
/*
          PRINT FROM THE SECOND AREA USING A BASE OFFSET INSTEAD OF AN
          INDEX OFFSET.  THIS MAY BOTHER SOME SYSTEMS.  IT IS USED IN
          SIMCMP, SO IT MUST WORK PROPERLY.  CONSULT THE IMPLEMENTATION
          GUIDE IF IT DOES NOT.
*/
	k = j-i+1;
	if (ioop(1,4,list+i-1,1,&k) != 0) fprintf(stderr,"STOP 46\n");
/*
          CHECK TO ENSURE THAT AN ENDFILE IS CORRECTLY RECOGNIZED.
*/
	if (ioop(-1,1,list,i,&j) != 1) fprintf(stderr,"STOP 15\n");
}
