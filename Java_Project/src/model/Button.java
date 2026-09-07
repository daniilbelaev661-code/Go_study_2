package model;

import behavioral.observer.PanelElementVisitor;

public class Button implements PanelElement {
    private boolean pressed;

    public void press() {
        pressed = !pressed;
    }

    @Override
    public String getVisualization() {
        return pressed ? "o" : "O";
    }

    @Override
    public void accept(PanelElementVisitor visitor) {
        visitor.visit(this);
    }

    public boolean isPressed() {
        return pressed;
    }
}


