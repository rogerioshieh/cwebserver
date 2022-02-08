#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include "modules/unix/x86_64-linux-gnu/socket/unix_x86_64_linux_tcp_socket.h"
#include "modules/utilities/utils.h"
#include "controller/cwes_controller.h"

#define PORT 8000

void signal_handler(int sig);

int server;
pid_t pid_first;
pid_t pid_second;

int main(int argc, char const *argv[])
{
	signal(SIGINT,signal_handler);
    server = unix_x86_64_linux_tcp_socket_open_listener(PORT);
    _("%s - [INFO] Server running on 0::%d\n", time_get_timestamp(), PORT);
    struct sockaddr_storage client_addr;
    unsigned int address_size = sizeof(client_addr);
    int connect;
    char* message = (char*)malloc(__SIZE_EXTRA__);
    pid_t pid;
    while(1)
    {
        connect = accept(server,(struct sockaddr *)&client_addr,&address_size);
        struct sockaddr_in *coming = (struct sockaddr_in *)&client_addr;
        unsigned char *ip = (unsigned char *)&coming->sin_addr.s_addr;
        unsigned short port = coming->sin_port;
        if (connect < 0)
        {
            /* Can't accept connect from client */
            _("%s - [ERROR] Can't connect with %d.%d.%d.%d:%d\n",
                   time_get_timestamp(),
                   ip[0], ip[1], ip[2], ip[3], port);
            continue;
        }
        _("%s - [INFO] Connect with %d.%d.%d.%d:%d\n",
               time_get_timestamp(),
               ip[0], ip[1], ip[2], ip[3], port);

        pid_first = fork();
        if (pid_first == 0)
        {
            signal(SIGINT, SIG_DFL);
            pid_second = fork();
            if (pid_second == 0)
            {
            	signal(SIGINT, SIG_DFL);
            	unix_x86_64_linux_tcp_socket_read_msg(connect, message, __SIZE_EXTRA__);
            	unix_x86_64_linux_tcp_socket_send_msg(connect, controller_control_everything(message));
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
    _("\n%s - [INFO] Server exit\n", time_get_timestamp());
    close(server);
    exit(0);
}
