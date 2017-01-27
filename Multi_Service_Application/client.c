#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 2094

void fortune_cookies_service(char *argv);
void encrypt_decrypt_service(char *argv);

int main(int argc, char *argv[])
{
	printf("************ WELCOME ************\n\n");
  	char serv[2];
  	while(1)
    	{ 
      		//Asks User for the options to select service
  		bzero((char *)&serv, sizeof(serv));
    
	      printf("Select the services:\nEnter 1 for fortune cookies\nEnter 2 for Encrption/decryprion\nEnter 3 for quit:\n");
	      scanf("%s", serv); 
	      //fgets(serv,2,stdin);     
		switch(atol(serv))
		{
			case 1:
		  		fortune_cookies_service(argv[1]);
		  		break;
			case 2:
			  	encrypt_decrypt_service(argv[1]);  
			  	break;
			case 3:
			 	 printf("************ THANK YOU ************\n");
			  	return(0);
			default:
			  	printf("Please Enter the Correct option.\n");
			  	break;
		}
	}
}
 
void fortune_cookies_service(char *argv)
 {
   /* Description: This function will connect to the fortune cookies service.
      This function will ask client to enter the number of cookies, sends the request to server
      and prints the result.
      Arguements: N/A
      Return value: N/A
   */

	int serv_sockfd, no_of_cookies,ret;
	char buff[MAXLEN];
	struct sockaddr_in sock_addr_serv;

	printf("How many cookies do you want to generate?\nMaximum Nine Fortune Cookies are allowed.\n");
	scanf("%s",buff);

	if(atol(buff) > 9)
	{
		printf("Maximum Nine cookies are allowed. Please Enter correct input.\n\n");
		return;
	}
	if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket Error\n");
		exit(0);
	}

	bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
	ret = inet_aton(argv, &sock_addr_serv.sin_addr.s_addr);
	sock_addr_serv.sin_family = AF_INET;
	sock_addr_serv.sin_port = htons(atoi("6001"));

	// Connecting to the Server
	if(connect(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv))< 0)
	{
		close(serv_sockfd);
		perror("Connect Error\n");
		exit(0);
	}
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
	{
		close(serv_sockfd);
		perror("Write Error\n");
		exit(0);
	}
	bzero((char *)&buff, sizeof(buff));
	if(read(serv_sockfd, buff, sizeof(buff)) < 0)
	{
		close(serv_sockfd);
		perror("Read Error\n");
		exit(0);
	}
	printf("Fortune Cookie(s):\n%s\n\n",buff);
	close(serv_sockfd);
}

void encrypt_decrypt_service(char *argv)
{
   /* Description: This function will connect to the encryption and decryption service.
      This function will ask user to enter the message to be encrypted or decrypted, then sends the request to server
      and prints the message.
      Arguements: N/A
      Return value: N/A
   */

	int serv_sockfd, rd_value,ret;
	char en_dec[2], mesg[MAXLEN];
	struct sockaddr_in sock_addr_serv;
	struct in_addr inaddr;

	// Creating socket
	if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		perror("Socket Error\n");
		exit(0);
	}

	bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
	ret = inet_aton(argv, &sock_addr_serv.sin_addr.s_addr);
	sock_addr_serv.sin_family = AF_INET;
	sock_addr_serv.sin_port = htons(atoi("6002"));

	// Connecting to the Server
	if(connect(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv))< 0)
	{
		close(serv_sockfd);
		perror("Connect Error\n");
		exit(0);
	}
	for(;;)
	{
		bzero((char *)&en_dec,sizeof(en_dec));

		//Asks user to select Encryption/Decryption Mode.
		printf("Enter 1 for Encryption\nEnter 2 for Decryption\nEnter 3 to Stop\n");
		scanf("%s",en_dec);

		if (write(serv_sockfd, en_dec, 1) != 1)
		{
			close(serv_sockfd);
			perror("Write Error\n");
			exit(0);
		}

		switch(atol(en_dec))
		{
			// Encryption Mode
			case 1:
				bzero((char *)&mesg,sizeof(mesg));
				printf("Enter the message you want to Encrypt.\n");
				fgetc(stdin);
				fgets(mesg,MAXLEN,stdin); 

				if(write(serv_sockfd,mesg,sizeof(mesg)) != sizeof(mesg))
				{
					close(serv_sockfd);
					perror("Write Error\n");
					exit(0);
				}

				bzero((char *)&mesg,sizeof(mesg));
				if(read(serv_sockfd,mesg,sizeof(mesg)) < 0)
				{
					close(serv_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				printf("Encrypted message:%s\n\n",mesg);
				break;

			// Decryption Mode
			case 2:
				bzero((char *)&mesg,sizeof(mesg));
				printf("Enter the Message you want to Decrypt\n");
				fgetc(stdin);
				fgets(mesg,MAXLEN,stdin);
				if(write(serv_sockfd,mesg,sizeof(mesg)) != sizeof(mesg))
				{
					close(serv_sockfd);
					perror("Write Error\n");
					exit(0);
				}

				bzero((char *)&mesg, sizeof(mesg));
				if(read(serv_sockfd,mesg,sizeof(mesg)) < 0)
				{
					close(serv_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				printf("Decrypted Message: %s\n\n", mesg);
				break;
			case 3:
				printf("************ RETURNING BACK TO THE MAIN MENU ************\n");
				close(serv_sockfd);
				return;
			default:
				printf("Please Enter the Correct Option.\n");
				break;
		}
	}
}
