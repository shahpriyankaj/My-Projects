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
import java.awt.Graphics;
import java.awt.Color;

public abstract class Shape implements Printable, Comparable<Shape>
{
    private Color color;
    private int x1;
    private int y1;
    
    public Shape(Color color,int x1, int y1)
    {
        this.color = color;
        this.x1 = x1;
        this.y1 = y1;
    }
    
    public Color getColor()
    {
        return color;
    }
    
    public int getX1()
    {
        return x1;
    }
    
    public int getY1()
    {
        return y1;
    }
    public abstract void drawObject(Graphics g);
    public abstract void printObject();
}
