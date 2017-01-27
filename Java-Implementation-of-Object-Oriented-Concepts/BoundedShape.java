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

public abstract class BoundedShape extends Shape
{
    private int width;
    private int height;
    private Color borderColor;
    protected double area;
    protected double perimeter;
    
    public BoundedShape(int x, int y, int width, int height, Color fillColor, Color borderColor)
    {
        super(fillColor,x,y);
        this.width = width;
        this.height = height;
        this.borderColor = borderColor;
    }
    
    public int getWidth()
    {
        return width;
    }
    
    public int getHeight()
    {
        return height;
    }
    
    public double getArea()
    {
        return area;
    }
    
    public double getPerimeter()
    {
        return perimeter;
    }
    
    public Color getBorderColor()
    {
        return borderColor;
    }
    
    public abstract void drawObject(Graphics g);
    public abstract void printObject();
}
