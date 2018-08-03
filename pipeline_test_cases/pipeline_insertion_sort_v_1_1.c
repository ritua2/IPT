// C program for insertion sort
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include "mpi.h"
#include <stdbool.h>
#include <string.h>

//Function declarations 
void generateRandomNum(int [], int);
void printArr (int [], int, bool, FILE *);
 
/* Driver program to test insertion sort */
int main(int argc, char *argv []){
	//Variables for MPI time
	double initialTime, endTime;

	//Local variables
	int i, j, index = 0, temp, theIndex, bigArrIndex = 0, end;

	//Open a file so that all printf will be printed out to this file
	FILE* file = fopen("parallel_insertion_sort_11_elements.txt", "w");

	//Number of elements:
	int numOfElement = 11;
	
	//Array
	int localArr[2];
	int globalArr[2];
	
	//MPI variables
	int rank;
	int size;
	
	//MPI initialization
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	//Array
	int * bigArr;
	int * subArray;
	int * tempBigArr;
	
	if(rank == 0){
		//Record the intial time
		initialTime = MPI_Wtime();	

		//Dynamically create an array for storing the list of unsorted numbers
		bigArr = (int *)malloc(numOfElement*sizeof(int));	

		//Generate random numbers to array arr
		generateRandomNum(bigArr, numOfElement);

		//Print the unsorted array
		printArr(bigArr, numOfElement, false, file);
	}
        
    int myArrayLength = (numOfElement/size);
    if (rank == 0){
       myArrayLength = myArrayLength + (numOfElement%size);
     
    }

	//3 Cases:
	//#elements % #size = 0
	//#elements % #size < #size && #elements > #size
	//#elements % #size < #size && #elements < #size 

	//Split the array into several small arrays and scatter each small array to each corresponding process
	subArray = (int *)malloc(myArrayLength*sizeof(int));
	subArray[0] = 9999;

    int displacements[size], counts[size];

    for ( int i=0; i<size; i++) {
           if (i == 0){
              counts[i] = (numOfElement/size) + (numOfElement%size);
              displacements[i] = 0;
           }else{
			  counts[i] = (numOfElement/size);
			  displacements[i] = counts[i-1] + displacements [i-1];
           }
    //printf("\nThe rank is %d, counts is %d, and displacements is %d\n", i, counts[i], displacements[i] );
    }

    MPI_Scatterv( bigArr, counts, displacements, MPI_INT, subArray, counts[rank], MPI_INT, 0, MPI_COMM_WORLD); 

    //Print out to the screen the sub array for each process
	if(rank == 0)
		end = ((numOfElement-(numOfElement%size))/size) + numOfElement%size;
	else
		end = ((numOfElement-(numOfElement%size))/size);
	MPI_Barrier(MPI_COMM_WORLD);
	printf("The unsorted array of rank - %d: [", rank);
	for(theIndex = 0; theIndex < end; theIndex++){
		printf("%d ", subArray[theIndex]);
	}
	printf( "]\n\n");
       
	
	//Sort the sub array in ascending order within each process by using serial insertion sort
	for(i = 0; i < end; i++){
		temp = subArray[i];
		j = i - 1;
		while(j >= 0 && subArray[j] > temp){
			subArray[j+1] = subArray[j];
			j--;
		}
		subArray[j+1] = temp;
	}

	//Wait until each process has its own sorted sub array
	MPI_Barrier(MPI_COMM_WORLD);

	//Merge all sorted sub arrays into one big sorted array in ascending order
	for(i = 0; i < numOfElement; i++){
		MPI_Barrier(MPI_COMM_WORLD);
		localArr[0] = subArray[0];
		localArr[1] = rank;
		MPI_Allreduce(localArr, globalArr, 1, MPI_2INT, MPI_MINLOC, MPI_COMM_WORLD);
		
		//Sort the big array in ascending order
		if(rank == 0){
			bigArr[i] = globalArr[0];
		}
		
		if(rank == globalArr[1]){
			index += 1;
			if(rank == 0 && index == ((numOfElement-(numOfElement%size))/size)+(numOfElement%size)){
				subArray[0] = 9999;
			}
			else if(rank != 0 && index == (numOfElement-(numOfElement%size))/size){
				subArray[0] = 9999;
			}
			else
				subArray[0] = subArray[index];
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	if(rank == 0){
		printArr(bigArr, numOfElement, true, file);
		endTime = MPI_Wtime();
		printf( "Time spent: %lf\n", (float)(endTime - initialTime));
	}

	//MPI Finalization
	//MPI_Barrier(MPI_COMM_WORLD);
	MPI_Finalize();

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
		printf( "\nThe sorted array: [");
	else
		printf( "The unsorted array: [");
	for (index = 0; index < tempNumOfElement; index++){
		if(index != tempNumOfElement - 1)
			printf("%d, ", tempArr[index]);
		else
			printf("%d", tempArr[index]);
	}
	printf("]\n\n");
}


