#include <stdio.h>

int main () {
	FILE *f;
	int a[100];
	f = fopen("test.txt","r");
	char* string1;
	fscanf(f,"%s\n",string1);
	for (int i =0; i < 100; i++) {
		fscanf(f,"%d",a[i]);
	}
	char*string2;
	fscanf(f,"\n%s\n",string2);
	fclose(f);
	printf("%s",string1);
	for (int i = 0; i < 100; i ++) {
		printf("a[%d] is %d\n");
	}
	printf("%s",string2);
	/*
	f = fopen("test.txt","w+");

	fprintf(f,"start writing\n");
	for (int i =0; i < 100;i++) {
		fprintf(f,"%d",a[i]);
	}
	fprintf(f,"finish writing\n");
	fclose(f);
	*/
	return 0;
}
