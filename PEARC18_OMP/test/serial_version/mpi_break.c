#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv) {

	int sum = 0;
	int end = atoi(argv[1]);
	for (int i = 0; i < end; i++) {
		sum+=i*2;
		if (i*10 > 1000) {
			break;
		}
	}
	printf("sum is : %d\n",sum);
	return 0;
}
