// C program for insertion sort
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <stdbool.h>
//Function declarations 
void generateRandomNum(int [], int);
void printArr (int [], int, bool, FILE *);
 
/* Driver program to test insertion sort */
int main(int argc, char *argv [])
{
	//Variables for serial insertion sort	
	int i, key, j;	

	//Open a file so that all printf will be printed out to this file
	FILE* file = fopen("serial_insertion_sort_300_elements.txt", "w");

	//Number of elements:
	int numOfElement = 300;

	//Array to be sorted
	int theArr[numOfElement];

	//Record the initial time 
	clock_t begin = clock();

	//Generate random numbers
	generateRandomNum(theArr, numOfElement);
	
	//Print the unsorted array
	printArr(theArr, numOfElement, false, file);
   
	//Serial insertion sort
   	for (i = 1; i < numOfElement; i++)
   	{
       		key = theArr[i];
       		j = i-1;
 		
		//Sort the array in an ascending order
       		while (j >= 0 && theArr[j] > key)
       		{
           		theArr[j+1] = theArr[j];
           		j = j-1;
       		}
       		
		theArr[j+1] = key;
   	}

	//Print the sorted Array
	printArr(theArr, numOfElement, true, file);

	//Record the End time
	clock_t end = clock();

	//Time spent
	printf("\nTime spent to fully execute this program: %f", (double)(end - begin) / CLOCKS_PER_SEC);	

	return 0;
}

//Function definitions
//Generate random number for the unsorted array
void generateRandomNum(int tempArr [], int tempNumOfElement){
	srand(time(NULL));
	int index;
	
	//Generate a random number for each element of array tempArr
	for (index = 0; index < tempNumOfElement; index++){
		tempArr[index] = rand() % 201 - 100;
	}
}

//Print the array
void printArr(int tempArr[], int tempNumOfElement, bool sorted, FILE * file){
	int index;
	
	if(sorted)
		printf("\nThe sorted array: [");
	else
		printf("The unsorted array: [");
	for (index = 0; index < tempNumOfElement; index++){
		if(index != tempNumOfElement - 1)
			printf("%d, ", tempArr[index]);
		else
			printf("%d", tempArr[index]);
	}
	printf("]\n");
}
