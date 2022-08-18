#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include <unistd.h>
#include <iostream>

int main() {
    //$ 1
    // system("roscore");

    //$ 2
    // FILE* fp;
    // char buffer[65536];
    // fp = popen("roscore","r");
    // fgets(buffer,sizeof(buffer),fp);
    // printf("%s",buffer);
    // pclose(fp);

    //%2-2
    FILE *fp = NULL;
    char *buff = NULL;

    buff = (char*)malloc(80);
    if(buff == NULL) {
        perror("malloc:");
        return -1;
    }
    memset(buff,0,80);
    fp = popen("nohup roscore >log.file 2>&1 &","r");
    if(fp == NULL) {
        perror("pop error: ");
        free(buff);
        return -1;
    }

    fgets(buff,80,fp);
    std::cout<<buff<<std::endl;
    pclose(fp);
    free(buff);

    //$ 3
    // pid_t fpid;
    // char* order[]={"date"};
    // fpid = vfork();
    // if(fpid<0)
    // {
    //     perror("vfork");
    //     return -1;
    // }
    // if(fpid == 0)
    // {
    //     execv("/bin/date",order);
    // }else
    // {

    // }

    return 0;
}