#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 516

#define OP_RRQ		1	/* TFTP op-codes */
#define OP_WRQ 		2
#define OP_DATA		3
#define OP_ACK		4
#define OP_ERROR	5

#define MODE "octet"



int main(int argc, char *argv[]) {
		

    if (argc != 4) {
        fprintf(stderr, "Usage: %s [gettftp|puttftp] host file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // allocation of memory for arguments
    char *command = (char *)malloc(strlen(argv[1]) + 1);
    char *host = (char *)malloc(strlen(argv[2]) + 1);
    char *filename = (char *)malloc(strlen(argv[3]) + 1);
	char   buffer[MAX_BUFFER_SIZE], *p;


    
    // copy of the arguments 
    strcpy(command, argv[1]);
    strcpy(host, argv[2]);
    strcpy(filename, argv[3]);
    
    // Using getaddrinfo to obtain server address
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof hints);
    hints.ai_socktype = SOCK_DGRAM;  // Datagram socket for UDP

	int status = getaddrinfo(host,"1069", &hints, &result);
	//If Error
	if (status != 0) {
		fprintf(stderr, "Error resolving server address: %s\n", gai_strerror(status));
		exit(EXIT_FAILURE);
	}

    // Accessing server address information from result
    struct sockaddr_in *server_address = (struct sockaddr_in *)(result->ai_addr);
	
	// Creating a socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd == -1) {								//error socket message
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }
    

	if (strcmp(command, "gettftp") == 0) {
		
        // Building the RRQ packet
        *(short *)buffer = htons(OP_RRQ);	/* The op-code   */
		p = buffer + 2;
		strcpy(p, filename);				/* The file name */
		p += strlen(filename)+ 1;			/* Keep the nul  */
		strcpy(p, MODE);					/* The Mode      */
		p += strlen(MODE) + 1;				/* Keep the nul again */
		
        
		/* Send Read Request to tftp server */
		ssize_t  count = sendto(sockfd, buffer, p-buffer, 0,
					   (struct sockaddr *)server_address, sizeof(*server_address));
					   
		if (count == -1) {					/* error send request error */
            perror("Error sending request packet");
            exit(EXIT_FAILURE);}
						
		

		/* Receive Data from tftp server */
		while(1){
		
		int blockNumber = 0;
		ssize_t receivedBytes = recvfrom(sockfd, buffer, sizeof(buffer), 0,
                                 NULL, NULL);
                                 
        if (receivedBytes == -1) {			/* error reception of data message */
            perror("Error receiving data packet");
            exit(EXIT_FAILURE);}
            
        //write the file 
		write(1, buffer+4, receivedBytes-4); //+4 and -4 because we don't 
											// take in account the opcode and blocknumber
											
										
		 // Building the ACK packet
        *(short *)buffer = htons(OP_ACK);	/* The op-code   */
		p = buffer + 2;
		*(short *)buffer = htons(blockNumber);
		p = buffer +2;
		
		//send ACK packet
		ssize_t  acquit = sendto(sockfd, buffer, p-buffer, 0,
					   (struct sockaddr *)server_address, sizeof(*server_address));
					   
		if (acquit == -1) {					/* error ack packet send message */
            perror("Error sending ACK packet");
            exit(EXIT_FAILURE);}
            
		blockNumber++;    // for next data packet 
		
		
		break;
		}}

	
		
	else if (strcmp(command, "puttftp")==0){
		
		// Building the WRQ packet
        *(short *)buffer = htons(OP_WRQ);	/* The op-code   */
		p = buffer + 2;
		strcpy(p, filename);				/* The file name */
		p += strlen(filename)+ 1;			/* Keep the nul  */
		strcpy(p, MODE);					/* The Mode      */
		p += strlen(MODE) + 1;				/* Keep the nul again */
		
        
		/* Send Write Request to tftp server */
		ssize_t  count = sendto(sockfd, buffer, p-buffer, 0,
					   (struct sockaddr *)server_address, sizeof(*server_address));
					   
		if (count == -1) {					/* error send request error */
            perror("Error sending write request packet");
            exit(EXIT_FAILURE);}
            
	}
	
	return 0;
	
}
