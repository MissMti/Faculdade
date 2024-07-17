#ifndef ep2
	
	#define ep2

        #include <stdio.h> 
        #include <time.h> 
        #include <stdlib.h>
        #include <string.h>

        #include <pthread.h>

        struct Participante;

        int tamanho(int a);    
        int sorteia();
        void printa_pista();
        void desce();
        void ordena(int n);
        void anda(struct Participante at);
        void* corrida(void *arg);
#endif