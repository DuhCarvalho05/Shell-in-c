
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_HISTORY_SIZE 10
#define MAX_COMMAND_LENGTH 50

void imprimir(char[]);
void my_exit(int code);
void show_history(char history[][MAX_COMMAND_LENGTH], int history_count);
void execute_command(char *entrada[], char history[][MAX_COMMAND_LENGTH], int *history_count);
char* getHost();
char* getpath();

int main() {
    time_t mytime;
    mytime = time(NULL);
    struct tm tm = *localtime(&mytime);
    const char s[2] = " ";
    char entrada[MAX_COMMAND_LENGTH];

    char history[MAX_HISTORY_SIZE][MAX_COMMAND_LENGTH] = {0}; // Histórico de comandos
    int history_count = 0; // Contador de comandos no histórico

    printf("SEJA BEM VINDO AO TERMINAL PERSONALIZADO!\n");
    printf("PARA SAIR DIGITE APENAS 'exit' NO TERMINAL!\n\n\n");

    while (1) {
        //Captam o tempo
        mytime = time(NULL);
        struct tm tm = *localtime(&mytime);

        printf("%s@%s[%02d:%02d:%02d]:%s $  ", getenv("USER"),getHost(),tm.tm_hour, tm.tm_min, tm.tm_sec, getpath());   
        fgets(entrada, MAX_COMMAND_LENGTH, stdin);

        // Removendo o caractere de nova linha da entrada
        entrada[strcspn(entrada, "\n")] = 0;

        char *token;
        const char s[2] = " ";
        char *args[MAX_COMMAND_LENGTH];
        int contador_args = 0;

        token = strtok(entrada, s);
        args[contador_args] = token;
        contador_args++;
        while (token != NULL)
        {
            token = strtok(NULL, s);
            args[contador_args] = token;
            contador_args++;
        }

        args[contador_args] = NULL;

        

        if (strcmp(entrada, "exit") == 0) {
            my_exit(2);
        } else if (strcmp(entrada, "history") == 0) {
            // Chama histórico de comandos
            show_history(history, history_count);
        } else if (strcmp(args[0], "cd") == 0){
            // Chama o comando interno cd
            chdir(args[1]);
        } else{
            // Executa comando e atualiza histórico
            execute_command(args, history, &history_count);
        }
    }

    return 0;
}
char* getpath(){
    char* c = malloc(sizeof(char) * 100);
    return getcwd(c, 100);
}
char* getHost(){
    char*  h = malloc(sizeof(char) * 50);
    gethostname(h, 50);
    return h;
}

void execute_command(char *args[], char history[][MAX_COMMAND_LENGTH], int *history_count) {
    

    pid_t pid = fork();
    if (pid == 0) {
        // processo filho que executa o comando
        execvp(args[0], args);
        // se nao exisitir o comando erro
        perror(args[0]);
        // e entao sai do processo filho
        exit(1);
    } else if (pid > 0) {
        // Processo pai espera pelo término do processo filho
        wait(NULL);
        // att o histórico de comandos
        if (*history_count < MAX_HISTORY_SIZE) {
            strcpy(history[*history_count], *args);
            (*history_count)++;
        } else {
            for (int i = 0; i < MAX_HISTORY_SIZE - 1; i++) {
                strcpy(history[i], history[i + 1]);
            }
            strcpy(history[MAX_HISTORY_SIZE - 1], *args);
        }
    } else {
        // erro ao criar processo filho
        perror("execvp");
    }
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
