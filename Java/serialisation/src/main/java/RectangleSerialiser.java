import java.util.Scanner;

public class RectangleSerialiser implements ShapeSerializer<Rectangle> {

    @Override
    public String code() {
        return "Rectangle";
    }

    @Override
    public String serialize(Rectangle shape) {
        return String.format("%s %.1f %.1f %.1f %.1f", code(), shape.getX(), shape.getY(), shape.getWidth(), shape.getHeight());
    }

    @Override
    public Rectangle unserialize(String s) {
        try(Scanner scanner = new Scanner(s)) {
            String code = scanner.next();
            assert code.equals(code());
            double x = scanner.nextDouble();
            double y = scanner.nextDouble();
            double width = scanner.nextDouble();
            double height = scanner.nextDouble();

            return new Rectangle(x, y, width, height);
        }
    }

    public static RectangleSerialiser instance = new RectangleSerialiser();
}
