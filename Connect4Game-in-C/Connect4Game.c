#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<unistd.h>
#include<pthread.h>
#define ROWS 6
#define COLS 7 

void *referee(void*);
void *player(void*);
void print_board();
int  check_winner();
int  check_horizonatal_tiles();
int  check_diagonal_tiles();
int  board_filled();

char  board[ROWS][COLS];
int play_game,game_finished;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var = PTHREAD_COND_INITIALIZER;


int main(int argc,char *argv[])
{
  int i,j;
  play_game = 1;
  game_finished = 0;
  
  //Initialize the array board[][]
  for(i=0;i<6;i++)
    {
    for(j = 0; j < 7; j++)
      board[i][j]='0';
    }

  pthread_t referee_thread, player1_thread, player2_thread;
  pthread_create(&referee_thread, NULL, referee, NULL);
  pthread_create(&player1_thread, NULL, player,(void*) 'Y');
  pthread_create(&player2_thread, NULL, player,(void*) 'R');
  pthread_join(referee_thread, NULL);
  pthread_join(player1_thread, NULL);
  pthread_join(player2_thread, NULL);
  return(0);
}

void *referee(void *argument)
{
  printf("Board is Empty and Winner is not declared.\n\n********************* Please Start the Game.********************\n\n");
  print_board();
  for(;;)
    {
      pthread_mutex_lock(&mutex);
      while (play_game == 0)
			pthread_cond_wait(&cond_var,&mutex);
      
      play_game = 0;
      if ((check_winner() == 0) && (board_filled() == 0))
	  pthread_cond_signal(&cond_var);
      else
	{
	  game_finished = 1;
	  printf("\n****************** GAME OVER *******************\n");
	  pthread_cond_signal(&cond_var);
	  pthread_mutex_unlock(&mutex);
	  return(NULL);
	}
      
      pthread_mutex_unlock(&mutex);
    }
}

void *player(void *tile)
{
  char *local_tile;
  int i,column_no,find_new_tile;
 
  time_t tloc;
  local_tile = (char*)tile;
  srand(time(&tloc));
  for(;;)
    {
      pthread_mutex_lock(&mutex);

      while(play_game == 1)
	pthread_cond_wait(&cond_var,&mutex);

      if(game_finished == 1)
	{
	  pthread_cond_signal(&cond_var);
	  pthread_mutex_unlock(&mutex);
	  return(NULL);
	}
      else
	{
	  printf("Player %c is playing..\n",local_tile);
	  do
	    {
	      sleep(1);
	      column_no = rand()% COLS + 1;
	      printf("\nColumn Number Selected is:%d\n",column_no);
	      for(i = ROWS-1; i >= 0; i--)
		{
		  if(board[i][column_no-1] == '0')
		    {
		      board[i][column_no-1] = local_tile;
		      print_board();
		      printf("Passing to Referee to Check for the winner..\n\n");
		      play_game = 1;
		      find_new_tile = 0;
		      pthread_cond_signal(&cond_var);
		      break;
		    } 
		  else
		    find_new_tile = 1;
		}
	    } while(find_new_tile == 1);
	}
      pthread_mutex_unlock(&mutex);
    }
  return(NULL);
}

void print_board()
{
   int i,j;
   for(i = 0; i < ROWS; i++)
     {
       for(j = 0; j < COLS; j++)
	 {
	   printf("%c\t",board[i][j]);
	 }
       printf("\n_____________________________________________________\n");
      }
}

int check_winner()
{
  if ( check_horizontal_tiles() == 1 ||
       check_diagonal_tiles() == 1)
    return(1);
  return(0);
}

int check_horizontal_tiles()
{
  int i,j;
  for(i = 0; i < ROWS;i++)
    {
      for(j = 0; j < COLS; j++)
	{
	  if (( board[i][j] == board[i][j+1] && j+1 < COLS) &&
	      ( board[i][j] == board[i][j+2] && j+2 < COLS) &&
	      ( board[i][j] == board[i][j+3] && j+3 < COLS) &&
	      board[i][j] != '0')
            { 
            printf("\n***The Winner is %c with Horizontal Pattern.***\n",board[i][j]);
	    return(1);
            }
	}
    }
  return(0);
}

int check_diagonal_tiles()
{
  int i,j;
  for(i = 0; i < ROWS;i++)
    {
      for(j = 0; j < COLS; j++)
	{
	  if (((( board[i][j] == board[i+1][j+1] && i+1 < ROWS && j+1 < COLS) && 
	        ( board[i][j] == board[i+2][j+2] && i+2 < ROWS && j+2 < COLS) &&
		( board[i][j] == board[i+3][j+3] && i+3 < ROWS && j+3 < COLS)) ||
       	       (( board[i][j] == board[i+1][j-1] && i+1 < ROWS && j-1 > -1) &&
		( board[i][j] == board[i+2][j-2] && i+2 < ROWS && j-2 > -1) &&
		( board[i][j] == board[i+3][j-3] && i+3 < ROWS && j-3 > -1))) &&
	           board[i][j] != '0')
            {
 	    printf("\n***The Winner is %c with Diagonal Pattern.***\n",board[i][j]);
	    return(1);
            }
	}
    }
  return(0);
}

int board_filled()
{
  int i,j;
  for(i = 0;i < ROWS; i++)
    {
      for(j = 0;j < COLS; j++)
	{
	  if (board[i][j] == '0')
	     return(0);
	}
    }
   printf("\nTie between the Players.\n");
  return(1);
}
