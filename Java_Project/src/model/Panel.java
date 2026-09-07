package model;

import creational.singleton.Logger;
import behavioral.observer.PanelElementVisitor;

public class Panel {
    private PanelElement[][] grid;
    private Logger logger = Logger.getInstance();

    public Panel(int width, int height) {
        grid = new PanelElement[width][height];
    }

    public int getHeight() {
        return grid[0].length;
    }

    public void accept(PanelElementVisitor visitor) {
        for (int x = 0; x < grid.length; x++) {
            for (int y = 0; y < grid[0].length; y++) {
                grid[x][y].accept(visitor); // Теперь безопасно
            }
        }
    }

    public void addElement(int x, int y, PanelElement element) {
        grid[x][y] = element;
        logger.log("Добавлен элемент в (" + x + "," + y + ")");
    }

    public String visualize() {
        StringBuilder sb = new StringBuilder();
        for (int y = 0; y < grid[0].length; y++) {
            for (int x = 0; x < grid.length; x++) {
                sb.append(grid[x][y].getVisualization()).append(" - ");
            }
            sb.append("\n");
        }
        return sb.toString();
    }

    public PanelElement getElement(int x, int y) {
        return grid[x][y];
    }
}
