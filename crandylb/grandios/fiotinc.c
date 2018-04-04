/* fiotinc.c -- F4IO Test in C, CRB,  Oct 1, 2012 */
// CRB Initial coding, Dec 11, 1994

#include <stdio.h>
#include "grandios.h"

extern char LINE[MAXCHAN+2];
extern int LBW;

char MINE[MAXLEN+1];

/**********************************************************************\
    progr -- The Grandios Application Program
\**********************************************************************/
void progr(void)
{
    if ( ioop(-1, 1, LINE) ) puts("STOP 11");	/* read a line */
    if ( ioop( 1, 3, LINE) ) puts("STOP 31");	/* copy it out */
    LINE[45] += 1;				/* change the 3 to 4 */
    if ( ioop( 1, 4, LINE) ) puts("STOP 41");	/* copy it out */
    iwrch(-1);
    if ( ioop( 1, 4, LINE) ) puts("STOP 42");	/* copy it out */
    if ( ioop(-1, 1, LINE) ) puts("STOP 12");	/* read a line */
    if ( ioop( 1, 4, LINE) ) puts("STOP 43");	/* copy it out */
    LINE[0] += 1;				/* bump line number */
    LBW = 2;					// move the write index
    iwrch(-1);					/* clear rest of line */
    if ( ioop( 1, 4, LINE) ) puts("STOP 44");	/* output number */
    if ( ioop(-1, 1, LINE) ) puts("STOP 13");	/* read a line */
    if ( ioop(-1, 1, MINE) ) puts("STOP 14");	/* read another */
    if ( ioop( 1, 4, LINE) ) puts("STOP 45");	/* output line */
    if ( ioop( 1, 4, MINE) ) puts("STOP 46");	/* output other */
    if ( ioop(-1, 1, MINE) != 1 )
	puts("STOP 15");			/* read EOF */
    return;
}
