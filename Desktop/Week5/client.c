#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#define LIST "list"
#define GET "get"

int main(int argc, char **argv) 
{
	int sockfd = 0;
	int bytesReceived = 0;
	char recvBuff[256];

	memset(recvBuff, '0', sizeof(recvBuff));	// memset() is used to fill a block of memory with a particular value.

	struct sockaddr_in serv_addr;

		  /* Create a socket first */

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf("\n Error : Could not create socket \n");
	        return 1;
  	}


	  /* Initialize sockaddr_in data structure */

	serv_addr.sin_family = AF_INET;				// A sockaddr_in is a structure containing an internet address. 
								// This structure is defined in <netinet/in.h>.
	serv_addr.sin_port = htons(5050);  			// port
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");	// local host IP address: 127.0.0.1

	  /* Attempt a connection */
	  
	if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) // connect - initiate a connection on a socket.
	{
		puts("\n Error : Connect Failed \n");
		return 1;
	} else {
		puts("Connection established");
	}
	if (strcmp(LIST, argv[1]) == 0)
	{
		// TODO:  Send LIST to the server using the write() system call using the "sockfd" file descriptor
		puts("list request sent ...\n");
		sleep(1);
		write(sockfd,LIST,256); //Sending the LIST signal to the server through sockfd	
		// TODO:  Use read() system call within a loop to read from the server using the "sockfd" file descriptor
		while ((bytesReceived = read(sockfd,recvBuff,256)) > 0) //receiving the list from the server through sockfd and storing it in recvBuff
		{
			printf("%s\n",recvBuff);
			memset(recvBuff, '0', sizeof(recvBuff)); //to reset recvBuff to 0
		}
  	}
	
		/*Create file where data will be stored */

	if (strcmp(GET, argv[1]) == 0)
	{
		// TODO:  Send GET to the server using the "sockfd" file descriptor
		//puts("sending get request\n");
		write(sockfd, GET, 256);
		// TODO:  Send the filename to the server using the "sockfd" file descriptor
		write(sockfd, argv[2], 256);
		// TODO: Create a file with the same name using the open() system call using appropriate flags
		int file = open("temp.txt",O_WRONLY |O_RDONLY | O_CREAT | O_TRUNC);
		/*TODO:  Receive data in chunks of 256 bytes */
    	// TODO:  Use the read() system call within a loop to fetch the contents of the file from the server
		//puts("Getting the file...\n");
		int totalBytes = 0;	
		while ((bytesReceived = read(sockfd, recvBuff, 256)) > 0)
		{
			// TODO:  Use the write() system call to write those contents in the file which was created above.
			write(file, recvBuff, bytesReceived);
			//printf("%d\n",bytesReceived);
			memset(recvBuff, '0', sizeof(recvBuff));

			printf("\nBytes received: %d\n", bytesReceived);
			totalBytes += bytesReceived;
		}
		printf("\nRead Complete\n");
	}
	return 0;
}
