#ifndef __SOC_H__
#define __SOC_H__
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

int open_tcp_soc(int listen_port) {
    int sd;
    sd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(listen_port);
    if ((bind(sd, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0)
        return -1;
    if (listen(sd, 10) != 0)
        return -1;
    return sd;
}

int send_message(int socket, const char* msg)
{
    int result = send(socket, msg, strlen(msg), 0);
    return result;
}

int receive_message(int socket, char* buf, int len)
{
    char *s = buf;
    int slen = len;
    int c = recv(socket, s, slen, 0);
    if (c < 0)
        return c;
    else if (c == 0)
        buf[0] = '\0';
    else
        s[c]='\0';
    return len - slen;
}

#endif
