package assignment_4;

/**
 *
 * @author Priyanka
 */
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;

public class Game 
{
    // Main class
    public static void main(String[] args)
    {
        System.out.printf("******************* WELCOME *******************\n");
        
        // Added below code to create three threads: One for Referee and two for Players.
        ExecutorService es = Executors.newCachedThreadPool();
        
        // Creating object of Board which will be shared by Referee and Players threads.
        
        Board board = new Board();
        board.initializeBoard();
        
        es.execute(new Referee(board));
        es.execute(new Player(board,'R'));
        es.execute(new Player(board,'Y'));
        
        es.shutdown();
    }
}

