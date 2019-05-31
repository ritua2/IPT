#include <stdio.h>
#include<math.h>
bool checkPrime(int n) {
    if (n == 2) return true;
    if (n < 2 || n % 2 == 0) return false;      
	#pragma omp parallel for
    for (int i = 3; i <= static_cast<int>(sqrt(n)); i += 2) {
        if (n % i == 0) return false;
    }
    return true;        
}

int main(){
	bool prime = checkPrime(1000);
	printf("%d \n ", prime);
	return 0;
}


