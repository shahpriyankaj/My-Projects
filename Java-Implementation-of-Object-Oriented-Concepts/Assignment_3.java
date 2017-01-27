/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package assignment_3;

/**
 *
 * @author Priyanka
 */

import javax.swing.JFrame;

public class Assignment_3 
{

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) 
    {
        System.out.printf("************** WELCOME **************\n\n");
        DrawPanel panel = new DrawPanel();
        JFrame frame = new JFrame();
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.add(panel);
        frame.setSize(600,600);
        frame.setVisible(true);
    }
    
}
