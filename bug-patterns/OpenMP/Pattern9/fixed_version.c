#include <stdio.h>

bool checkPrime(int n) {
    if (n == 2) return true;
    if (n < 2 || n % 2 == 0) return false;
    bool prime = true;

    #pragma omp parallel for
    for (int i = 3; i <= static_cast<int>(sqrt(n)); i += 2) {
        if (n % i == 0) prime = false;
    }

    return prime;        
}

int main(){
	checkPrime(1000);
	return 0;
}


