public interface Shape {
    public int pointsCount();
    public Point2D point(int index);
    public void draw(GraphicsContext gc);
}
