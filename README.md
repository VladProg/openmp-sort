Компіляція:

```shell
g++ -fopenmp -O2 openmp.cpp -o openmp.exe
```

Запуск:

```shell
set OMP_NUM_THREADS=$THREADS && echo $N | openmp.exe > output.txt
```
