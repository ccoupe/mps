#include <stdio.h>
#include <ctype.h>
#include "f4io.h"

int jchan[] = {
	0,			/* Dummy to make 1-origin indexing */
	1, 0, 0, 0, 0, 2,	/* Standard input is default */
	1, 1, 1, 2, 0, 1,	/* "scratch" is default */
	0, 1, 0, 0, 1, 3,	/* Standard output is default */
	0, 1, 0, 0, 2, 3};	/* Standard error is default */

int maxch = 4;

char *name[] = {
	"",
	"scratch",
	"",
	""};

int
main()
{
	int k, l;

	progr();
	l = 1;
	for (k = 1; k < maxch; k++) (void)ioop(0, k, jchan, 1, &l);
	return 0;
}

int
ioop(jop, jch, jba, jp1, jp2)
int jop, jch, jba[], jp1, *jp2;
{
	int kch, k, jres;

	if (jch < 0 || jch > maxch) return 2;

	if (jch == 0) {
		if (jop < 0) return 1; else return 0;
	}

	kch = (jch - 1) * 6;
	if ((k = jchan[kch+6]) == 0) return 2;
	if (jop == 0) {
		if (jchan[kch+6] == 1) return 2;
		if (jchan[kch+4] != 0) return icntl(kch,jba,jp1,jp2);
		if (jp1 != *jp2) return 2;
		jchan[kch+6] = 1;
		return 0;
	}
	if (jop < 0) {
		switch (k) {
		case 3: /* Open for writing */ return 2;
		case 5: /* EOF */ return 1;
		case 1:	/* Closed */
			if (jchan[kch+1] == 0) return 2;
			if (jchan[kch+3] != 0) {
				if ((jres = iopen(0,kch,jba,jp1,jp2)) != 0)
					return jres;
			}
			jchan[kch+6] = 2;
		case 2:	/* Open for reading */
		case 4:	/* Open for reading and writing */
			jres = iread(-jop,kch,jba,jp1,jp2);
		}
	} else {
		switch (k) {
		case 2:	/* Open for reading */ return 2;
		case 5: /* EOF */ return 1;
		case 1: /* Closed */
			if (jchan[kch+2] == 0) return 2;
			if (jchan[kch+3] != 0) {
				if ((jres = iopen(1,kch,jba,jp1,jp2)) != 0)
					return jres;
			}
			jchan[kch+6] = 3;
		case 3:	/* Open for writing */
		case 4: /* Open for reading and writing */
			jres = iwrit(jop,kch,jba,jp1,jp2);
		}
	}
	if (jres == 1) jchan[kch+6] = 5;
	return jres;
}

int
iread(jop, kch, jba, jp1, jp2)
int jop, kch, jba[], jp1, *jp2;
{
	char c;

	if (read(jchan[kch+5], &c, 1) <= 0) return 1;

	while (toascii(c) != '\n') {
		if (jp1 < *jp2) jba[jp1++] = toascii(c);
		if (read(jchan[kch+5], &c, 1) <= 0) break;
	}

	*jp2 = jp1; return 0;
}

int
iwrit(jop, kch, jba, jp1, jp2)
int jop, kch, jba[], jp1, *jp2;
{
	char c;

	while (jp1 < *jp2) {
		c = jba[jp1++];
		if (write(jchan[kch+5], &c, 1) == -1) return 1;
	}
	(void)write(jchan[kch+5], "\n", 1); return 0;
}

int
iopen(jop, kch, jba, jp1, jp2)
int jop, kch, jba[], jp1, *jp2;
{
	switch (jop) {
	case 0:
		if ((jchan[kch+5] = open(name[kch/6], 0)) == -1) return 2;
		break;
	case 1:
		if ((jchan[kch+5] = creat(name[kch/6], 0644)) == -1) return 2;
		break;
	}
	return 0;
}

int
icntl(kch, jba, jp1, jp2)
int kch, jba[], jp1, *jp2;
{
	if (close(jchan[kch+5]) == 0) {
		jchan[kch+6] = 1;
		return 0;
	}
	return 2;
}
