#include "sorts/sorts.hpp"
#include "consts.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <csignal>
#include <iomanip>
#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>
#include <random>

#include <omp.h>

#define fillvec(_vec, _size, _dist, _rng) for(long unsigned int i = 0; i < _size; i++)\
        _vec[i] = _dist(_rng);

namespace sc = std::chrono;

typedef sc::_V2::steady_clock::rep timerep_t;

std::ofstream arq;
long_double_t **tempos = nullptr;

void thread() {
    int cthread = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    int runs = RUNS_PER_SIZE / thread_count;

    if(cthread < RUNS_PER_SIZE % thread_count) runs++;

    // Configura o RNG, com a seed sendo o relógio local em millisegundos.
    std::mt19937 rng;
    std::uniform_int_distribution<int_fast32_t> dist;
    rng.seed(sc::duration_cast<sc::milliseconds>(sc::system_clock::now().time_since_epoch()).count());

    int vec[MAX_VECTOR_SIZE];
    sc::steady_clock::time_point comeco, fim;

    for(int r = 0; r < runs; r++) {

        for(size_t vecsize = VEC_SIZE_JUMP; vecsize <= MAX_VECTOR_SIZE; vecsize += VEC_SIZE_JUMP) {
            int i = (vecsize / VEC_SIZE_JUMP) - 1;

            /*
             *  BUBBLE SORT
             
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            bubblesort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][0] += (long_double_t) (fim - comeco).count() / DIVISOR_TEMPO;
*/


            /*
             *  INSERTION SORT
             
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            insertion(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][1] += (long_double_t) (fim - comeco).count() / DIVISOR_TEMPO;
*/


            /*
             *  QUICKSORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            quicksort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][2] += (long_double_t) (fim - comeco).count() / DIVISOR_TEMPO;



            /*
             *  QUICKSORT + INSERTION
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            quicksort(vec, 0, vecsize - 1, 70);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][3] += (long_double_t)  (fim - comeco).count() / DIVISOR_TEMPO;


        
            /*
             *  SHELLSORT
             
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            shellsort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();

#           pragma omp atomic
            tempos[i][4] += (long_double_t) (fim - comeco).count() / DIVISOR_TEMPO;
*/


            /*
             *  STD::SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            std::sort(&vec[0], &vec[vecsize - 1]);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][5] += (long_double_t)  (fim - comeco).count() / DIVISOR_TEMPO;



            /*
             *  HEAPSORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            heapsort(vec, vecsize);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][6] += (long_double_t)  (fim - comeco).count() / DIVISOR_TEMPO;



            /*
             *  MERGE SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            mergesort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][7] += (long_double_t)  (fim - comeco).count() / DIVISOR_TEMPO;


            
            /*
             *  SELECTION SORT
             
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            selectionsort(vec, vecsize);
            fim = sc::steady_clock::now();
#           pragma omp atomic
            tempos[i][8] += (long_double_t)  (fim - comeco).count() / DIVISOR_TEMPO;
*/


//#           pragma omp critical
//            printf("[%d] Size %ld on run %d/%d\n", cthread, vecsize, r + 1, runs);
        }


#       pragma omp critical
        printf("[%d] Run %d/%d ended\n", cthread, r + 1, runs);
    }
}

void closeFile() {
    if(arq.is_open()) {
        const char msg[] = "\nEscrevendo resultados ao disco.\n";
        std::fwrite(&msg, sizeof(msg), 1, stdout);

        for(int i = 1; i < (MAX_VECTOR_SIZE / VEC_SIZE_JUMP + 1); i++) {
            arq << i * VEC_SIZE_JUMP << ',';
            
            for(int j = 0; j < ALG_COUNT; j++) {
                arq << std::setprecision(9) 
                << tempos[i - 1][j] / (long_double_t) RUNS_PER_SIZE;
                if(j == ALG_COUNT - 1) arq << '\n';
                else arq << ',';
            }
        }
        arq << std::flush;
        arq.close();
    }
}

void handleSignal(int sig) {
    exit(0);
}

int main() {
    const auto thread_count = std::max(1u, std::thread::hardware_concurrency());

    // Configure o gerador de sementes
    std::mt19937 rng;
    std::uniform_int_distribution<int_fast32_t> dist;
    rng.seed(time(NULL));

    std::string filename = "comparacao-";
    filename += std::to_string(time(NULL));
    filename += ".csv";

    arq = std::ofstream(filename);
    arq << "TamanhoVetor,BubbleSort,InsertionSort,QuickSort,Misto,ShellSort,std::sort,HeapSort,MergeSort,SelectionSort\n";

    std::atexit(closeFile);
    std::signal(SIGINT, handleSignal);
    
    tempos = new long_double_t*[MAX_VECTOR_SIZE / VEC_SIZE_JUMP];
    for(int i = 0; i < (MAX_VECTOR_SIZE / VEC_SIZE_JUMP); i++)
        tempos[i] = new long_double_t[ALG_COUNT](0);

#   pragma omp parallel num_threads(thread_count)
    thread();

    return 0;
}
