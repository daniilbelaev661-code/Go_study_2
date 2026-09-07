package creational.factorymethod;

import structural.decorator.BasicLamp;
import model.PanelElement;

public class LampFactory implements ElementFactory {
    @Override
    public PanelElement createElement() {
        return new BasicLamp();
    }
}