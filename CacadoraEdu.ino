#include "SumoIR.h"
#include "LEDFX.h"
#include "edu.h"
#include "maquina.h"
#include "piao.h"
#include "zigzag.h"


#define PINO_IR 15
SumoIR IR;


enum estrategia estrategia = PIAO;

void setup() {
  init_edu(9600);
  IR.begin(PINO_IR);
  IR.setLed(2, HIGH, 180);
}


void loop() {

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

enum simbolo prox_simbolo(struct leitura sensores) {
  if (sensores.frente_esq && sensores.frente_dir) return FRENTE;
  else if (sensores.esq)                          return ESQ;
  else if (sensores.dir)                          return DIR;
  else if (sensores.frente_esq)                   return FRENTE_ESQ;
  else if (sensores.frente_dir)                   return FRENTE_DIR;
  else                                            return NADA;
}
