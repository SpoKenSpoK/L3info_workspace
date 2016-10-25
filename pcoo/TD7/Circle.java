public class Circle implements Shape {
    private double x;
    private double y;
    private double radius;

    public Circle(double x, double y, double radius){
        this.x = x;
        this.y = y;
        this.radius = radius;
    }

    public void paint(GraphicsContext gc){
        gc.setFill(Color.rgb(0,255,0,0.2))

        double left = x - radius;
        double top = y - radius;
        double diam = 2*radius;

        gc.fillOval(left, top, diam, diam);
        gc.strokeOval(left, top, diam, diam);
    }

    public boolean contains(double dx, double dy){
        return  ( Math.pow(dx-x) + Math.pow(dy-y) ) <= Math.pow(radius);
        // return Math.hypot( (dx - x), (dy - y) ) <= radius;
    }

    public void translate(double dx, double dy){
        x+=dx; y+=dy;
    }
}
