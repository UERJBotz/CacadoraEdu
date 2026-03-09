#include "maquina.h"

enum zig_zag {
  Z_ZERO,
  Z_UM,
  Z_DOIS,
  Z_TRES,
  Z_QUATRO,
};

bool resetar = true;

enum zig_zag prox_zig_zag(enum zig_zag e, enum simbolo s) {
  unsigned long t0_zig_zag = 0;
  const unsigned long dt = millis() - t0_zig_zag;
  switch (e) {
      case Z_ZERO:
              t0_zig_zag = millis();
              return Z_UM;
        
      case Z_UM:
          if (dt < 160) return e;
          else {
              t0_zig_zag = millis();
              return Z_DOIS;
          }
          
      case Z_DOIS:
          if (dt < 120) return e;
          else {
              t0_zig_zag = millis();
              return Z_TRES;
          }

      case Z_TRES:
          if (dt < 160) return e;
          else {
              t0_zig_zag = millis();
              return Z_QUATRO;
          }

      case Z_QUATRO:
          if (dt < 120) return e;
          else {
              t0_zig_zag = millis();
              estrategia PIAO;
              resetar = true;
              return Z_ZERO;
          }
  }
  return e;
}

void acao_zig_zag(enum zig_zag e) {
  const int16_t vel = 50, vel_max = 100;

  switch (e) {
    case Z_ZERO: break;
    case Z_UM: {
        Serial.println("GIRANDO PRA ESQUERDA");
        mover(vel, -vel);
      } break;
    case Z_DOIS: {
        Serial.println("ANDANDO RETO");
        mover(vel_max, vel_max);
      } break;
    case Z_TRES: {
        Serial.println("GIRANDO PRA DIREITA");
        mover(-vel, vel);
      } break;
    case Z_QUATRO: {
        Serial.println("ANDANDO RETO");
        mover(vel_max, vel_max);
      } break; 
  }
}
