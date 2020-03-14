

/*this program is for signal handler
  send the signal from the terminal
  command can be: killall -SIGTERM sig
                  Killall -SIGTSTP sig
                  killall -SIGINT sig
*/
#include <stdio.h>
#include <signal.h>
#include <sys/signal.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*global variable**/
const int _BUFFER = 1024;
char buffer[_BUFFER];
char *delim = "++";
clock_t start;
int tick = 0;
int global_count;

/*
 signal handler- handle each recived signal
*/
void sig_handler(int signum){
    
    /*signal alarm for tick*/
    if(signum == SIGALRM) {
        tick = tick + 10;
        printf("tick %d ...\n", tick);
        alarm(10);
    }
    
    /*signal int handler interrrupt and continue running program*/
    /*can use the keyboard: control + c*/
    if(signum == SIGINT){
        clock_t end_int;
        double cpu_time_int;
        end_int = clock();
        cpu_time_int = ((double)(end_int - start))/CLOCKS_PER_SEC;
        printf("Time spend = %f seconds\n", cpu_time_int);
    }
    
    /*signal term handler print the time and exit the program*/
   if(signum == SIGTERM) {
        clock_t end_term;
        double cpu_time_term;
        end_term = clock();
        cpu_time_term = ((double)(end_term - start))/CLOCKS_PER_SEC;
        printf("Time spend = %f seconds\n", cpu_time_term);
        exit(0);
        
    }
    
    /*signal tstp handler print the last 10 lines user input*/
    /*can use keyboard: cotrol + z*/
    if(signum == SIGTSTP) {
          //seperate the user input with the "++" in buffer
          int i=0;
          char *p = strtok(buffer, "++");
          char *array[global_count];
          while(p!=NULL){
              array[i++] = p;
              p=strtok(NULL, "++");
          }
        //if the user input less than 10 lines, directly print
        if(global_count<10){
              for(i=global_count-1; i>=0; i--){
              printf("\nUser input No.%d : %s\n", i+1, array[i]);
          }
        }else{ //otherwise start count last 10 lines.
            for(i=global_count-1; i>=global_count-10; i--){
                printf("\nUser input No.%d : %s\n", i+1, array[i]);
            }
        }
       signal(SIGTSTP, SIG_DFL);
    }
}


/*
 main function
*/
int main(int argc, char* argv[]){
    /*start clock start here*/
    start = clock();
    
    /*signal*/
    signal(SIGINT, sig_handler);
    signal(SIGTERM, sig_handler);
    signal(SIGTSTP, sig_handler);
    signal(SIGALRM, sig_handler);
    printf("tick 0 ...\n");
    alarm(10);

    /*program keep running. wait signal*/
    while(1){
        
       scanf("%s", argv[0]);
       printf("%s\n", argv[0]);
        
        /*put the each time input in the one string*/
        if(global_count==0){
            strcpy(buffer, argv[0]);
            strcat(buffer, delim);
        }else{
            strcat(buffer, argv[0]);
            strcat(buffer, delim);
        }
       // printf("BUFFER =%s \n", buffer);
        
        /*global count for calculate how many user input*/
        global_count++;
    }
    return 0;
}
