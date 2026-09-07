package behavioral.observer;

import model.Button;
import structural.decorator.Lamp;

public interface PanelElementVisitor {
    void visit(Button button);
    void visit(Lamp lamp);
}
