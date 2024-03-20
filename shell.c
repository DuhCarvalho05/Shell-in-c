#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void imprimir(char[]);
void my_exit(int code);

int main()
{
	time_t mytime;
	mytime = time(NULL);
	struct tm tm = *localtime(&mytime);
	char entrada[50];
	char* aux[100];
	int contador_args = 0;
	const char s[2] = " ";

	printf("SEJA BEM VINDO AO TERMINAL PERSONALIZADO!\n");
	printf("PARA SAIR DIGITE APENAS 'exit' NO TERMINAL!\n\n\n");

	while (1)
	{
		printf("user@host[%02d:%02d:%02d] $ ", tm.tm_hour, tm.tm_min, tm.tm_sec);
		fgets(entrada, 50, stdin);

		if (strcmp(entrada, "exit\n") == 0)
		{
			my_exit(2);
		}

		
		
		char *token;
		token = strtok(entrada, s);
		aux[contador_args] = token;
		contador_args++;
		while (token != NULL){
			token = strtok(NULL, s);
			aux[contador_args] = token;
			contador_args++;
		}

		aux[(contador_args+1)] = NULL;
		char* args[contador_args];

		for(int i = 0; i < contador_args; i++){
			args[i] = aux[i];
		}

		execvp(args[0], args);



		


		// if (fork() != 0){
		// 	waitpid(-1, &status, 0);
		// }else{
		// 	execve(command, parameters, 0);
		// }

		// imprimir(entrada);
	}

	return 0;
}

void imprimir(char l[])
{
	puts(l);
}
void my_exit(int code)
{
	asm(
		"movl $1, %%eax\n\t"
		"movl %[code], %%ebx\n\t"
		"int $0x80\n\t"
		: /* no output */
		: [code] "m"(code));
}