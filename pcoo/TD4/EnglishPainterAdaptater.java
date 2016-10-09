public class EnglishPainterAdaptater implements Painter {
    private EnglishPainter englishPainter;

    public EnglishPainterAdaptater(EnglishPainter englishPainter){
        this.englishPainter = englishPainter;
    }

    @Override
    public void drawRectangle(double x, double y, double width, double height){
        Point2D pOne = new Point2D(x,y);
        Point2D pTwo = new Point2D(x+width, y+height);
        englishPainter.paintRectangle(pOne, pTwo);
    }

    @Override
    public void drawCircle(double x, double y, double radius){
        Point2D center = new Point2D(x,y);
        Point2D moon = new Point2D(x+radius,y);
        englishPainter.paintCircle(center, moon);
    }
}
