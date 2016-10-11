public class ShapeContainer{
    private List<Shape> shapes;
    public ShapeContainer(){
        shapes = new ArrayList<Shape>();
    }
    public void add(Shape shape){
        shapes.add(shape);
    }

    public void draw(GraphicsContext gc){
        for(Shape shape : shapes)
            shape.draw(gc);
    }


}
