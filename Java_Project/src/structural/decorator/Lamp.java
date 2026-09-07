package structural.decorator;

import model.PanelElement;
import behavioral.observer.PanelElementVisitor;
public interface Lamp extends PanelElement {
    void activate();
    void deactivate();
    @Override
    default void accept(PanelElementVisitor visitor) {
        visitor.visit(this);
    }
}
