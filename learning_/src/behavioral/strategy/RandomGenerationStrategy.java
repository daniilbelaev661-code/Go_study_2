package behavioral.strategy;

import creational.factorymethod.*;
import model.Panel;
import model.PanelElement;
import structural.adapter.ColorToSymbolAdapter;
import structural.decorator.*;
import java.util.Random;

public class RandomGenerationStrategy implements GenerationStrategy {
    @Override
    public Panel generatePanel(int width, int height) {
        Panel panel = new Panel(width, height);
        ElementFactory[] factories = {new ButtonFactory(), new LampFactory()};
        Random rand = new Random();

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                PanelElement element = factories[rand.nextInt(2)].createElement();

                if (element instanceof BasicLamp lamp) {
                    element = new ColoredLampDecorator(lamp, new ColorToSymbolAdapter());
                }

                panel.addElement(x, y, element);
            }
        }
        return panel;
    }
}