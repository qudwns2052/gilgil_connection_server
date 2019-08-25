#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/types.h>
#include <linux/netfilter.h>		/* for NF_ACCEPT */
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <iostream>
#include <cstring>
#include <sys/ioctl.h>
#include <net/if.h>
#include <libnetfilter_queue/libnetfilter_queue.h>

#define BUF_LEN 128

int main(int argc, char ** argv)
{
    char buffer[BUF_LEN];
    struct sockaddr_in server_addr, client_addr;
    char temp[20];
    int server_fd, client_fd;
    //server_fd, client_fd : 각 소켓 번호
    int len;

    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {// 소켓 생성
        printf("Server : Can't open stream socket\n");
        exit(0);
    }
    memset(&server_addr, 0x00, sizeof(server_addr));
    //server_Addr 을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(0xabcd);
    //server_addr 셋팅

    int nSockOpt;

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &nSockOpt, sizeof(nSockOpt));

    if(bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0)
    {//bind() 호출
        printf("Server : Can't bind local address.\n");
        exit(0);
    }

    if(listen(server_fd, 5) < 0)
    {//소켓을 수동 대기모드로 설정
        printf("Server : Can't listening connect.\n");
        exit(0);
    }

    memset(buffer, 0x00, sizeof(buffer));
    printf("Server : wating connection request.\n");
    len = sizeof(client_addr);


    int i = 1;

    while(1)
    {
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, (socklen_t*)&len);

        if(client_fd < 0)
        {
            printf("Server: accept failed.\n");
            exit(0);
        }

        inet_ntop(AF_INET, &client_addr.sin_addr.s_addr, temp, sizeof(temp));
        printf("Server : %s client connected. cnt = %d \n", temp, i++);

    }
    close(client_fd);
    close(server_fd);
}
