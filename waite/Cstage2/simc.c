#include <stdio.h>
#define MAXLIST 32768	/* Size of the macro definition array */

int list[MAXLIST];

progr()
{
	int i, j, k, l, m, n;

	i = MAXLIST;
	if (ioop(-1,1,list,1,&i) != 0) {
		fprintf(stderr,"STOP 10\n");
		return;
	}

	list[6]=100; m=17;

	do {
		k = m+1; list[k++] = -1;

		i = MAXLIST;
		if (ioop(-1,1,list,k,&i) != 0) {
			fprintf(stderr,"STOP 11\n");
			return;
		}
		if (i == MAXLIST) {
			fprintf(stderr,"STOP 20\n");
			return;
		}

		list[i]=list[1]; while (list[k++] != list[1]) ;

		for (;;) {
			j = MAXLIST;
			if (ioop(-1,1,list,k,&j) != 0) {
				fprintf(stderr,"STOP 12\n");
				return;
			}
			if (j == MAXLIST) {
				fprintf(stderr,"STOP 21\n");
				return;
			}

			list[j]=list[3]; if (list[k] == list[3]) break;

			do {
				if (list[k] == list[4]) {
					list[k++] = -2;
					list[k++] -= (list[5]-7);
					list[k] -= list[5];
					if (list[k-1] == 7 &&
							list[m+1] < list[k])
						list[m+1]=list[k];
				}
				k++;
			} while (list[k] != list[3]);
			list[k++] = -1;
		}
		m = (list[m] = k);
	} while (list[k+1] != list[3] || k+1 == j);

source:
	n = MAXLIST;
	if (ioop(-1,1,list,k,&n) != 0) return;
	if (n == MAXLIST) {
		fprintf(stderr, "STOP 30\n");
		return;
	}

	list[n]=list[1]; m=17;
	do {
		l=8; j=m+1;
		for (i = k; i <= n; i++) {
			j++;
			if (list[j] == list[1]) break;
			if (list[j] == list[2]) {
				if (l == 17) {
					fprintf(stderr,"STOP 31\n");
					return;
				}
				list[l++] = list[i];
			} else if (list[j] != list[i]) break;
		}
		if (list[j++] == list[i]) {
			i = k;
			while (j < list[m]) {
				if (i > MAXLIST-4) {
					fprintf(stderr,"STOP 32\n");
					return;
				}
				if (list[j] == -1) {
					if (ioop(1,3,list,k,&i) != 0) {
						fprintf(stderr,"STOP 33\n");
						return;
					}
					i = k;
				} else if (list[j] == -2) {
					l = list[j+1]; j = j+2;
					if (l == 7) list[7] = list[j] + list[6];
					if (list[j] == 0 && l != 7)
						list[i++] = list[l];
					else {
						list[i]=list[l]/100;
						n = list[l]/10;
						list[i+1]=n-list[i]*10+list[5];
						list[i+2]=list[l]-n*10+list[5];
						list[i]+=list[5];
						if (list[i]==list[5]) {
							list[i]=' ';
							if (list[i+1]==list[5])
								list[i+1]=' ';
						}
						i += 3;
					}
				} else list[i++] = list[j];
				j++;
			}
		list[6] += list[m+1] + 1;
		goto source;
		}
		m = list[m];
	} while (m < k);
	if (ioop(1,3,list,k,&n) != 0) {
		fprintf(stderr,"STOP 34\n");
		return;
	}
	goto source;
}
