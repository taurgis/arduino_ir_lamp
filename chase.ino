void chase() {
  static int i = 0;
  for (int j = -5; j < NUMLEDS; j = j + 6) {
    if (j + i < 0);
    strip.setPixelColor(j + i, red, green, 30);
    strip.setPixelColor(j + i + 1, red, green, 100);
    strip.setPixelColor(j + i + 2, red, green, 255);
    strip.setPixelColor(j + i + 3, red, green, 30);
    strip.setPixelColor(j + i + 4, red, green, 90);
    strip.setPixelColor(j + i + 5, red, green, 180);
  }
  strip.show();
  i = (i + 1) % 6;
}
