import javafx.scene.control.Alert;
import javafx.stage.FileChooser;

import java.io.*;
import java.util.*;

public class ShapeReader {
    public static Map<String, ShapeSerializer> map = new HashMap<>();

    public static void add(ShapeSerializer ss){
        map.put(ss.code(), ss);
    }

    public static Shape scanLine(String line) throws IOException {
        try (Scanner scanner = new Scanner(line)) {
            String code = scanner.next();
            System.out.println(line);
            ShapeSerializer<?> ser = map.get(code);
            if (ser == null) throw new IOException();
            return ser.unserialize(line);
        }
    }

    public static List<Shape> read(File file) throws IOException {
        List<Shape> listShape = new LinkedList<Shape>();
        BufferedReader buffer = new BufferedReader(new InputStreamReader(new FileInputStream(file)));

        String str;

        while( (str = buffer.readLine()) != null ){
            listShape.add( scanLine(str) );
        }

        return listShape;
    }

    static {
        CircleSerialiser circleSerializer = new CircleSerialiser();
        map.put(circleSerializer.code(), circleSerializer);

        RectangleSerialiser rectangleSerializer = new RectangleSerialiser();
        map.put(rectangleSerializer.code(), rectangleSerializer);
    }
}
