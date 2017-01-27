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

public class Oval extends BoundedShape
{
    //private double area, perimeter;
    public Oval(int x, int y, int width, int height, Color fillColor, Color borderColor)
    {
        super(x, y, width, height, fillColor, borderColor);
    }
    
    @Override
    public void drawObject(Graphics g)
    {
       g.setColor(super.getBorderColor());
       g.drawOval(super.getX1(), super.getY1(), super.getWidth(), super.getHeight());
       g.setColor(super.getColor());
       g.fillOval(super.getX1(), super.getY1(), super.getWidth(), super.getHeight());
    }
    
    @Override
    public void printObject()
    {
        System.out.printf("< %d, %d, %d >\n", super.getColor().getRed(), 
                                              super.getColor().getGreen(),
                                              super.getColor().getBlue());
    }
    
    @Override
    public void printObjDetails()
    {
        System.out.printf("Oval:\nx1 = %d, y1 = %d\nWidth = %d, Height = %d\nColor = %s, Fill Color = %s\nArea = %.2f, Perimeter = %.2f\n\n",
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
        area = Math.PI * (super.getWidth()/2) * (super.getHeight()/2);
        perimeter = (Math.PI/2) * (Math.sqrt(
                                    (2 * Math.pow((double)super.getWidth(),2)) + 
                                    (2 * Math.pow((double)super.getHeight(),2))));
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
