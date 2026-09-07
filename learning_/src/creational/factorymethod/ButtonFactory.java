package creational.factorymethod;

import model.Button;
import model.PanelElement;

public class ButtonFactory implements ElementFactory {
    @Override
    public PanelElement createElement() {
        return new Button();
    }
}
