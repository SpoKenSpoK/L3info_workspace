public abstract class AbstractShape{
    private List<Point2D> points;
    protected AbstractShape(){ points = new ArrayList<Point2D>(); }
    public int pointsCount(){ return point.size(); }
    public Point2D point(int index){ return points.get(index); }
    public void addPoints(Point2D ... points){
        for(Point2D p : points)
            this.points.add(p);
    }
    @Override
    public abstract void draw(GraphicsContext); // A implementer dans les sous-classes
}
