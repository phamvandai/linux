#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

void error(char *msg)
{
	perror(msg);
	exit(-1);
}


int main(int argc, char *argv[])
{
	if(argc < 4) {
		printf("Usage %s <req_msg> <ack_msg> <nRetry> \n", argv[0]);
		return -1;
	}

	int nRetry = atoi(argv[3]);
	if(nRetry <= 0) {
		nRetry = 1;
	}

	struct sockaddr_in server_addr;
	char buf[BUFSIZE];
	int  sockfd;
	int  server_len = sizeof(server_addr);

	sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(sockfd < 0)
		error("Error creating socket");

	/* Enable broadcast */
	int broadcast_en = 1;
	if(setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast_en,
			sizeof(broadcast_en)) < 0)
	error("Error setsockopt broadcast");

	/* Enable recv timeout */
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	if(setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv,	sizeof(tv)) < 0)
	error("Error setsockopt timeout ");

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8668);
	server_addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);

	int n = 0;
	bool success = false;
	for(n = 0; n < nRetry; n++) {
		printf("Send request (%d)\n", n + 1);
		if(sendto(sockfd, argv[1], strlen(argv[1]), 0,
				(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
			error("Error send datagram");
		}

		printf("Wait for response\n");
		if(recvfrom(sockfd, buf, BUFSIZE, 0, (struct sockaddr *) &server_addr,
				(socklen_t *) &server_len) < 0) {
			continue;
		}
		else {
			if(memcmp(buf, argv[2], strlen(argv[2])) == 0) {
				char *hostaddrp = inet_ntoa(server_addr.sin_addr);
				if(hostaddrp != NULL) {
					printf("Server at %s\n", hostaddrp);
					success = true;
				}
				break;
			}
		}
	}

	if(!success) {
		printf("Error finding\n");
	}

	return 0;
}
