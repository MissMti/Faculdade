#ifndef ep1
	
	#define ep1

        #include <stdio.h> 
        #include <string.h> 
        #include <sys/time.h>
        #include <pthread.h>
        #include <sched.h>

        void* Gasta_Tempo();
        int min(int a, int b);
        int max(int a, int b);
        void troca(int* A, int* C);
        int quant(int dl, int t);
        void EcP(FILE* a, FILE* b, int n);
        void RR(FILE* a, FILE* b, int n);
        void SJF(FILE* a, FILE* b, int n);

#endif