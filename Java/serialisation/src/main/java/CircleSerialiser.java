import java.util.Scanner;

public class CircleSerialiser implements ShapeSerializer<Circle> {

    public String code(){ return "Circle"; }
    public String serialize(Circle shape) { return code() + " " + shape.getX() + " " + shape.getY() + " " + shape.getRadius(); }
    public Circle unserialize(String s) {

        try(Scanner scanner = new Scanner(s)) {
            String code = scanner.next();
            assert code.equals(code());
            String[] coords = s.split(" ");
            return new Circle(Double.valueOf(coords[1]), Double.valueOf(coords[2]), Double.valueOf(coords[3]));
        } // try resource autocloseable
    } // unserialize

    public static CircleSerialiser instance = new CircleSerialiser();
}
