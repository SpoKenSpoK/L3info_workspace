public class Drawer extends Canvas {
    private List<Shape> shapes;

    public Drawer(double width, double height){
        super(width, height);
        shapes = new ArrayList<Shape>();
    }

    public void add(Shape shape){
        shapes.add(shape);
    }

    public void repaint(){
        GraphicsContext gc = getGraphicsContext2D();
        gc.clearRect(0, 0, getWidth(), getHeight());
        drawShapes(gc);
    }

    private void drawShapes(GraphicsContext gc){
        for(Shape shape : shapes){
            shape.paint(gc);
        }
    }

    public Shape shapeContaining(double x, double y){
        // on renvoie celle qui est au dessus des autres s'il y a en a plusieurs.
        // Balayage de la droite vers la gauche dans shapes
        // On renvoie null si on ne trouve rien

        for(int i=shapes.size() - 1; i >= 0; --i){
            Shape shape = shapes.get(i);
            if(shape.contains(x,y)) return shape;
        }

        return null;
    }
}
