package structural.decorator;

import structural.adapter.ColorAdapter;

public class ColoredLampDecorator implements Lamp {
    private Lamp wrappee;
    private ColorAdapter colorAdapter;

    public ColoredLampDecorator(Lamp lamp, ColorAdapter adapter) {
        this.wrappee = lamp;
        this.colorAdapter = adapter;
    }

    @Override
    public void activate() {
        wrappee.activate();
    }

    @Override
    public void deactivate() {
        wrappee.deactivate();
    }

    @Override
    public String getVisualization() {
        return wrappee.getVisualization().replace("?", colorAdapter.getSimpleColor());
    }
}
