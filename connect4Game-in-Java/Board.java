package assignment_4;

/**
 *
 * @author Priyanka
 */
import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Board 
{
    // This class contains the Baord details.
    private final int ROWS = 6;
    private final int COLS = 7;
    
    private char connect4Board[][] = new char[ROWS][COLS];
    private final Lock lock = new ReentrantLock();
    private final Condition cond = lock.newCondition();
    private boolean playGame = false, gameFinished = false;
    
    public void initializeBoard()
    {
        // Initializes the board to 0.
        for(int i = 0;i < ROWS;i++)
        {
            for(int j = 0; j < COLS; j++)
            connect4Board[i][j] = '0';
        }
    }
    
    public char[][] getBoard()
    {
        // returns entire baord which is shared by Referee and Players.
        return connect4Board;
    }
    
    public int getRows()
    {
        // returns number of Rows.
        return ROWS;
    }
    
    public int getCols()
    {
        // returns number of Columns.
        return COLS;
    }
    
    public Condition getCond()
    {
        // returns condtional variable cond  which is shared by Referee and Players.
        return cond;
    }
    
    public Lock getLock()
    {
        // returns lock which is shared by Referee and Players.
        return lock;
    }
    
    public boolean getPlayGame()
    {
        //returns condition variable playGame which is shared by Referee and Players.
        return playGame;
    }
    
    public boolean getGameFinished()
    {
        //returns condition variable gameFinished which is shared by Referee and Players.
        return gameFinished;
    }
    
    
    public void setBoard(char[][] connect4Board)
    {    
        // Setting the board.
        this.connect4Board = connect4Board;
    }
    
    public void setPlayGame(boolean playGame)
    {
        // Setting the condition variable playGame.
        this.playGame = playGame;
    }
    
    public void setGameFinished(boolean gameFinished)
    {
        // Setting the condition variable gameFinished.
        this.gameFinished = gameFinished;
    }
    
    public void printBoard()
    {
        // Prints the baord.
	for(int i = 0; i < ROWS; i++)
	{
		for(int j = 0; j < COLS; j++)
		{
                    System.out.printf("%c\t",connect4Board[i][j]);
		}
		System.out.printf("\n_____________________________________________________\n");
        }
    }
}

