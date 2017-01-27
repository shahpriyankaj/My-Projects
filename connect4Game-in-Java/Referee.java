package assignment_4;

/**
 *
 * @author Priyanka
 */

public class Referee implements Runnable
{
    // This class monitors the board and decides the winner.
    private char connect4Board[][] = new char[6][7];
    private Board board;
    
    public Referee(Board board)
    {
        this.board = board;
        connect4Board = new char[board.getRows()][board.getCols()];
    }
    
    public void run()
    {
        // Implements the run() method of Runnable interface.
        try
        {
            monitor();
        }
        catch(InterruptedException e)
        {
            System.out.printf(e.toString() + "\n");
        }
    }
    
    private void monitor() throws InterruptedException
    {
        /* Method Description: This method will check for the winner.0
        */
        System.out.printf("Board is Empty and Winner is not declared.\n\n********************* Please Start the Game.********************\n\n");
	board.printBoard();
        
	for(;;)
        {
            board.getLock().lock(); // Locks the board
            try
            {
                // Waits while player is playing the game.
                while (board.getPlayGame() == true)
                {
                    board.getCond().await();
                }
                
                board.setPlayGame(true);
                connect4Board = board.getBoard();
                
                /* Checks the winner conditions: Horizontal/ Diagonal Checks and 
                   full board. If no player is winning, signal player to play again.
                   Else, finish the game and exit from the game. */
                if (( checkWinner() == 0 ) && ( boardFilled() == 0 ))
                {
                    System.out.printf("\nChecking winner\n");
                    board.getCond().signalAll();
                }
                else
                {
                    board.setGameFinished(true);
                    System.out.printf("\n****************** GAME OVER *******************\n");
                    board.getCond().signalAll();
                    return;
                }
            }
            finally
            {
                board.getLock().unlock();
            }
        }
    }
    
    private int checkWinner()
    {
        /* Method Description: This method will call the methods to check horizontal
           and diagonal pattern to decide the winner.
        */
        try
        {
            /*Calls below functions to check horizontal and diagonal pattern
              to decide the winner. */
            if(checkHorizontalTiles() == 1 || checkDiagonalTiles() == 1)
            {
                return(1);
            }
        }
        catch(ArrayIndexOutOfBoundsException e)
        {
            System.out.printf(e.toString() + "\n");
        }
	return(0);
    }

    private int checkHorizontalTiles() throws ArrayIndexOutOfBoundsException
    {
        /* Method Description: This method will check the horizontal pattern 
           to decide the winner. 
        */
        for(int i = 0; i < board.getRows(); i++)
        {
            for(int j = 0; j < board.getCols(); j++)
            {
                // Checks boundary conditions.
                if((j+1) < board.getCols() && 
                   (j+2) < board.getCols() && 
                   (j+3) < board.getCols())
                {
                    if (( connect4Board[i][j] == connect4Board[i][j+1] && (j+1) < board.getCols()) &&
                        ( connect4Board[i][j] == connect4Board[i][j+2] && (j+2) < board.getCols()) &&
                        ( connect4Board[i][j] == connect4Board[i][j+3] && (j+3) < board.getCols()) &&
                      connect4Board[i][j] != '0')
                    { 
                        System.out.printf("\n***The Winner is %c with Horizontal Pattern.***\n",
                                                        connect4Board[i][j]);
                        return(1);
                    }
                }
            }
        }
        return(0);
    }

    private int checkDiagonalTiles() throws ArrayIndexOutOfBoundsException
    {
        /* Method Description: This method will check the diagonal pattern 
           to decide the winner. 
        */
        for(int i = 0; i < board.getRows();i++)
        {
            for(int j = 0; j < board.getCols(); j++)
            {
                // Checks boundary conditions.
                if(j+1 < board.getCols() && 
                   j+2 < board.getCols() && 
                   j+3 < board.getCols() && 
                   i+1 < board.getRows() &&
                   i+2 < board.getRows() && 
                   i+3 < board.getRows())
                {
                    if (connect4Board[i][j] == connect4Board[i+1][j+1] &&  
                    connect4Board[i][j] == connect4Board[i+2][j+2] && 
                    connect4Board[i][j] == connect4Board[i+3][j+3] &&
                    connect4Board[i][j] != '0')
                    {
                        System.out.printf("\n***The Winner is %c with Diagonal Pattern.***\n",
                                                        connect4Board[i][j]);
                        return(1);                          
                    }
                }
                if(j-1 > -1 && 
                   j-2 > -1 && 
                   j-3 > -1 &&
                   i+1 < board.getRows() &&
                   i+2 < board.getRows() && 
                   i+3 < board.getRows())
                {
                    if(connect4Board[i][j] == connect4Board[i+1][j-1] && 
                    connect4Board[i][j] == connect4Board[i+2][j-2] &&
                    connect4Board[i][j] == connect4Board[i+3][j-3] &&
                    connect4Board[i][j] != '0')
                    {
                        System.out.printf("\n***The Winner is %c with Diagonal Pattern.***\n",
                                                        connect4Board[i][j]);
                        return(1);
                    }
                }
            }
        }
        return(0);
    }

    private int boardFilled()
    {
        /* Method Description: This method will check the full baord 
           to decide the winner. 
        */
        for(int i = 0; i < board.getRows(); i++)
        {
            for(int j = 0; j < board.getCols(); j++)
            {
                if (connect4Board[i][j] == '0')
                {
                    return(0);
                }    
            }
        }
        System.out.printf("\nTie between the Players.\n");
        return(1);
    }   
    
}