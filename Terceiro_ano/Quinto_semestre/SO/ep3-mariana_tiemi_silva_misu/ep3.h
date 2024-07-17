#ifndef ep3
	
	#define ep3

        #include <stdio.h> 
        #include <time.h> 
        #include <stdlib.h>
        #include <string.h>
        #include <readline/readline.h> 

        #include <pthread.h>

        void desmonta();
        void status();
        void lista(char** parametro);
        void apaga(char** parametro);
        void toca(char** parametro);
        void mostra(char** parametro);
        void criadir(char** parametro);
        void copia (char** parametro);
        void monta(char** parametro);
        int Comandos(char* comando, char** parametro);

#endif