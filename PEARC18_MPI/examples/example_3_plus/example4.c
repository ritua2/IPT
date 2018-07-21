//Code courtesy:
//https://cvw.cac.cornell.edu/mpicc/broadcast
//
#include <stdio.h>
#include <string.h> 
int main(int argc, char **argv)
{
  char message[20];
  
  strcpy(message, "Hello, world");
 
  printf( "Message is: %s\n", message);

}

