#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <netinet/in.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include<pthread.h>
#define MAXLEN 2048*5

// Mail structure
struct mail{
    int sqno;
    int read;
    char sendername[50]; // Username of the Sender
    char sfname[50];    //First name of the sender
    char slname[50];    // Last name of the sender
    char dateTime[50];
    char subject[100];
    char message[300];
    int isAttachment;
    char attFileName[50];
    char attachment[200];
};

// calendar structure
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

//User structure - it will store user data and array of email and calendar
struct user{
    	char username[50];
    	char password[50];
    	char fname[50];
    	char lname[50];
    	char mail_file[50];
    	char cal_file[50];
	int is_logged_in; //To check whther user have logged in or not. 1 for Login and 2 for log out.
    	struct mail mail_list[50];
    	struct calendar cal_list[50];
};

int serv_sockfd, cli_sockfd, client_len, shmid, semid,shmid_event;
char buff[MAXLEN], *EVENT_ID ;
struct sockaddr_in sock_addr_serv,sock_addr_cl;
struct sembuf lock, unlock;

struct user user_list[50], *shared_user_list;

void server_services();
void print_user_array();
void print_error(char*);
void create_user_array();
void send_mail_data();
void send_mail_service();
void create_and_attach_shared_mem();
void get_sem_and_shm_for_client();
void exit_mail_service(char*);
void save_to_config_file(char*);
void signup();
void login();
void check_background_mail();
void check_background_cal();
void check_background_cal_participant();
void send_cal_data();
void read_cal_events();
void read_mail();
void send_cal_request(char*);
void delete_old_cal_event(char[][50],int,int);
void delete_mail();
void delete_cal();
int* get_date(char*);
int* get_time(char*);
void send_usernames();
void save_to_event_file();

void print_user_array_wo_shared();

int main(int argc,char *argv[])
{
	printf("********** SERVER IS RUNNING ***********\n\n");


	// Creates array of structure when server loads.
	create_user_array();
	//Implements Server services
	server_services(argv[1]);
	
	return(0);
}

void print_error(char *msg)
{
  perror(msg);
  exit(0);
}

void create_user_array()
{
	/*This function retrieves user details from configuration.txt file.
	Then it will retrieve all mails from mail_<username>.txt files and all calendar events from cal_<username>.txt files.
	All data will be stored in array. */

    FILE *fp, *fp1;
    int len,  option = 0,i = 0, mail_count = 0,user_count = -1,cal_count = 0, mail_seqno = 0, each_mail_option = 0,  email_count = 0, calendar_count = 0;
    char *file_data, *token;

	//printf("In create user array\n");

    // Retrieving data from configuration.txt file
    if((fp = fopen("configuration.txt","r")) == NULL)
	{
		print_error("Error in opening config file.\n");
	}

	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	rewind(fp);
	bzero((char *)&file_data,sizeof(file_data));
	file_data = (char*)malloc(sizeof(char)*len);
	if(file_data == NULL){
		fclose(fp);
		print_error("Memory Error\n");
	}
	fread(file_data,1,len,fp);
	
	//printf("config file data:%s\n",file_data);
	//printf("checkpoint1\n");
	token = strtok(file_data,"|~");


	while(token != NULL)
        {
		//printf("checkpoint2\n");
		user_count++;
		strcpy(user_list[user_count].username, token);

		token = strtok(NULL,"|~");
		strcpy(user_list[user_count].password, token);
		//printf("checkpoint3\n");
		token = strtok(NULL,"|~");
		strcpy(user_list[user_count].fname, token);

		token = strtok(NULL,"|~");
		strcpy(user_list[user_count].lname, token);

		token = strtok(NULL,"|~");
		strcpy(user_list[user_count].mail_file, token);

		token = strtok(NULL,"|~");
		strcpy(user_list[user_count].cal_file, token);

		token = strtok(NULL,"|~");
		user_list[user_count].is_logged_in = 2;
		//printf("checkpoint4\n");
		//printf("token:%s\n",token);
		//if (user_count == 1)
			//break;
	}
	fclose(fp);
	bzero((char *)&file_data,sizeof(file_data));
	//printf("checkpoint5\n");

	//printf("user count:%d",user_count);

	// Retrieving data from mail_username.txt file
	for( mail_count = 0; mail_count <= user_count; mail_count++ )
    	{
	  	email_count = 0;
		if((fp = fopen(user_list[mail_count].mail_file,"r")) == NULL)
		{
		    print_error("Error in opening mail file.\n");
		}
		fseek(fp,0,SEEK_END);
		len = ftell(fp);
		rewind(fp);

		file_data = (char*)malloc(sizeof(char)*len);
		if(file_data == NULL)
		{
		    fclose(fp);
		    print_error("Memory Error\n");
		}
		fread(file_data,1,len,fp);
		token = strtok(file_data,"|~");

		while(token != NULL)
		    {
			if(atoi(token) == 0)
			  	break;
			
			    user_list[mail_count].mail_list[email_count].sqno = atoi(token);
			    
			    token = strtok(NULL,"|~");
			    user_list[mail_count].mail_list[email_count].read = atoi(token);
				//printf("filename:%s\n",user_list[mail_count].mail_file);
				//printf("read value:%d\n",user_list[mail_count].mail_list[mail_count].read );
        
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].sendername, token);
	
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].sfname, token);
	
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].slname, token);
	
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].dateTime, token);
	
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].subject, token);
	
			    token = strtok(NULL,"|~");
			    strcpy(user_list[mail_count].mail_list[email_count].message, token);
	
			    token = strtok(NULL,"|~");
			    user_list[mail_count].mail_list[email_count].isAttachment = atoi(token);
	
			    if (user_list[mail_count].mail_list[email_count].isAttachment == 1)
			    {
				token = strtok(NULL,"|~");
				strcpy(user_list[mail_count].mail_list[email_count].attFileName, token);

				token = strtok(NULL,"|~");
				strcpy(user_list[mail_count].mail_list[email_count].attachment, token);
			    }
			    token = strtok(NULL,"|~");
			    email_count++;
	
        		}
        fclose(fp);
	bzero((char *)&file_data,sizeof(file_data));
    }

	// Retrieving data from cal_username.txt file
	for( cal_count = 0; cal_count <= user_count; cal_count++ )
    	{
		calendar_count = 0;
		//printf("checkpoint9\n");
		//printf("username:%s\n",user_list[cal_count].username);
		if((fp = fopen(user_list[cal_count].cal_file,"r")) == NULL)
		{
		    print_error("Error in opening a file.\n");
		}
		fseek(fp,0,SEEK_END);
		len = ftell(fp);
			//printf("length : %d\n",len );
		rewind(fp);

			//printf("checkpoint9.1\n");
		file_data = (char*)malloc(sizeof(char)*len);
			//printf("checkpoint9.2\n");
		if(file_data == NULL){
		    fclose(fp);
		    print_error("Memory Error\n");
		}
		fread(file_data,1,len,fp);
			//printf("file data: %s\n",file_data);
		token = strtok(file_data,"|~");	

		while(token != NULL)
		{
				if(atoi(token) == 0)
				  	break;
			
				//printf("checkpoint10\n");
				user_list[cal_count].cal_list[calendar_count].event_id = atoi(token);
			
				token = strtok(NULL,"|~");
				user_list[cal_count].cal_list[calendar_count].sqno = atoi(token);
			
				token = strtok(NULL,"|~");
				user_list[cal_count].cal_list[calendar_count].read = atoi(token);
			
				//printf("read value:%d\n",user_list[cal_count].cal_list[calendar_count].read);
	
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].sendername, token);

				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].sfname, token);

				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].slname, token);

				//printf("checkpoint11\n");
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].start_date, token);
			
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].start_time, token);
			
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].end_date, token);
			
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].end_time, token);
			
				//printf("end date:%s\n",user_list[cal_count].cal_list[calendar_count].end_date);
			
				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].place, token);

				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].subject, token);

				token = strtok(NULL,"|~");
				strcpy(user_list[cal_count].cal_list[calendar_count].message, token);
			
				//printf("message:%s\n",user_list[cal_count].cal_list[calendar_count].message);

				token = strtok(NULL,"|~");
				user_list[cal_count].cal_list[calendar_count].isAttachment = atoi(token);

				if (user_list[cal_count].cal_list[calendar_count].isAttachment == 1)
				{
					token = strtok(NULL,"|~");
					strcpy(user_list[cal_count].cal_list[calendar_count].attFileName, token);

					token = strtok(NULL,"|~");
					strcpy(user_list[cal_count].cal_list[calendar_count].attachment, token);
				}
				//printf("checkpoint12\n");
			
				token = strtok(NULL,"|~");
				user_list[cal_count].cal_list[calendar_count].no_of_participants = atoi(token);
			
				//printf("no_of_participants:%d\n",user_list[cal_count].cal_list[calendar_count].no_of_participants);
			
				for(i = 0; i <= user_list[cal_count].cal_list[calendar_count].no_of_participants; i++)
				{
					token = strtok(NULL,"|~");
					strcpy(user_list[cal_count].cal_list[calendar_count].participants_list[i], token);
					//printf("%s\n",user_list[cal_count].cal_list[calendar_count].participants_list[i]);
				}
				token = strtok(NULL,"|~");
				calendar_count++; 
					//printf("checkpoint13\n");
		}
		fclose(fp);
		bzero((char *)&file_data,sizeof(file_data));
    }
	//printf("successful creating array\n");
	//print_user_array_wo_shared();
	create_and_attach_shared_mem();
}
/*
void print_user_array_wo_shared()
{
	//printf("inside print\n");
	int i = 0, j = 0, k = 0;
	for(i = 0; i < 100; i++)
	{
		//printf("i:%d\n",i);
		if(user_list[i].is_logged_in == 0)
				break;
		
		printf("%s\n%s\n%s\n%s\n%s\n%s\n",user_list[i].username,user_list[i].password, user_list[i].fname, user_list[i].lname, user_list[i].mail_file, user_list[i].cal_file);
		printf("\nMail Lists\n");
		//j = 0;
		for(j = 0; j < 100; j++)
		{
			//printf("mail count:%d and j:%d\n",mail_count,j);
			if(user_list[i].mail_list[j].sqno == 0)
				break;
			
			
			printf("%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n",user_list[i].mail_list[j].sqno, 
															user_list[i].mail_list[j].read, 
															user_list[i].mail_list[j].sendername, 
															user_list[i].mail_list[j].sfname, 
															user_list[i].mail_list[j].slname, 
															user_list[i].mail_list[j].dateTime, 
															user_list[i].mail_list[j].subject, 
															user_list[i].mail_list[j].message, 
															user_list[i].mail_list[j].isAttachment);
		}	
		printf("-----------------------------------------\n\n");
		
		printf("\nCalendar Lists\n");
		//j = 0;
		for(j = 0; j < 100; j++)
		{
			//printf("mail count:%d and j:%d\n",mail_count,j);
			if(user_list[i].cal_list[j].sqno == 0)
				break;
			
			
			printf("%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n",user_list[i].cal_list[j].sqno, 
																	user_list[i].cal_list[j].read, 
																	user_list[i].cal_list[j].sendername,
																	user_list[i].cal_list[j].sfname,
																	user_list[i].cal_list[j].slname, 
																	user_list[i].cal_list[j].start_date,
																	user_list[i].cal_list[j].end_date,
																	user_list[i].cal_list[j].place,
																	user_list[i].cal_list[j].subject, 
																	user_list[i].cal_list[j].message, 
																	user_list[i].cal_list[j].isAttachment, 
																	user_list[i].cal_list[j].no_of_participants);
			for(k = 0; k < user_list[i].cal_list[j].no_of_participants; k++)
			{
				printf("%s\n",user_list[i].cal_list[j].participants_list[k]);
			}
		}	
		printf("-----------------------------------------\n\n");
	}
	//printf("print\n\n");
 
}
*/
void print_user_array()
{
	//printf("inside print\n");
	int i = 0, j = 0, k = 0;
	for(i = 0; i < 100; i++)
	{
		//printf("i:%d\n",i);
		if(shared_user_list[i].is_logged_in == 0)
				break;
		
		printf("%s\n%s\n%s\n%s\n%s\n%s\n",shared_user_list[i].username,shared_user_list[i].password, shared_user_list[i].fname, shared_user_list[i].lname, shared_user_list[i].mail_file, shared_user_list[i].cal_file);
		printf("\nMail Lists\n");
		//j = 0;
		for(j = 0; j < 100; j++)
		{
			//printf("mail count:%d and j:%d\n",mail_count,j);
			if(shared_user_list[i].mail_list[j].sqno == 0)
				break;
			
			
			printf("%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n",shared_user_list[i].mail_list[j].sqno, 
															shared_user_list[i].mail_list[j].read, 
															shared_user_list[i].mail_list[j].sendername, 
															shared_user_list[i].mail_list[j].sfname, 
															shared_user_list[i].mail_list[j].slname, 
															shared_user_list[i].mail_list[j].dateTime, 
															shared_user_list[i].mail_list[j].subject, 
															shared_user_list[i].mail_list[j].message, 
															shared_user_list[i].mail_list[j].isAttachment);
		}	
		printf("-----------------------------------------\n\n");
		
		printf("\nCalendar Lists\n");
		//j = 0;
		for(j = 0; j < 100; j++)
		{
			//printf("mail count:%d and j:%d\n",mail_count,j);
			if(shared_user_list[i].cal_list[j].sqno == 0)
				break;
			
			
			printf("%d\n%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n",shared_user_list[i].cal_list[j].event_id, 
																	shared_user_list[i].cal_list[j].sqno, 
																	shared_user_list[i].cal_list[j].read, 
																	shared_user_list[i].cal_list[j].sendername,
																	shared_user_list[i].cal_list[j].sfname,
																	shared_user_list[i].cal_list[j].slname, 
																	shared_user_list[i].cal_list[j].start_date,
																	shared_user_list[i].cal_list[j].start_time,
																	shared_user_list[i].cal_list[j].end_date,
																	shared_user_list[i].cal_list[j].end_time,
																	shared_user_list[i].cal_list[j].place,
																	shared_user_list[i].cal_list[j].subject, 
																	shared_user_list[i].cal_list[j].message, 
																	shared_user_list[i].cal_list[j].isAttachment, 
																	shared_user_list[i].cal_list[j].no_of_participants);
			for(k = 0; k <= shared_user_list[i].cal_list[j].no_of_participants; k++)
			{
				printf("%s\n",shared_user_list[i].cal_list[j].participants_list[k]);
			}
		}	
		printf("-----------------------------------------\n\n");
	}
	//printf("print\n\n");
 
}




void create_and_attach_shared_mem()
{
	/* This function will create shared memory and copy all data to shared memory array.
	It is using semaphore for synchronization. */

	char *temp_event_id;
	FILE *fp;
	int len;
	lock.sem_num = 0;
	lock.sem_op = -1;
	lock.sem_flg = SEM_UNDO;

	unlock.sem_num = 0;
	unlock.sem_op = 1;
	unlock.sem_flg = SEM_UNDO;
  
	// Creating semaphores
	if((semid = semget(ftok(".", 1),1, IPC_CREAT | 0666))  < 0)
		print_error("Error in Creating lock Sem");

	if((semctl(semid, 0, SETVAL, 1)) < 0)
		print_error("Error in init lock Sem");

	semop(semid, &lock, 1);


	// Creates shared memory for user_list
	if ((shmid = shmget(ftok(".",2),sizeof(user_list),IPC_CREAT | 0666)) < 0)
		print_error("Error in Creating shared memory");

	//printf("shared memory error\n");
	shmctl(shmid, IPC_RMID, NULL);	// removing old shared memorry for the same key.

	if ((shmid = shmget(ftok(".",2),sizeof(user_list),IPC_CREAT | 0666)) < 0)
		print_error("Error in Creating shared memory");

	//printf("shared memory errorcbfghgj\n");
	shared_user_list = (struct user*)shmat(shmid,NULL,0);


	memcpy(shared_user_list, user_list, sizeof(user_list));

	//print_user_array();
	
	shmdt((void *) shared_user_list);
	
	// Create shared memory for EVENT_ID
	if ((shmid_event = shmget(ftok(".",3),sizeof(char)*20,IPC_CREAT | 0666)) < 0)
		print_error("Error in Creating shared memory");

	//printf("EVENT_ID shared memory error\n");
	shmctl(shmid_event, IPC_RMID, NULL);	// removing old shared memorry for the same key.

	if ((shmid_event = shmget(ftok(".",3),sizeof(char)*20,IPC_CREAT | 0666)) < 0)
		print_error("Error in Creating shared memory");

	//printf("EVENT_ID shared memory errorcbfghgj\n");
	EVENT_ID = shmat(shmid_event,NULL,0);
	
	if((fp = fopen("event.txt","r")) == NULL)
	{
		print_error("Error in opening config file.\n");
	}

	fseek(fp,0,SEEK_END);
	len = ftell(fp);
	rewind(fp);

	bzero((char *)&temp_event_id,sizeof(temp_event_id));
	temp_event_id = (char*)malloc(sizeof(char)*len);
	if(temp_event_id == NULL)
	{
		fclose(fp);
		print_error("Memory Error\n");
	}
	fread(temp_event_id,1,len,fp);
	
	fclose(fp);
	
	sprintf(EVENT_ID, "%d", atoi(temp_event_id));
	//strcpy(EVENT_ID,temp_event_id);

	
	//printf("Event ID:%s\n", EVENT_ID);
	
//	EVENT_ID[strlen(EVENT_ID)] = '\0'; 
	
	shmdt((void *) EVENT_ID);
	//printf("detach array\n");
	semop(semid, &unlock, 1);
	//printf("unlock array\n");
}



void get_sem_and_shm_for_client()
{
	/* This function will create shared memory whenever client connects to the server, and attaching to server's shared memory. */ 
	if((semid = semget(ftok(".", 1),1, 0666))  < 0)
     		print_error("Error in Creating lock Sem");


	if ((shmid = shmget(ftok(".",2),sizeof(user_list),0666)) < 0)
		print_error("Error in Creating shared memory for user list");
	
	
	if ((shmid_event = shmget(ftok(".",3),sizeof(char)*20,0666)) < 0)
		print_error("Error in Creating shared memory for Event ID");
}

void server_services(char *argv)
{
	/* This function is implementing all functionalities of mail and calendar services. */
  	char cmd[3],cmd_ar[4],*token, *input;
	int ret;

	// Creating socket
	if((serv_sockfd = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		print_error("Socket Error\n");
	}

	bzero((char *)&sock_addr_serv,sizeof(sock_addr_serv));
	sock_addr_serv.sin_family = AF_INET;
	sock_addr_serv.sin_port = htons(atoi(argv));
	sock_addr_serv.sin_addr.s_addr = htonl(INADDR_ANY);

	// Binding address to the socket
	if (bind(serv_sockfd,(struct sockaddr *) &sock_addr_serv, sizeof(sock_addr_serv)) < 0)
	{
		close(serv_sockfd);
		print_error("Bind Error\n");
	}

	// Listening the connection
	if (listen(serv_sockfd,5)<0)
	{
		close(serv_sockfd);
		print_error("Listen Error\n");
	}

	for(;;)
	{
		client_len = sizeof(sock_addr_cl);

		// Accepting the client connection
		if ((cli_sockfd = accept(serv_sockfd,(struct sockaddr *)&sock_addr_cl,&client_len))<0)
        	{
			close(serv_sockfd);
			print_error("Accept Error\n");
		}

        	// Following code will implement concurrent server
		if(fork()==0)
		{
			close(serv_sockfd);
			get_sem_and_shm_for_client();

			for(;;)
			{
				bzero((char *)&buff,sizeof(buff));
				
				if (read(cli_sockfd, buff, sizeof(buff)) < 0)
				{
					close(cli_sockfd);
					perror("Read Error\n");
					exit(0);
				}
			
				//printf("buffer : %s\n",buff);
				token = strtok(buff,"|");
				
				strcpy(cmd, token);
				//printf("%s\n",cmd);
				
				// Sign up
				if (strcmp(cmd,"SIG") == 0)
				{
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter SIG\n");
					
					signup();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit SIG\n");
					//break;	
				}
				
				// Login
				if (strcmp(cmd,"LOG") == 0)
				{
					//token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter LOG\n");
					
					login();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit LOG\n");
					//break;	
				}

				// User chooses mail service, sends all data from structure to client
				if (strcmp(cmd,"MAI") == 0)
				{
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter MAI\n");
					send_mail_data();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit MAI\n");
					
				}
				
				
				
				
				// Send Mail functionality
				if(strcmp(cmd,"SNM")==0)
				{
					token = strtok(NULL,"~");
					//printf("reached inside SNM\n");
					//strcpy(input,token);
					//input = token;
					//printf("input:%s\n",input);
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter SNM\n");
					send_mail_service(token);

					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit SNM\n");
					
				}
				// Read mail functionality
				if (strcmp(cmd,"REM") == 0)
				{
					//token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter calendar background thread\n");
					
					read_mail();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit from ream mail\n");
					//break;	
				}
				
								
				// Checking new mails  thruogh background thread
				if (strcmp(cmd,"CHK") == 0)
				{
					//token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter background thread\n");
					
					check_background_mail();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("mail exit background thread\n");
					//break;	
				}
				
				
				// User chooses calendaer service, sends all data from structure to client
				if (strcmp(cmd,"CAL") == 0)
				{
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter CAL\n");
					send_cal_data();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit CAL\n");
					
				}

				// Checking participants list thruogh background thread
				if (strcmp(cmd,"CHP") == 0)
				{
					//token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter participants list thruogh background thread\n");
					
					check_background_cal_participant();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit participants list thruogh background thread\n");
					//break;	
				}
				
				// Checking new calendar events  thruogh background thread
				if (strcmp(cmd,"CHC") == 0)
				{
					//token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter calendar background thread\n");
					
					check_background_cal();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("calendar exit background thread\n");
					//break;	
				}
				
				// Sends Available User's list
				if(strcmp(cmd,"UNA")==0)
				{
					
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter UNA\n");
					
					send_usernames();
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("enter UNA\n");
				
				}
				//delete from mail struct
				if (strcmp(cmd,"DEM") == 0)
				{
					token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter DEM\n");
					delete_mail(token);
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit DEM\n");
					
				}
				//delete from cal struct
				if (strcmp(cmd,"DEC") == 0)
				{
					token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter DEC\n");
					delete_cal(token);
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit DEC\n");
					
				}
				
				// Read calendar functionality
				if (strcmp(cmd,"REC") == 0)
				{
					token = strtok(NULL,"~");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					printf("enter read calendaer\n");
					
					read_cal_events(token);
					
					shmdt((void *) shared_user_list);
					semop(semid, &unlock, 1);
					printf("exit from read calendar\n");
					//break;	
				}
				
				// Send Calendar request functionality
				if(strcmp(cmd,"SNC")==0)
				{
					//printf("In SNC if\n");
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					//printf("bzero user list\n");
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					
					//printf("user list\n");

				
					//bzero((char *)&EVENT_ID, sizeof(char)*20);
					EVENT_ID = (char*)malloc(sizeof(char)*20);
					//printf("event id\n");
					//get_event_id();
					//EVENT_ID = 0;
					EVENT_ID = shmat(shmid_event,NULL,0);
					//sprintf(EVENT_ID, "%d", atoi(EVENT_ID));
					//printf("eventid: %s\n", EVENT_ID);
					printf("enter SNC\n");
					
					token = strtok(NULL,"~");
					send_cal_request(token);
					
					shmdt((void *) EVENT_ID);
					shmdt((void *) shared_user_list);
					
					semop(semid, &unlock, 1);
					printf("exit SNC\n");
					
				}
				
				// User logs out.
				if (strcmp(cmd,"OUT") == 0)
				{
					semop(semid, &lock, 1);
					// Attaching shared memory
					bzero((char *)&shared_user_list, sizeof(shared_user_list));
					shared_user_list = (struct user*)shmat(shmid,NULL,0);
					
					//bzero((int)EVENT_ID, sizeof(EVENT_ID));
					//EVENT_ID = (char *)shmat(shmid_event,NULL,0);
					//printf("enter OUT\n");
					
					token = strtok(NULL,"~");
					save_to_config_file(token);
					
					EVENT_ID = (char*)malloc(sizeof(char)*20);
					//printf("event id\n");
					EVENT_ID = shmat(shmid_event,NULL,0);
					//printf("eventid: %s\n", EVENT_ID);
					save_to_event_file();
					shmdt((void *) EVENT_ID);

					exit_mail_service(token);
					
					shmdt((void *) shared_user_list);
					//shmdt((void *) EVENT_ID);
					semop(semid, &unlock, 1);
					printf("exit OUT\n");
					
					break;	
				}
				
			}
			close(cli_sockfd);
			exit(0);
		}
		else
		{
			close(cli_sockfd);
		}
		
	}
}

void read_cal_events(char *input)
{
	/* This function will update read variable of calendar events for the logged in user, if user read a particular event.*/
  //printf("inside function\n");
	char username[50], *token, input_l[50];
	int i = 0, j = 0, k = 0, sqno, read;

	//printf("input:%s\n",input);

	//free(token);
	//bzero((char *)&token,sizeof(token));
		
	strcpy(input_l, input);
	//printf("input :%s\n",input_l);
	
	token = strtok(input_l,"|");
	
	//printf("token %s\n", token);
	//token = strtok(NULL,"|");
	strcpy(username, token);
	
	//printf("username%s\n", username);
	token = strtok(NULL,"|");
	read = atoi(token);
	//printf("read:%d\n",read);
	
	token = strtok(NULL,"|");
	sqno = atoi(token);
	
	//printf("sqno %d\n", sqno);
	
	bzero((char *)&buff,sizeof(buff));

	//print_user_array();
	strcpy(buff,"REC|Read not updated successfully");
	
	//printf("buff:%s\n", buff);
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  if(strcmp(shared_user_list[i].username,username) == 0)
	    {
			for(j = 0; j < 100; j++)
			{
				if(shared_user_list[i].cal_list[j].sqno == 0)
				  break;
			  
				if(shared_user_list[i].cal_list[j].sqno == sqno)
				{
					shared_user_list[i].cal_list[j].read = read;
					strcpy(buff,"REC|Read updated successfully");
					//printf("buff:%s\n", buff);
					break;
				}
			}
			break;
	    }
	}
	//printf("read:%d\n",shared_user_list[i].cal_list[j].read);
	//printf("%s\n",buff);
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  {
	    close(cli_sockfd);
	    perror("Read Error\n");
	    exit(0);
	  }
}

void send_usernames()
{
	// This function will send available users in the system to the client.
	int i;
	char *token,uname[50];
	//printf("***********************");
	token = strtok(NULL,"~");
	strcpy(uname,token);
	//printf("%s\n",uname);
	bzero((char *)&buff,sizeof(buff));
	strcpy(buff,"UNA|");

	for(i=0;i<100;i++)
	{
		//printf("in loop");
		
		if(shared_user_list[i].is_logged_in==0)
			break;
		
		
		/*	if(strcmp(shared_user_list[i].username,uname) == 0)
			{
				i=i+1;
			}*/
			strcat(buff,shared_user_list[i].username);
			strcat(buff,"|");
			//printf("%s",shared_user_list[i].username);
		
	}
	//bzero((char *)&buff1,sizeof(buff1));
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	{
		close(cli_sockfd);
		perror("Read Error\n");
		exit(0);
	}
	
	
		
}

void send_cal_request(char *input)
{
	/* This function will send calendar event request to the recipients mentioned by the logged in user. 
		It will check for recipient's availability. If all recipients are available, then request will be sent to all recipeints including sender.
		If not available, then server will ask client to change date and time. */
	int i = 0, j = 0, k = 0,count,ret = 0,receipient_count, mail_count = 0, user_available = 0, is_modify_event = 0, seq_no = 0, event_id = 0;
	char sendername[50];
    char sfname[50];    
    char slname[50];    
    char start_date[50], temp_start_date[50], temp_start_date1[50];
    char start_time[50], temp_start_time[50], temp_start_time1[50];
    char end_date[50];
    char end_time[50], temp_end_time[50], temp_end_time1[50];
    char place[100];
    char subject[100];
    char message[300];
    int isAttachment;
    char attFileName[50];
    char attachment[200];
	char recipient_list[50][50];
	char *token,rusername[200],username[50], *temp_time;
	char txt[5], input_l[1000];
	int start_date_s[3], end_date_s[3], start_time_s[3], end_time_s[3], start_date_i[3], end_date_i[3], start_time_i[3], end_time_i[3];
	int start_mins_i = 0, start_mins_s = 0, end_mins_i = 0, end_mins_s = 0, temp_count = -1;
	
	//printf("inside function\n");
	//printf("input:%s\n",input);

	//free(token);
	//free(temp_time);
		
	strcpy(input_l, input);
	//printf("inpput_l :%s\n",input_l);
	
	token = strtok(input_l,"|");
	
	//printf("token :%s\n",token);
	// |ramya; pri|

	//copying the receipiant value in two variuables just for checking.
	strcpy(rusername, token);

	//printf("token before for loop:%s\n",token);

	//printf("Rusername:%s\n",rusername);
	
	token = strtok(NULL,"|");
	strcpy(sendername, token);
	
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
		if (strcmp(sendername,shared_user_list[i].username) == 0)
		{
			strcpy(sfname, shared_user_list[i].fname);
			strcpy(slname, shared_user_list[i].lname);
			break;
		}
	}
	
	token = strtok(NULL,"|");
	strcpy(start_date, token);
	//printf("token :%s\n",token);
	
	token = strtok(NULL,"|");
	strcpy(end_date, token);
	//printf("token :%s\n",token);
	
	token = strtok(NULL,"|");
	strcpy(start_time, token);
	//printf("token :%s\n",token);
	
	token = strtok(NULL,"|");
	strcpy(end_time, token);
	//printf("token :%s\n",token);
	
	token = strtok(NULL,"|");
	strcpy(place, token);
	//printf("token :%s\n",token);
	
	token = strtok(NULL,"|");
	strcpy(subject, token);
	//printf("token :%s\n",token);
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].subject);
	
	token = strtok(NULL,"|");
	strcpy(message, token);
	//printf("token :%s\n",token);
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].message);

	token = strtok(NULL,"|");
	isAttachment = atoi(token);
	//printf("token :%s\n",token);
	//printf("token1:%d\n",shared_user_list[mail_count].mail_list[i].isAttachment);

	if(isAttachment == 1)
	{
		token = strtok(NULL,"|");
		strcpy(attFileName, token);
		token = strtok(NULL,"|");
		strcpy(attachment, token);
	}
	
	token = strtok(NULL,"|");
	is_modify_event = atoi(token);
	printf("is_modify_event :%d\n",is_modify_event);
	//printf("token1:%d\n",shared_user_list[mail_count].mail_list[i].isAttachment);

	if(is_modify_event == 2)
	{
		token = strtok(NULL,"|");
		seq_no = atoi(token);
		token = strtok(NULL,"|");
		event_id = atoi(token);
	}
 
	//free(token);
	//bzero((char *)&token,sizeof(token));
	token = strtok(rusername,";");
	
	//printf("receipients :%s\n",token);
	receipient_count = -1;
	while(token != NULL)
	{
		//printf("token:%s\n", token);
		receipient_count++;
		//printf("recipient count:%d\n", receipient_count);
		strcpy(recipient_list[receipient_count], token);
		token = strtok(NULL,";");
	}
	printf("recipient count:%d\n", receipient_count);
	for(k = 0; k <= receipient_count; k++)
	{		
		printf("recipient list : %s\n", recipient_list[k]);
	}
	
	//printf("exit from for loop\n");

	//token = strtok(NULL,"|");
	// input start date
	//free(token);
	//bzero((char *)&token,sizeof(token));
	strcpy(temp_start_date1,start_date);
	token = strtok(temp_start_date1,"/");

	//printf("token:%s\n", token);
	
	temp_count = -1;
	// Copying Month, day and year of an event in an array.
	while(token != NULL)
	{
		//printf("token:%s\n", token);
		temp_count++;
		//printf("temp count:%d\n", temp_count);
		start_date_i[temp_count] =  atoi(token);
		//printf("start TIME :%d\n", start_date_i[temp_count]);
		token = strtok(NULL,"/");
	}
	
	// input start time
	//free(token);
	//bzero((char *)&token,sizeof(token));
	strcpy(temp_start_time1,start_time);
	token = strtok(temp_start_time1,":");

	//printf("token:%s\n", token);
	
	temp_count = -1;
	
	// Copying hours, minutes and seconds of start time of an event in an array.
	while(token != NULL)
	{
		//printf("token:%s\n", token);
		temp_count++;
		//printf("temp count:%d\n", temp_count);
		start_time_i[temp_count] =  atoi(token);
		//printf("start TIME :%d\n", start_time_i[temp_count]);
		token = strtok(NULL,":");
	}
	
	
	// input end time
	//free(token);
	//bzero((char *)&token,sizeof(token));
	strcpy(temp_end_time1,end_time);
	token = strtok(temp_end_time1,":");

	//printf("token:%s\n", token);
	
	// Copying hours, minutes and seconds of end time of an event in an array.
	temp_count = -1;
	while(token != NULL)
	{
		//printf("token:%s\n", token);
		temp_count++;
		//printf("temp count:%d\n", temp_count);
		end_time_i[temp_count] =  atoi(token);
		//printf("start TIME :%d\n", end_time_i[temp_count]);
		token = strtok(NULL,":");
	}
	/*
	memcpy(start_date_i, get_date(start_date), sizeof(start_date_i));
	memcpy(end_date_i, get_date(end_date), sizeof(end_date_i));
	memcpy(start_time_i, get_time(start_time), sizeof(start_time_i));
	memcpy(end_time_i, get_time(end_time), sizeof(end_time_i));*/
	
	/*printf("start date :%d\n", start_date_i[0]);
	printf("start date :%d\n", start_date_i[1]);
	printf("start date :%d\n", start_date_i[2]);
	*/
	//printf("start time :%d\n", start_time_i[0]);
	//printf("start time :%d\n", start_time_i[1]);
	//printf("start time :%d\n", start_time_i[2]);
	
	// converting hours into minutes.
	start_mins_i = (start_time_i[0] * 60) + start_time_i[1];
	end_mins_i = (end_time_i[0] * 60) + end_time_i[1];
	
	
	/*printf("start min i:%d\n", start_mins_i);
	printf("end min i:%d\n", end_mins_i);*/
	
	if(is_modify_event == 2)
		{
			// If client requests for modifying event, then delete old event from all recipeint's array.
			//printf("modified and deleting from server structure\n");
			delete_old_cal_event(recipient_list, event_id, receipient_count);
			//printf("modified and deleting from server structure\n");
			// delete old Event : Apexa
			// delete_old_event(recipient_list[],seq_no, event_id, receipient_count);
		}
	for(k = 0; k <= receipient_count; k++)
	{		
		for(i = 0; i < 100; i++)
		{
			if(shared_user_list[i].is_logged_in == 0)
				break;
			if (strcmp(recipient_list[k],shared_user_list[i].username) == 0)
			{
				for(j = 0; j < 100; j++)
				{
					if(shared_user_list[i].cal_list[j].sqno == 0)
					{
						if(j == 0)
							user_available = 1;
						break;
					}
					
					//printf("start date:%s\n",shared_user_list[i].cal_list[j].start_date);
					//printf("inside for\n");
					
					//printf("seq no:%d\n",shared_user_list[i].cal_list[j].sqno);
					//printf("user_available:%d\n",user_available);
					// structure start date
					//free(token);
					//bzero((char *)&token,sizeof(token));
					strcpy(temp_start_date,shared_user_list[i].cal_list[j].start_date);
					token = strtok(temp_start_date,"/");
					//printf("token:%s", token);
					
					temp_count = -1;
					// Copying Month, day and year of an event in an array.
					while(token != NULL)
					{
						//printf("token:%s\n", token);
						temp_count++;
						//printf("temp count:%d\n", temp_count);
						//sprintf(token, "%d", atoi(token));
						start_date_s[temp_count] =  atoi(token);
						//printf("start date :%d\n", start_date_s[temp_count]);
						token = strtok(NULL,"/");
					}
					
					//memcpy(start_date_s,get_date(shared_user_list[i].cal_list[j].start_date),sizeof(start_date_s));
					/*
					printf("start date :%d\n", start_date_s[0]);
					printf("start date :%d\n", start_date_s[1]);
					printf("start date :%d\n", start_date_s[2]);
					
					*/
					// structure start time
					//free(token);
					//bzero((char *)&token,sizeof(token));
					strcpy(temp_start_time,shared_user_list[i].cal_list[j].start_time);
					token = strtok(temp_start_time,":");

					//printf("token:%s\n", token);
					
					temp_count = -1;
					// Copying hours, minutes and seconds of start time of an event in an array.
					while(token != NULL)
					{
						//printf("token:%s\n", token);
						temp_count++;
						//printf("temp count:%d\n", temp_count);
						start_time_s[temp_count] =  atoi(token);
						//printf("start TIME :%d\n", start_time_s[temp_count]);
						token = strtok(NULL,":");
					}
					
						
					//printf("start time :%d\n", start_time_s[0]);
					//printf("start time :%d\n", start_time_s[1]);
					//printf("start time :%d\n", start_time_s[2]);
					
					// structure end time
					//free(token);
					//bzero((char *)&token,sizeof(token));
					strcpy(temp_end_time,shared_user_list[i].cal_list[j].end_time);
					token = strtok(temp_end_time,":");

					//printf("token:%s\n", token);
					
					temp_count = -1;
					// Copying hours, minutes and seconds of end time of an event in an array.
					while(token != NULL)
					{
						//printf("token:%s\n", token);
						temp_count++;
						//printf("temp count:%d\n", temp_count);
						end_time_s[temp_count] =  atoi(token);
						//printf("start TIME :%d\n", end_time_s[temp_count]);
						token = strtok(NULL,":");
					}
					
					// converting hours into minutes.
					start_mins_s = (start_time_s[0] * 60) + start_time_s[1];
					end_mins_s = (end_time_s[0] * 60) + end_time_s[1];
					
					/*
					printf("start min s:%d\n", start_mins_s);
					printf("end min s:%d\n", end_mins_s);*/
					/* Checking for recipient's availability. It checks for same date, if date is same, then checking time.
					if clashes, then ask client to change date and time. Else, send event invitation to all recipients. */
					if(start_date_i[0] == start_date_s[0] &&
						start_date_i[1] == start_date_s[1] &&
						start_date_i[2] == start_date_s[2])  
						// Check user's array and input dates are same
					{
						//printf("checkimg date\n");
						if(end_mins_i < start_mins_s || 
						   start_mins_i > end_mins_s)
						   
						{
							//printf("checking time\n");
								user_available = 1;
								//printf("same date user available : %d\n", user_available);
						}
						else
						{
							user_available = 2;
							//printf("same date user not available : %d\n", user_available);
						}
						
					}
					else
					{
						user_available = 1;
						//printf("different date user available : %d\n", user_available);
					}
					
					if(user_available == 2)
					{
						// Send to client to change date and time.
						//printf("User is not available\n");
						
						bzero((char *)&buff,sizeof(buff));
						strcpy(buff,"SNC|2");
						if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
						{
							close(cli_sockfd);
							perror("Read Error\n");
							exit(0);
						}
						break;
					}
				}
			}
			
		if(user_available == 2)
			break;
		}
		if(user_available == 2)
			break;
	}
	
	if (user_available == 1)
	{
		// If user is available, send invitation to all receipients.
		//printf("If user is available\n");
		sprintf(EVENT_ID, "%d", atoi(EVENT_ID) + 1);
		//EVENT_ID = EVENT_ID + 1;
		for(k = 0; k <= receipient_count; k++)
		{		
			for(i = 0; i < 100; i++)
			{
				if(shared_user_list[i].is_logged_in == 0)
					break;
				if (strcmp(recipient_list[k],shared_user_list[i].username) == 0)
				{
					for(j = 0; j < 100; j++)
					{
						if(shared_user_list[i].cal_list[j].sqno == 0)
						{
							shared_user_list[i].cal_list[j].event_id = atoi(EVENT_ID);
							//printf("event id:%d\n",shared_user_list[i].cal_list[j].event_id);
							
							shared_user_list[i].cal_list[j].sqno = j + 1;
							//printf("sqno:%d\n",shared_user_list[i].cal_list[j].sqno);

							//printf("token in for loop:%s\n",token);			
				
							//if(token == NULL)
								//printf("token NULL\n");
							strcpy(shared_user_list[i].cal_list[j].sendername, sendername);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].sendername);

							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].cal_list[j].sfname, sfname);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].sfname);

							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].cal_list[j].slname, slname);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].slname);
							
							strcpy(shared_user_list[i].cal_list[j].start_date, start_date);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].start_date);
							
							strcpy(shared_user_list[i].cal_list[j].start_time, start_time);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].start_time);
							
							
							strcpy(shared_user_list[i].cal_list[j].end_date, end_date);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].end_date);
							
							strcpy(shared_user_list[i].cal_list[j].end_time, end_time);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].end_time);
							
							
							strcpy(shared_user_list[i].cal_list[j].place, place);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].place);
							
									
							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].cal_list[j].subject, subject);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].subject);
							
							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].cal_list[j].message, message);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].message);

							//token = strtok(NULL,"|");
							shared_user_list[i].cal_list[j].isAttachment = isAttachment;
							//printf("token1:%d\n",shared_user_list[i].cal_list[j].isAttachment);

							if(shared_user_list[i].cal_list[j].isAttachment == 1)
							{
								//token = strtok(NULL,"|");
								strcpy(shared_user_list[i].cal_list[j].attFileName, attFileName);
								//token = strtok(NULL,"|");
								strcpy(shared_user_list[i].cal_list[j].attachment, attachment);
							}
							shared_user_list[i].cal_list[j].read = 0;
							//printf("token1:%d\n",shared_user_list[i].cal_list[j].read);
							shared_user_list[i].cal_list[j].no_of_participants = receipient_count;
							//printf("token1:%d\n",shared_user_list[i].cal_list[j].no_of_participants);
							memcpy(shared_user_list[i].cal_list[j].participants_list, recipient_list, sizeof(recipient_list));
							int k = 0;
							for(k = 0; k <= receipient_count; k++)
							{
								printf("shared_user_list[i].cal_list[j].participants_list: %s\n",shared_user_list[i].cal_list[j].participants_list[k]);
							}

							//printf("time:%s\n",asctime( localtime(&ltime) ));
							//temp_time = asctime( localtime(&ltime) );
							//if(temp_time == NULL)
								//printf("Time NULL\n");

							//strcpy(shared_user_list[i].cal_list[j].dateTime, dateTime);
							//printf("token1:%s\n",shared_user_list[i].cal_list[j].dateTime);
							
							//count=i;
							break;
						}
					}
				}
			}
		}
		//printf("habdahdu\n");
		/*if(is_modify_event == 2)
		{
			// If client requests for modifying event, then delete old event from all recipeint's array.
			//printf("modified and deleting from server structure\n");
			delete_old_cal_event(recipient_list, event_id, receipient_count);
			//printf("modified and deleting from server structure\n");
			// delete old Event : Apexa
			// delete_old_event(recipient_list[],seq_no, event_id, receipient_count);
		}*/
		//printf("event is modified and deleted from server struct and now passing flag to delete from client structure\n");
		bzero((char *)&buff,sizeof(buff));
		strcpy(buff,"SNC|1");
		if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
		{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
		}
	}
	
	
}


void delete_mail(char *input)
{
	int i,j,sqno;
	char *token,username[50], input_l[50];
	//free(token);
	printf("token :%s\n",input);
	strcpy(input_l, input);
	
	printf("input_l :%s\n",input_l);
	token = strtok(input_l,"|");
	printf("token :%s\n",token);
	
	//token = strtok(NULL,"|");
	strcpy(username,token);
	printf("%s\n",username);
	
	//bzero((char *)&token,sizeof(token));
	//free(token);
	
	token = strtok(NULL,"|");
	printf("sqno:%s",token);
	sqno = atoi(token);
	printf("%d\n",sqno);
	
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  //printf("user:%s\n",shared_user_list[i].username);
	  if(strcmp(shared_user_list[i].username,username) == 0)
	    {
	
						for(j=0 ; j < 100 ; j++)
						{
							if(shared_user_list[i].mail_list[j].sqno == sqno)
							{
								while(shared_user_list[i].mail_list[j+1].sqno!=0)
								{
									
									
									shared_user_list[i].mail_list[j].read = shared_user_list[i].mail_list[j+1].read;
									strcpy(shared_user_list[i].mail_list[j].sendername , shared_user_list[i].mail_list[j+1].sendername);
									strcpy(shared_user_list[i].mail_list[j].sfname , shared_user_list[i].mail_list[j+1].sfname);
									strcpy(shared_user_list[i].mail_list[j].slname , shared_user_list[i].mail_list[j+1].slname);
									strcpy(shared_user_list[i].mail_list[j].dateTime , shared_user_list[i].mail_list[j+1].dateTime);
									strcpy(shared_user_list[i].mail_list[j].subject , shared_user_list[i].mail_list[j+1].subject);
									strcpy(shared_user_list[i].mail_list[j].message , shared_user_list[i].mail_list[j+1].message);
									shared_user_list[i].mail_list[j].isAttachment = shared_user_list[i].mail_list[j+1].isAttachment;
									strcpy(shared_user_list[i].mail_list[j].attFileName , shared_user_list[i].mail_list[j+1].attFileName);
									strcpy(shared_user_list[i].mail_list[j].attachment , shared_user_list[i].mail_list[j+1].attachment);
									j++;
									
								}
								
								shared_user_list[i].mail_list[j].sqno = 0;
								shared_user_list[i].mail_list[j].read = 0; 
								strcpy(shared_user_list[i].mail_list[j].sendername , "");
								strcpy(shared_user_list[i].mail_list[j].sfname , "");
								strcpy(shared_user_list[i].mail_list[j].slname , ""); 
								strcpy(shared_user_list[i].mail_list[j].dateTime , "");
								strcpy(shared_user_list[i].mail_list[j].subject , ""); 
								strcpy(shared_user_list[i].mail_list[j].message , ""); 
								shared_user_list[i].mail_list[j].isAttachment = 0; 
								strcpy(shared_user_list[i].mail_list[j].attFileName , ""); 
								strcpy(shared_user_list[i].mail_list[j].attachment , "");
								
								
								break;
								
							}
						}
						
						for(j = 0; j < 100; j++)
						{
								if(shared_user_list[i].mail_list[j].sqno == 0)
								break;
									printf("\n\nSequence Number:%d\nSender's Name:%s\nFisrt Name:%s\nLast Name:%s\ndateTime:%s\nSubject:%s\nMessage:%s\nRead:%d\n",
											shared_user_list[i].mail_list[j].sqno,
											shared_user_list[i].mail_list[j].sendername, 
											shared_user_list[i].mail_list[j].sfname,
											shared_user_list[i].mail_list[j].slname,
											shared_user_list[i].mail_list[j].dateTime,
											shared_user_list[i].mail_list[j].subject, 
											shared_user_list[i].mail_list[j].message, 
											shared_user_list[i].mail_list[j].read);
								if(shared_user_list[i].mail_list[j].isAttachment == 1)
								{
									printf("%s\n%s\n",shared_user_list[i].mail_list[j].attFileName, shared_user_list[i].mail_list[j].attachment);
								}

						}
	
	
		}
	}
}



void delete_cal(char *input)
{
	int i,j,r,k,point,sqno,temp_event_id,temp_no_part,flag=0;
	char *token,username[50],temp_part_list[50][50],input_l[50];
	
	//free(token);
	printf("token :%s\n",input);
	strcpy(input_l, input);
	
	printf("input_l :%s\n",input_l);
	token = strtok(input_l,"|");
	printf("token :%s\n",token);
	
	//token = strtok(NULL,"|");
	strcpy(username,token);
	printf("%s\n",username);
	
	//bzero((char *)&token,sizeof(token));
	//free(token);
	
	token = strtok(NULL,"|");
	printf("sqno:%s",token);
	sqno = atoi(token);
	printf("%d\n",sqno);
	
	
	for(i = 0; i < 100; i++)
	{
		
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  //printf("user:%s\n",shared_user_list[i].username);
	  printf("i=%d\n",i);
		if(strcmp(shared_user_list[i].username,username) == 0)
	    {
			flag=1;
	
						for(j=0 ; j < 100 ; j++)
						{
							if(shared_user_list[i].cal_list[j].sqno == 0)
							break;
							if(shared_user_list[i].cal_list[j].sqno == sqno)
							{
								temp_event_id = shared_user_list[i].cal_list[j].event_id;
								temp_no_part = shared_user_list[i].cal_list[j].no_of_participants;
								for(r = 0; r <= shared_user_list[i].cal_list[j].no_of_participants; r++)
								{
											 strcpy(temp_part_list[r] , shared_user_list[i].cal_list[j].participants_list[r]);
								}
								
								while(shared_user_list[i].cal_list[j].event_id != shared_user_list[i].cal_list[j+1].event_id && shared_user_list[i].cal_list[j+1].sqno!=0)
								{
									
									
									shared_user_list[i].cal_list[j].event_id = shared_user_list[i].cal_list[j+1].event_id;
									shared_user_list[i].cal_list[j].read = shared_user_list[i].cal_list[j+1].read;
									strcpy(shared_user_list[i].cal_list[j].sendername , shared_user_list[i].cal_list[j+1].sendername);
									strcpy(shared_user_list[i].cal_list[j].sfname , shared_user_list[i].cal_list[j+1].sfname);
									strcpy(shared_user_list[i].cal_list[j].slname , shared_user_list[i].cal_list[j+1].slname);
									strcpy(shared_user_list[i].cal_list[j].start_date , shared_user_list[i].cal_list[j+1].start_date);
									strcpy(shared_user_list[i].cal_list[j].start_time , shared_user_list[i].cal_list[j+1].start_time);
									strcpy(shared_user_list[i].cal_list[j].end_date , shared_user_list[i].cal_list[j+1].end_date);
									strcpy(shared_user_list[i].cal_list[j].end_time , shared_user_list[i].cal_list[j+1].end_time);
									strcpy(shared_user_list[i].cal_list[j].place , shared_user_list[i].cal_list[j+1].place);
									strcpy(shared_user_list[i].cal_list[j].subject , shared_user_list[i].cal_list[j+1].subject);
									strcpy(shared_user_list[i].cal_list[j].message , shared_user_list[i].cal_list[j+1].message);
									shared_user_list[i].cal_list[j].isAttachment = shared_user_list[i].cal_list[j+1].isAttachment;
									shared_user_list[i].cal_list[j].no_of_participants = shared_user_list[i].cal_list[j+1].no_of_participants;
									for(r = 0; r <= shared_user_list[i].cal_list[j+1].no_of_participants; r++)
									{
											strcpy(shared_user_list[i].cal_list[j].participants_list[r] , shared_user_list[i].cal_list[j+1].participants_list[r]);
									}
									j++;
									
								}
								
								shared_user_list[i].cal_list[j].sqno = 0;
								shared_user_list[i].cal_list[j].event_id = 0;
								shared_user_list[i].cal_list[j].read = 0; 
								strcpy(shared_user_list[i].cal_list[j].sendername , "");
								strcpy(shared_user_list[i].cal_list[j].sfname , "");
								strcpy(shared_user_list[i].cal_list[j].slname , ""); 
								strcpy(shared_user_list[i].cal_list[j].start_date , "");
								strcpy(shared_user_list[i].cal_list[j].start_time , "");
								strcpy(shared_user_list[i].cal_list[j].end_date , "");
								strcpy(shared_user_list[i].cal_list[j].end_time , "");
								strcpy(shared_user_list[i].cal_list[j].place , "");
								strcpy(shared_user_list[i].cal_list[j].subject , ""); 
								strcpy(shared_user_list[i].cal_list[j].message , ""); 
								shared_user_list[i].cal_list[j].isAttachment = 0; 
								shared_user_list[i].cal_list[j].no_of_participants = 0;
								for(r = 0; r <= shared_user_list[i].cal_list[j].no_of_participants; r++)
								{
									strcpy(shared_user_list[i].cal_list[j].participants_list[r] , "");
								}
								
								
								break;
								
							}
						}
						
						for(j = 0; j < 100; j++)
						{	
					
							if(shared_user_list[i].cal_list[j].sqno == 0)
							break;
							printf("------------------Calender Appointment:%d-------------------",j+1);
				
							printf("%d\n%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n",shared_user_list[i].cal_list[j].event_id, 
																					shared_user_list[i].cal_list[j].sqno, 
																					shared_user_list[i].cal_list[j].read, 
																					shared_user_list[i].cal_list[j].sendername,
																					shared_user_list[i].cal_list[j].sfname,
																					shared_user_list[i].cal_list[j].slname, 
																					shared_user_list[i].cal_list[j].start_date,
																					shared_user_list[i].cal_list[j].start_time,
																					shared_user_list[i].cal_list[j].end_date,
																					shared_user_list[i].cal_list[j].end_time,
																					shared_user_list[i].cal_list[j].place,
																					shared_user_list[i].cal_list[j].subject, 
																					shared_user_list[i].cal_list[j].message, 
																					shared_user_list[i].cal_list[j].isAttachment, 
																					shared_user_list[i].cal_list[j].no_of_participants);
							if(shared_user_list[i].cal_list[j].isAttachment == 1)
							{
									printf("%s\n%s\n",shared_user_list[i].cal_list[j].attFileName, shared_user_list[i].cal_list[j].attachment);
							}
							for(r = 0; r <= shared_user_list[i].cal_list[j].no_of_participants; r++)
							{
								printf("%s\n",shared_user_list[i].cal_list[j].participants_list[r]);
							}

							
						}
					
			
		}
		//break;
		if(flag==1)
		{
			break;
		}
		//printf("one iteration\n");
		
	}
	
	
	printf("deleted from server\n");
	printf("no of participant:%d",temp_no_part);
	for(k=0 ; k <= temp_no_part ; k++)
	{
		printf("k=%d\n",k);
		printf("OUT IFF temp_part_list[%d]=%s\n",k,temp_part_list[k]);
		if(strcmp(temp_part_list[k],username)!=0)
		{
			printf("IN IFF temp_part_list[%d]=%s\n",k,temp_part_list[k]);
			for(i=0 ; i < 100 ; i++)
			{
				if(shared_user_list[i].is_logged_in == 0)
				break;
				printf("i=%d\n",i);
				if(strcmp(temp_part_list[k],shared_user_list[i].username) == 0)
				{
					printf("found username in userlist\n");
					for(j=0 ; j < 100 ; j++)
					{
						printf("j=%d\n",j);
						if(shared_user_list[i].cal_list[j].sqno == 0)
						break;
					    printf("temp_evet_id=%d\n",temp_event_id);
						if(shared_user_list[i].cal_list[j].event_id == temp_event_id)
						{
							printf("found event id in cal=%d\n",shared_user_list[i].cal_list[j].event_id);
							for(r=0 ; r <= shared_user_list[i].cal_list[j].no_of_participants ; r++)
							{
								if(strcmp(shared_user_list[i].cal_list[j].participants_list[r],username)==0)
								{
									printf("participant found at position=%d\n",r);
										point = r;
										break;
								}
								
							}
							printf("point=%d\n",point);
							while(strlen(shared_user_list[i].cal_list[j].participants_list[point]) > 0)
							{
								printf("one stepped move in front string:%s\n",shared_user_list[i].cal_list[j].participants_list[point+1]);
								strcpy(shared_user_list[i].cal_list[j].participants_list[point],shared_user_list[i].cal_list[j].participants_list[point+1]);
								point++;
								printf("point:%d\n",point);
							}
							strcpy(shared_user_list[i].cal_list[j].participants_list[point],"");
							break;
						}
					
					}
					break;
				}
				
				
			}
			

		}
		
	}
	
	
	
}





void delete_old_cal_event(char recipient_list[][50], int event_id, int receipient_count)
{
	/* This function will delete old event if client requests for modify event. */
	int i = 0, j = 0, k = 0, r = 0;
	/*printf("jsnfdjkmsl\n");
	printf("receipient_count=%d\n",receipient_count);
	printf("event_id=%d\n",event_id);*/
		for(k = 0; k <= receipient_count; k++)
		{		
		
			//printf("k=%d\n",k);
			
			for(i = 0; i < 100; i++)
			{
				//if(shared_user_list[i].is_logged_in == 0)
					//break;
				//printf("i=%d\n",i);
				if (strcmp(recipient_list[k],shared_user_list[i].username) == 0)
				{
					//printf("receipiant:%s\n",recipient_list[k]);
					
					for(j = 0; j < 100; j++)
					{
						//printf("j=%d\n",j);
						if(shared_user_list[i].cal_list[j].sqno == 0)
							break;
						//printf("event_id in struct=%d\n",shared_user_list[i].cal_list[j].event_id);
						if(shared_user_list[i].cal_list[j].event_id == event_id)
						{
							while(shared_user_list[i].cal_list[j].event_id != shared_user_list[i].cal_list[j+1].event_id && shared_user_list[i].cal_list[j+1].sqno!=0)
							{
								shared_user_list[i].cal_list[j].event_id = shared_user_list[i].cal_list[j+1].event_id;
								shared_user_list[i].cal_list[j].read = shared_user_list[i].cal_list[j+1].read;
								strcpy(shared_user_list[i].cal_list[j].sendername , shared_user_list[i].cal_list[j+1].sendername);
								strcpy(shared_user_list[i].cal_list[j].sfname , shared_user_list[i].cal_list[j+1].sfname);
								strcpy(shared_user_list[i].cal_list[j].slname , shared_user_list[i].cal_list[j+1].slname);
								strcpy(shared_user_list[i].cal_list[j].start_date , shared_user_list[i].cal_list[j+1].start_date);
								strcpy(shared_user_list[i].cal_list[j].start_time , shared_user_list[i].cal_list[j+1].start_time);
								strcpy(shared_user_list[i].cal_list[j].end_date , shared_user_list[i].cal_list[j+1].end_date);
								strcpy(shared_user_list[i].cal_list[j].end_time , shared_user_list[i].cal_list[j+1].end_time);
								strcpy(shared_user_list[i].cal_list[j].place , shared_user_list[i].cal_list[j+1].place);
								strcpy(shared_user_list[i].cal_list[j].subject , shared_user_list[i].cal_list[j+1].subject);
								strcpy(shared_user_list[i].cal_list[j].message , shared_user_list[i].cal_list[j+1].message);
								shared_user_list[i].cal_list[j].isAttachment = shared_user_list[i].cal_list[j+1].isAttachment;
								shared_user_list[i].cal_list[j].no_of_participants = shared_user_list[i].cal_list[j+1].no_of_participants;
								for(r = 0; r <= shared_user_list[i].cal_list[j+1].no_of_participants; r++)
								{
										strcpy(shared_user_list[i].cal_list[j].participants_list[r] , shared_user_list[i].cal_list[j+1].participants_list[r]);
								}
								j++;
								
							}
							
							shared_user_list[i].cal_list[j].sqno = 0;
							shared_user_list[i].cal_list[j].event_id = 0;
							shared_user_list[i].cal_list[j].read = 0; 
							strcpy(shared_user_list[i].cal_list[j].sendername , "");
							strcpy(shared_user_list[i].cal_list[j].sfname , "");
							strcpy(shared_user_list[i].cal_list[j].slname , ""); 
							strcpy(shared_user_list[i].cal_list[j].start_date , "");
							strcpy(shared_user_list[i].cal_list[j].start_time , "");
							strcpy(shared_user_list[i].cal_list[j].end_date , "");
							strcpy(shared_user_list[i].cal_list[j].end_time , "");
							strcpy(shared_user_list[i].cal_list[j].place , "");
							strcpy(shared_user_list[i].cal_list[j].subject , ""); 
							strcpy(shared_user_list[i].cal_list[j].message , ""); 
							shared_user_list[i].cal_list[j].isAttachment = 0; 
							shared_user_list[i].cal_list[j].no_of_participants = 0;
							for(r = 0; r <= shared_user_list[i].cal_list[j].no_of_participants; r++)
							{
								strcpy(shared_user_list[i].cal_list[j].participants_list[r] , "");
							}
							
						}
					}
					break;
				}
			}
		}
		
		/*
		for(k = 0; k <= receipient_count; k++)
		{	
			printf("*****************************Recipient:%d************************************",k+1);
			for(i = 0; i < 100; i++)
			{
				//if(shared_user_list[i].is_logged_in == 0)
					//break;
				if (strcmp(recipient_list[k],shared_user_list[i].username) == 0)
				{
					for(j = 0; j < 100; j++)
					{
						if(shared_user_list[i].cal_list[j].sqno == 0)
						break;
			
			
						printf("%d\n%d\n%d\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%s\n%d\n%d\n",shared_user_list[i].cal_list[j].event_id, 
																				shared_user_list[i].cal_list[j].sqno, 
																				shared_user_list[i].cal_list[j].read, 
																				shared_user_list[i].cal_list[j].sendername,
																				shared_user_list[i].cal_list[j].sfname,
																				shared_user_list[i].cal_list[j].slname, 
																				shared_user_list[i].cal_list[j].start_date,
																				shared_user_list[i].cal_list[j].start_time,
																				shared_user_list[i].cal_list[j].end_date,
																				shared_user_list[i].cal_list[j].end_time,
																				shared_user_list[i].cal_list[j].place,
																				shared_user_list[i].cal_list[j].subject, 
																				shared_user_list[i].cal_list[j].message, 
																				shared_user_list[i].cal_list[j].isAttachment, 
																				shared_user_list[i].cal_list[j].no_of_participants);
						if(shared_user_list[i].cal_list[j].isAttachment == 1)
						{
								printf("%s\n%s\n",shared_user_list[i].cal_list[j].attFileName, shared_user_list[i].cal_list[j].attachment);
						}
						for(r = 0; r < shared_user_list[i].cal_list[j].no_of_participants; r++)
						{
							printf("%s\n",shared_user_list[i].cal_list[j].participants_list[r]);
						}
						printf("------------------Calender Appointment:%d-------------------",j+1);
					}
					break;
				}
				
				
			}
			
			
		}*/
		
		//printf("delete close\n");
		
}




/*int* get_date(char *input_date)
{
	printf("inisde function");
	char *token;
	int temp_count = -1, temp_date[3];
	/*
	struct tm tm1;
	
	char *date1;
	//date1 = malloc(strlen("03/25/2016")+1);
	date1 = "03/25/2016";
	printf("Time1:%s",date1);
	printf("Before strptime\n");
	
	strptime(date1,"%m/%d/%Y",&tm1);

	printf("date_st:%d\n", tm1.tm_mday);
	printf("ddone");
	
	
	
	printf("input date:%s", input_date);
	free(token);
	token = strtok(input_date,"/");

	
	printf("token:%s", token);
	
	
	//temp_count = -1;
	while(token != NULL)
	{
		printf("token:%s\n", token);
		temp_count++;
		printf("temp count:%d\n", temp_count);
		temp_date[temp_count] =  atoi(token);
		token = strtok(NULL,"/");
	}
	return(temp_date);
}

int* get_time(char input_time[])
{
	char *token;
	int temp_count = -1, temp_time[3];

		printf("input time:%s", input_time);
	free(token);
	token = strtok(input_time,":");

	printf("token:%s", token);
	//temp_count = -1;
	while(token != NULL)
	{
		printf("token:%s\n", token);
		temp_count++;
		printf("temp count:%d\n", temp_count);
		temp_time[temp_count] =  atoi(token);
		token = strtok(NULL,":");
	}
	return(temp_time);
	/*
	struct tm time_st;	
	printf("reached time_st\n");
	strptime(time ,"%H:%M:%S",&time_st);
	return(time_st);
	
}*/

void check_background_mail()
{
	/* This function will be called by background thread. 
	It will check for new mails for particular username, and if it is available, pass new mails details to the client. */
	int i = 0,j = 0,k = 0,client_count = 0,server_count = 0,count_diff = 0;
	char username[50],*token,sqno[8],isAttach[2],str_server_count[8],read[2];
	//free(token);
	//bzero((char *)&token,sizeof(token));
	//printf("inside mail backgroubd\n");
	
	token = strtok(NULL,"|");
	//printf("token:%s\n", token);
	client_count = atoi(token);
	token = strtok(NULL,"|");
	//printf("token:%s\n", token);
	strcpy(username, token);
	//printf("username:%s\n", username);
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
		//printf("user:%s\n",shared_user_list[i].username);
		if(strcmp(shared_user_list[i].username,username) == 0)
	    	{
	      
			for(j = 0; j < 100; j++)
			{
				
				if(shared_user_list[i].mail_list[j].sqno == 0)
				{
					break;
				}
				server_count++;
				//printf("server count:%d\n", server_count);
			}
		break;
		
	    	}
	  
	}
	count_diff = server_count - client_count;
	//printf("count diff:%d\n",count_diff);
	bzero((char *)&buff,sizeof(buff));
	if(count_diff > 0)
	{
		for(i = 0; i < 100; i++)
		{
			if(shared_user_list[i].is_logged_in == 0)
				break;
			if(strcmp(shared_user_list[i].username,username) == 0)
	    		{		
					//shared_user_list[i].is_logged_in = 1;
					strcpy(buff,"CHK|");
					sprintf(str_server_count,"%d",server_count);
					strcat(buff,str_server_count);
					strcat(buff,"|");
					for(k = client_count ; k < server_count ; k++)//k=server_count; k>client_count; k--
					{
						//printf("k:%d\n",k);
						if(shared_user_list[i].mail_list[k].sqno == 0)
						break;
			
						//printf("sqno:%d\n",shared_user_list[i].mail_list[k].sqno);
						
						
						sprintf(sqno,"%d",shared_user_list[i].mail_list[k].sqno);
						strcat(buff,sqno);
						strcat(buff,"|");

						//printf("read:%d\n",shared_user_list[i].mail_list[k].read);
						sprintf(read,"%d",shared_user_list[i].mail_list[k].read);
						strcat(buff,read);
						strcat(buff,"|");

						//printf("sender:%s\n\n",shared_user_list[i].mail_list[k].sendername);
						strcat(buff,shared_user_list[i].mail_list[k].sendername);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].mail_list[k].sfname);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].mail_list[k].slname);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].mail_list[k].dateTime);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].mail_list[k].subject);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].mail_list[k].message);
						strcat(buff,"|");
						sprintf(isAttach,"%d",shared_user_list[i].mail_list[k].isAttachment);
						strcat(buff,isAttach);
						strcat(buff,"|");
						if(shared_user_list[i].mail_list[k].isAttachment == 1)
						{
							strcat(buff,shared_user_list[i].mail_list[k].attFileName);
							strcat(buff,"|");
							strcat(buff,shared_user_list[i].mail_list[k].attachment);
							strcat(buff,"|~");
						}
						else
						{
							strcat(buff,"~");
						}
						//printf("%d\n",k);
						//printf("%s\n",buff);
					}
					//printf("exited for loop:%d\n",k);
				break;
			}
			/*else
			{
				printf("Else Part\n");
			}*/
		}
		//printf("%s\n",buff);
		if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  	{
	    		close(cli_sockfd);
	    		perror("Read Error\n");
	    		exit(0);
	  	}
	}
	else
	{
		
		strcpy(buff,"CHK|");
		sprintf(str_server_count,"%d",server_count);
		strcat(buff,str_server_count);
		strcat(buff,"|");
		//printf("%s\n",buff);
		if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  	{
	    		close(cli_sockfd);
	    		perror("Read Error\n");
	    		exit(0);
	  	}
	}
	
}

void check_background_cal_participant()
{
	/* This function will be called by background thread running at client. 
	If user deletes any calendar event, then that username will be deleted from recipeints list. 
	So this function will check such events, and return to the client. */
	int sqno, no_of_part, i, j, k;
	char username[50],*token, str_no_of_part[8];
	//free(token);
	//bzero((char *)&token,sizeof(token));
	token = strtok(NULL,"|");
	no_of_part = atoi(token);
	token = strtok(NULL,"|");
	sqno = atoi(token);
	token = strtok(NULL,"|");
	strcpy(username, token);
	//printf("username:%s", username);
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
		printf("user:%s\n",shared_user_list[i].username);
		if(strcmp(shared_user_list[i].username,username) == 0)
	    	{
	      
			for(j = 0; j < 100; j++)
			{
				
				if(shared_user_list[i].cal_list[j].sqno == 0)
				{
					if(j == 0)
					{
						strcpy(buff,"CHP|0");
						if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
					  	{
					    		close(cli_sockfd);
					    		perror("Read Error\n");
					    		exit(0);
					  	}
					}
					break;
				}
				if(shared_user_list[i].cal_list[j].sqno == sqno && shared_user_list[i].cal_list[j].no_of_participants != no_of_part)
				{
					//no_of_part = 0;
					strcpy(buff,"CHP|");
					sprintf(str_no_of_part,"%d",shared_user_list[i].cal_list[k].no_of_participants);
					strcat(buff,str_no_of_part);
					strcat(buff,"|");
					for(k = 0; k <= shared_user_list[i].cal_list[j].no_of_participants; k++)
					{
						strcat(buff,shared_user_list[i].cal_list[j].participants_list[k]);
						strcat(buff,"|");
					}
					strcat(buff,"~");
					//printf("buff:%s\n", buff);
					if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
				  	{
				    		close(cli_sockfd);
				    		perror("Read Error\n");
				    		exit(0);
				  	}
					break;
				}
				else
				{
					strcpy(buff,"CHP|0");
					//printf("buff:%s\n", buff);
					if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
				  	{
				    		close(cli_sockfd);
				    		perror("Read Error\n");
				    		exit(0);
				  	}
					break;
				}	
			}
		
	    	}
		//break;
	  
	}
	
}

void check_background_cal()
{
	
	/* This function will be called by background thread running at client. 
	It will check for new events for particular username, and if it is available, pass new event details to the client. */
	int i = 0,j = 0,k = 0,client_count = 0,server_count = 0,count_diff = 0;
	char username[50],*token,sqno[8],isAttach[2],str_server_count[8],read[2], no_of_part[2], eventid[8];
	//free(token);
	//bzero((char *)&token,sizeof(token));
	token = strtok(NULL,"|");
	client_count = atoi(token);
	printf("client count:%d\n",client_count);
	token = strtok(NULL,"|");
	strcpy(username, token);
	printf("username:%s\n",username);
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
		//printf("user:%s\n",shared_user_list[i].username);
		if(strcmp(shared_user_list[i].username,username) == 0)
	    {
	      
			for(j = 0; j < 100; j++)
			{
				
				if(shared_user_list[i].cal_list[j].sqno == 0)
				{
					break;
				}
				server_count++;
				//printf("server count:%d\n", server_count);
			}
		
	    }
	  
	}
	count_diff=server_count-client_count;
	//printf("count diff:%d\n", count_diff);
	bzero((char *)&buff,sizeof(buff));
	if(count_diff > 0)
	{
		for(i = 0; i < 100; i++)
		{
			if(strcmp(shared_user_list[i].username,username) == 0)
	    	{		
					//shared_user_list[i].is_logged_in = 1;
					strcpy(buff,"CHC|");
					sprintf(str_server_count,"%d",server_count);
					strcat(buff,str_server_count);
					strcat(buff,"|");
					//printf("buff:%s\n", buff);
					for(k = client_count ; k < server_count ; k++)//k=server_count; k>client_count; k--
					{
						//printf("k:%d\n",k);
						if(shared_user_list[i].cal_list[k].sqno == 0)
							break;
						
						sprintf(eventid,"%d",shared_user_list[i].cal_list[k].event_id);
						strcat(buff,eventid);
						strcat(buff,"|");
						
						//printf("sqno:%d\n",shared_user_list[i].cal_list[j].sqno);
						sprintf(sqno,"%d",shared_user_list[i].cal_list[k].sqno);
						strcat(buff,sqno);
						strcat(buff,"|");

						//printf("read:%d\n",shared_user_list[i].cal_list[j].read);
						sprintf(read,"%d",shared_user_list[i].cal_list[k].read);
						strcat(buff,read);
						strcat(buff,"|");

						//printf("sender:%s\n\n",shared_user_list[i].cal_list[j].sendername);
						strcat(buff,shared_user_list[i].cal_list[k].sendername);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].sfname);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].slname);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].start_date);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].start_time);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].end_date);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].end_time);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].place);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].subject);
						strcat(buff,"|");
						strcat(buff,shared_user_list[i].cal_list[k].message);
						strcat(buff,"|");
						sprintf(isAttach,"%d",shared_user_list[i].cal_list[k].isAttachment);
						strcat(buff,isAttach);
						strcat(buff,"|");
						if(shared_user_list[i].cal_list[k].isAttachment == 1)
						{
							strcat(buff,shared_user_list[i].cal_list[k].attFileName);
							strcat(buff,"|");
							strcat(buff,shared_user_list[i].cal_list[k].attachment);
							strcat(buff,"|");
						}
						
						sprintf(no_of_part,"%d",shared_user_list[i].cal_list[k].no_of_participants);
						strcat(buff,no_of_part);
						strcat(buff,"|");
						for(j = 0; j <= shared_user_list[i].cal_list[k].no_of_participants; j++)
						{
							strcat(buff,shared_user_list[i].cal_list[k].participants_list[j]);
							strcat(buff,"|");
						}
						strcat(buff,"~");

						//printf("%d\n",k);
						//printf("%s\n",buff);
					}
					break;
					//printf("exited for loop:%d\n",k);
			}
			/*else
			{
				printf("Else Part\n");
			}*/
		}
		//printf("%s\n",buff);
		if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  	{
	    		close(cli_sockfd);
	    		perror("Read Error\n");
	    		exit(0);
	  	}
	}
	else
	{
		strcpy(buff,"CHC|");
		sprintf(str_server_count,"%d",server_count);
		strcat(buff,str_server_count);
		strcat(buff,"|");
		//printf("%s\n",buff);
		if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  	{
	    		close(cli_sockfd);
	    		perror("Read Error\n");
	    		exit(0);
	  	}
	}
	
}

void send_mail_data()
{
	/* When user logged in, and ask for the mail service, all mail related data of that user will be passed to the client. */
  //printf("inside function\n");
  char username[50],sqno[8],read[2],isAttach[2],*token;
  int i = 0, j = 0;

	token = strtok(NULL,"|");
	strcpy(username, token);
	//printf("username:%s\n",username);
	
	bzero((char *)&buff,sizeof(buff));

	//print_user_array();

	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  //printf("user:%s\n",shared_user_list[i].username);
	  if(strcmp(shared_user_list[i].username,username) == 0)
	    {
	      //user_found = 1;
	      //shared_user_list[i].is_logged_in = 1;
			//j = 0;
			strcat(buff,"MAI|");
	        
		for(j = 0; j < 100; j++)
		{
			//printf("j:%d\n",j);
			if(shared_user_list[i].mail_list[j].sqno == 0)
			  break;
			
			//printf("sqno:%d\n",shared_user_list[i].mail_list[j].sqno);
			sprintf(sqno,"%d",shared_user_list[i].mail_list[j].sqno);
			strcat(buff,sqno);
			strcat(buff,"|");

			//printf("read:%d\n",shared_user_list[i].mail_list[j].read);
			sprintf(read,"%d",shared_user_list[i].mail_list[j].read);
			strcat(buff,read);
			strcat(buff,"|");

			//printf("sender:%s\n\n",shared_user_list[i].mail_list[j].sendername);
			strcat(buff,shared_user_list[i].mail_list[j].sendername);
			strcat(buff,"|");
			strcat(buff,shared_user_list[i].mail_list[j].sfname);
			strcat(buff,"|");
			strcat(buff,shared_user_list[i].mail_list[j].slname);
			strcat(buff,"|");
			strcat(buff,shared_user_list[i].mail_list[j].dateTime);
			strcat(buff,"|");
			strcat(buff,shared_user_list[i].mail_list[j].subject);
			strcat(buff,"|");
			strcat(buff,shared_user_list[i].mail_list[j].message);
			strcat(buff,"|");
			sprintf(isAttach,"%d",shared_user_list[i].mail_list[j].isAttachment);
			strcat(buff,isAttach);
			strcat(buff,"|");
			if(shared_user_list[i].mail_list[j].isAttachment == 1)
			  {
			    strcat(buff,shared_user_list[i].mail_list[j].attFileName);
			    strcat(buff,"|");
			    strcat(buff,shared_user_list[i].mail_list[j].attachment);
			    strcat(buff,"|~");
			  }
			else
			  {
			    strcat(buff,"~");
			  }
			//printf("%d\n",j);
			//printf("%s\n",buff);
		  }
		//printf("exited for loop:%d\n",j);
		//if(user_found == 1)
		  //break;
	    }
	 /* else
	    {
	      printf("Else Part\n");
	    }*/
	}

	//printf("%s\n",buff);
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  {
	    close(cli_sockfd);
	    perror("Read Error\n");
	    exit(0);
	  }
}

void read_mail()
{
	/* This function will update read variable of emails for the logged in user, if user read a particular mail.*/
  //printf("inside function\n");
	char username[50],sqno[8],read[2], *token;
	int i = 0, j = 0, k = 0;

	token = strtok(NULL,"|");
	strcpy(username, token);
	
	token = strtok(NULL,"|");
	strcpy(read, token);
	
	token = strtok(NULL,"|");
	strcpy(sqno, token);
	//printf("username:%s\n",username);
	
	bzero((char *)&buff,sizeof(buff));

	//print_user_array();

	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  if(strcmp(shared_user_list[i].username,username) == 0)
	    {
			for(j = 0; j < 100; j++)
			{
				if(shared_user_list[i].mail_list[j].sqno == 0)
				  break;
			  
				if(shared_user_list[i].mail_list[j].sqno == atoi(sqno))
				{
					shared_user_list[i].mail_list[j].read = atoi(read);
					strcpy(buff,"REM|Read updated successfully");
					break;
				}
			}
	    }
	}

	//printf("%s\n",buff);
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  {
	    close(cli_sockfd);
	    perror("Read Error\n");
	    exit(0);
	  }
}

void send_cal_data()
{
  //printf("inside function\n");
  char username[50],sqno[8],read[2],isAttach[2],no_of_part[2], eventid[8], *token;
  int i = 0, j = 0, k = 0;

	token = strtok(NULL,"|");
	strcpy(username, token);
	//printf("username:%s\n",username);
	
	bzero((char *)&buff,sizeof(buff));

	print_user_array();

	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
	  //printf("user:%s\n",shared_user_list[i].username);
	  if(strcmp(shared_user_list[i].username,username) == 0)
	    {
	      //user_found = 1;
	      //shared_user_list[i].is_logged_in = 1;
			//j = 0;
			strcat(buff,"CAL|");
	        
			for(j = 0; j < 100; j++)
			{
			//printf("j:%d\n",j);
				if(shared_user_list[i].cal_list[j].sqno == 0)
				  break;
				// Copying data to buffer and send it at client side.
				sprintf(eventid,"%d",shared_user_list[i].cal_list[j].event_id);
				strcat(buff,eventid);
				strcat(buff,"|");
						
				//printf("sqno:%d\n",shared_user_list[i].cal_list[j].sqno);
				sprintf(sqno,"%d",shared_user_list[i].cal_list[j].sqno);
				strcat(buff,sqno);
				strcat(buff,"|");

				//printf("read:%d\n",shared_user_list[i].cal_list[j].read);
				sprintf(read,"%d",shared_user_list[i].cal_list[j].read);
				strcat(buff,read);
				strcat(buff,"|");

				//printf("sender:%s\n\n",shared_user_list[i].cal_list[j].sendername);
				strcat(buff,shared_user_list[i].cal_list[j].sendername);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].sfname);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].slname);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].start_date);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].start_time);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].end_date);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].end_time);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].place);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].subject);
				strcat(buff,"|");
				strcat(buff,shared_user_list[i].cal_list[j].message);
				strcat(buff,"|");
				sprintf(isAttach,"%d",shared_user_list[i].cal_list[j].isAttachment);
				strcat(buff,isAttach);
				strcat(buff,"|");
				if(shared_user_list[i].cal_list[j].isAttachment == 1)
				{
					strcat(buff,shared_user_list[i].cal_list[j].attFileName);
					strcat(buff,"|");
					strcat(buff,shared_user_list[i].cal_list[j].attachment);
					strcat(buff,"|");
				}
				
				sprintf(no_of_part,"%d",shared_user_list[i].cal_list[j].no_of_participants);
				strcat(buff,no_of_part);
				strcat(buff,"|");
				for(k = 0; k <= shared_user_list[i].cal_list[j].no_of_participants; k++)
				{
					strcat(buff,shared_user_list[i].cal_list[j].participants_list[k]);
					strcat(buff,"|");
				}
				strcat(buff,"~");
			//else
			//{
			  //  strcat(buff,"~");
			//}
			//printf("%d\n",j);
			//printf("%s\n",buff);
		  }
		//printf("exited for loop:%d\n",j);
		//if(user_found == 1)
		  //break;
	    }
	 /* else
	    {
	      printf("Else Part\n");
	    }*/
	}

	//printf("%s\n",buff);
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	  {
	    close(cli_sockfd);
	    perror("Read Error\n");
	    exit(0);
	  }
}

void send_mail_service(char *input)
{
	/* if user sends an email, then this function will send that mail to all recipeints. */
	int i = 0, j = 0, k = 0,count,ret = 0,receipient_count, mail_count = 0;
	char sendername[50];
    char sfname[50];    
    char slname[50];    
    char dateTime[50];
    char subject[100];
    char message[300];
    int isAttachment;
    char attFileName[50];
    char attachment[200];
	char recipient_list[50][50];
	char *token,rusername[200],username[50], *temp_time;
	char txt[5], input_l[1000];
	
	//printf("inside function\n");
	//printf("input:%s\n",input);

	//bzero((char *)&token,sizeof(token));
	
		
	strcpy(input_l, input);
	//printf("token :%s\n",token);
	
	token = strtok(input_l,"|");
	// |ramya; pri|

	//copying the receipiant value in two variuables just for checking.
	strcpy(rusername, token);

	//printf("token before for loop:%s\n",token);

	//printf("Rece username:%s\n",rusername);
	

	//open config file.find the sequence number of the receipient in the config file which gives us mail_count;
//	 bzero((char *)&buff,sizeof(buff));

/*
	for(i=0; i < 100; i++)
	{
		printf("%s \n%d\n",shared_user_list[i].username, i);
		ret = strcmp(rusername,shared_user_list[i].username);
		printf("ret:%d\n",ret);
		if(ret == 0)
		{
			mail_count = i;
			//printf("mail_count:%d\n",mail_count);
			break;
		}

	}*/
	/*token = strtok(NULL,"|");
	strcpy(sfname, token);
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].sfname);

	token = strtok(NULL,"|");
	strcpy(slname, token);*/
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].slname);
	
	token = strtok(NULL,"|");
	strcpy(sendername, token);
	
	for(i = 0; i < 100; i++)
	{
		if(shared_user_list[i].is_logged_in == 0)
			break;
		if (strcmp(sendername,shared_user_list[i].username) == 0)
		{
			strcpy(sfname, shared_user_list[i].fname);
			strcpy(slname, shared_user_list[i].lname);
			break;
		}
	}
	
	token = strtok(NULL,"|");
	strcpy(subject, token);
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].subject);
	
	token = strtok(NULL,"|");
	strcpy(message, token);
	//printf("token1:%s\n",shared_user_list[mail_count].mail_list[i].message);
	
	token = strtok(NULL,"|");
	strcpy(dateTime, token);

	token = strtok(NULL,"|");
	isAttachment = atoi(token);
	//printf("token1:%d\n",shared_user_list[mail_count].mail_list[i].isAttachment);

	if(isAttachment == 1)
	{
		token = strtok(NULL,"|");
		strcpy(attFileName, token);
		token = strtok(NULL,"|");
		strcpy(attachment, token);
	}
	//shared_user_list[mail_count].mail_list[i].read = 0;
	//printf("token1:%d\n",shared_user_list[mail_count].mail_list[i].read);

	//printf("time:%s\n",asctime( localtime(&ltime) ));
	//temp_time = asctime( localtime(&ltime) );
	//if(temp_time == NULL)
		//printf("Time NULL\n");

	 
	//free(token);
	//bzero((char *)&token,sizeof(token));
	token = strtok(rusername,";");
	receipient_count = -1;
	// copying all recipients in an array.
	while(token != NULL)
	{
		receipient_count++;
		strcpy(recipient_list[receipient_count], token);
		token = strtok(NULL,";");
	}
	
	//printf("mail_count:%d\n",mail_count);

	//token = strtok(NULL,"|");
		
	for(k = 0; k <= receipient_count; k++)
	{		
		for(i = 0; i < 100; i++)
		{
			if(shared_user_list[i].is_logged_in == 0)
				break;
			if (strcmp(recipient_list[k],shared_user_list[i].username) == 0)
			{
				for(j = 0; j < 100; j++)
				{
					if(shared_user_list[i].mail_list[j].sqno == 0)
					{
						shared_user_list[i].mail_list[j].sqno = j + 1;
						//printf("sqno:%d\n",shared_user_list[i].mail_list[j].sqno);

						//printf("token in for loop:%s\n",token);			
			
						//if(token == NULL)
							//printf("token NULL\n");
						strcpy(shared_user_list[i].mail_list[j].sendername, sendername);
					//	printf("token1:%s\n",shared_user_list[i].mail_list[j].sendername);

						//token = strtok(NULL,"|");
						strcpy(shared_user_list[i].mail_list[j].sfname, sfname);
						//printf("token1:%s\n",shared_user_list[i].mail_list[j].sfname);

						//token = strtok(NULL,"|");
						strcpy(shared_user_list[i].mail_list[j].slname, slname);
						//printf("token1:%s\n",shared_user_list[i].mail_list[j].slname);
								
						//token = strtok(NULL,"|");
						strcpy(shared_user_list[i].mail_list[j].subject, subject);
						//printf("token1:%s\n",shared_user_list[i].mail_list[j].subject);
						
						//token = strtok(NULL,"|");
						strcpy(shared_user_list[i].mail_list[j].message, message);
						//printf("token1:%s\n",shared_user_list[i].mail_list[j].message);

						//token = strtok(NULL,"|");
						shared_user_list[i].mail_list[j].isAttachment = isAttachment;
						//printf("token1:%d\n",shared_user_list[i].mail_list[j].isAttachment);

						if(shared_user_list[i].mail_list[j].isAttachment == 1)
						{
							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].mail_list[j].attFileName, attFileName);
							//token = strtok(NULL,"|");
							strcpy(shared_user_list[i].mail_list[j].attachment, attachment);
						}
						shared_user_list[i].mail_list[j].read = 0;
						//printf("token1:%d\n",shared_user_list[i].mail_list[j].read);

						//printf("time:%s\n",asctime( localtime(&ltime) ));
						//temp_time = asctime( localtime(&ltime) );
						//if(temp_time == NULL)
							//printf("Time NULL\n");

						strcpy(shared_user_list[i].mail_list[j].dateTime, dateTime);
						//printf("token1:%s\n",shared_user_list[i].mail_list[j].dateTime);
						
						//count=i;
						break;
					}
				}
			}
		}
	}
		//printf("count:%d\n",i);
		//user_list[mail_count].mail_list[count].sendername="erer";
		
	//token = strtok(NULL,"|");
	//shared_user_list[mail_count].mail_list[count].sendername=token;
	//printf("%s\n",shared_user_list[mail_count].mail_list[count].sendername);
        //while(token != NULL)
            //{
			
			//printf("token1:%s\n",token);
			
            //token = strtok(NULL,"|");

        //}
	/*	for(i=0;i<8;i++)
		{
			printf("%s",shared_user_list[0].mail_list[i].subject);
		}*/
		//printf("Data written111\n");
	//print_user_array();

	//printf("Data written\n");
	bzero((char *)&buff,sizeof(buff));
	strcat(buff,"SNM|Mail Sent Successfully.");
	if (write(cli_sockfd, buff, sizeof(buff)) !=  sizeof(buff))
	{
		close(cli_sockfd);
		perror("Read Error\n");
		exit(0);
	}
		
}

void save_to_config_file(char *input)
{
	/* This function will save all data from an array to the configuration file when user logs out. */
	FILE *fp;
	char *token, username[20], input_l[1000];
	int i = 0;
	//free(token);
	//bzero((char *)&token,sizeof(token));
	
	strcpy(input_l, input);
	token = strtok(input_l,"|");

	/*if(token == NULL)
		printf("Token NULL\n");
	printf("token:%s\n",token);*/
	strcpy(username,token);
	if((fp = fopen("configuration.txt","w")) == NULL)
	{
		perror("Error in opening a file.\n");
		exit(0);
	}
	for(i = 0; i < 100; i++)
	{
		//printf("i:%d\n", i);
		if(shared_user_list[i].is_logged_in == 0)
			break;
		if(strcmp(shared_user_list[i].username,username) == 0)
			shared_user_list[i].is_logged_in = 2;

		fprintf(fp,"%s",shared_user_list[i].username);
		fprintf(fp,"|");
		fprintf(fp,"%s",shared_user_list[i].password);
		fprintf(fp,"|");
		fprintf(fp,"%s",shared_user_list[i].fname);
		fprintf(fp,"|");
		fprintf(fp,"%s",shared_user_list[i].lname);
		fprintf(fp,"|");
		fprintf(fp,"%s",shared_user_list[i].mail_file);
		fprintf(fp,"|");
		fprintf(fp,"%s",shared_user_list[i].cal_file);
		fprintf(fp,"|~");
	}
	fclose(fp);
}

void save_to_event_file()
{
	/* This function will save event id to event.txt file when user logs out. */
	FILE *fp;
	if((fp = fopen("event.txt","w")) == NULL)
	{
		perror("Error in opening a file.\n");
		exit(0);
	}
	fprintf(fp,"%s",EVENT_ID);
	fclose(fp);
}

void exit_mail_service(char *input)
{
	/* This function will save all data from an array to the email file when user logs out. */
	FILE *fp, *fp1;
	char *token, username[20], input_l[1000];
	int i = 0,j = 0, mail_count = 0,te;
	//free(token);
	//bzero((char *)&token,sizeof(token));
	
	strcpy(input_l, input);
	token = strtok(input_l,"|");
	/*
	if(token == NULL)
		printf("Token NULL\n");
	printf("token:%s\n",token);*/
	strcpy(username,token);
	//strcat(username,".txt");
	
	//printf("username:%s\n", username);
	//open config file.find the sequence number of the receipient in the config file which gives us mail_count;
	for(i = 0; i < 100; i++)
	{
		//printf("i:%d\n", i);
		if(shared_user_list[i].is_logged_in == 0)
			break;
		if(strcmp(shared_user_list[i].username,username)==0)
		{
			//printf("username:%s\n",shared_user_list[i].username);
			if((fp = fopen(shared_user_list[i].mail_file,"w")) == NULL)
			{
				perror("Error in opening a file.\n");
				exit(0);
			}
			
			//printf("shared_user_list[i].mail_file: %s\n", shared_user_list[i].mail_file);
			//shared_user_list[i].is_logged_in = 2;	
			for(j = 0; j < 100; j++)
			{
				//printf("j:%d\n", j);
				if(shared_user_list[i].mail_list[j].sqno == 0)
				{
					printf("seq no is zero\n");
					break;
				}
					
				
				//if(shared_user_list[i].mail_list[j].sqno != 0)
				//{
				fprintf(fp,"%d",shared_user_list[i].mail_list[j].sqno);
				fprintf(fp,"|");
				fprintf(fp,"%d",shared_user_list[i].mail_list[j].read);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].sendername);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].sfname);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].slname);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].dateTime);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].subject);
				fprintf(fp,"|");
				fprintf(fp,"%s",shared_user_list[i].mail_list[j].message);
				fprintf(fp,"|");
				fprintf(fp,"%d",shared_user_list[i].mail_list[j].isAttachment);
				
				//printf("copying data\n");
				if(shared_user_list[i].mail_list[j].isAttachment == 1)
				{
					fprintf(fp,"|");
					fprintf(fp,"%s",shared_user_list[i].mail_list[j].attFileName);
					fprintf(fp,"|");
					fprintf(fp,"%s",shared_user_list[i].mail_list[j].attachment);
				}
				
				if(shared_user_list[i].mail_list[j].sqno != 0)
					fprintf(fp,"|~");
				//}
				
			}
			fclose(fp);
			
		}
	}
		for(i = 0; i < 100; i++)
	{
		if(strcmp(shared_user_list[i].username,username)==0)
		{
			printf("username:%s\n",shared_user_list[i].username);
			if((fp1 = fopen(shared_user_list[i].cal_file,"w")) == NULL)
			{
				perror("Error in opening a file.\n");
				exit(0);
			}
		
			printf("shared_user_list[i].cal_file: %s\n", shared_user_list[i].cal_file);
			//shared_user_list[i].is_logged_in = 2;	
			for(j = 0; j < 100; j++)
			{
				printf("j:%d\n", j);
				if(shared_user_list[i].cal_list[j].sqno == 0)
				{
					printf("seq no is zero\n");
					break;
				}
				fprintf(fp1,"%d",shared_user_list[i].cal_list[j].event_id);
				fprintf(fp1,"|");
				fprintf(fp1,"%d",shared_user_list[i].cal_list[j].sqno);
				fprintf(fp1,"|");
				fprintf(fp1,"%d",shared_user_list[i].cal_list[j].read);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].sendername);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].sfname);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].slname);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].start_date);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].start_time);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].end_date);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].end_time);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].place);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].subject);
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].message);
				fprintf(fp1,"|");
				fprintf(fp1,"%d",shared_user_list[i].cal_list[j].isAttachment);
		
				if(shared_user_list[i].cal_list[j].isAttachment == 1)
				{
					fprintf(fp1,"|");
					fprintf(fp1,"%s",shared_user_list[i].cal_list[j].attFileName);
					fprintf(fp1,"|");
					fprintf(fp1,"%s",shared_user_list[i].cal_list[j].attachment);
				}
				fprintf(fp1,"|");
				fprintf(fp1,"%d",shared_user_list[i].cal_list[j].no_of_participants);
				
				for(te=0;te<=shared_user_list[i].cal_list[j].no_of_participants;te++)
				{
				fprintf(fp1,"|");
				fprintf(fp1,"%s",shared_user_list[i].cal_list[j].participants_list[te]);
				}
		
				//i++;
				if(shared_user_list[i].cal_list[j].sqno != 0)
					fprintf(fp1,"|~");
		
			}
			
			fclose(fp1);
			break;
		}
	}
	
}

void signup()
{
	/* When new user registers into the system, this function will save user's data into an array. */
	FILE *fp;
	int flag = 1;
	int i,j,new_user;
	char *token, input_l[1000], mail_file[50], cal_file[50];
	
	//free(token);
	//bzero((char *)&token,sizeof(token));
	//strcpy(input_l, input);
	//token = strtok(input_l,"|");
	
	do
	{
		bzero((char *)&buff,sizeof(buff));
		if (read(cli_sockfd, buff, sizeof(buff)) < 0)
		{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
		}
		//token = strtok(NULL,"|");
		//strcpy(buff, token);
//username = %s\n" , buff);
		for(i = 0;i < 100;i++)
		{
			//if(i<user_count)
			//{
			if(strcmp(shared_user_list[i].username,buff)==0)
			{
				flag = 1; // This varaible is set when user enters existing username at the time of registration.
				bzero((char *)&buff,sizeof(buff));
				strcpy(buff,"1");
				//printf("username already exist so passing flag value=%s\n",buff);
				if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
					{
						close(cli_sockfd);
						perror("Write Error\n");
						exit(0);
					}
				break;
			}
			//}
			if(shared_user_list[i].is_logged_in == 0)
			{
				// If username is unique, then add it in an array.
				//user_count++;
				strcpy(shared_user_list[i].username,buff);
				//printf("valid username=%S\n",shared_user_list[i].username);
				flag=0;
				bzero((char *)&buff,sizeof(buff));
				strcpy(buff,"0");
  				//printf("new username so passing flag value=%s\n",buff);
				if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    				{
      						close(cli_sockfd);
      						perror("Write Error\n");
     						exit(0);
    				}
				new_user = i;
				break;
			}
		}
	}while(flag == 1);
	
	bzero((char *)&buff,sizeof(buff));
	if (read(cli_sockfd, buff, sizeof(buff)) < 0)
	{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
	}
	//token = strtok(NULL,"|");
	//strcpy(buff, token);
	//printf("password = %s\n" , buff);
	strcpy(shared_user_list[new_user].password,buff);
	bzero((char *)&buff,sizeof(buff));
	if (read(cli_sockfd, buff, sizeof(buff)) < 0)
	{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
	}
	//token = strtok(NULL,"|");
	//strcpy(buff, token);
	//printf("firstname = %s\n" , buff);
	strcpy(shared_user_list[new_user].fname,buff);
	bzero((char *)&buff,sizeof(buff));
	if (read(cli_sockfd, buff, sizeof(buff)) < 0)
	{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
	}
	//token = strtok(NULL,"|");
	//strcpy(buff, token);
	//printf("lastname = %s\n" , buff);
	strcpy(shared_user_list[new_user].lname,buff);
	shared_user_list[new_user].is_logged_in = 2;
	sprintf(mail_file,"mail_%s.txt",shared_user_list[new_user].username);
	strcpy(shared_user_list[new_user].mail_file, mail_file);
	fp = fopen(mail_file , "w+");
	fclose(fp);
	sprintf(cal_file,"cal_%s.txt",shared_user_list[new_user].username);
	strcpy(shared_user_list[new_user].cal_file, cal_file);
	fp = fopen(cal_file , "w+");
	fclose(fp);
/*
	for(j=0;j<=new_user;j++)
	{
		printf("\nusername:%s\n",shared_user_list[j].username);
		printf("\npassword:%s\n",shared_user_list[j].password);
		printf("\nfirstname:%s\n",shared_user_list[j].fname);
		printf("\nlastname:%s\n",shared_user_list[j].lname);
		printf("\nusermailfile:%s\n",shared_user_list[j].mail_file);
		printf("\nusercalfile:%s\n",shared_user_list[j].cal_file);
		printf("\nusercalfile:%d\n",shared_user_list[j].is_logged_in);
	}
   */     
}

void login()
{
	/* This function will validate correct username and password at the time of login.
	If it is successful, it will allow to login, else it won't. */
	int flag=0;
	int check=1;
	int loop=0;
	int i;
	char *token,username[50],password[50], input_l[1000];
	
	//free(token);
	//bzero((char *)&token,sizeof(token));
	//strcpy(input_l, input);
	//token = strtok(input_l,"|");
	
	do
	{
		bzero((char *)&buff,sizeof(buff));
		if (read(cli_sockfd, buff, sizeof(buff)) < 0)
		{
			close(cli_sockfd);
			perror("Read Error\n");
			exit(0);
		}
		
		//printf("buff:%s\n",buff);
		//free(token);
		//bzero((char *)&token,sizeof(token));
		token = strtok(buff,"|");
		strcpy(username, token);
		//printf("%s\n",username);
		for(i = 0 ; i < 100; i++)
		{   
			check = 1;
			flag = 0;
			//printf("i = %d\n", i);
			if(shared_user_list[i].is_logged_in == 0)
			{
				//printf("problem\n");
				bzero((char *)&buff,sizeof(buff));
				strcpy(buff,"0");
  	
				if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    				{
      					close(cli_sockfd);
      					perror("Write Error\n");
     					exit(0);
    				}
				break;
			}
			if(strcmp(shared_user_list[i].username,username)==0)
				{	/*	
					bzero((char *)&buff,sizeof(buff));
					strcpy(buff,"1");
					//check=0;
					printf("%s\n",username);
					if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    				{
      					close(cli_sockfd);
      					perror("Write Error\n");
     					exit(0);
    				}*/
					if(shared_user_list[i].is_logged_in == 1)
					{
						bzero((char *)&buff,sizeof(buff));
						strcpy(buff,"2");
						check = 1;
						if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    						{
      							close(cli_sockfd);
      							perror("Write Error\n");
     							exit(0);
    						}
					}
					else
						check = 0;
					break;
				}
		}
		
		//for(i = 0 ; i < 100; i++)
		if(check == 0)
		{   
			token = strtok(NULL,"|");
			strcpy(password, token);
			//printf("%s\n",password);	
			//printf("check = 0\n");
			if(strcmp(shared_user_list[i].password,password)==0)
			{
					//printf("user is successfully logged in\n");
					//printf("1.Send Email\n2.Read Email\n3.Make an appointment\n4.View Calendar\n"); 					                              							
					flag = 1;
					bzero((char *)&buff,sizeof(buff));
						
					shared_user_list[i].is_logged_in = 1;
					strcpy(buff,"1");	
					
					
					//printf("valid username and password so passing flag=%s\n",buff);
					if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
						{
							close(cli_sockfd);
							perror("Write Error\n");
							exit(0);
						}
					break;
			}
			else
			{	
				//printf("invalid password or username check = 1, flag = 0\n");
				flag = 0;
				//check = 1;
				bzero((char *)&buff,sizeof(buff));
				strcpy(buff,"0");

				if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
					{
						close(cli_sockfd);
						perror("Write Error\n");
						exit(0);
					}
				//break;
			}
			//printf("before break\n");
				//break;
		}
		

		
				
				
				
				
			/*	
			if(check==1)
			{	
				printf("check =1\n");
				printf("second username:%s\n",shared_user_list[i].username);
				if(strcmp(shared_user_list[i].username,username)==0)
				{		
					check=0;
					printf("%s\n",username);
				}		
			}
			
			if(check==0)
			{
				token = strtok(NULL,"|");
				strcpy(password, token);
				printf("%s\n",password);	
				printf("check = 0\n");
				if(strcmp(shared_user_list[i].password,password)==0)
				{
						printf("user is successfully logged in\n");
						//printf("1.Send Email\n2.Read Email\n3.Make an appointment\n4.View Calendar\n"); 					                              							
						flag = 1;
						bzero((char *)&buff,sizeof(buff));
						strcpy(buff,"1");
  						printf("valid username and password so passing flag=%s\n",buff);
						if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    						{
      							close(cli_sockfd);
      							perror("Write Error\n");
     							exit(0);
    						}
						break;
				}
				else
				{	
					printf("invalid password or username check = 1, flag = 0\n");
					flag = 0;
					check = 1;
					bzero((char *)&buff,sizeof(buff));
					strcpy(buff,"0");

					if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
						{
							close(cli_sockfd);
							perror("Write Error\n");
							exit(0);
						}
					break;
				}
				printf("before break\n");
				break;
			}
				
			loop++;	
			printf("loop : %d", loop);
	    }
		//if(loop==user_count && check==1)
		if(check == 1 && shared_user_list[i].is_logged_in == 0)
		{
			printf("Invalid username or password\n");
			flag=0;
			bzero((char *)&buff,sizeof(buff));
			strcpy(buff,"0");
  			printf("invalid username and password so passing flag=%s\n",buff);
			if (write(cli_sockfd, buff, sizeof(buff)) != sizeof(buff))
    			{
      					close(cli_sockfd);
      					perror("Write Error\n");
     					exit(0);
    			}
			break;
		}*/
	}while(flag==0);
}


