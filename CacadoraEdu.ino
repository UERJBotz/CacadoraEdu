#include "edu.h"
#include "SumoIR.h"
#include "LEDFX.h"

#define PINO_IR 15
SumoIR IR;

enum simbolo {
  NADA = 0,
  FRENTE,
  ESQ,
  DIR,
  FRENTE_ESQ,
  FRENTE_DIR,
};

enum estrategia {
  PIAO = 4,
  ZIGZAG = 6,
  MADMAX = 9,
};

enum piao {
  G_ESQ,
  G_FRENTE_ESQ,
  RETO,
  G_FRENTE_DIR,
  G_DIR,
};
enum zig_zag {
  Z_ZERO,
  Z_UM,
  Z_DOIS,
  Z_TRES,
  Z_QUATRO,
};


enum estrategia estrategia = PIAO;
unsigned long t0_zig_zag = 0;

void setup() {
  init_edu(9600);
  IR.begin(PINO_IR);
  IR.setLed(2, HIGH, 180);
}


void loop() {
  static bool resetar = true;

  struct leitura sensores = leitura_sensores();
  mostra_sensores_no_led(sensores);

  IR.update();
  if (IR.available()) {   /* quando o sensor tiver ativado */
    int cmd = IR.read();  // salva o número lido pelo sensor, estando ou não de 1 a 3
    if (cmd == 3) resetar = true;
    if (cmd >= 4 && cmd <= 9) {
      estrategia = (enum estrategia)cmd;  // substitui a estratégia atual por esse número
      mover(0, 0);
      mostra_estrategia_no_led(estrategia);
      delay(100);
    }
  }

  if (IR.prepare()) { /* robô em preparação */
    Serial.println("Preparar");
    mover(0, 0);

  } else if (IR.start()) {
    Serial.println("-> sumo start");

  } else if (IR.on()) {
    enum simbolo simb = prox_simbolo(sensores);
    switch (estrategia) {
      default:
      case PIAO: {
        static enum piao estado;
        if (resetar) estado = G_DIR;
        estado = prox_piao(estado, simb);
        acao_piao(estado);
      } break;

      case MADMAX: {
        Serial.println("FAZENDO MADMAX");
        mover(1023, 1023);
      } break;

      case ZIGZAG: {
        static enum zig_zag estado;
        if (resetar) estado = Z_ZERO;
        estado = prox_zig_zag(estado, simb);
        acao_zig_zag(estado);
      } break;
    }

    resetar = false;
  } else if (IR.stop()) {
    Serial.println("-> sumo stop");
    mover(0, 0);
  } 
}

// Define os próximos estados com base no estado atual e símbolo lido
enum piao prox_piao(enum piao e, enum simbolo s) {
  switch (e) {
    case G_DIR:
      switch (s) {
        case FRENTE:         return RETO;
        case DIR:            return G_DIR;
        case FRENTE_DIR:     return G_FRENTE_DIR;
        case FRENTE_ESQ:     return G_FRENTE_ESQ;
        case ESQ:            return G_ESQ;
        case NADA:           return G_DIR;
      }break;

    case RETO:
      switch (s) {
        case FRENTE:         return RETO;
        case DIR:            return G_DIR;
        case FRENTE_DIR:     return G_FRENTE_DIR;
        case FRENTE_ESQ:     return G_FRENTE_ESQ;
        case ESQ:            return G_ESQ;
        case NADA:           return G_DIR;
      }break;

    case G_ESQ:
      switch (s) {
        case FRENTE:         return RETO;
        case ESQ:            return G_ESQ;
        case FRENTE_DIR:     return G_FRENTE_DIR;
        case FRENTE_ESQ:     return G_FRENTE_ESQ;
        case DIR:            return G_DIR;
        case NADA:           return G_ESQ;
      }break;

    case G_FRENTE_ESQ:
      switch (s) {
        case FRENTE:         return RETO;
        case ESQ:            return G_ESQ;
        case FRENTE_ESQ:     return G_FRENTE_ESQ;
        case FRENTE_DIR:     return G_FRENTE_DIR;
        case DIR:            return G_DIR;
        case NADA:           return G_ESQ;
      }break;

    case G_FRENTE_DIR:
      switch (s) {
        case FRENTE:         return RETO;
        case ESQ:            return G_ESQ;
        case FRENTE_ESQ:     return G_FRENTE_ESQ;
        case FRENTE_DIR:     return G_FRENTE_DIR;
        case DIR:            return G_DIR;
        case NADA:           return G_DIR;
      }break;
  }
  return e;  // valor padrão de segurança
}


enum zig_zag prox_zig_zag(enum zig_zag e, enum simbolo s) {
  const unsigned long dt = millis() - t0_zig_zag;
  switch (e) {
      case Z_ZERO:
          {
              t0_zig_zag = millis();
              return Z_UM;
          }
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
              estrategia = PIAO;
              return Z_ZERO;
          }
  }
  return e;
}

// Esses números são feitos para uso com bateria 4S.
// Não alterar sem comentar aqui anteriormente.
void acao_piao(enum piao e) {
  switch (e) {
    case RETO:
      {
        Serial.println("EMPURRANDO");
        mover(1023, 1023);
      }break;
    case G_ESQ:
      {
        Serial.println("GIRANDO PRA ESQUERDA");
        mover(70, -70);
      }break;
    case G_FRENTE_ESQ:
      {
        Serial.println("GIRANDO LEVE PARA ESQUERDA");
        mover(800, 1023);
      }break;
    case G_FRENTE_DIR:
      {
        Serial.println("GIRANDO LEVE PARA DIREITA");
        mover(1023, 800);
      }break;
    case G_DIR:
      {
        Serial.println("GIRANDO PRA DIREITA");
        mover(-70, 70);
      }break;
  }
}
void acao_zig_zag(enum zig_zag e) {
  const int16_t vel = 200, vel_max = 500;

  switch (e) {
    case Z_ZERO: break;
    case Z_UM: {
        Serial.println("GIRANDO PRA ESQUERDA");
        mover(-vel, vel);
      } break;
    case Z_DOIS: {
        Serial.println("ANDANDO RETO");
        mover(vel_max, vel_max);
      } break;
    case Z_TRES: {
        Serial.println("GIRANDO PRA DIREITA");
        mover(vel, -vel);
      } break;
    case Z_QUATRO: {
        Serial.println("ANDANDO RETO");
        mover(vel_max, vel_max);
      } break; 
  }
}


enum simbolo prox_simbolo(struct leitura sensores) {
  if (sensores.frente_esq && sensores.frente_dir) return FRENTE;
  else if (sensores.esq)                          return ESQ;
  else if (sensores.dir)                          return DIR;
  else if (sensores.frente_esq)                   return FRENTE_ESQ;
  else if (sensores.frente_dir)                   return FRENTE_DIR;
  else                                            return NADA;
}
