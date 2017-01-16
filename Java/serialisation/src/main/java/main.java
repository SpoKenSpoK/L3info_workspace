import javafx.application.Application;
import javafx.event.EventHandler;
import javafx.scene.Group;
import javafx.scene.Scene;
import javafx.scene.input.KeyEvent;
import javafx.stage.Stage;

public class main extends Application {
    private Drawer container;
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) {
        System.out.println("Start");
        Group root = new Group();
        container = new Drawer(800, 600);
        /*container.add(new Rectangle(10, 10, 100, 100));
        container.add(new Circle(400, 300, 100));*/

        container.setOnKeyPressed(new EventHandler<KeyEvent>() {

            @Override
            public void handle(KeyEvent event) {
                container.keyPressed(event);
            }
        });

        root.getChildren().add(container);
        container.repaint();
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }


}
