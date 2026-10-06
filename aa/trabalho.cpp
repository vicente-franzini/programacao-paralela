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

// tempos[vecsize][alg][run]
int64_t ***tempos = nullptr;

void thread() {
    int cthread = omp_get_thread_num();
    int thread_count = omp_get_num_threads();
    std::pair<int, int> runs;

    // 0:1 2:3 4:6 7:9 10:12 13:15
    // 2 =>  4:6  => 16 - (4*3) : 
    // 3 =>  7:9  => 
    // 4 => 10:12 => 
    // 5 => 13:15 => 

    int lct = thread_count - (RUNS_PER_SIZE % thread_count);
    int rc = (RUNS_PER_SIZE / thread_count);
    if(cthread < lct) {
        runs.first = cthread * rc;
    } else {
        rc++;
        runs.first = RUNS_PER_SIZE - ((thread_count - cthread) * rc);
    }

    runs.second = runs.first + rc;

    // Configura o RNG, com a seed sendo o relógio local em millisegundos.
    std::mt19937 rng;
    std::uniform_int_distribution<int_fast32_t> dist;
    rng.seed(sc::duration_cast<sc::milliseconds>(sc::system_clock::now().time_since_epoch()).count());

    int vec[VEC_SIZE_JUMP * JUMP_COUNT];
    sc::steady_clock::time_point comeco, fim;

    for(int r = runs.first; r < runs.second; r++) {

        for(int i = 0; i < JUMP_COUNT; i++) {
            size_t vecsize = (i + 1) * VEC_SIZE_JUMP;

            /*
             *  BUBBLE SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            bubblesort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][0][r] = (fim - comeco).count();



            /*
             *  INSERTION SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            insertion(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][1][r] = (fim - comeco).count();



            /*
             *  QUICKSORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            quicksort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][2][r] = (fim - comeco).count();



            /*
             *  QUICKSORT + INSERTION
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            quicksort(vec, 0, vecsize - 1, 70);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][3][r] = (fim - comeco).count();


        
            /*
             *  SHELLSORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            shellsort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();

#           pragma omp atomic write
            tempos[i][4][r] = (fim - comeco).count();



            /*
             *  STD::SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            std::sort(&vec[0], &vec[vecsize - 1]);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][5][r] = (fim - comeco).count();



            /*
             *  HEAPSORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            heapsort(vec, vecsize);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][6][r] = (fim - comeco).count();



            /*
             *  MERGE SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            mergesort(vec, 0, vecsize - 1);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][7][r] = (fim - comeco).count();


            
            /*
             *  SELECTION SORT
             */
            fillvec(vec, vecsize, dist, rng);
            comeco = sc::steady_clock::now();

            selectionsort(vec, vecsize);
            fim = sc::steady_clock::now();
#           pragma omp atomic write
            tempos[i][8][r] = (fim - comeco).count();



//#           pragma omp critical
//            printf("[%d] Size %ld on run %d/%d\n", cthread, vecsize, r + 1, runs);
        }


#       pragma omp critical
        printf("[%d] Run %d/%d ended\n", cthread, r + 1, runs.second);
    }
}

void closeFile() {
    if(arq.is_open()) {
        const char msg[] = "\nEscrevendo resultados ao disco.\n";
        std::fwrite(&msg, sizeof(msg), 1, stdout);

        for(int i = 0; i < JUMP_COUNT; i++) {
            arq << (i + 1) * VEC_SIZE_JUMP << ',';
            for(int j = 0; j < ALG_COUNT; j++) {
                double perc = 0.1;

                long_double_t mean = 0, variance = 0;
                long_double_t ly = round((double) RUNS_PER_SIZE * (1.0 - perc));
                long_double_t n = 0;

                std::sort(&tempos[i][j][0], &tempos[i][j][RUNS_PER_SIZE - 1]);

                for(int r = floor((double) RUNS_PER_SIZE * (perc / 2.0)); r < ly; r++) { 
                    n++;
                    mean += (long_double_t) tempos[i][j][r] / DIVISOR_TEMPO;
                }

                mean /= n;
                n--;

                for(int r = 0; r < ly; r++) {
                    variance += powl(((long_double_t) tempos[i][j][r] / DIVISOR_TEMPO) - mean, 2) / n;
                }

                long_double_t a = ((mean * mean) / (variance * variance));
                long_double_t b = (variance * variance) / mean;

                if(a > 1) {
                    arq << ((a - 1) * b);
                } else {
                    arq << mean;
                    std::cout << "mean used at " << i << '\n';
                }
                if(j != ALG_COUNT - 1) arq << ',';
                else arq << '\n';
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
    
    // tempos[vecsize][alg][run]
    tempos = new int64_t**[JUMP_COUNT];
    for(int i = 0; i < JUMP_COUNT; i++) {
        tempos[i] = new int64_t*[ALG_COUNT];
        for(int j = 0; j < RUNS_PER_SIZE; j++) {
            tempos[i][j] = new int64_t[RUNS_PER_SIZE](-1);
        }
    }

#   pragma omp parallel num_threads(thread_count)
    thread();

    return 0;
}
