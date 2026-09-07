import behavioral.strategy.GenerationStrategy;
import behavioral.strategy.RandomGenerationStrategy;
import model.Button;
import model.Panel;
import model.PanelElement;
import behavioral.observer.ButtonActionVisitor;
import java.util.Scanner;

public class ControlPanelApp {
    public static void main(String[] args) {
        GenerationStrategy strategy = new RandomGenerationStrategy();
        Panel panel = strategy.generatePanel(4, 4);

        Scanner scanner = new Scanner(System.in);
        while (true) {
            System.out.println(panel.visualize());
            System.out.print("Координаты (x y): ");
            int x = scanner.nextInt();
            int y = scanner.nextInt();

            PanelElement element = panel.getElement(x, y);
            if (element instanceof Button button) {
                button.press();
                panel.accept(new ButtonActionVisitor(panel, x));
            }
        }
    }
}