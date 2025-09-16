#ifndef ledFX_H
#define ledFX_H
#include "edu.h"
#include <Adafruit_NeoPixel.h>

#define PIN 2 //pino do anel de leds
#define NUMPIXELS 8 // quantidade de leds do anel
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800); // necessario


void setar_cor_leds(uint8_t r, uint8_t g, uint8_t b) {
  for (uint8_t i = 0; i < NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(r, g, b));
  }
}

void mostra_sensores_no_led(struct leitura leitura) {
  setar_cor_leds(3, 5, 3); // Define a cor padrão para todos os LEDs

  const auto azul = pixels.Color(0,   0,   150);
  
  if (leitura.esq)        pixels.setPixelColor(5, azul);
  if (leitura.frente_esq) pixels.setPixelColor(4, azul);
  if (leitura.frente_dir) pixels.setPixelColor(3, azul);
  if (leitura.dir)        pixels.setPixelColor(2, azul);

  pixels.show();
}

#endif //ledFX_H