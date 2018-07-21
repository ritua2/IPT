#include <stdio.h>

int main() {
	FILE *fp;
	fp = fopen("test.txt", "w+");
	fprintf(fp,"nothing to see here\n");
	for (int i =0; i < 100; i++) {
		fprintf(fp, "printing %d rank of %d\n",i,100);
	}
   //fputs("This is testing for fputs...\n", fp);	
	fclose(fp);
	return 0;
}
