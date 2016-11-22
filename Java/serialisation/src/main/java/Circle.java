import javafx.scene.canvas.GraphicsContext;
import javafx.scene.paint.Color;

public class Circle implements Shape {
    private double x;
    private double y;

    public double getX() { return x; }
    public double getY() { return y; }
    public double getRadius() { return radius; }

    private double radius;

    public Circle(double x, double y, double radius){
        this.x = x;
        this.y = y;
        this.radius = radius;
    }

    public void paint(GraphicsContext gc){
        gc.setFill(Color.rgb(0,255,0,0.2));

        double left = x - radius;
        double top = y - radius;
        double diam = 2*radius;

        gc.fillOval(left, top, diam, diam);
        gc.strokeOval(left, top, diam, diam);
    }

    public boolean contains(double dx, double dy){
        return Math.hypot( (dx - x), (dy - y) ) <= radius;
    }

    public void translate(double dx, double dy){
        x+=dx; y+=dy;
    }

    public <R> R accept(ShapeVisitor <R> v) {
        return v.visit(this);
    }


}