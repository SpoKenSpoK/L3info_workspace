import java.util.Scanner;

public class CircleSerialiser implements ShapeSerializer<Circle> {

    public String code(){ return "Circle"; }
    public String serialize(Circle shape) { return String.format("%s %.1f %.1f %.1f", code(), shape.getX(), shape.getY(), shape.getRadius()); }
    public Circle unserialize(String s) {

        try(Scanner scanner = new Scanner(s)) {
            String code = scanner.next();
            assert code.equals(code());
            double x = scanner.nextDouble();
            double y = scanner.nextDouble();
            double radius = scanner.nextDouble();

            return new Circle(x, y, radius);
        } // try resource autocloseable
    } // unserialize

    public static CircleSerialiser instance = new CircleSerialiser();
}
