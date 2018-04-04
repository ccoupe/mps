#include <stdio.h>

void
iwrch(jchar,jbuff,jindx,jflag,jleng)
int jchar,jbuff[BUFSIZ-1],*jindx,*jflag,*jleng;
/*
 * This routine enters a character into the line buffer.  If the
 * character is negative, it is taken to be a terminator.  If the line
 * buffer is full, it is also terminated and the character is ignored.
 * If the line buffer is terminated, a flag is returned to the user
 * indicating this fact.  The line buffer limit is set to index the
 * position beyond the last significant character.  If the line buffer
 * is not terminated, then the character is entered at the indexed
 * position and the index is incremented.  The limit index is not
 * altered in this case.
 *
 *        jchar  is the integer representing the character
 *
 *        jbuff  is the address of the line buffer
 *
 *        jindx  is the index to the line buffer
 *
 *        jflag is set to zero if the line buffer is not full and the
 *             character inserted was not cr/lf.  It is set to 1
 *             otherwise
 *
 *       jleng is the last+1 index of information in the buffer
 */
{
/*
 *        If the character is negative, the buffer must be terminated.
 *        It is immaterial whether the buffer is full or not.
 */
	if (jchar >= 0 &&
/*
 *       If the user is attempting to put a significant character into
 *        the line buffer, we must ensure that there is enough room.
 */
		BUFSIZ-2 >= *jindx) {
/*
 *        If there is enough room, put the character into the buffer and
 *        increment the current character position.
 */
		jbuff[*jindx]=jchar;
		*jindx = *jindx+1;
/*
 *        Set the flag to indicate acceptance of the current character
 *        and return to the caller.
 */
		*jflag=0;
	} else {
/*
 *        The line buffer has been terminated.  Set the terminating
 *        character and the limit index.
 */
		jbuff[*jindx] = -1;
		*jleng = *jindx;
/*
 *        Reset the user index for the next line.
 */
		*jindx=1;
/*
 *        Set the flag to indicate rejection of the current character
 *        and return to the caller.
 */
		*jflag=1;
	}
}
