package model;
import behavioral.observer.PanelElementVisitor;
public interface PanelElement {
    String getVisualization();
    void accept(PanelElementVisitor visitor);
}
