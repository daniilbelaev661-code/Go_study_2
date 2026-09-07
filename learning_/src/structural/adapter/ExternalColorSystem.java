package structural.adapter;

public class ExternalColorSystem {
    public String getColorCode(int r, int g, int b) {
        return String.format("#%02X%02X%02X", r, g, b);
    }
}