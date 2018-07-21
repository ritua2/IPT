#include <stdio.h>

int main(int argc,char *argv[]){

    int i, sum,upToVal;

    upToVal = 10000;

    sum = 0;

    for(i=0; i<= upToVal; i++){
         sum = sum +i;
    }
    printf("\nSum of first %d numbers is: %d\n\n",upToVal, sum);
    return 0;
}

