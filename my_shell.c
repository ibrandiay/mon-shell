#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <signal.h>
#include<readline/readline.h> 
#include<readline/history.h> 
#define MAXCOM 2000 
#define MAXLIST 200 
  
// gerer l'historique des commande et le prompt custozer 

int historique(char* str) 
{ 
    char* buf; 
  
    buf = readline("\n>>> ");
      if(!buf){
               exit(0);  // CTRL + D
               } 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    }
} 
  
 // affichage du prompt 

void affichage() 
 {   
    char p[20] ;
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd));
    int ii = gethostname(p,100);  
    printf("\n%s@%s:%s$ ",getlogin(),p,cwd); 
} 
  // pour gerer les signal avec CTRL+C
  void sigint(int sint)
 { 

    fprintf(stderr, "SIGINT recu \n");
    kill(getpid(), SIGINT);
    exit(1);
}
// exection des commande  d'un fils 

void execArgs(char** parsed) 
{ 
    
    pid_t pid = fork();  
  
    if (pid == -1) { 
        printf("\n erreur fork "); 
        return; 
    } else if (pid == 0) { 
        signal(SIGINT, sigint);
        if (execvp(parsed[0], parsed) < 0) { 
            printf("\nCould not execute command.."); 
        } 
        exit(0); 
    } else { 
        // waiting for child to terminate 
        wait(NULL);  
        return; 
    } 
} 
  
// execution des tuyau  2 commande seulement 

void execArgsPiped(char** parsed, char** parsedpipe) 
{ 
    
    int pipefd[2];  
    pid_t p1, p2; 
  
    if (pipe(pipefd) < 0) { 
        printf("\nPipe could not be initialized"); 
        return; 
    } 
    p1 = fork(); 
    if (p1 < 0) { 
        printf("\nCould not fork"); 
        return; 
    } 
  
    if (p1 == 0) { 
        // le fils 1 execute  
        // le fils ecrit seUlement
        close(pipefd[0]); 
        dup2(pipefd[1], STDOUT_FILENO); 
        close(pipefd[1]); 
  
        if (execvp(parsed[0], parsed) < 0) { 
            perror("execvp");
            exit(0); 
        } 
    } else { 
        // Parent executing 
        p2 = fork(); 
  
        if (p2 < 0) { 
            printf("\nCould not fork"); 
            return; 
        } 
  
        // Child 2 executing 
        // ecriture 
        if (p2 == 0) { 
            close(pipefd[1]); 
            dup2(pipefd[0], STDIN_FILENO); 
            close(pipefd[0]); 
            if (execvp(parsedpipe[0], parsedpipe) < 0) { 
                printf("\nCould not execute command 2.."); 
                exit(0); 
            } 
        } else { 
            // parent executing,  
            wait(NULL); 
            wait(NULL); 
        } 
    } 
} 
  
  
// les commande et leur execution 

int commande(char** argv) 
{ 
    int N = 4, i, s = 0; 
    char* L[N];  
    char path[400];
                          // Mais princal commande sans compter ceux qui sont sur Path comme ls ..
    L[0] = "exit"; 
    L[1] = "cd"; 
    L[2] = "pwd"; 
    L[3] = "mv"; 
  
    for (i = 0; i < N; i++) { 
        if (strcmp(argv[0], L[i]) == 0) { 
            s = i + 1; 
            break; 
        } 
    } 
  
    switch (s) { 
    case 1: 
           printf("\nGoodbye\n"); 
           exit(0); 
    case 2: 
           chdir(argv[1]); 
           return 1; 
    case 3:  
          // pwd
            getcwd(path, 400);
    
            printf("%s\n", path);
            return 1; 
    case 4: 
           if(argv[1] != NULL)
           return 0;
           else 
            rename(argv[0], argv[1]);
           return 1; 
     default: 
          break; 
    } 
  
    return 0; 
} 
  
// function for finding pipe 
int parsePipe(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, "|"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // si on trove pas on retourn 0
    else { 
        return 1; 
    } 
} 
  
// fonction pour charger argv
void chargement(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int processString(char* str, char** parsed, char** parsedpipe) 
{ 
  
    char* strpiped[2]; 
    int piped = 0; 
  
    piped = parsePipe(str, strpiped); 
  
    if (piped) { 
        chargement(strpiped[0], parsed); 
        chargement(strpiped[1], parsedpipe); 
  
    } else { 
  
        chargement(str, parsed); 
    } 
  
    if (commande(parsed)) 
        return 0; 
    else
        return 1 + piped; 
} 
  
int main() 
{ 
     char taille[MAXCOM], *Argv[MAXLIST]; 
     char* parsedArgsPiped[MAXLIST]; 
     int execFlag = 0; 
     signal(SIGINT,SIG_IGN);
     
    while (1) {   
                 affichage(); 
       
                    if (historique(taille)) 
                    continue; 
       
               execFlag = processString(taille,Argv, parsedArgsPiped); 
             if (execFlag == 1) 
                    execArgs(Argv); 
  
                 if (execFlag == 2) 
                 execArgsPiped(Argv, parsedArgsPiped); 
    } 

    return 0; 
} 