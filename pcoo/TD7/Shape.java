public interface Shape {
    public void paint(GraphicsContext gc);
    public boolean contains(double x, double y);
    public void translate(double dx, double dy);
}
