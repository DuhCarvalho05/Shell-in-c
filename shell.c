#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_HISTORY_SIZE 10
#define MAX_COMMAND_LENGTH 50

void imprimir(char[]);
void my_exit(int code);
void show_history(char history[][MAX_COMMAND_LENGTH], int history_count);

int main() {
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    const char s[2] = " ";
    int contador_args = 0; 
    char entrada[MAX_COMMAND_LENGTH];
    char *aux[MAX_COMMAND_LENGTH];

    char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH] = {0}; // Histórico de comandos
    int history_count = 0; // Contador de comandos no histórico



    printf("SEJA BEM VINDO AO TERMINAL PERSONALIZADO!\n");
    printf("PARA SAIR DIGITE APENAS 'exit' NO TERMINAL!\n\n\n");

    while (1) {
        printf("user@host[%02d:%02d:%02d] $ ", tm.tm_hour, tm.tm_min, tm.tm_sec);   
        fgets(entrada, MAX_COMMAND_LENGTH, stdin);

        // Removendo o caractere de nova linha da entrada
         entrada[strcspn(entrada, "\n")] = 0;

        if (strcmp(entrada, "exit") == 0) {
            my_exit(2);
        } else if (strcmp(entrada, "history") == 0) {
            // Chama histórico de comandos
            show_history(history, history_count);
        } else {
            // Se não for hisotry vai apenas salvar os comandos no array
            if (history_count < MAX_HISTORY_SIZE) {
                strcpy(history[history_count], entrada);
                history_count++;
            } else {
                // Se for colocado mais de 10 comandos, exclui os antigos e adiciona os mais novos, deslocamento de array 
                for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
                    strcpy(history[i], history[i + 1]);
                }
                strcpy(history[MAX_HISTORY_SIZE - 1], entrada);
            }
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

    }

    return 0;
}

void imprimir(char l[]) {
    puts(l);
}

void my_exit(int code) {
    asm(
        "movl $1, %%eax\n\t"
        "movl %[code], %%ebx\n\t"
        "int $0x80\n\t"
        :
        : [code] "m"(code));
}

void show_history(char history[][MAX_COMMAND_LENGTH], int history_count) {
   
    printf("Histórico dos últimos %d comandos:\n", history_count);
    for (int i = 0; i < history_count; i++) {
        printf("%d: %s\n", i + 1, history[i]);
    }
}
