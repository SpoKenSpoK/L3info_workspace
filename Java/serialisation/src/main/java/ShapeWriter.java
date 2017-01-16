import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;

public class ShapeWriter implements ShapeVisitor<String> {
    public String visit(Circle c){
        return CircleSerialiser.instance.serialize(c);
    }

    public String visit(Rectangle r){
        return RectangleSerialiser.instance.serialize(r);
    }

    public static void write(File file, Iterable<Shape> shapes) throws FileNotFoundException {
        PrintWriter writer = new PrintWriter(file);

        for(Shape shape:shapes){
            String string = shape.accept(instance);
            writer.println(string);
            System.out.println(string);
        }

        writer.close();
    }

    public static ShapeWriter instance = new ShapeWriter();
}
