#ifndef newsh
	
	#define newsh

        #include <stdio.h> 
        #include <string.h> 
        #include <stdlib.h> 
        #include <readline/readline.h> 
        #include <readline/history.h> 
        #include <unistd.h> 
        #include <sys/types.h> 
        #include <sys/wait.h> 
        #include <sys/utsname.h>
        #include <pwd.h>
        #include <time.h>

        char* Username(uid_t id);
        void Prompt();
        int Leitura(char* str);
        void executaresto(char** parametros);
        int execomsys(char** parametro);
        void Processacomando(char* comando, char** parametros);

#endif