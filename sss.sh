cc search.c -I$GETAROOT/include -L$GETAROOT/lib -ldw -ldws -ldwut -lm -ldl
cc split-omp.c mylist.c -I$GETAROOT/include -L$GETAROOT/lib -ldw -ldws -ldwut -lm -ldl -fopenmp -lgomp -o split-omp
cc split-omp.c mylist.c travedir.c -I$GETAROOT/include -L$GETAROOT/lib -ldw -ldws -ldwut -lm -ldl -fopenmp -lgomp -o split-omp
export OMP_NUM_THREADS=2
qrsh -nostdin -now no -pe smp 3 -q canis4.q 'export OMP_NUM_THREADS=3;export GETAROOT=/home/ko/local/geta3;./split-omp NTCIR 3 20 >& yyyyyyyyy.txt'&
qrsh -nostdin -N all3 -q canis6.q GETAROOT=/home/ko/local/geta3 ./split-omp NTCIR all 20 3 >& all3.txt &
