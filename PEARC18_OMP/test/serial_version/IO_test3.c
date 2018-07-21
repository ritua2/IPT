#include <stdio.h>

int main () {
	FILE *f;
	f = fopen("test.txt","w+");
	int a =0;
	fprintf(f,"start writing\n");
	for (int i =0; i < 100;i++) {
		fprintf(f,"data : %d\n",i);
		fprintf(f,"...\n");
	}
	fprintf(f,"finish writing\n");
	fclose(f);
	return 0;
}
