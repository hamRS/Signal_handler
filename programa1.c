#include<unistd.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdlib.h>
#include<string.h>


struct plan{
    int proces_id;
    int signal_to_send;
    int delay;
};


/*prototypes*/
void send_signals(FILE * ptrfile);
/*end of prototypes*/
int main(){
    
    
    /*open file for reading*/
    FILE * ptrfile;
    if((ptrfile = fopen("list.txt" , "r") )== NULL){
        fprintf(stderr, "\nNo existe el archivo");
        return 1;
    }else{
        printf("File openned!\n");
    }
    send_signals(ptrfile);
    return 0;
}

/*send signals*/
void send_signals(FILE * ptrfile){
    struct plan tmp;
    printf("hello people\n");
    while(fread(&tmp , sizeof(struct plan) , 1 , ptrfile)){
        printf("PID: %d , SIG: %d, DELAY: %d\n" , tmp.proces_id,
        tmp.signal_to_send , tmp.delay);
        sleep(tmp.delay);
        kill(tmp.proces_id, tmp.signal_to_send);
    }
}
/*end of send signals*/



