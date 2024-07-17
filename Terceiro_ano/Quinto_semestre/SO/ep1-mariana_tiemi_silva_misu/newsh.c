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
#include "newsh.h"

#define MLetra 1000 //Maximo de char na comando
#define MCom 100 //Maximo de parametross

struct utsname data;

//Funcao para pegar o username
char* Username(uid_t id){
  struct passwd *pw = getpwuid(id);
  if (pw){
    return pw->pw_name;
  }
  return "";
}

// Funcao imprime prompt
void Prompt() { 
	//pegar id para devolver o nome do usuario como argumento
	uid_t id = geteuid();
	char* nome = Username(id);

	//pegar horas
	time_t ref = time(NULL);
	struct tm *tempo = localtime(&ref);

	//prompt da linha
	printf("\n%s [%d:%d:%d]: ",nome,tempo->tm_hour,tempo->tm_min,tempo->tm_sec); 
} 

// Funcao de Leitura
int Leitura(char* comando) { 
	
	char* a = readline(""); 
	if(strlen(a)==0){
		return 0;
	}

	add_history(a); 
	strcpy(comando, a); 
	return 1;
} 


// Funcao que executa o resto dos parametross
void executaresto(char** parametros) { 
	pid_t id = fork();
	if(id!=-1){
		if(id!=0){
			waitpid(-1,NULL,0); 
			return; 
		}
		else{
			execvp(parametros[0], parametros);
			exit(0); 
		}
	}

} 


// Funcao executa parametross com as syscalls
int execomsys(char** parametro) { 
 
	char* Com[5];
    Com[0] = "cd";
	Com[1] = "rm"; 
    Com[2] = "uname"; 

	int tmp;
	for (int i = 0; i < 3; i++) { 
		if (strcmp(parametro[0], Com[i]) == 0) { 
            tmp=i;
		} 
	} 

    if(tmp==0){
        chdir(parametro[1]);
        return 1; 
    }
    if(tmp==1){
        unlink(parametro[1]);
        return 1; 
    }
    if(tmp==2){
        if (strcmp(parametro[1], "-a") == 0){
                //printf("achou!\n");
            uname (&data);
			printf("%s %s %s %s %s", data.sysname, data.nodename,data.release,data.version,data.machine);
			return 1;
		}
    }
    
	return 0;//retona 1 se jah fez a acao e retorna 0 se nao fez... 
} 


void Processacomando(char* comando, char** parametros) { 
	// para separar os parametros do comando
	for (int i = 0; i < MCom; i++) { 
		parametros[i]= strsep(&comando, " "); 
		if (parametros[i]== NULL){
			break; 
		}				
	} 

	int a=execomsys(parametros);
	if (a==1) {//para executar cd, uname -a e rm...
        return;
    }	 
	
	executaresto(parametros);
    return; 
		
} 

int main() { 

	char comando[MLetra];
    char* parametros [MCom];

	while (1) { 
		// Imprime prompt
		Prompt();

		// Lê comando
		int ref=Leitura(comando);

		if(ref==1){
			// Processa
			Processacomando(comando, parametros);
		}
			
	} 

	return 0; 

} 


