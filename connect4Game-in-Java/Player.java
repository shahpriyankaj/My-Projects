package assignment_4;

/**
 *
 * @author Priyanka
 */
import java.security.SecureRandom;

public class Player implements Runnable
{
    //This class will handle the player taking move to play the game.
    private SecureRandom randNo = new SecureRandom(); 
    private final char localTile;
    private Board board;
    private char connect4Board[][];
     
    public Player(Board board, char tile)
    {  
        localTile = tile; // Assigning localTile to 'R' or 'Y'
        this.board = board;
        connect4Board = new char[board.getRows()][board.getCols()];
    }
    public void run()
    {
        // Implements the run() method of Runnable interface.
        try
        {    playGame();
        }
        catch(InterruptedException e)
        {
            System.out.printf(e.toString() + "\n");
        }
    }
    
    private void playGame() throws InterruptedException
    {
        /* 
        Method Description: This method will generates the column randomly and 
        put the chip in next available row.
        */
	int i,columnNo,findNewTile;
        
        findNewTile = 0;
	for(;;)
        {
            board.getLock().lock();
            connect4Board = board.getBoard();
            try
            {
                // Waits while Referee is checking the winner conditions.
                while(board.getPlayGame() == false)
                    board.getCond().await();

                // If winner is decided, then exit from the Game.
                if(board.getGameFinished() == true)
                {
                    board.getCond().signalAll();
                    return;
                }
                else
                {
                    System.out.printf("Player %c is playing..\n",localTile);
                    do
                    {
                        /* Generates the column number randomly to put the chip 
                           in next available row. If row is full, then generate 
                           another colulmn and perform same operation.
                        */
                        columnNo = randNo.nextInt(7) + 1;
                        System.out.printf("\nColumn Number Selected is:%d\n",columnNo);
                        
                        for(i = board.getRows()-1; i >= 0; i--)
                        {
                            if(connect4Board[i][columnNo - 1] == '0')
                            {
                                connect4Board[i][columnNo - 1] = localTile;
                                board.setBoard(connect4Board);
                                board.printBoard();
                                Thread.sleep(1000);
                                System.out.printf("Passing to Referee to Check for the winner..\n\n");
                                board.setPlayGame(false);
                                findNewTile = 0;

                                board.getCond().signalAll();
                                break;
                            } 
                            else
                                findNewTile = 1;
                        }
                        
                    } while(findNewTile == 1);
                }
            }
            finally
            {
                board.getLock().unlock();
            }
        }
    }
}

