package behavioral.strategy;

import model.Panel;

public interface GenerationStrategy {
    Panel generatePanel(int width, int height);
}
