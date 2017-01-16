import java.util.Scanner;

public class RectangleSerialiser implements ShapeSerializer<Rectangle> {

    @Override
    public String code() {
        return "Rectangle";
    }

    @Override
    public String serialize(Rectangle shape) {
        return code() + " " + shape.getX() + " " + shape.getY() + " " + shape.getWidth() + " " + shape.getHeight();
    }

    @Override
    public Rectangle unserialize(String s) {
        try(Scanner scanner = new Scanner(s)) {
            String code = scanner.next();
            assert code.equals(code());
            String[] coords = s.split(" ");
            return new Rectangle(Double.valueOf(coords[1]), Double.valueOf(coords[2]), Double.valueOf(coords[3]), Double.valueOf(coords[4]));
        }
    }

    public static RectangleSerialiser instance = new RectangleSerialiser();
}
