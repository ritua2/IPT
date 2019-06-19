//1. Initialize global variables
//2. Initialize MPI, get  number of processes, get rank 
//3. All processes create two dimensional arrays 
    array1 = (char **) malloc(sizeArray1 * sizeof(char *));
    array1[0] = (char *) malloc(sizeArray1 * lineLength * sizeof(char)); 
    for(i = 1; i < sizeArray1; i++)
    {
            array1[i] = array1[i - 1] + lineLength;
    }
    //4. Only server will populate it's arrays, then broadcast to all processes
    if(rank == 0)
    {
            f = fopen("path..../testFile1.txt", "r");
            if(NULL == f) {
                    perror("FAILED: ");
                    return -1;
            }
            numWords = 0;
            while(err != EOF && numWords < sizeArray2)
            {
                    err = fscanf(f, "%[^\n]\n", array2[numWords]);
                    numWords ++;
            }
            fclose(f);

    }

//5. Broadcast each line from both arrays to all processes 
MPI_Bcast(array1, sizeArrray1 * lineLength, MPI_CHAR, 0, MPI_COMM_WORLD);
 //6. do further work on arrays 