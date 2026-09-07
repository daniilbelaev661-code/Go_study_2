package structural.decorator;

public class BasicLamp implements Lamp {
    private boolean active;

    @Override
    public void activate() {
        active = true;
    }

    @Override
    public void deactivate() {
        active = false;
    }

    @Override
    public String getVisualization() {
        return active ? "Л_?" : "Л_";
    }
}