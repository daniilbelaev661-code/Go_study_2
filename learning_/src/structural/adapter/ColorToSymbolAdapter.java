package structural.adapter;

import java.util.Random;

public class ColorToSymbolAdapter implements ColorAdapter {
    private ExternalColorSystem externalColor;

    public ColorToSymbolAdapter() {
        this.externalColor = new ExternalColorSystem();
    }

    @Override
    public String getSimpleColor() {
        String color = externalColor.getColorCode(
                new Random().nextInt(256),
                new Random().nextInt(256),
                new Random().nextInt(256)
        );
        return color.substring(1, 2);
    }
}