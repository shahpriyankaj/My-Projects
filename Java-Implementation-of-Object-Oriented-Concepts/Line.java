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

public class Line extends Shape
{
    private int x2;
    private int y2;
    private double length, slope;
    
    public Line(int x1, int x2, int y1, int y2, Color color)
    {
        super(color,x1,y1);
        this.x2 = x2;
        this.y2 = y2;
    }
    public double getLength()
    {
        return length;
    }
    
    public double getSlope()
    {
        return slope;
    }
    
    @Override
    public void drawObject(Graphics g)
    {
       g.setColor(super.getColor());
       g.drawLine(super.getX1(), super.getY1(), x2, y2);
    }
    
    @Override
    public void printObject()
    {
        System.out.printf("< %d, %d, %d >", super.getColor().getRed(), 
                                            super.getColor().getGreen(),
                                            super.getColor().getBlue());
        System.out.println();
    }
    
    @Override
    public void printObjDetails()
    {
        
        System.out.printf("Line:\nx1 = %d, y1 = %d, x2 = %d, y2 = %d \nColor = %s \nLength = %.2f, Slope = %.2f\n\n",
                            super.getX1(),
                            super.getY1(),
                            x2,
                            y2,
                            super.getColor().toString(),
                            length, 
                            slope);
    }
    
    @Override
    public void calculate()
    {
        double sq1, sq2, x, y;
        
        x = Math.abs((double)(x2 - super.getX1()));
        y = Math.abs((double)(y2 - super.getY1()));
        
        sq1 = Math.pow(x, 2);
        sq2 = Math.pow(y, 2);
        
        length = Math.sqrt(sq1 + sq2);
        slope = y / x;
    }
    
    @Override
    public int compareTo(Shape s)
    {
        if(s instanceof Line)
        {
            Line l1 = (Line)s;
            if(getLength() > l1.getLength())
                return(1);
            else if(getLength() < l1.getLength())
                return(-1);
        }
        
        if(s instanceof Rectangle)
        {
            Rectangle r1 = (Rectangle)s;
            if(getLength() > r1.getArea())
                return(1);
            else if(getLength() < r1.getArea())
                return(-1);
        }
        
        if(s instanceof Oval)
        {
            Oval o1 = (Oval)s;
            if(getLength() > o1.getArea())
                return(1);
            else if(getLength() < o1.getArea())
                return(-1);
        }
        return(0);
    }
}
