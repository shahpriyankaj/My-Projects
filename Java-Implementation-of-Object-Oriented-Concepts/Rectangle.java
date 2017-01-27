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

public class Rectangle extends BoundedShape
{
    //private int area, perimeter;
    public Rectangle(int x, int y, int width, int height, Color fillColor, Color borderColor)
    {
        super(x, y, width, height, fillColor, borderColor);
    }
    
   
    
    @Override
    public void drawObject(Graphics g)
    {
        g.setColor(super.getBorderColor());
        g.drawRect(super.getX1(), super.getY1(), super.getWidth(), super.getHeight());
        g.setColor(super.getColor());
        g.fillRect(super.getX1(), super.getY1(), super.getWidth(), super.getHeight());
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
        System.out.printf("Rectangle:\nx1 = %d, y1 = %d\nWidth = %d, Height = %d\nColor = %s, Fill Color = %s\nArea = %.2f, Perimeter = %.2f\n\n",
                            super.getX1(),
                            super.getY1(),
                            super.getWidth(),
                            super.getHeight(),
                            super.getBorderColor().toString(),
                            super.getColor().toString(),
                            area, 
                            perimeter);
    }
    
    @Override
    public void calculate()
    {
        area = super.getWidth() * super.getHeight();
        perimeter = 2 * (super.getWidth() + super.getHeight());
    }
    
    @Override
    public int compareTo(Shape s)
    {
        if(s instanceof Line)
        {
            Line l1 = (Line)s;
            if(getArea() > l1.getLength())
                return(1);
            else if(getArea() < l1.getLength())
                return(-1);
        }
        
        if(s instanceof Rectangle)
        {
            Rectangle r1 = (Rectangle)s;
            if(getArea() > r1.getArea())
                return(1);
            else if(getArea() < r1.getArea())
                return(-1);
        }
        
        if(s instanceof Oval)
        {
            Oval o1 = (Oval)s;
            if(getArea() > o1.getArea())
                return(1);
            else if(getArea() < o1.getArea())
                return(-1);
        }
        return(0);
    }
    
}