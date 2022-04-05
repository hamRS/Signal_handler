#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
/*structure*/
struct plan{
    int proces_id;
    int signal_to_send;
    int delay;
};
/*end of structure*/

/*prototypes*/
int gen_sig_num();
int gen_delay_num();
void signal_handler(int sig);
void modify_sigaction(struct sigaction * sa);
/*end of prototypes*/

int main(int  argc , char * argv[]){
    if(argc != 2){
        fprintf(stderr , "Usage : %s <number of processes>\n" , argv[0]);
        return 1;
    }
    srand(time(0));
    int n = atoi(argv[1]); // the number of processes
    pid_t childPID;
    int i;
    struct plan tmp;
    /*open file to write planning*/
    FILE * ptrfile;
    int count = 0;
    struct sigaction sa;
    modify_sigaction(&sa);
    /*fork program and write processes*/
    for(i = 0 ; i <= n ; i++){ //create a fan of processes
        childPID = fork(); //make a fork from parent
        if(childPID == -1){
            perror("Failed to fork!");
            return 1;
        }else if(childPID == 0){ // from the child proces
            break;
        }else {
            if(count == 0){
                if((ptrfile = fopen("list.txt", "w")) == NULL){
                    fprintf(stderr , "Could not open the file");
                    return 1;
                }
                count = 1;
            }
            tmp.proces_id = childPID;
            tmp.signal_to_send = gen_sig_num();
            tmp.delay = gen_delay_num();
            fwrite(&tmp , sizeof(struct plan) , 1 , ptrfile);
            printf("write to file %d, %d, %d\n" , tmp.proces_id , tmp.signal_to_send
            , tmp.delay);
        }
    }
    //from the child make the call to alarm
    if(childPID == 0){
        for(; ; ){
            sleep(rand() % (10 - 3 + 1) + 3);
            printf("beep! [%d]\n" , getpid());
        }
    }else{ 
        tmp.proces_id = getpid();
        tmp.signal_to_send = SIGKILL;
        tmp.delay = gen_delay_num();
        fwrite(&tmp , sizeof(struct plan) , 1 , ptrfile);
        fclose(ptrfile);
        pid_t t = wait(NULL);
        while(t > 0)
            t = wait(NULL);
    }
    return 0;
}


/*generate signal number*/
int gen_sig_num(){
    int lower = 1;
    int  upper = 31;
    return rand() % (upper - lower + 1) + lower;
}
/*end of generate signal number*/

/*generate delay number */
int gen_delay_num(){
    int lower = 2;
    int upper = 30;
    return rand() % (upper - lower + 1) + lower;
}
/*end of generate delay number*/


/*signal handler*/
void signal_handler(int sig){
    printf("P:[%d] signal [%d] recieved \n" , getpid() , sig);
    signal(SIGKILL , SIG_DFL);
    raise(SIGKILL);
}
/*end of signal handler*/



/*modify handler with sigaction*/
void modify_sigaction(struct sigaction * sa){
    struct sigaction tmp = *sa;
    tmp.sa_handler = &signal_handler;
    tmp.sa_flags = SA_RESTART;
    int i = 0;
    for(int i = 1 ; i < 31; i++)
        sigaction(i , &tmp , NULL);
    
}
/*end of modify handlre with sigaction*/