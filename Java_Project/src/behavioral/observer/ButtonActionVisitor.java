package behavioral.observer;

import model.Button;
import model.Panel;
import model.PanelElement;
import structural.decorator.Lamp;

public class ButtonActionVisitor implements PanelElementVisitor {
    private final Panel panel;
    private final int pressedX;

    public ButtonActionVisitor(Panel panel, int x) {
        this.panel = panel;
        this.pressedX = x;
    }

    @Override
    public void visit(Button button) {
        if (button.isPressed()) {

            for (int y = 0; y < panel.getHeight(); y++) {
                PanelElement element = panel.getElement(pressedX, y);
                if (element instanceof Lamp lamp) {
                    lamp.activate();
                }
            }
        }
    }

    @Override
    public void visit(Lamp lamp) {

    }
}
