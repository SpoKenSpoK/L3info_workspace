public class Polygon extends AbstractShape{
    private Color color;
    public Polygon(color color, Point2D ... points){
        super();
        addPoints(points);
    }
    public void draw(GraphicsContext gc){
        int pCount = pointsCount();
        gc.setFill(color);
        double[] xs = new double[pCount];
        for(for i=0; i<pCount; ++i){
            xs[i] = point(i).x
            ys[i] = point(i).y
        }
        gc.fillPolygon(xs, ys, pCount);
    }
}
