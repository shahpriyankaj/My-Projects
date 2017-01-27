#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#define MAXLEN 2048*5
#define MAX_NAME_SZ 256

// Mail Structure
struct mail{
	int sqno;
	int read;
	char sendername[50];
	char sfname[50];
	char slname[50];
	char dateTime[50];
	char subject[100];
	char message[300];
	int isAttachment;
	char attFileName[50];
	char attachment[300];
};

// Calendar Structure
struct calendar{
	int event_id;
    int sqno;
    int read;
    char sendername[50]; // Username of the Sender
    char sfname[50];    //First name of the sender
    char slname[50];    // Last name of the sender
    char start_date[50]; // Start Date and time of Event
	char start_time[50];
	char end_date[50]; // End Date and time of Event
	char end_time[50];
	char place[300];	// Place of an event
    char subject[100];	
    char message[300];	
    int isAttachment;
    char attFileName[50];
    char attachment[200];
	int no_of_participants;
	char participants_list[50][50]; // List of all participants who are involved in an event.
};

// Array of mail and calendar structure.
struct mail mail_list[50];
struct calendar cal_list[50];

int serv_sockfd, USER_LOGOUT = 0, NEW_CALS = 0, NEW_MAILS = 0;//, check_mail = 1;
char buff[MAXLEN];
struct sockaddr_in sock_addr_serv;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void mail_background(char username[])
{	
	/* This function will check any new mails at the server side, if new mails are there, it will add it into mail array. */
	//printf("In mail Background\n");
	
	int i, j, k, client_count = 0, server_count = 0, mail_count = 0;
	char cmd[4],*mail_line,*token,str_client_count[8],buff1[MAXLEN];
		
	client_count = 0;
	server_count = 0;
	
	pthread_mutex_lock(&mutex);
	//printf("mail after lock\n");
	if (USER_LOGOUT == 1)
	{
		//check_mail == 1;
		//pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		exit(0);
	}
	for(i = 0; i < 100; i++)
	{
		if(mail_list[i].sqno == 0)
			break;
		client_count++;
		//printf("client count:%d\n", client_count);
	}
	pthread_mutex_unlock(&mutex);
	bzero((char *)&buff1, sizeof(buff1));
	//bzero((char *)&mail_line, sizeof(mail_line));
	strcpy(buff1,"CHK|");
	sprintf(str_client_count,"%d",client_count);
	strcat(buff1,str_client_count);
	strcat(buff1,"|");
	strcat(buff1,username);
	strcat(buff1,"|");
	//printf("buff:%s\n", buff);
	if (write(serv_sockfd, buff1, sizeof(buff1)) != sizeof(buff1))
		{
			 close(serv_sockfd);
			 perror("Write Er+ror\n");
			 exit(0);
		}
	bzero((char *)&buff1, sizeof(buff1));
	//printf("buff:%s\n", buff);
	if(read(serv_sockfd, buff1, sizeof(buff1)) < 0)
		{
			 close(serv_sockfd);
			 perror("Read Error\n");
			 exit(0);
		}
	//printf("buff:%s\n",buff);
	mail_line = strtok(buff1,"|");
	strcpy(cmd, mail_line);
	if(strcmp(cmd,"CHK")==0)
		{
			mail_line = strtok(NULL,"|");
			server_count= atoi(mail_line);
			//for(k = client_count  ; k < server_count ; k++)
			//{
			pthread_mutex_lock(&mutex);
				
			if(client_count < server_count )
			{
				NEW_MAILS = server_count - client_count;
				k = client_count;
				mail_line= strtok(NULL,"|~");
				while(mail_line != NULL && k < server_count)
				{
					//printf("inside CHK\n");
					mail_list[k].sqno = atoi(mail_line);
					mail_line = strtok(NULL,"|~");
					mail_list[k].read = atoi(mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].sendername, mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].sfname, mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].slname, mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].dateTime, mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].subject, mail_line);
					mail_line = strtok(NULL,"|~");
					strcpy(mail_list[k].message, mail_line);
					mail_line = strtok(NULL,"|~");
					mail_list[k].isAttachment = atoi(mail_line);
					if (mail_list[k].isAttachment == 1)
					{
						mail_line = strtok(NULL,"|~");
						strcpy(mail_list[k].attFileName, mail_line);
						mail_line = strtok(NULL,"|~");
						strcpy(mail_list[k].attachment, mail_line);
					}
					mail_line = strtok(NULL,"|~");
					k++;
					
				}
			
			//}
					/*mail_count = k;
					printf("mail_count: %d\n",mail_count);
					for(j = 0; j < mail_count; j++)
					{
						printf("%d.\nSender: %s\nFirst Name: %s\nLast Name: %s\nSubject: %s\nDateTime of the Message received: %s\n\n",
									mail_list[j].sqno,
									mail_list[j].sendername,
									mail_list[j].sfname,
									mail_list[j].slname,
									mail_list[j].subject,
									mail_list[j].dateTime);				
						
					}*/
			}
			pthread_mutex_unlock(&mutex);
				/*else
				{
					printf("No extra data\n");
				}*/
	}
	
	//check_mail = 0;
	//pthread_cond_signal(&cond);
	//pthread_mutex_unlock(&mutex);
	//printf("out mail Background\n");
	
}

void cal_background(char username[])
{	
	/* This function will check any new calendar events at the server side, if new mails are there, it will add it into calendar array. */
	//printf("In calendar Background\n");
	
	int i = 0, j, k, client_count = 0, server_count = 0, cal_count = 0;
	char cmd[4],*cal_line,*token,str_client_count[8], str_no_of_part[8], str_sqno[8], buff1[MAXLEN];
	
		
	client_count = 0;
	server_count = 0;
	bzero((char *)&cal_line,sizeof(cal_line));
	pthread_mutex_lock(&mutex);
	//printf("In calendar Background after lock\n");
	if (USER_LOGOUT == 1)
	{
		//check_mail == 1;
		//pthread_cond_signal(&cond);
		pthread_mutex_unlock(&mutex);
		exit(0);
	}
	do
	{
	// Checking participants list
		//for(i = 0; i < 100; i++)
		//{
			
			/*if(cal_list[i].sqno == 0)
			{
				
				break;
			}*/
			//printf("In calendar Background inside do\n");				
		bzero((char *)&buff1, sizeof(buff1));
		strcpy(buff1,"CHP|");
		sprintf(str_no_of_part,"%d",cal_list[i].no_of_participants);
		strcat(buff1,str_no_of_part);
		strcat(buff1,"|");
		sprintf(str_sqno,"%d",cal_list[i].sqno);
		strcat(buff1,str_sqno);
		strcat(buff1,"|");
		strcat(buff1,username);
		strcat(buff1,"|");
		//printf("buff:%s\n", buff);
		if (write(serv_sockfd, buff1, sizeof(buff1)) != sizeof(buff1))
		{
			 close(serv_sockfd);
			 perror("Write Error\n");
			 exit(0);
		}
		bzero((char *)&buff1, sizeof(buff1));

		if(read(serv_sockfd, buff1, sizeof(buff1)) < 0)
		{
			 close(serv_sockfd);
			 perror("Read Error\n");
			 exit(0);
		}
		//printf("buff:%s\n", buff);
		cal_line = strtok(buff1,"|");
		strcpy(cmd, cal_line);
		if(strcmp(cmd,"CHP") == 0)
		{
			cal_line = strtok(NULL,"|~");
			//printf("Returned value:%s\n", cal_line);
			//token = strtok(NULL,"|");
			if(atoi(cal_line) != 0)
			{
				cal_list[k].no_of_participants = atoi(cal_line);
				for(i = 0; i <= cal_list[k].no_of_participants; i++)
				{
					cal_line = strtok(NULL,"|");
					strcpy(cal_list[k].participants_list[i], cal_line);
				}
			}
		}
		i++;
		//}
	}while(cal_list[i].sqno != 0);
		
	//pthread_mutex_unlock(&mutex);
	//pthread_mutex_lock(&mutex);
	//printf("aftr participiants\n");
	for(i = 0; i < 100; i++)
	{
		if(cal_list[i].sqno == 0)
			break;
		client_count++;
		//printf("client count:%d\n", client_count);
	}
	//pthread_mutex_unlock(&mutex);
	bzero((char *)&buff1, sizeof(buff1));
	//bzero((char *)&cal_line, sizeof(cal_line));
	strcpy(buff1,"CHC|");
	sprintf(str_client_count,"%d",client_count);
	strcat(buff1,str_client_count);
	
	strcat(buff1,"|");
	strcat(buff1,username);
	//strcat(buff,"|");
	//strcat(buff,cal_list);
	strcat(buff1,"|");
	//printf("buff:%s\n",buff1);
	if (write(serv_sockfd, buff1, sizeof(buff1)) != sizeof(buff1))
		{
			 close(serv_sockfd);
			 perror("Write Error\n");
			 exit(0);
		}
	bzero((char *)&buff1, sizeof(buff1));
	
	if(read(serv_sockfd, buff1, sizeof(buff1)) < 0)
		{
			 close(serv_sockfd);
			 perror("Read Error\n");
			 exit(0);
		}
	//printf("buff:%s\n",buff1);
	cal_line = strtok(buff1,"|");
	strcpy(cmd, cal_line);
	if(strcmp(cmd,"CHC")==0)
	{
		token = strtok(NULL,"|");
		server_count= atoi(token);
		//for(k = client_count  ; k < server_count ; k++)
		//{
		//pthread_mutex_lock(&mutex);
		if(client_count < server_count )
		{
			//printf("insede if\n");
			NEW_CALS = server_count - client_count;
			k = client_count;
			cal_line= strtok(NULL,"|~");
			while(cal_line != NULL && k < server_count)
			{
				//printf("inside CHK\n");
				cal_list[k].event_id = atoi(cal_line);
				cal_line = strtok(NULL,"|~");
				cal_list[k].sqno = atoi(cal_line);
				cal_line = strtok(NULL,"|~");
				cal_list[k].read = atoi(cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].sendername, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].sfname, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].slname, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].start_date, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].start_time, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].end_date, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].end_time, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].place, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].subject, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[k].message, cal_line);
				cal_line = strtok(NULL,"|~");
				cal_list[k].isAttachment = atoi(cal_line);
				if (cal_list[k].isAttachment == 1)
				{
					cal_line = strtok(NULL,"|~");
					strcpy(cal_list[i].attFileName, cal_line);
					cal_line = strtok(NULL,"|~");
					strcpy(cal_list[i].attachment, cal_line);
				}
				cal_line = strtok(NULL,"|~");
				cal_list[k].no_of_participants = atoi(cal_line);
				for(i = 0; i <= cal_list[k].no_of_participants; i++)
				{
					cal_line = strtok(NULL,"|~");
					strcpy(cal_list[k].participants_list[i], cal_line);
				}
				cal_line = strtok(NULL,"|~");
				k++;
				
			}
		
		}
		
	}
	pthread_mutex_unlock(&mutex);
	
}

void *backgroundthread(void *username)
{	
	/* This is the backgroundt thread which will check for new calendar and mails. */
	//printf("In Background Thread\n");
	
	//int i, j, k, client_count = 0, server_count = 0, mail_count = 0;
	//char cmd[4],*mail_line,*token,str_client_count[8];
	for(;;)
	{
		sleep(1);
		// Calling mail function
		mail_background(username);
		sleep(1);
		// Calling calendar function
		cal_background(username);
	}
		
}

void retrieve_mails(char username[])
{
	/* This function will retireve mails from the server side when user logs in and store it in mail array. */
	char cmd[4], *mail_line;
  	int i = 0, j = 0, mail_count = 0;
  	
	bzero((char *)&buff, sizeof(buff));
	strcpy(buff,"MAI|");
	strcat(buff,username);
	strcat(buff,"|");
	//printf("buff:%s\n",buff);
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
	//printf("buff:%s\n",buff);
	mail_line = strtok(buff,"|");
	//printf("mail_line:%s\n",mail_line);
	strcpy(cmd, mail_line);
	
	pthread_mutex_lock(&mutex);
	if(strcmp(cmd,"MAI") == 0)
	{
		mail_line = strtok(NULL,"|~");
		while(mail_line != NULL)
		{ 
			mail_list[i].sqno = atoi(mail_line);
			mail_line = strtok(NULL,"|~");
			//printf("mail line read: %s\n", mail_line);
			mail_list[i].read = atoi(mail_line);
			//printf("mail line read: %d\n",mail_list[i].read);
			mail_line = strtok(NULL,"|~");
			
			strcpy(mail_list[i].sendername, mail_line);
			mail_line = strtok(NULL,"|~");
			strcpy(mail_list[i].sfname, mail_line);
			mail_line = strtok(NULL,"|~");
			strcpy(mail_list[i].slname, mail_line);
			mail_line = strtok(NULL,"|~");
			strcpy(mail_list[i].dateTime, mail_line);
			mail_line = strtok(NULL,"|~");
			strcpy(mail_list[i].subject, mail_line);
			mail_line = strtok(NULL,"|~");
			strcpy(mail_list[i].message, mail_line);
			mail_line = strtok(NULL,"|~");
			mail_list[i].isAttachment = atoi(mail_line);
			if (mail_list[i].isAttachment == 1)
			{
				mail_line = strtok(NULL,"|~");
				strcpy(mail_list[i].attFileName, mail_line);
				mail_line = strtok(NULL,"|~");
				strcpy(mail_list[i].attachment, mail_line);
			}
			mail_line = strtok(NULL,"|~");
			
			i++;
		}
		mail_count = i; 
		//printf("mail_count: %d\n",mail_count);
/*
		for(j = 0; j < mail_count; j++)
		{
		  printf("%d.\nSender: %s\nFirst Name: %s\nLast Name: %s\nSubject: %s\nDateTime of the Message received: %s\n\n",mail_list[j].sqno,mail_list[j].sendername,mail_list[j].sfname, mail_list[j].slname,mail_list[j].subject, mail_list[j].dateTime);
		}*/
	}
	pthread_mutex_unlock(&mutex);

}

char** print_usernames(char username[])
{
	char *token,list_username[50][50];
	int i=-1;
	bzero((char *)&buff, sizeof(buff));
	
	//printf("username:%s\n",username);
	strcpy(buff,"UNA|");
	strcat(buff,username);
	strcat(buff,"~");
	//printf("printing different unames\n");
	if(write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
	{
		close(serv_sockfd);
		perror("Write Error\n");
		exit(0);
    }
	bzero((char *)&buff, sizeof(buff));
	//free(token);
	//printf("Available USER LIST:\n");
	pthread_mutex_lock(&mutex);
	if(read(serv_sockfd, buff, sizeof(buff)) < 0)
    	{
		close(serv_sockfd);
		perror("Read Error\n");
		exit(0);
    	}
	
	//printf("%s\n",buff);
	token = strtok(buff,"|");
  //strcpy(cmd, mail_line);
	if(strcmp(token,"UNA")==0)
    {
		token = strtok(NULL,"|");
		while(token != NULL)
		{
			printf("token:%s\n",token);
			i++;
			//printf("in while");
			strcpy(list_username[i],token);
			token = strtok(NULL,"|");
			//printf("# user:%d\n",i);
		}
		
	}
	pthread_mutex_unlock(&mutex);
	return(list_username);
}


void send_mail(char username[], int mail_type, int seq_no)
{
	/* This function will asks user to input details related to sending mail and send mail to all given recipients. */
	char uname[50],fname[50],lname[50],subject[100],message[600],isAtt1[10],*token,cmd[4], dateTime[50], str_curr_time[20];
	char att[30], *file_data;
	char attachment[300],hh[50],mm[50],ss[50],dTime[20];
	int opt,isAtt, is_valid_file, len,op,i=-1,val;
	time_t raw_time;
	FILE *fp;
	
	struct tm tm1;
	time_t now = time(NULL);
	char *date;
	
	date = ctime(&now);
	
	strptime(date,"%a %b %d %H:%M:%S %Y",&tm1);//local time

	switch(mail_type)
	{
		case 1: // Send mail
			do{
			printf("Enter Recipient:\n(If multiple recipients are present, kindly separated recipient by ';'.)\n");
			scanf("%s",uname);
			val=check_username(uname,username);
			if(val==1)
			{
				printf("ENter valid username");
			}
		}while(val==1);
			
			printf("Enter the Subject:");
			fgets (subject, MAX_NAME_SZ, stdin);
			scanf("%[^\n]s",subject);
			printf("Enter the Message you want to send and press * to stop:");
			//multiple lines
			do{
				i++;
				message[i]=getchar();	
	}while(message[i] != '*');
			message[strlen(message)]='\0';
			break;
		case 2: // Forward Mail
			pthread_mutex_lock(&mutex);
			/*do{
			printf("Enter Recipient:\n(If multiple recipients are present, kindly separated recipient by ';'.)\n");
			scanf("%s",uname);
			val=check_username(uname,username);
			if(val==1)
			{
				printf("ENter valid username");
			}
		}while(val==1);*/
			printf("Enter Recipient:\n(If multiple recipients are present, kindly separated recipient by ';'.)\n");
			scanf("%s",uname);
			printf("Subject:%s\n",mail_list[seq_no - 1].subject);
			strcpy(subject,mail_list[seq_no - 1].subject);
			printf("Enter the Message you want to send");
			//fgets (message, MAX_NAME_SZ, stdin);
			do{
				i++;
				message[i]=getchar();	
			}while(message[i] != '*');
			message[strlen(message)]='\0';
			strcat(message,"\n\n\n");
			strcat(message,mail_list[seq_no - 1].message);
			pthread_mutex_unlock(&mutex);
			break;
		case 3: // Reply Mail
			pthread_mutex_lock(&mutex);
			printf("To:%s\n",mail_list[seq_no - 1].sendername);
			strcpy(uname,mail_list[seq_no - 1].sendername);
			printf("From:%s\n",username);
			printf("Subject:%s\n",mail_list[seq_no - 1].subject);
			strcpy(subject,mail_list[seq_no - 1].subject);
			printf("Enter the Message you want to send and press * to stop:");
			//fgets (message, MAX_NAME_SZ, stdin);
			do{
				i++;
				message[i]=getchar();	
			}while(message[i] != '*');
			message[strlen(message)]='\0';
			strcat(message,"\n\n");
			strcat(message,mail_list[seq_no - 1].message);
			pthread_mutex_unlock(&mutex);
			break;
		default:
			break;
	}
	
	do{
	printf("Enter Date: (Format of Data should be MM/DD/YYYY.)");
	scanf("%s",dateTime);
	strcpy(dTime,dateTime);
	op=valid_date(dateTime,1,"00");
	}while(op == 1);
	
	printf("datetime:%s\n", dTime);
	
				sprintf(hh,"%02d",tm1.tm_hour);
				strcat(dTime,";");
				strcat(dTime,hh);
				//printf("%s\n",dateTime);
				sprintf(mm,"%02d",tm1.tm_min);
				strcat(dTime,":");
				strcat(dTime,mm);
				//printf("%s",dateTime);
	sprintf(ss,"%02d",tm1.tm_sec);
	strcat(dTime,":");
	strcat(dTime,ss);
	printf("%s\n",dTime);
	//printf("datetime:%s\n", dateTime);
	
	//printf("str_curr_time:%s\n", str_curr_time);
	printf("If Attachment is present, please enter 1 or enter 0.\n");
	scanf("%d",&isAtt);
	
	if(isAtt == 1)
	{
		do
		{
			is_valid_file = 0;
			printf("Enter Attachment File Name. It should be in '.txt' format.");
			
			fgets (att, MAX_NAME_SZ, stdin);
			scanf("%s",att);
			if((fp = fopen(att,"r")) == NULL)
			{
				printf("File is not present\n");
				is_valid_file = 1;
			}
			else
			{
				fseek(fp,0,SEEK_END);
				len = ftell(fp);
				if(len == 0)
				{
					printf("File %s does not contain any data.\n", att);
					is_valid_file = 1;
					fclose(fp);
				}	
			}
		}while(is_valid_file == 1);

		
		rewind(fp);
		//printf("check 1\n");
		bzero((char *)&file_data,sizeof(file_data));
		file_data = (char*)malloc(sizeof(char)*len);
		
		fread(file_data,1,len,fp);
		fclose(fp);
		//printf("file data: %s\n", file_data);
		strcpy(attachment, file_data);
	/*
		printf("Enter text to be entered in an Attachment file.");
		//fgets(attachment,MAX_NAME_SZ, stdin);
		scanf("%s",attachment);*/
	//isAtt=1;
	}
	
	// SNM ; username of recipiant ; uname of sender
	strcpy(buff,"SNM|");
	strcat(buff,uname);
	strcat(buff,"|");
	strcat(buff,username);
	strcat(buff,"|");
	strcat(buff,subject);
    	strcat(buff,"|");
	strcat(buff,message);
    	strcat(buff,"|");
	strcat(buff,dTime);
    	strcat(buff,"|");
	sprintf(isAtt1,"%d",isAtt);
	strcat(buff,isAtt1);
	if(isAtt == 1)
	{
		strcat(buff,"|");
		strcat(buff,att);
		strcat(buff,"|");
		strcat(buff,attachment);
		//strcat(buff,"|");
	}
	strcat(buff,"~");
	
	//printf("buff:%s\n", buff);
	if(write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
	{
		close(serv_sockfd);
		perror("Write Error\n");
		exit(0);
    }
	bzero((char *)&buff, sizeof(buff));
	//free(token);
	//bzero((char *)&token,sizeof(token));
	
	if(read(serv_sockfd, buff, sizeof(buff)) < 0)
    {
		close(serv_sockfd);
		perror("Read Error\n");
		exit(0);
    }
	//printf("buff:%s\n",buff);
	token = strtok(buff,"|");
  //strcpy(cmd, mail_line);
	if(strcmp(token,"SNM") == 0)
    	{
		token = strtok(NULL,"|");
		printf("%s\n",token);
	}
}

void delete_mail(char username[],int mail_seqno)
{
	int i,j;
	char sqno[8];
	
	bzero((char *)&buff, sizeof(buff));
	strcpy(buff,"DEM|");
	strcat(buff,username);
	strcat(buff,"|");
	sprintf(sqno,"%d",mail_seqno);
	strcat(buff,sqno);
	strcat(buff,"|");
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
	{
			close(serv_sockfd);
			perror("Write Error\n");
			exit(0);
	}
	
	
	for(i=0 ; i < 100 ; i++)
	{
		if(mail_list[i].sqno == mail_seqno)
		{
			while(mail_list[i+1].sqno!=0)
			{
				
				
				mail_list[i].read = mail_list[i+1].read;
				strcpy(mail_list[i].sendername , mail_list[i+1].sendername);
				strcpy(mail_list[i].sfname , mail_list[i+1].sfname);
				strcpy(mail_list[i].slname , mail_list[i+1].slname);
				strcpy(mail_list[i].dateTime , mail_list[i+1].dateTime);
				strcpy(mail_list[i].subject , mail_list[i+1].subject);
				strcpy(mail_list[i].message , mail_list[i+1].message);
				mail_list[i].isAttachment = mail_list[i+1].isAttachment;
				strcpy(mail_list[i].attFileName , mail_list[i+1].attFileName);
				strcpy(mail_list[i].attachment , mail_list[i+1].attachment);
				i++;
				
			}
			
			mail_list[i].sqno = 0;
			mail_list[i].read = 0; 
			strcpy(mail_list[i].sendername , "");
			strcpy(mail_list[i].sfname , "");
			strcpy(mail_list[i].slname , ""); 
			strcpy(mail_list[i].dateTime , "");
			strcpy(mail_list[i].subject , ""); 
			strcpy(mail_list[i].message , ""); 
			mail_list[i].isAttachment = 0; 
			strcpy(mail_list[i].attFileName , ""); 
			strcpy(mail_list[i].attachment , "");
			
			
			break;
			
		}
	}
	
	for(i = 0; i < 100; i++)
	{
			if (mail_list[i].sqno == 0)
			break;
				printf("\n\nSequence Number:%d\nSender's Name:%s\nFisrt Name:%s\nLast Name:%s\ndateTime:%s\nSubject:%s\nMessage:%s\nRead:%d\n",
						mail_list[i].sqno,
						mail_list[i].sendername, 
						mail_list[i].sfname,
						mail_list[i].slname,
						mail_list[i].dateTime,
						mail_list[i].subject, 
						mail_list[i].message, 
						mail_list[i].read);
			if(mail_list[i].isAttachment == 1)
			{
				printf("%s\n%s\n",mail_list[i].attFileName, mail_list[i].attachment);
			}

	}
	
}




void read_mail_in_detail(char username[])
{
	/* This function will read mail of entered sequence no and asks to modify, delete, forward an email. */ 
	FILE *fp1;
	int i = 0, each_mail_option = 0, len = 0, mail_seqno = 0;
	char *filename, *att_buff, *token, str_read[4], str_sqno[4];
	
	//free(token);
	//bzero((char *)&token,sizeof(token));
	
	
	printf("Enter sequence number of an email to be read: ");
	scanf("%d",&mail_seqno);
	//printf("mail seq_no:%d\n",mail_seqno);
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 100; i++)
		{
		if (mail_list[i].sqno == 0)
			break;
		//printf("i:%d\n",i);
		if(mail_seqno == mail_list[i].sqno)
		{
			if(mail_list[i].read == 0)
			{
				
				mail_list[i].read = 1;
				//pthread_mutex_unlock(&mutex);
				bzero((char *)&buff, sizeof(buff));
				strcpy(buff,"REM|");
				strcat(buff,username);
				strcat(buff,"|");
				sprintf(str_read,"%d",1);
				strcat(buff,str_read);
				strcat(buff,"|");
				sprintf(str_sqno,"%d",mail_seqno);
				strcat(buff,str_sqno);
				strcat(buff,"|");
				printf("buff:%s\n",buff);
				if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
				{
					close(serv_sockfd);
					perror("Write Error\n");
					exit(0);
				}
				//printf("after wirte\n");
				bzero((char *)&buff, sizeof(buff));
				// printf("empty buffer\n");
				if(read(serv_sockfd, buff, sizeof(buff)) < 0)
				{
					close(serv_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				//printf("buff:%s\n", buff); 
				token = strtok(buff,"|");
				//printf("token:%s\n", token);
				if(strcmp(token,"REM")==0)
				{
					token = strtok(NULL,"|");
					//printf("Reply : %s\n",token);
				}
			}
			//printf("before print\n");
			//pthread_mutex_lock(&mutex);
			printf("\n\nSequence Number:%d\nSender's Name:%s\nFisrt Name:%s\nLast Name:%s\ndateTime:%s\nSubject:%s\nMessage:%s\nRead:%d\n",
						mail_list[i].sqno,
						mail_list[i].sendername, 
						mail_list[i].sfname,
						mail_list[i].slname,
						mail_list[i].dateTime,
						mail_list[i].subject, 
						mail_list[i].message, 
						mail_list[i].read);
			
			if(mail_list[i].isAttachment == 1)
			{
				printf("%s\n%s\n",mail_list[i].attFileName, mail_list[i].attachment);
			}
			pthread_mutex_unlock(&mutex);
			do
			{
				printf("\n\nEnter 1 for Reply to the sender\nEnter 2 to save the attachment\nEnter 3 to forward an e-mail. \nEnter 4 to delete an e-mail\nEnter 5 to go to previous menu.\n");
				scanf("%d",&each_mail_option);
				switch(each_mail_option)
				{
					case 1:
						//Reply an Email
						send_mail(username, 3,mail_seqno);
						break;
					case 2: // Save an attachment
						//printf("Inside attachment");
						pthread_mutex_lock(&mutex);
						if(mail_list[i].isAttachment == 1)
						{
							filename = mail_list[i].attFileName;
							//printf("filename:%s", filename);
							if((fp1 = fopen(filename,"w+")) == NULL)
								{
									printf("Error in opening a file.\n");
									//exit(0);
								}
							else
							{
								int str_len = 0;
								size_t file_len = 0;
								str_len = strlen(mail_list[i].attachment);
								file_len = fwrite(mail_list[i].attachment, 1, str_len, fp1);
								fseek(fp1,0,SEEK_END);
								len = ftell(fp1);
								rewind(fp1);

								att_buff = (char*)malloc(sizeof(char)*len);
								if(att_buff == NULL)
								{
									perror("Memory Error\n");
									exit(0);
								}
								fread(att_buff,1,len,fp1);
								printf("downloaded an attachment successfully\n");
								fclose(fp1);
							}
							
						}
						else
						{
							printf("This mail does not contain any attachment.\n");
						}
						pthread_mutex_unlock(&mutex);
						break;
					case 3:
						// Forwarding an email
						printf("forwarding mail\n seq no:%d\n",mail_seqno);
						send_mail(username,2,mail_seqno);
						break;
					case 4:
						//Deleting mail: Apexa
						delete_mail(username,mail_seqno);
						break;				 
					case 5:// Previous menu
						break;
					default:
						printf("Please Enter the Correct option.\n");
						break;
				} 
			}while(each_mail_option != 5); 
			break;
		}
		
	}
	//pthread_mutex_unlock(&mutex);
}

void read_mail(char in_username[])
{
	/* This function implements reading emails functionality. */
	int j = 0, option = 0, filter_op = 0, record_found = 0;
	char username[50],dateTime[50],subject[100];
	//printf("mail Count:%d\n", mail_count);
	
	
	if(NEW_MAILS > 0)
	{
		printf("You have %d New E-mails. KIndly check last %d e-mails.\n",NEW_MAILS, NEW_MAILS);
		NEW_MAILS = 0;
	}
	pthread_mutex_lock(&mutex);	
	for(j = 0; j < 100; j++)
	{
		if(mail_list[j].sqno == 0)
			break;
		//printf("read value:%d\n", mail_list[j].read);
		if(mail_list[j].read == 1)
		{
			printf("%d. You have already read this email.\n", mail_list[j].sqno );
		}
		else
		{
			printf("%d. This is an unread email.\n", mail_list[j].sqno);
		}
		printf("Sender: %s\nSubject: %s\nDateTime of the e-mail received: %s\n\n",
									mail_list[j].sendername,
									mail_list[j].subject, 
									mail_list[j].dateTime);
	}	
		
	pthread_mutex_unlock(&mutex);
	//printf("j:%d\n", j);
	if (j > 0)
	{
		read_mail_in_detail(in_username);
	}
	else
	{
		printf("There are no E-mails!\n");
	}
				
}

void filter_mail(char in_username[])
{
	/* This function implements filter mail functionality. */
	int j = 0, option = 0, filter_op = 0, record_found = 0, date_found = 0, if_month = 0, if_day = 0;
	char filter_criteria[50], username[50],dateTime[50],subject[100], month[50], year[50], day[50], filter_month[50], filter_year[50], filter_day[50];
	do
	{
		record_found = 0;
		printf("Choose options: 1. Search by Sender's username.\n2.Search by received date.\n3.Search by Subject.\n4.Go to previous menu.\n");
		scanf("%d",&filter_op);
		
		switch(filter_op)
		{
			case 1:
			  {
				//Search by Sender's username
				printf("Enter Username:\n");
				scanf("%s",filter_criteria);
				
				if(strlen(filter_criteria) > 0)
				{
					pthread_mutex_lock(&mutex);
					for(j = 0; j < 100; j++)
					  {
						if(mail_list[j].sqno == 0)
							break;
						strcpy(username,mail_list[j].sendername);
						if(strcmp(filter_criteria,username) == 0)
						  {
							printf("%d.\nSender: %s\nFirst Name: %s\nLast Name: %s\nSubject: %s\nDateTime of the Message received: %s\n\n",
														mail_list[j].sqno,
														mail_list[j].sendername,
														mail_list[j].sfname, 
														mail_list[j].slname,
														mail_list[j].subject, 
														mail_list[j].dateTime);
							record_found = 1;
						  }
						 
					  }
					 
					pthread_mutex_unlock(&mutex);
				}
				if(record_found == 0)
				{
					printf("No e-mails found for this criteria.\n");
					break;
				}
				read_mail_in_detail(in_username);
				break;
			  }
			case 2:
			{
				//Search by received date.
				printf("Enter Date: (Date should be in format '[MM]/[DD]/YYYY'.)\n");
				printf("Enter Year to be filtered: ");
				scanf("%s",filter_year);

				printf("Do you want to filter by month and year? If Yes, enter 1 or enter 0: ");
				scanf("%d", &if_month);
				//printf("if+month:%d\n",if_month);
				if(if_month == 1)
				{
					printf("Enter Month to be filtered: ");
					scanf("%s",filter_month);
					printf("Do you want to filter by day, month and year? If Yes, enter 1 or enter 0: ");
					scanf("%d", &if_day);
					if(if_day == 1)
					{
						printf("Enter Day to be filtered: ");
						scanf("%s",filter_day);
					}
					
				}
				if(strlen(filter_year) > 0)
				{
					//printf("filter year:%s\n",filter_year);
					pthread_mutex_lock(&mutex);
					for(j = 0; j < 100; j++)
					  {
						date_found = 0;
						//printf("inside for\n");
						//printf("sqno:%d\n",mail_list[j].sqno);
						if(mail_list[j].sqno == 0)
						{
							//printf("sqno:%d\n",mail_list[j].sqno);
							break;
						}
						strcpy(dateTime,mail_list[j].dateTime);
						//printf("date and time:%s\n", dateTime);
						strcpy(month, strtok(dateTime,"/"));
						strcpy(day, strtok(NULL,"/"));
						strcpy(year, strtok(NULL,";"));

						//printf("year:%s\nmonth:%s\nday:%s\n",year,month,day);
						if(strlen(filter_year) > 0)
						{
							if(strstr(year, filter_year) != NULL)
							{
								date_found = 1;
								//printf("inside year:%s\n",filter_year);
								if(strlen(filter_month) > 0)
								{
									if(strstr(month, filter_month) != NULL)
									{									
										date_found = 1;
										//printf("date found1:%d\n",date_found);
										//printf("inside month:%s\n",filter_month);
										if(strlen(filter_day) > 0)
										{
											//rintf("filter day:%s\n",filter_day);
											if(strstr(day, filter_day) != NULL)
											{
												date_found = 1;
												//printf("date found:2%d\n",date_found);
												//printf("inside day:%s\n",filter_day);
											}
											else{
												date_found = 0;
												//printf("date found3:%d\n",date_found);
												}
										}
									}
									else{
										date_found = 0;
										//printf("date found4:%d\n",date_found);
									}
								}
							
							
							}
							else{
								date_found = 0;
								//printf("date found5:%d\n",date_found);
							}	
							//printf("date found6:%d\n",date_found);
							if(date_found == 1)
							{
								record_found = 1;
								printf("%d.\nSender: %s\nFirst Name: %s\nLast Name: %s\nSubject: %s\nDateTime of the Message received: %s\n\n",
															mail_list[j].sqno,
															mail_list[j].sendername,
															mail_list[j].sfname, 
															mail_list[j].slname,
															mail_list[j].subject, 
															mail_list[j].dateTime);
							}
						}
						
						
							
					}
					
					pthread_mutex_unlock(&mutex);
				}
				if(record_found == 0)
				{
					printf("No e-mails found for this criteria.\n");
					break;
				}
				read_mail_in_detail(in_username);
				break;
			  }
			case 3:
			{
				//Search by Subject.
				printf("Enter any word present in subject of an e-mail.\n");
				scanf("%s",filter_criteria);
				
				if(strlen(filter_criteria) != 0)
				{
					pthread_mutex_lock(&mutex);
					for(j = 0; j < 100; j++)
					{
						if(mail_list[j].sqno == 0)
							break;
						strcpy(subject,mail_list[j].subject);
						if(strstr(subject,filter_criteria) != NULL)
						{
							printf("%d.\nSender: %s\nFirst Name: %s\nLast Name: %s\nSubject: %s\nDateTime of the Message received: %s\n\n",
														mail_list[j].sqno,
														mail_list[j].sendername,
														mail_list[j].sfname, 
														mail_list[j].slname,
														mail_list[j].subject, 
														mail_list[j].dateTime);
							record_found = 1;
						}
					}
					
					pthread_mutex_unlock(&mutex);
				}
				if(record_found == 0){
					printf("No e-mails found for this criteria.\n");
					break;
				}
				read_mail_in_detail(in_username);
				break;
			}
			case 4:
				//Previous menu.
				break;
			default:
			  {
				printf("Please Enter the Correct option.\n");
				break;
			  }
		}
	}while(filter_op != 4);
			
}

void mail_service(char username[])
{
	/* This function implements entire email functionalty. */
	int mail_menu = 0,i;
	char list_uname[50][50];
	// Retrieving mails from server and add it to mail array.
	retrieve_mails(username);
		
	do
	{
		printf("Enter: 1.Available Users\n 2.Send Mail\n3. Read Mails\n4. Filter Mails\n5.Go to previous menu.\n");
    scanf("%d", &mail_menu); 
	
		switch(mail_menu)
		{
			case 1: //list available users
			   	 memcpy(list_uname,print_usernames(username),sizeof(list_uname));
				for(i=0;strlen(list_uname[i])>0;i++)
				{
					if(strcmp(list_uname[i],username) != 0)
					{
						printf("%s\n",list_uname[i]);
					}
				}
				break;
			case 2: // Ramya:Create Mail
				send_mail(username,1,0);
				break;
			case 3: // Priyanka:Read Mails
				read_mail(username);
				break;
			case 4: // Priyanka:Filter Mails
				filter_mail(username);
				break;
			case 5:// Go to previous Menu
				break;
			default:
				printf("Please Enter the Correct option.\n");
				break;		
		}
	}while(mail_menu!= 5);
}

void retrieve_cal(char username[])
{
	/* This function will retireve calendar events from the server side when user logs in and store it in calendar array. */
	char cmd[4], *cal_line;
  	int i = 0, j = 0, k = 0, cal_count = 0;
  
	//printf("inside retireve mail\n");	
	bzero((char *)&buff, sizeof(buff));
	strcpy(buff,"CAL|");
	strcat(buff,username);
	strcat(buff,"|");
	//printf("buff:%s\n",buff);
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
	cal_line = strtok(buff,"|");
	strcpy(cmd, cal_line);
	
	pthread_mutex_lock(&mutex);
	if(strcmp(cmd,"CAL") == 0)
	{
		cal_line = strtok(NULL,"|~");
		while(cal_line != NULL)
		{ 
			cal_list[i].event_id = atoi(cal_line);
			cal_line = strtok(NULL,"|~");
			cal_list[i].sqno = atoi(cal_line);
			cal_line = strtok(NULL,"|~");
			cal_list[i].read = atoi(cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].sendername, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].sfname, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].slname, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].start_date, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].start_time, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].end_date, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].end_time, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].place, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].subject, cal_line);
			cal_line = strtok(NULL,"|~");
			strcpy(cal_list[i].message, cal_line);
			cal_line = strtok(NULL,"|~");
			cal_list[i].isAttachment = atoi(cal_line);
			if (cal_list[i].isAttachment == 1)
			{
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[i].attFileName, cal_line);
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[i].attachment, cal_line);
			}
			cal_line = strtok(NULL,"|~");
			cal_list[i].no_of_participants = atoi(cal_line);
			for(k = 0; k <= cal_list[i].no_of_participants; k++)
			{
				cal_line = strtok(NULL,"|~");
				strcpy(cal_list[i].participants_list[k], cal_line);
			}
			cal_line = strtok(NULL,"|~");
			i++;
		}
		cal_count = i; 
		//printf("mail_count: %d\n",mail_count);
		/*
		for(j = 0; j < cal_count; j++)
		{
			printf("%d.\nEvent id:%d\nSender: %s\nFirst Name: %s\nLast Name: %s\nStart Date: %s\nStart Time: %s\nEnd Date: %s\nEnd Time: %s\nPlace: %s\nsubject of the Event received: %s\n\n",
							cal_list[j].sqno,
							cal_list[j].event_id,
							cal_list[j].sendername,
							cal_list[j].sfname,
							cal_list[j].slname,
							cal_list[j].start_date,
							cal_list[j].start_time,
							cal_list[j].end_date,
							cal_list[j].end_time,
							cal_list[j].place,
							cal_list[j].subject);
							
			for(k = 0; k <= cal_list[j].no_of_participants; k++)
			{
				printf("Recipients Name:%s\n",cal_list[j].participants_list[k]);
			}				
			
		}*/
	}
	pthread_mutex_unlock(&mutex);

}


int valid_date(char dateTime[],int type,char startTime[])  //type=1 -> send mail  ; type =2 -> send cal  
{
	char *token;
	struct tm tm1;
	struct tm tm;
	struct tm tm2;
	int mon,day,year;
	time_t day_name=time(NULL);
	time_t now = time(NULL);
	char *date,*date1,*time1;
	
	date = ctime(&now);
	
	strptime(date,"%a %b %d %H:%M:%S %Y",&tm);//local time
	strptime(dateTime,"%m/%d/%Y",&tm1);//input time
	strptime(startTime,"%H:%M:%S",&tm2);//input start time
	//printf("In valid date");
	mon=atoi(strtok(dateTime,"/"));
	day=atoi(strtok(NULL,"/"));
	year=atoi(strtok(NULL,"/"));
	
	
	
	
	if(mon<1 || mon>12 || day<0 || day>31)
	{
		printf("Wrong input enter valid value in the range\n");
		return 1;
	}
	
	
	//cancel event if input is till yesterday date
	if(type==2)    
	{
		
		if(tm1.tm_year<tm.tm_year)
		{
			printf("Invalid year\n");
			return 1;
		}
		else 
		{
			if(tm1.tm_mon < tm.tm_mon && tm1.tm_year==tm.tm_year)
			{
				printf("Invalid month\n");
				return 1;
			}
			else
			{
				if(tm1.tm_mday < tm.tm_mday && tm1.tm_mon == tm.tm_mon && tm1.tm_year==tm.tm_year)
				{
					printf("Invalid date\n");
					return 1;
				}
				if(tm1.tm_mday == tm.tm_mday && tm1.tm_mon == tm.tm_mon && tm2.tm_hour<tm.tm_hour && tm1.tm_year==tm.tm_year)
				{
					
						printf("Invalid Time:\n");
						return 1;
				}
				if(tm1.tm_year==tm.tm_year && tm1.tm_mday == tm.tm_mday && tm1.tm_mon == tm.tm_mon && tm2.tm_hour==tm.tm_hour)
					{
						if(tm2.tm_min<tm.tm_min)
						{
							printf("Enter valid Minutes\n");
							return 1;
						}
					}
		
			}
		}
	}
	return 0;
	
}
int valid_time(char inputTime[])
{
	char *token;
	int hh,mm,ss;
	hh=atoi(strtok(inputTime,":"));
	mm=atoi(strtok(NULL,":"));
	ss=atoi(strtok(NULL,":"));
	//printf("In valid time");
		if(hh > 23 || mm>59 || ss>59)
		{
			printf("Invalid Time\n");
			return 1;
		}
		return 0;
	
}



int check_username(char uname[],char username[])//uname contains list of receipiants 
{
	char list_uname[50][50],*token;
	int i,ret=0;
	memcpy(list_uname,print_usernames(username),sizeof(list_uname));
	//list_uname=print_usernames(username);
	//printf("size:%d\n",(sizeof(list_uname)/sizeof(list_uname[0])));
	/*for(i=0;strlen(list_uname[i])>0;i++)
	{
		printf("USER:%d %s\n",i,list_uname[i]);
		
	}*/
	
	//printf("in check username:\n");
	
	
	//free(token);
	token=strtok(uname,";");
	//printf("user name:%s\n",token);
	while(token!= NULL)
	{
		ret=0;
		for(i=0;strlen(list_uname[i])>0;i++)
		{
			//printf("token:%s\n",token);
			//printf("%s",list_uname[i]);
			if(strcmp(list_uname[i],token) == 0)
			{
				ret=1;
			}
			
		}
		if(ret==0)
		{
		return 1;
		}
	token=strtok(NULL,";");
	}
	//return 0;
	
	return 2;
}



	
	
void delete_old_event(int flag , char username[] , int seq_no)
{
	int i,j;
	char sqno[8];
	
	
	
	if(flag == 2)
	{
		bzero((char *)&buff, sizeof(buff));
		strcpy(buff,"DEC|");
		strcat(buff,username);
		strcat(buff,"|");
		sprintf(sqno,"%d",seq_no);
		strcat(buff,sqno);
		strcat(buff,"|");
		if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
		{
				close(serv_sockfd);
				perror("Write Error\n");
				exit(0);
		}
	}
	
	for(i=0 ; i < 100 ; i++)
	{
		if(cal_list[i].sqno == seq_no)
		{
			while(cal_list[i+1].sqno != 0)
			{
				
				cal_list[i].event_id = cal_list[i+1].event_id;
				cal_list[i].read = cal_list[i+1].read;
				strcpy(cal_list[i].sendername , cal_list[i+1].sendername);
				strcpy(cal_list[i].sfname , cal_list[i+1].sfname);
				strcpy(cal_list[i].slname , cal_list[i+1].slname);
				strcpy(cal_list[i].start_date , cal_list[i+1].start_date);
				strcpy(cal_list[i].start_time , cal_list[i+1].start_time);
				strcpy(cal_list[i].end_date , cal_list[i+1].end_date);
				strcpy(cal_list[i].end_time , cal_list[i+1].end_time);
				strcpy(cal_list[i].place , cal_list[i+1].place);
				strcpy(cal_list[i].subject , cal_list[i+1].subject);
				strcpy(cal_list[i].message , cal_list[i+1].message);
				cal_list[i].isAttachment = cal_list[i+1].isAttachment;
				cal_list[i].no_of_participants = cal_list[i+1].no_of_participants;
				for(j = 0; j <= cal_list[i+1].no_of_participants; j++)
				{
						strcpy(cal_list[i].participants_list[j] , cal_list[i+1].participants_list[j]);
				}
				i++;
				
			}
			
			cal_list[i].sqno = 0;
			cal_list[i].event_id = 0;
			cal_list[i].read = 0; 
			strcpy(cal_list[i].sendername , "");
			strcpy(cal_list[i].sfname , "");
			strcpy(cal_list[i].slname , ""); 
			strcpy(cal_list[i].start_date , "");
			strcpy(cal_list[i].start_time , "");
			strcpy(cal_list[i].end_date , "");
			strcpy(cal_list[i].end_time , "");
			strcpy(cal_list[i].place , "");
			strcpy(cal_list[i].subject , ""); 
			strcpy(cal_list[i].message , ""); 
			cal_list[i].isAttachment = 0; 
			cal_list[i].no_of_participants = 0;
			for(j = 0; j <= cal_list[i].no_of_participants; j++)
			{
				strcpy(cal_list[i].participants_list[j] , "");
			}
			break;
			
		}
	}
	
	for(i = 0; i < 100; i++)
	{
			if (cal_list[i].sqno == 0)
			break;
				printf("\n\nEvent id:%d\nSeq No:%d\nSender Name:%s\nFisrt Name:%s\nLast Name:%s\nSubject:%s\nStart Date:%s\nEnd Date:%s\nPlace:%s\nMessage:%s\nRead:%d\n",
								cal_list[i].event_id,
								cal_list[i].sqno,
								cal_list[i].sendername, 
								cal_list[i].sfname,
								cal_list[i].slname,
								cal_list[i].subject,
								cal_list[i].start_date,
								cal_list[i].end_date,
								cal_list[i].place, 
								cal_list[i].message,
								cal_list[i].read);
			if(cal_list[i].isAttachment == 1)
			{
				printf("%s\n%s\n",cal_list[i].attFileName, cal_list[i].attachment);
			}
			printf("Name of Participants who are involved in this Event.\n");
			for(j = 0; j <= cal_list[i].no_of_participants; j++)
			{
				printf("%d. %s\n", j + 1, cal_list[i].participants_list[j]);
			}
	}
	
	
}





void send_cal(char username[], int event_type, int seq_no, int event_id, char user_list[50])
{
	/* This function implements send calendar event functionality */
	
	// Event_type: 1 for Create New Event. 2 for Modify Event. For Modify, seqno and event_id needs to be passed from calling function.
	char uname[50],uname1[50],uname2[50],fname[50],lname[50],subject[100],message[600],isAtt1[10],*token,startDate[50],endDate[50],startTime[50],endTime[50],place[50];
	char att[30], str_event_id[50], str_seq_no[50], *file_data;
	char attachment[300],sDate[30],eTime[30],sTime[30];
	int opt,isAtt, create_again = 0, is_valid_file = 0, len = 0,val,op,op1,op2,op3,i=-1;
	FILE *fp;
	
	do
	{
		if(create_again != 2 && event_type == 1)
		{
			do{
				val=0;
				printf("Enter Recipient:\n(If multiple recipients are present, kindly separated recipient by ';'.)\n");
				scanf("%s",uname);
				
				strcpy(uname2,uname);
				strcat(uname2,";");
				val = check_username(uname,username);
				//printf("user %s\n",uname);
				if(val==1)
				{
					printf("Enter valid username\n");
				}
			}while(val==1);
		}
		printf("ulist:%s\n",user_list);
		
		do{
			
			printf("Enter Start Date:(Format should be MM/DD/YYYY) ");
			scanf("%s",startDate);
			strcpy(sDate,startDate);
			
			printf("Enter Start time of the Event: (Format should be HH:MM:SS) ");
			scanf("%s",startTime);
			strcpy(sTime,startTime);
			op3=valid_date(startDate,2,startTime);
			op=valid_time(startTime);
			//printf(" op3:%d op:%d",op3,op);
		}while(op3 == 1 || op == 1);
		
		do{
		
			printf("Enter End time of the Event: (Format should be HH:MM:SS) ");
			scanf("%s",endTime);
			strcpy(eTime,endTime);
			op1=valid_time(endTime);
		}while(op1 == 1);

		if(create_again != 2)
		{
			printf("Enter the Place of an Event: ");
			fgets (subject, MAX_NAME_SZ, stdin);
			scanf("%[^\n]s",place);
			printf("Enter the Subject:");
			fgets (subject, MAX_NAME_SZ, stdin);
			scanf("%[^\n]s",subject);
			printf("Enter the Message you want to send and press * to stop:");
			//fgets (message, MAX_NAME_SZ, stdin);
			do{
				i++;
				message[i]=getchar();	
			}while(message[i] != '*');
			message[strlen(message)]='\0';
			printf("If Attachment is present, please enter 1 or enter 0.\n");
			scanf("%d",&isAtt);
			if(isAtt == 1)
			{
				do
				{
					is_valid_file = 0;
					printf("Enter Attachment File Name. It should be in '.txt' format.\n");
			
					fgets (att, MAX_NAME_SZ, stdin);
					scanf("%s",att);
					if((fp = fopen(att,"r")) == NULL)
					{
						printf("File is not present\n");
						is_valid_file = 1;
					}
					else
					{
						fseek(fp,0,SEEK_END);
						len = ftell(fp);
						if(len == 0)
						{
							printf("File %s does not contain any data.\n", att);
							is_valid_file = 1;
							fclose(fp);
						}	
					}
				}while(is_valid_file == 1);

		
				rewind(fp);
				//printf("check 1\n");
				bzero((char *)&file_data,sizeof(file_data));
				file_data = (char*)malloc(sizeof(char)*len);
		
				fread(file_data,1,len,fp);
				fclose(fp);
				//printf("file data: %s\n", file_data);
				strcpy(attachment, file_data);
			/*
				printf("Enter text to be entered in an Attachment file.");
				//fgets(attachment,MAX_NAME_SZ, stdin);
				scanf("%s",attachment);*/
			//isAtt=1;
			}
		}

		// SNM ; username of recipiant ; uname of sender
		
		// Concatenating username in recipient's list, so that at server side, it will check for username's availability.
		//printf("uname:%s\n",uname2);
		if(event_type == 2)
			strcpy(uname1,user_list);
		else{
			strcpy(uname1,uname2);
			strcat(uname1,username);
			strcat(uname1,";");
		}
		//strcat(uname1,";");
		
		printf("uname:%s\n",uname1);
		strcpy(buff,"SNC|");
		strcat(buff,uname1); // recipients list
		strcat(buff,"|");
		strcat(buff,username); // sender
		strcat(buff,"|");
		strcat(buff,sDate);
		strcat(buff,"|");
		strcat(buff,sDate);
		strcat(buff,"|");
		strcat(buff,sTime);
		strcat(buff,"|");
		strcat(buff,eTime);
		strcat(buff,"|");
		strcat(buff,place);
		strcat(buff,"|");
		strcat(buff,subject);
		strcat(buff,"|");
		strcat(buff,message);
		strcat(buff,"|");
		sprintf(isAtt1,"%d",isAtt);
		strcat(buff,isAtt1);
		if(isAtt == 1)
		{
			strcat(buff,"|");
			strcat(buff,att);
			strcat(buff,"|");
			strcat(buff,attachment);
			//strcat(buff,"|");
		}
		//printf("event type:%d\n", event_type);
		if(event_type == 2)// Modify Event
		{
			strcat(buff,"|2|");
			sprintf(str_seq_no,"%d",seq_no);
			strcat(buff,str_seq_no);
			strcat(buff,"|");
			sprintf(str_event_id,"%d",event_id);
			strcat(buff,str_event_id);
			
		}
		else
		{
			strcat(buff,"|1");
		}
		strcat(buff,"~");
		
		//printf("buff:%s\n", buff);
		if(write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
		{
			close(serv_sockfd);
			perror("Write Error\n");
			exit(0);
		}
		
		bzero((char *)&buff, sizeof(buff));
		//free(token);
		
		//printf("before read\n");
		if(read(serv_sockfd, buff, sizeof(buff)) < 0)
		{
			close(serv_sockfd);
			perror("Read Error\n");
			exit(0);
		}
		//printf("after read\n");
		token = strtok(buff,"|");
		
		//printf("after string token\n");
	  //strcpy(cmd, mail_line);
		if(strcmp(token,"SNC")==0)
		{
			if(event_type == 2)
				{
					//("Event is modified now deleteing from client structure\n");
					
					delete_old_event(1,username,seq_no);
					//delete() - Apexa
				}
			token = strtok(NULL,"|");
			//printf("%s\n",token);
			create_again = atoi(token);
			if(create_again == 2)
			{
				printf("Recipients are busy at this time. Kindly Change Date and Time of the event.\n");
				
			}
			else
			{
				create_again = 1;
				printf("All recipients are invited for the Event.\n");
				//printf("sefdhd\n");
				//printf("%d\n",event_type);
				
				
			}
			
		}
	}while(create_again == 2);
}

void read_cal_in_detail(char username[])
{
	/* This function will read calendar event of entered sequence no and asks to modify, delete of an event. */ 
	FILE *fp1;
	int i = 0,j = 0, each_cal_option = 0, len = 0, cal_seqno = 0;
	char *filename, *att_buff, *token, str_read[4], str_sqno[4],user_list[50];
	
	//free(token);
	//bzero((char *)&token,sizeof(token));
	bzero((char *)&buff, sizeof(buff));
	
	
	
	printf("Enter sequence number of an calendar appointment to be read: ");
	scanf("%d",&cal_seqno);
	
	//printf("cal seqno:%d\n", cal_seqno);
	pthread_mutex_lock(&mutex);
	for(i = 0; i < 100; i++)
		{
		if (cal_list[i].sqno == 0)
			break;
		printf("i:%d\n",i);
		if(cal_seqno == cal_list[i].sqno)
		{
			if(cal_list[i].read == 0)
			{
				cal_list[i].read = 1;
				
				printf("read:%d", cal_list[i].read);
				bzero((char *)&buff, sizeof(buff));
				strcpy(buff,"REC|");
				strcat(buff,username);
				strcat(buff,"|");
				sprintf(str_read,"%d",cal_list[i].read);
				strcat(buff,str_read);
				strcat(buff,"|");
				sprintf(str_sqno,"%d",cal_seqno);
				strcat(buff,str_sqno);
				strcat(buff,"|");
				
				//printf("buff:%s\n",buff);
				if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
				{
					close(serv_sockfd);
					perror("Write Error\n");
					exit(0);
				}
				
				//printf("buff:%s\n",buff);
				bzero((char *)&buff, sizeof(buff));
				  
				if(read(serv_sockfd, buff, sizeof(buff)) < 0)
				{
					close(serv_sockfd);
					perror("Read Error\n");
					exit(0);
				}
				//printf("after read:%s", buff);
				token = strtok(buff,"|");
				if(strcmp(token,"REC")==0)
				{
					token = strtok(NULL,"|");
					//printf("%s\n",token);
				}
			}
			
			printf("\n\nSeq No:%d\nEvent id:%d\nSender Name:%s\nFisrt Name:%s\nLast Name:%s\nSubject:%s\nDate of the Event:%s\nStart Time:%s\nEnd Time:%s\nPlace:%s\nMessage:%s\n",
								cal_list[i].sqno,
								cal_list[i].event_id,
								cal_list[i].sendername, 
								cal_list[i].sfname,
								cal_list[i].slname,
								cal_list[i].subject,
								cal_list[i].start_date,
								cal_list[i].start_time,
								cal_list[i].end_time,
								cal_list[i].place, 
								cal_list[i].message);
			if(cal_list[i].isAttachment == 1)
			{
				printf("%s\n%s\n",cal_list[i].attFileName, cal_list[i].attachment);
			}
			printf("Name of Participants who are involved in this Event.\n");
			for(j = 0; j <= cal_list[i].no_of_participants; j++)
			{
				printf("%d. %s\n", j + 1, cal_list[i].participants_list[j]);
			}
			pthread_mutex_unlock(&mutex);
			do
			{
				printf("\n\nEnter 1 To Modify An Appointment\nEnter 2 To Delete An Appointment\n Enter 3 to Save an Attachment.\nEnter 4 To Go To Previous Menu.\n");
				scanf("%d",&each_cal_option);
				switch(each_cal_option)
				{
					case 1:
						//Modify an Event
						pthread_mutex_lock(&mutex);
						printf("Here is your Event Details. Kindly Enter Details.\n");
						printf("\n\nSeq No:%d\nEvent id:%d\nSender Name:%s\nFisrt Name:%s\nLast Name:%s\nSubject:%s\nDate of the Event:%s\nStart Time:%s\nEnd Time:%s\nPlace:%s\nMessage:%s\n",
								cal_list[i].sqno,
								cal_list[i].event_id,
								cal_list[i].sendername, 
								cal_list[i].sfname,
								cal_list[i].slname,
								cal_list[i].subject,
								cal_list[i].start_date,
								cal_list[i].start_time,
								cal_list[i].end_time,
								cal_list[i].place, 
								cal_list[i].message);
						if(cal_list[i].isAttachment == 1)
						{
							printf("%s\n%s\n",cal_list[i].attFileName, cal_list[i].attachment);
						}
						printf("Name of Participants who are involved in this Event.\n");
						for(j = 0; j <= cal_list[i].no_of_participants; j++)
						{
							printf("%d. %s\n", j + 1, cal_list[i].participants_list[j]);
							strcat(user_list,cal_list[i].participants_list[j]);
							strcat(user_list,";");
							
						}
						pthread_mutex_unlock(&mutex);
						send_cal(username, 2, cal_list[i].sqno, cal_list[i].event_id,user_list);
						break;
					case 2:
						// delete an event
						//delete_cal_appointment();
						delete_old_event(2,username,cal_seqno);
						break;
					case 3:
						// Save an Attachment
						pthread_mutex_lock(&mutex);
						if(cal_list[i].isAttachment == 1)
						{
							filename = cal_list[i].attFileName;
							if((fp1 = fopen(filename,"w+")) == NULL)
								{
									perror("Error in opening a file.\n");
									exit(0);
								}
							int str_len = 0;
							size_t file_len = 0;
							str_len = strlen(cal_list[i].attachment);
							file_len = fwrite(cal_list[i].attachment, 1, str_len, fp1);
							fseek(fp1,0,SEEK_END);
							len = ftell(fp1);
							rewind(fp1);

							att_buff = (char*)malloc(sizeof(char)*len);
							if(att_buff == NULL)
							{
								perror("Memory Error\n");
								exit(0);
							}
							fread(att_buff,1,len,fp1);
							printf("downloaded an attachment successfully.\n");
							fclose(fp1);
						}
						else
						{
							printf("This mail does not contain any attachment.\n");
						}
						pthread_mutex_unlock(&mutex);
						break;
					case 4:// Go to previous menu.
						break;
					default:
						printf("Please Enter the Correct option.\n");
						break;
				} 
			}while(each_cal_option != 4); 
			break;
		}
		
	}
	//pthread_mutex_unlock(&mutex);
}

void read_cal(char in_username[])
{
	/* This function reads the calendar event. */
	int j = 0, option = 0, filter_op = 0, record_found = 0;
	char filter_criteria[50], username[50],dateTime[50],subject[100];
	//printf("mail Count:%d\n", mail_count);
	printf("view cal %d\n", NEW_CALS);
	pthread_mutex_lock(&mutex);
	
	if(NEW_CALS > 0)
	{
		printf("You have %d New Calendar Events. KIndly check last %d Events.\n",NEW_CALS, NEW_CALS);
		NEW_CALS = 0;
	}
	for(j = 0; j < 100; j++)
	{
		if (cal_list[j].sqno == 0)
		{
			printf("j: %d\n",j);
			break;
		}
		if(cal_list[j].read == 1)
		{
			printf("%d. You have already read this event.\n", cal_list[j].sqno );
		}
		else
		{
			printf("%d. This is an unread event.\n", cal_list[j].sqno);
		}
		printf("Sender: %s\nSubject: %s\nDate:%s\nStart time:%s\nEnd time:%s\nPlace:%s\n\n",
							cal_list[j].sendername,
							cal_list[j].subject, 
							cal_list[j].start_date,
							cal_list[j].start_time,
							cal_list[j].end_time,
							cal_list[j].place);
	}
	pthread_mutex_unlock(&mutex);
	if (j > 0)
	{
		read_cal_in_detail(in_username);
	}
	else
	{
		printf("There are no Calendar Events!\n");
	}
	
	
				
}

void calendar_service(char username[])
{
	/* This function implements entire calendar functionality */
	int cal_menu = 0, temp = 1,i;
	char list_uname[50][50];
	//printf("Inside calendar service\n");
	retrieve_cal(username);
	
	do
	{
		printf("Enter: 1. Display Available Users. \n2.Send An Appointment\n3. View An Appointments\n4.Go to previous menu.\n");
		scanf("%d", &cal_menu); 
	
		switch(cal_menu)
		{
			case 1: // Dsiplay available users.
				//printf("HELLO SEND INVITATION");
				//print_usernames(username);
				memcpy(list_uname,print_usernames(username),sizeof(list_uname));
				for(i=0;strlen(list_uname[i])>0;i++)
				{
					if(strcmp(list_uname[i],username) != 0)
					{
						printf("%s\n",list_uname[i]);
					}
				}
				break;
			case 2: // Send an event
				temp = 1;
				send_cal(username, temp, 0, 0,"0");
				break;
			case 3: // View events
				read_cal(username);
				break;
			case 4: // Go to previous Menu
				break;
			/*case 5: //exit cal
			exit_cal(username);
			return(0);*/
			default:
				printf("Please Enter the Correct option.\n");
				break;		
		}
	}while(cal_menu!= 4);
}

void close_conn(char username[])
{
	/* if user logs out, notify server. */
	USER_LOGOUT = 1;
	bzero((char *)&buff, sizeof(buff));
  	strcpy(buff,"OUT|");
	strcat(buff,username);
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    {
		close(serv_sockfd);
		perror("Write Error\n");
		exit(0);
    }
	bzero((char *)&buff, sizeof(buff));
	
	if (read(serv_sockfd, buff, sizeof(buff)) < 0)
	{
		close(serv_sockfd);
		perror("Read Error\n");
		exit(0);
	}
		
		//printf("buff:%s\n", buff);
}

void signup()
{	
	/* This function will register new user in the system and stores all data on server. */
	int flag = 1;
	char username[50],password[12],fname[20],lname[20];

	bzero((char *)&buff, sizeof(buff));
	strcpy(buff,"SIG|");
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    	{
     		 close(serv_sockfd);
      		 perror("Write Er+ror\n");
      		 exit(0);
    	}
	
	do
	{
		bzero((char *)&buff, sizeof(buff));
		printf("Enter Username:");
		scanf("%s",username);
		strcpy(buff,username);
		if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    		{
      			close(serv_sockfd);
      			perror("Write Error\n");
     			exit(0);
    		}
		bzero((char *)&buff, sizeof(buff)); 
		if (read(serv_sockfd, buff, sizeof(buff)) < 0)
		{
			close(serv_sockfd);
			perror("Read Error\n");
			exit(0);
		}
		if(strcmp(buff,"1")==0)
		{
			flag = 1;
			printf("This user already exists, please enter another username.\n");
		}
		else
		{
			flag = 0;
		}
	}while(flag == 1); 

	bzero((char *)&buff, sizeof(buff));	
	printf("Enter Password:");
	   	
	scanf("%s",password);
	strcpy(buff,password);
  	//strcat(buff,"|");
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    	{
      			close(serv_sockfd);
      			perror("Write Error\n");
     			exit(0);
    	}
	bzero((char *)&buff, sizeof(buff)); 
           	
	printf("Enter Your First Name:");
	  	 
	scanf("%s",fname);
	strcpy(buff,fname);
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    	{
      			close(serv_sockfd);
      			perror("Write Error\n");
     			exit(0);
    	}
	bzero((char *)&buff, sizeof(buff)); 
	   	
	printf("Enter Youe Last Name:");
	   	
	scanf("%s",lname);
	strcpy(buff,lname);
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    	{
      			close(serv_sockfd);
      			perror("Write Error\n");
     			exit(0);
    	}
	bzero((char *)&buff, sizeof(buff)); 
}

char* login()
{
	/* This function checks username and password and allow user to login. */
	int flag = 0;
	char username[50],password[12],fname[20],lname[20];
	bzero((char *)&buff, sizeof(buff));
	strcpy(buff,"LOG|");
	if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    	{
     		 close(serv_sockfd);
      		 perror("Write Error\n");
      		 exit(0);
    	}
	bzero((char *)&buff, sizeof(buff));
	do
	{
		printf("Enter Username:");
		scanf("%s",username);
		printf("Enter Password:");
	   	scanf("%s",password);
		strcpy(buff,username);
  		strcat(buff,"|");
		strcat(buff,password);
		strcat(buff,"|");
		if (write(serv_sockfd, buff, sizeof(buff)) != sizeof(buff))
    		{
      			close(serv_sockfd);
      			perror("Write Error\n");
     			exit(0);
    		}
			
		//printf("buff:%s\n",buff);
		bzero((char *)&buff, sizeof(buff)); 
		
		if (read(serv_sockfd, buff, sizeof(buff)) < 0)
		{
			close(serv_sockfd);
			perror("Read Error\n");
			exit(0);
		}
		//printf("buff: %s\n", buff);
		if(strcmp(buff,"0") == 0)
		{
			flag = 0;
			printf("Please enter valid username and password.\n");
		}
		if(strcmp(buff,"1") == 0)
		{
			flag = 1;
			printf("You are successfully logged in. Please make your choice!\n");
		}
		if(strcmp(buff,"2") == 0)
		{
			flag = 0;
			printf("%s is already logged in on other machine, kindly enter another username to login.\n", username);
		}
		
	}while(flag==0); 
	
//printf("username in login:%s\n", username);
	return(username);
}


int main(int argc, char *argv[])
{

  printf("************ WELCOME ************\n\n");
  pthread_t bthread, cthread;
  char username[50];
  int main_menu = 0, ret = 0, isSignUp = 0, r = 0;
  for(;;)
    {
		if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
		{
			  perror("Socket Error\n");
			  exit(0);
		}

		bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
		ret = inet_aton(argv[1], &sock_addr_serv.sin_addr.s_addr);
		sock_addr_serv.sin_family = AF_INET;
		sock_addr_serv.sin_port = htons(atoi(argv[2]));

		  // Connecting to the Server
		if(connect(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv))< 0)
		{
			close(serv_sockfd);
			perror("Connect Error\n");
			exit(0);
		}
		
		do{
			
			printf("1.Sign-Up (If you are a new user, please enter 1 to register yourself.)\n");
			printf("2.Login (If you are an existing user, please enter 2 to login.)\n");
			scanf("%d",&r);
			//printf("r= %d\n", r);
			switch(r)
			{
				case 1:
					signup();
					isSignUp = 1;
					break;
				case 2:
					memcpy(username,login(), sizeof(username));
					isSignUp = 0;
					break;
			}
		}while(isSignUp == 1);
		
		//printf("username: %s\n", username);
		fflush(stdin);
		// Starting background thread after user logged in.
		//pthread_create(&bthread,NULL,&backgroundthread,(void*)username);// how to pass parameter in thread function.
		
		//printf("exit from backgroud\n");
		do
		{
		  //Asks User for the options to select service
		  printf("Enter: 1. Mail \n2. Calendar\n3. Log Out\n");
		  scanf("%d", &main_menu);
			switch(main_menu)
			{
				case 1:
					mail_service(username);
					break;
				case 2:
					calendar_service(username);
					break;
				case 3:
					close_conn(username);
					printf("************ THANK YOU ************\n");
					exit(0);
				default:
					printf("Please Enter the Correct option.\n");
					break;
			}
		}while(main_menu != 3);
	}
   close(serv_sockfd);
}
