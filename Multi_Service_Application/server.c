#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 2048

void *fortune_cookies_service();
void *encrypt_decrypt_service();

int main()
{
	printf("********** SERVER IS RUNNING ***********\n\n");

	// Creates two threads for two services to be run concurrently.
	pthread_t pid1, pid2;
	pthread_create(&pid1, NULL, fortune_cookies_service, NULL);
	pthread_create(&pid2, NULL, encrypt_decrypt_service, NULL);
	pthread_join(pid1, NULL);
	pthread_join(pid2, NULL);
	return(0);
}

void *fortune_cookies_service()
{
  /* - Description: This function implements the fortune cookies service.
     Client sends the number of cookies to be generated. This service will generate 
     total no. of cookies requested from the 50 fortune cookies and returns back to client. 
     - Arguements: N/A
     - Return values: N/A
  */

	int serv_sockfd, cli_sockfd, client_len, i, j, index_no, cookies_index[50];
	char cookies[50][300], no_of_cookies[2], buff[MAXLEN], str[5];
	struct sockaddr_in sock_addr_serv,sock_addr_cl;
	FILE *fp;
	time_t tloc;

	// Creating socket
	if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket Error\n");
		exit(0);
	}

	bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
	sock_addr_serv.sin_family = AF_INET;
	sock_addr_serv.sin_port = htons(atoi("6001"));
	sock_addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding address to the socket
	if (bind(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv)) < 0)
	{
		close(serv_sockfd);
		perror("Bind Error\n");
		exit(0);
	}

	// Listening the connection
	if (listen(serv_sockfd,5)<0)
	{
		close(serv_sockfd);
		perror("Listen Error\n");
		exit(0);
	}

	// Copying fortune cookies from text file to an array.
	i = 0;
	if((fp = fopen("fortune_cookies.txt","r")) == NULL)
	{
		close(serv_sockfd);
		perror("Error in opening a file.\n");
		exit(0);
	}	

	while(fgets(buff,MAXLEN,fp)!= NULL)
	{
		strcpy(cookies[i],buff);
		i++;
		bzero((char *)&buff,sizeof(buff));
	}

	fclose(fp);
	// Initializig an array
	for(i = 0; i < 50 ; i++)
		cookies_index[i] = 1;

	for(;;)
	{  
		client_len = sizeof(sock_addr_cl);

		// Accepting the client connection
		if ((cli_sockfd = accept(serv_sockfd,(struct sockaddr *)&sock_addr_cl,&client_len))<0)
		{
			close(serv_sockfd);
			perror("Accept Error\n");
			exit(0);
		}  

		bzero((char *)&no_of_cookies,sizeof(no_of_cookies));
		if (read(cli_sockfd,no_of_cookies, 1) < 0)
		{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
		}

		bzero((char *)&buff,sizeof(buff));

		// Randomly picks up the fortune cookies and copying it into 'buff' array.
		srand(time(&tloc));
		for(i = 0; i < atol(no_of_cookies); i++)
		{
			do
			{
				index_no = rand() % 50;
			}
			while(cookies_index[index_no] == 0);

			sprintf(str," %d. ", i+1);
			strcat(buff, str);
			strcat(buff, cookies[index_no]);
			cookies_index[index_no] = 0;
		}
		buff[MAXLEN] = '\0';
		if(write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
		{
			close(cli_sockfd);
			perror("Write Error\n");
			exit(0);
		}
		close(cli_sockfd);
	}
}

void *encrypt_decrypt_service()
{
  /* - Description: This function implements the encryption and decryption service.
     Client sends the message to be encrypted or decrypted, this service will encrypt/
     decrypt the message and sends the result to the client.
     - Arguements: N/A
     - Return values: N/A
  */

	int serv_sockfd, cli_sockfd, client_len, mesg_len, i;
	char en_dec[1], mesg[MAXLEN], no_of_cookies[MAXLEN],temp;
	struct sockaddr_in sock_addr_serv,sock_addr_cl;

	// Creates Socket
	if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("Socket Error\n");
		exit(0);
	}

	bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
	sock_addr_serv.sin_family = AF_INET;
	sock_addr_serv.sin_port = htons(atoi("6002"));
	sock_addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding the address to the socket
	if (bind(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv)) < 0)
	{
		close(serv_sockfd);
		perror("Bind Error\n");
		exit(0);
	}

	// Listening the connection
	if (listen(serv_sockfd,5)<0)
	{
		close(serv_sockfd);
		perror("Listen Error\n");
		exit(0);
	}

	for(;;)
	{
		client_len = sizeof(sock_addr_cl);

		// Accepting the client request.
		if ((cli_sockfd = accept(serv_sockfd,(struct sockaddr *)&sock_addr_cl,&client_len))<0)
		{
			close(serv_sockfd);
			perror("Accept Error\n");
			exit(0);
		}
		// Following code will implement concurrent server
		if(fork()==0)
		{
			close(serv_sockfd);
			
			for(;;)
			{
				bzero((char *)&en_dec,sizeof(en_dec));
				if (read(cli_sockfd, en_dec, 1) < 0)
				{
					close(cli_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				bzero((char *)&mesg,sizeof(mesg));
				if (read(cli_sockfd, mesg, sizeof(mesg)) < 0)
				{
					close(cli_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				switch(atol(en_dec))
				{
					// Encryption Algorithm
					case 1:
						mesg_len = strlen(mesg);

						// Adding 5
						for(i = 0; i < mesg_len; i++)
							mesg[i] = (mesg[i] + 5) % 128;

						mesg[mesg_len] = '\0';
						if( write(cli_sockfd, mesg, sizeof(mesg)) != sizeof(mesg))
						{
							close(cli_sockfd);
							perror("Write Error\n");
							exit(0);
						}
						break;
					// Decryption Algorithm
					case 2:
						mesg_len = strlen(mesg);
						
						// Substracting 5
						for(i = 0; i < mesg_len; i++)
							mesg[i] = (mesg[i] - 5) % 128;

						mesg[mesg_len] = '\0';
						if( write(cli_sockfd, mesg, sizeof(mesg)) != sizeof(mesg))
						{
							close(cli_sockfd);
							perror("Write Error\n");
							exit(0);
						}
						break;
					case 3:
						close(cli_sockfd);
						exit(0);
				}
			}
		}
		close(cli_sockfd);
	}
}
