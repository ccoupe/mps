/* grandios.c -- Grand I/O in C for Stage2, CRB, Mar 24, 2013 */
// 12/13/1994 CRB Approximate date of original coding.
// 04/24/2013 CRB Now using -fsigned-char to force EOL as -1 to work.
//

/**********************************************************************\
    grandios -- Grand Stage2 I/O in C, converted from FORTRAN F4IO
\**********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "grandios.h"

static int jread(int jch, char * jba);
static int jwrit(int jch, char * jba);
static int jopen(int jch);
static int jcntl(int jch);
static int getArgs(int argc, char * argv[]);
static int openChan(int jch, char * pFile);

#ifdef WITHMAIN
extern void progr(void);	/* the application program */
#endif

struct CHAN chan[MAXCHAN+1] =
{
/*    r  w  o  c  s  fp    pFile       chan use     stream */
    { 0, 0, 0, 0, 0, NULL, NULL },   /* 0  nullchan        */
    { 1, 0, 0, 1, 1, NULL, NULL },   /* 1  input    stdin  */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 2  scratch         */
    { 0, 1, 0, 2, 1, NULL, NULL },   /* 3  output   stdout */
    { 0, 3, 0, 0, 1, NULL, NULL },   /* 4  error    stderr */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 5  scratch  */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 6  scratch  */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 7  scratch  */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 8  scratch  */
    { 1, 1, 1, 2, 1, NULL, NULL },   /* 9  scratch  */
};

int LBR = 0, LBW = 0;		/* line buffer read and write indices */
char LINE[MAXLEN+2];		/* the line buffer */
char MB[] = "********* **** ERROR\n";	/* the error message buffer */

/**********************************************************************\
    ioop -- Input / Output Operation Processor
\**********************************************************************/
int ioop(int jop, int jch, char *jba)
{
    int iret;			/* return value */

    if ( jch == 0 ) goto i5;	/* null channel */
    if ( (jch < 0) || (jch > MAXCHAN) )
	goto i7;		/* invalid channel number */

    if ( chan[jch].s == 0 )
	goto i7;		/* channel number not available */

    if ( jop < 0 ) goto i10;	/* read operation */
    if ( jop > 0 ) goto i20;	/* write operation */

/**********************************************************************/
		/* Process Control Operations */
    if ( chan[jch].s == 1 )
	goto i7;		/* illegal on closed channel */
    if ( chan[jch].c == 0 )
	goto i3;
    return jcntl(jch);		/* return result from jcntl */

 i3:				/* close the channel */
    chan[jch].s = 1;

 i4:				/* common return for success */
    return 0;

 i5:				/* ignore null channel write or close */
    if ( jop >= 0 ) goto i4;

 i6:				/* common return for end of file */
    return 1;

 i7:				/* illegal operation return */
    return 2;

/**********************************************************************/
 i10:		/* Process Read Request */

    switch ( chan[jch].s )	/* process the status */
    {
    case 1:			/* channel is closed */
	if ( chan[jch].r == 0 )	/* if no reading allowed */
	    goto i7;		/* take illegal operation exit */
	if ( chan[jch].w )	// if writing also enabled
	    chan[jch].s = 4;	// set open for both
	else			// otheriwse
	    chan[jch].s = 2;	/* set channel open for reading */
	if ( chan[jch].o )	/* if any open action required */
	{
	    if ( iret = jopen(jch) )
		return iret;	/* return code if open fails */
	}
	// deliberately fall through after successful open
    case 2:			/* channel open for reading */
    case 4:			/* channel open for read and write */
	iret = jread(jch, jba);
	goto i30;

    case 3:			/* channel open for writing only */
	goto i7;		/* so reading is illegal */

    case 5:			/* channel is at end of file */
	goto i6;		/* so return EOF */
    }

/**********************************************************************/
 i20:		/* Process Write Request */

    switch ( chan[jch].s )	/* process the status */
    {
    case 1:			/* channel is closed */
	if ( chan[jch].w == 0 ) /* if no writing allowed */
	    goto i7;		/* take illegal operation exit */
	if ( chan[jch].r )	// if reading also enabled
	    chan[jch].s = 4;	// set open for both
	else			// otheriwse
	    chan[jch].s = 3;	/* set channel open for writing */
	if ( chan[jch].o )	/* if any open action required */
        {
	    if ( iret = jopen(jch) )
		return iret;	/* return code if open fails */
	}
	// deliberately fall through after successful open
    case 3:			/* channel open for writing */
    case 4:			/* channel open for read and write */
	iret = jwrit(jch, jba);
	goto i30;

    case 5:			/* channel is at end of file */
	goto i6;		/* so return EOF */

    case 2:			/* channel open for reading only */
	goto i7;		/* so reading is illegal */
    }

/**********************************************************************/
 i30:		/* End of File or End of Medium */
    if ( iret == 1 )
    {
	chan[jch].s = 5;	/* if end of file mark status */
	goto i6;		/* and return EOF */
    }
    return iret;		/* return read result */
}

/**********************************************************************\
    jread -- read one line, return 0 if okay, 1 if EOF or error
\**********************************************************************/
static int jread(int jch, char *jba)
{
    int iret;
    char *jbinit = jba;

    iret = (fgets(jbinit, MAXLEN+1, chan[jch].fp) == NULL);
    while ( *jba != '\n' && jba < jbinit + MAXLEN + 1 )
	jba++;			/* scan for end of string */
    *jba = -1;			/* set end of line with -1 */

    // if chan 1 file reaches EOF revert to stdin
    if ( jch == 1 && iret && chan[1].pFile && feof(chan[1].fp) )
    {
        chan[1].fp = stdin;
	chan[1].pFile = NULL;
	iret = (fgets(jbinit, MAXLEN+1, chan[jch].fp) == NULL);
	jba = jbinit;
	while ( *jba != '\n' && jba < jbinit + MAXLEN + 1 )
	    jba++;		/* scan for end of string */
	*jba = -1;		/* set end of line with -1 */
    }

    return iret;		/* return 0 if okay, 1 if EOF or error */
}

/**********************************************************************\
    jwrit -- write one line, return 0 if okay, 1 if EOF or error
\**********************************************************************/
static int jwrit(int jch, char *jba)
{
    int iret;
    char * p = jba;

    while ( *p != -1 )		// scan for EOL
        p++;
    if ( p - jba >= MAXLEN )	// truncate line if overrun
        p = jba + MAXLEN;
    *p++ = '\n';		// terminate line with newline
    *p = '\0';

    iret = (fputs(jba, chan[jch].fp) == EOF);	// write the line
    *(p-1) = -1;		// put the end-of-line back for flt2
    return iret;
}

/**********************************************************************\
    jopen -- reset the channel to beginning of file, always return 0
\**********************************************************************/
static int jopen(int jch)
{
    rewind(chan[jch].fp);
    return 0;
}

/**********************************************************************\
    jcntl -- perform a control operation, close or rewind the file
\**********************************************************************/
static int jcntl(int jch)
{
    if ( chan[jch].s == 2 || chan[jch].s == 3 )
    {			// if open for read or write close the file
        chan[jch].s = 1;
        LBR = LBW = 0;
        fclose(chan[jch].fp);
    } else if ( chan[jch].s == 4 )
    {                   // if open for both rewind, don't close
        LBR = LBW = 0;
	rewind(chan[jch].fp);
    }
    return 0;
}

/**********************************************************************\
    iwrch -- write a character to the LINE buffer
\**********************************************************************/
int iwrch(char ch)
{
    if ( ch < 0 || LBW >= MAXLEN )
    {
	LINE[LBW++] = '\n';	/* mark end of line with new line */
	LINE[LBW] = '\0';
	LBW = 0;		/* reset write index */
	return 1;		/* return end of line status */
    }
    LINE[LBW++] = ch;		/* stuff character in LINE */
    return 0;			/* return okay status */
}

/**********************************************************************\
    iprnt -- print an error message from the MB buffer
\**********************************************************************/
int iprnt(int jch)
{
    return ioop(1, jch, MB);	/* always use MB */
}

/**********************************************************************\
    iread -- read a line into the LINE buffer
\**********************************************************************/
int iread(int jch)
{
    LBR = 0;
    return ioop(-1, jch, LINE);	/* always use the LINE buffer */
}

/**********************************************************************\
    iwrite -- write out the LINE buffer
\**********************************************************************/
int iwrite(int jch)
{
    LBW = 0;
    return ioop(1, jch, LINE);	/* always use the LINE buffer */
}

/**********************************************************************\
    icntl -- rewind the channel
\**********************************************************************/
int icntl(int jch)
{
    return ioop(0, jch, LINE);  /* always use the LINE buffer */
}

/**********************************************************************\
    iclose -- close the channel or all channels if -1
\**********************************************************************/
int iclose(int jch)
{
    int i;

    if ( jch > 0 && jch <= MAXCHAN )
	return ioop(0, jch, LINE);
    else if ( jch == -1 )
    {
	for ( i = 1; i < MAXCHAN; i++ )
	    if ( ioop(0, i, LINE) )
		return 2;	// error trying to close
    }
    else return 2;		// invalid channel number
    return 0;			// normal return for all files closed
}

/**********************************************************************\
    getArgs -- get command line args and set channel attributes
\**********************************************************************/
static int getArgs(int argc, char* argv[] )
{
    int i, openfail = 0;
    char *openstr = NULL;

#ifdef DEBUG
    printf("argc %d\n", argc);
    for ( i = 0; i < argc; i++ )
	printf("argv[%d] '%s'\n", i, argv[i]);
#endif

    for ( i = 1; i < argc; i++ )
    {
	if ( argv[i][0] != '-' )
	{
	    openfail += openChan(i, argv[i]);
	}
    }
    if ( openfail ) 
    {
	fprintf(stderr, "Grandios error, %d open failure(s).\n",
		openfail);
    }
    return 0;
}

/**********************************************************************\
    openChan -- open a channel for read or write with a filename
\**********************************************************************/
static int openChan(int jch, char * pFile)
{
    int retval = 0;
    char *openstr = NULL;

    if  ( chan[jch].r && chan[jch].w )
	openstr = "r+";
    else if ( chan[jch].r )
	openstr = "r";
    else if ( chan[jch].w )
	openstr = "w";
    if ( (chan[jch].fp = fopen(pFile, openstr)) == NULL )
    {
	if  ( chan[jch].r && chan[jch].w )
	{
	    openstr = "w+";
	    if ( (chan[jch].fp = fopen(pFile, openstr)) == NULL )
	    {
		fprintf(stderr, "Can't open '%s' for '%s'\n",
			pFile, openstr);
		retval = 2;
	    }
	}
	else
	{
	    fprintf(stderr, "Can't open '%s' for '%s'\n",
		    pFile, openstr);
	    retval = 1;
	}
    }
    if ( !retval )		// if "rw" open successful
    {				// save filename ptr in chan data
	if ( chan[jch].pFile )	// free any previous filename space
	    free(chan[jch].pFile);
// WARNING: be sure strdup() is the ONLY way pFile is ever set
	if ( (chan[jch].pFile = strdup(pFile)) == NULL )
	{
	    fprintf(stderr, "Filename '%s' save failed\n", pFile);
	    retval = 3;
	}
    }
    return retval;
}

/**********************************************************************\
    iinit -- default initialization
\**********************************************************************/
int iinit(int argc, char* argv[] )
{
    int iret;

    // default stream initializations
    chan[1].fp = stdin;
    chan[3].fp = stdout;
    chan[4].fp = stderr;

    iret = getArgs(argc, argv);	/* get args and set channels */

    return iret;
}

#ifdef WITHMAIN
/**********************************************************************\
    main -- process command line arguments and call application program
\**********************************************************************/
int main(int argc, char* argv[] )
{
    int iret;

    iret = iinit(argc, argv);
    if ( iret )
	return iret;

    progr();			/* call the application program */

    iret = iclose(-1);		// close all files
    return 0;
}
#endif

#ifdef TEST
/**********************************************************************\
    main -- process command line arguments and call application program
\**********************************************************************/
int main(int argc, char* argv[] )
{
    int iret;

    iret = iinit(argc, argv);
    if ( iret )
	return iret;

    while ( (iret = iread(1)) == 0 )
        iwrite(3);

    iret = iclose(-1);		// close all files
    return 0;
}
#endif
