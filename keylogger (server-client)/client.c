#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAXADDRLEN 256
#define BUFLEN 4096

int main(){

    struct sockaddr_in addr_p;
    int sockfd_p, n;
    char request[MAXADDRLEN];
    char buf [50];
    char *ipv  = "server ip";
    int port_s = 8080; //port server

    sockfd_p = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd_p < 0)
        perror("socket error");

    addr_p.sin_family = AF_INET;
    addr_p.sin_port = htons(3128); //squid port

    if(inet_pton(AF_INET, "proxy ip", &addr_p.sin_addr) < 0)
        perror("inet_pton error");

    if(connect(sockfd_p, (struct sockaddr *)&addr_p, sizeof(addr_p)) < 0)
        perror("connect error");

    n = snprintf(request, sizeof(request), "CONNECT %s:%d HTTP/1.1\r\n"
                                                            "Host: %s:%d\r\n"
                                                            "\r\n", ipv, port_s, ipv, port_s);
    if(n < 0)
        perror("sprintf error");

    if(send(sockfd_p, request, strlen(request), 0) < 0)
        perror("send error");
    
    while(1){
        if(recv(sockfd_p, &buf, sizeof(buf), 0) >=0)
            write(STDOUT_FILENO, &buf, strlen(buf));
    }
}
