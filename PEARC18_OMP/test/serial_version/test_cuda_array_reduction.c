#include<stdio.h>

int main() {
	int array_main [100];
	for (int i = 0; i < 100; i++) {
		array_main[i] = i;
	}

	for (int iter = 0; iter < 1000; iter++) {
		for (int i = 0; i < 100; i++) {
			array_main[i] +=1;
		}
	}
	printf("%d\n",array_main[0]);
	return 0;
}
