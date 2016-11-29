import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

public class ShapeReader {
    public static Map<String, ShapeSerializer> map = new HashMap<>();

    public static void add(ShapeSerializer ss){
        map.put(ss.code(), ss);
    }

    public static Shape scanLine(String line) throws IOException {
        try(Scanner scanner = new Scanner(line)){
            String code = scanner.next();
            ShapeSerializer<?> ser = map.get(code);
            if(ser == null) throw new IOException();
            return ser.unserialize(line);
        }
    }
}
