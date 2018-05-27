/*
 * udpserver.c
 *
 *  Created on: Dec 9, 2017
 *      Author: joedalton
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define BUFSIZE 1024

FILE *logfile;
char req_msg[] = "pi_addr";
char answer[]  = "ACK";

void error(char *msg)
{
	perror(msg);
	fclose(logfile);
	exit(-1);
}


int main(int argc, char *argv[])
{
	int sockfd;		/* Socket */
	int portno = 8668;		/* Port */
	int client_len;	/* Size of client 's address*/
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	char buf[BUFSIZE];
	char *hostaddrp; /* Dotted decimal host addr string */
	int optval; 	/* Flag value for setsockopt */
	int n; 			/* Message byte size */
	time_t t;
	struct tm tm;

	logfile = fopen("./myip.log", "a");
	if(logfile == NULL)
		error("Error open logfile\n");

	/* Create the parent socket */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(sockfd < 0)
		error("Error openning socket");

	/* Set socket option */
	optval = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
			   (const void *) &optval, sizeof(int));

	bzero((char*) &server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons((unsigned short) portno);

	if(bind(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0){
		error("Error binding");
	}

	printf("Start listening at port %d\n", portno);
	t = time(NULL);
	tm = *localtime(&t);
	fprintf(logfile, "%d:%d:%d - %d:%d:%d Start listening\n",
			 tm.tm_mday, tm.tm_mon + 1, 1900 + tm.tm_year,
			 tm.tm_hour, tm.tm_min, tm.tm_sec);
	fflush(logfile);

	client_len = sizeof(client_addr);
	while(1) {
		/* Receive a UDP datagram from a client */
		bzero(buf, BUFSIZE);
		n = recvfrom(sockfd, buf, BUFSIZE, 0,
					(struct sockaddr *) &client_addr, (socklen_t*) &client_len);

		if(n < 0) {
			error("Error recvfrom");
		}

		hostaddrp = inet_ntoa(client_addr.sin_addr);
		if(hostaddrp == NULL)
			error("Error inet_ntoa");

		if(memcmp(buf, req_msg, strlen(req_msg)) == 0) {
			t = time(NULL);
			tm = *localtime(&t);
			fprintf(logfile, "%d:%d:%d recv request from %s\n",
					tm.tm_hour, tm.tm_min, tm.tm_sec, hostaddrp);
			fflush(logfile);

			/* Reply */
			n = sendto(sockfd, answer, strlen(answer), 0,
						(struct sockaddr *) &client_addr, client_len);
			if(n < 0)
				error("Error sendto");
		}
		else {
			fprintf(logfile, "Invalid request %s\n", buf);
			fflush(logfile);
		}
	}
}
