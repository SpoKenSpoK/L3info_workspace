import javafx.scene.canvas.Canvas;

public class FrenchPainter implements Painter {
    private GraphicsContext graphicsContext;

    public FrenchPainter(GraphicsContext graphicsContext){
        this.graphicsContext = graphicsContext;
    }

    @Override
    public void drawRectangle(double x, double y, double width, double height){
        graphicsContext.strockeRect(x, y, width, height);
    }

    @Override
    public void drawCircle(double x, double y, double radius){
        graphicsContext.strockeOval(x - radius, y - radius, 2*radius, 2*radius);
    }
}
