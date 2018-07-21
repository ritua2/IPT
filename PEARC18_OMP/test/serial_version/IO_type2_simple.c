#include <stdio.h>

int main () {
	int a[100];
	for (int i =0; i < 100; i++) {
		a[i] =i+1;
	}
	FILE* f;
	f = fopen("test.txt","w+");
	for (int i =0; i < 100; i++) {
		fprintf(f,"%d",a[i]); 
	}
	fclose(f);
	return 0;
}
