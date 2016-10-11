public class ShapeDecorator implements Shape{
    private Shape shape;
    protected ShapeDecorator(Shape shape){
        this.shape = shape;
    }
    public int pointsCount(){ return shape.pointsCount(); }
    public Point2D point(int index){ return shape.point(index); }
    public void draw(GraphicsContext gc){ shape.draw(gc); drawDecoraction(gc); } // Délégation + Décoration
    protected abstract void drawDecoration();
}
