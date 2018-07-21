#include <stdio.h>

int main () {
	FILE *f = NULL;
	int a[100];
	
	f = fopen("test.txt","r");
	
	for (int i =0; i < 100; i++) {
		fscanf(f,"%d",a[i]);
	}
	fclose(f);
	

	printf ("\n\n\nRead array\n");
	for ( int i =0; i < 100; i++) {
		a[i]  = i;
	}
	
	for (int i = 0; i < 100; i ++) {
		printf("a[%d] is %d\n",i,a[i]);
	}
	

	printf ("\n\n\nwriting array:\n");
	for ( int i =0; i < 100; i++) {
                a[i]  = i;
        }	
	f = fopen("test.txt","w+");

	for (int i =0; i < 100;i++) {
		fprintf(f,"%d",a[i]);
	}
	fclose(f);
	
	return 0;
}
