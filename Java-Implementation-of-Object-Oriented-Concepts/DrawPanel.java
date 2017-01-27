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

import java.awt.Color;
import java.awt.Graphics;
import java.security.SecureRandom;
import javax.swing.JPanel;

public class DrawPanel extends JPanel implements Comparable
{
    private SecureRandom random = new SecureRandom();
    private Line[] line;
    private Rectangle[] rect;
    private Oval[] oval;
    private int x1, y1, x2, y2;
    private int red, green, blue;
    private int minRed, minGreen, minBlue, maxRed, maxGreen, maxBlue;
    private static boolean firstTime = true;
    private static final int MAXSHAPE = 10;
    
    public DrawPanel()
    {
        setBackground(Color.BLACK);
        line = new Line[random.nextInt(MAXSHAPE)];
        rect = new Rectangle[random.nextInt(MAXSHAPE)];
        oval = new Oval[random.nextInt(MAXSHAPE)];
       
        //Creates Lines
        for(int i = 0; i < line.length; i++)
        {
            generateRandomNo();
            Color fillColor = new Color(red,green,blue);
            line[i] = new Line(x1, y1, x2, y2, fillColor);
        }
        
        //Creates Rectangles
        for(int i = 0; i < rect.length; i++)
        {
            generateRandomNo();
            Color fillColor = new Color(red,green,blue);
            
            red = random.nextInt(256);
            green = random.nextInt(256);
            blue = random.nextInt(256);
            Color borderColor = new Color(red,green,blue);
            
            //Here x2 is the width and y2 is the length of the Rectangle.
            rect[i] = new Rectangle(x1, y1, x2, y2, fillColor, borderColor);
        }
        
        //Creates Ovals
        for(int i = 0; i < oval.length; i++)
        {
            generateRandomNo();
            Color fillColor = new Color(red, green, blue);
            
            red = random.nextInt(256);
            green = random.nextInt(256);
            blue = random.nextInt(256);
            Color borderColor = new Color(red,green,blue);
            
            //Here x2 is the width and y2 is the length of the Oval.
            oval[i] = new Oval(x1, y1, x2, y2, fillColor, borderColor);
        }
    }
    
    public void paintComponent(Graphics g)
    {
        super.paintComponent(g);
        Shape[] shape = new Shape[line.length + 
                                  rect.length + 
                                  oval.length];
        /*Printable[] printable = new Printable[line.length + 
                                              rect.length + 
                                              oval.length];
        */
        for(int i = 0; i < line.length; i++)
            shape[i] = line[i];
        
        for(int i = 0; i < rect.length; i++)
            shape[line.length + i] = rect[i];
        
        for(int i = 0; i < oval.length; i++)
            shape[line.length + rect.length + i] = oval[i];
       
        System.out.printf("RGB Values(Filled Colors) of Shapes:\n");
        for(Shape shape1 : shape)
        {
            shape1.drawObject(g);
            shape1.printObject();
        }
        
        System.out.println();
        printRange();
        System.out.println();
       /* 
        printable = shape;
        double sortArray[] = new double[printable.length];
        for(int i = 0; i < printable.length; i++)
        {
            printable[i].calculate();
            
            if(printable[i] instanceof Line)
            {
                Line l1 = (Line)printable[i];
                sortArray[i] = l1.getLength();
            }
            else
            {
                BoundedShape b1 = (BoundedShape)printable[i];
                sortArray[i] = b1.getArea();
            }
        }
        
        printable = sort(printable,sortArray);
        */
        for(Shape shape1:shape)
            shape1.calculate();
        
        shape = sortShapes(shape);
        System.out.printf("Sorted Shapes based on Length\\Area:\n");
        for(Shape shape1 : shape)
            shape1.printObjDetails();
    }     
    
    private void generateRandomNo()
    {
        x1 = random.nextInt(350) + 5;
        y1 = random.nextInt(300) + 5;
        x2 = random.nextInt(250) + 5;
        y2 = random.nextInt(200) + 5;
        red = random.nextInt(256);
        green = random.nextInt(256);
        blue = random.nextInt(256);
        
        if (firstTime == true)
        {
            firstTime = false;
            minRed = red;
            minGreen = green;
            minBlue = blue;
            maxRed = red;
            maxGreen = green;
            maxBlue = blue;
        }
        else
        {
            // Checking minimum value in Range
            if(red < minRed)
                minRed = red;
            if(green < minGreen)
                minGreen = green;
            if(blue < minBlue)
                minBlue = blue;
            
            // Checking maximum value in Range
            if(red > maxRed)
                maxRed = red;
            if(green > maxGreen)
                maxGreen = green;
            if(blue > maxBlue)
                maxBlue = blue;
        }
    }
    
    private void printRange()
    {
        System.out.printf("Color Range:\n");
        System.out.printf("The Red Range was: %d - %d\n", minRed, maxRed);
        System.out.printf("The Green Range was: %d - %d\n", minGreen, maxGreen);
        System.out.printf("The Blue Range was: %d - %d\n", minBlue, maxBlue);
    }
    
    public int compareTo(Object o)
    {
        return(0);
    }
    private Printable[] sort(Printable[] printable, double[] sortArray)
    {
        //Selection Sort
        int j;
        
        for(int i = 0; i < sortArray.length - 1; i++)
        {
            int smallIndex = i;
            for(j = i + 1; j < sortArray.length; j++)
            {
                if(sortArray[j] < sortArray[smallIndex])
                    smallIndex = j;
            }
            double temp = sortArray[i];
            sortArray[i] = sortArray[smallIndex];
            sortArray[smallIndex] = temp;
            
            Printable pTemp = printable[i];
            printable[i] = printable[smallIndex];
            printable[smallIndex] = pTemp;
        }
        return(printable);
    }
    private Shape[] sortShapes(Shape[] shape)
    {
        //Selection Sort
        int j;
        
        for(int i = 0; i < shape.length - 1; i++)
        {
            int smallIndex = i;
            for(j = i + 1; j < shape.length; j++)
            {
                if(shape[j].compareTo(shape[smallIndex]) == -1)
                //if(sortArray[j] < sortArray[smallIndex])
                    smallIndex = j;
            }
            
            Shape pTemp = shape[i];
            shape[i] = shape[smallIndex];
            shape[smallIndex] = pTemp;
        }
        return(shape);
    }
}
