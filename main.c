#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "soc.h"

#define LISTEN_PORT 8080
#define REQUEST_SIZE 1024

int server;

char* handle_request(const char* msg);
void signal_handler(int sig);

int main(int argc, char const *argv[])
{
	signal(SIGINT,signal_handler);

	printf("server is listening on port %d\n", LISTEN_PORT);
    server = open_tcp_soc(LISTEN_PORT);
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect;
    char* message = (char*)malloc(REQUEST_SIZE);
    pid_t pid;
    while(1)
    {
        connect = accept(server,(struct sockaddr *)&client_addr,&address_size);
        struct sockaddr_in *coming = (struct sockaddr_in *)&client_addr;
        unsigned char *ip = (unsigned char *)&coming->sin_addr.s_addr;
        unsigned short port = coming->sin_port;
        if (connect < 0)
        {	
        	// can't accept connect from client
        	printf("can not connect with %d.%d.%d.%d:%d\n", ip[0], ip[1], ip[2], ip[3], port);
            continue;
        }
        printf("connected with %d.%d.%d.%d:%d\n", ip[0], ip[1], ip[2], ip[3], port);

        pid_first = fork();
        if (pid_first == 0)
        {
        	// clear signal handler in child process
            signal(SIGINT, SIG_DFL);

            pid_t pid_first;
			pid_t pid_second;
            pid_second = fork();
            if (pid_second == 0)
            {
            	signal(SIGINT, SIG_DFL);
            	receive_message(connect, message, REQUEST_SIZE);
            	send_message(connect, handle_request(message));
            	close(connect);
            }
            close(connect);
            exit(0);
        }
        wait(NULL);
        close(connect);
    }
}

void signal_handler(int sig)
{
    printf("\nserver exited\n");
    close(server);
    exit(0);
}

char* handle_request(const char* msg) {
	char* response_text = (char*)malloc(REQUEST_SIZE);
	sprintf(response_text, "HTTP/1.1 200 OK\n\nHello World\n");
	return response_text;
}
