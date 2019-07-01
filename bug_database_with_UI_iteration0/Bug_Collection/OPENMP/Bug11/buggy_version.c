


int array[1000];

#pragma omp parallel
{
    bool flag = true;
    while(flag){
        int x = rand()%1000;
        #pragma omp critical
        {
            array[x] = some_function(array[x]);
            if (some_condition(array[x])){
                flag = false;
            }
        }

    }

}
