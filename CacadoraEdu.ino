//! ver se os sensores da esquerda estão na direita e vice versa (incluindo frente)(acho que tão)

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
enum estado {
    G_ESQ,
    G_FRENTE_ESQ,
    RETO,
    G_FRENTE_DIR,
    G_DIR,
};
enum estrategia {
    PIAO = 4,
    ZIGZAG = 6,
    MADMAX = 9,
};

enum estado estado_atual;
enum estrategia estrategia = PIAO;

void setup() { 
    init_edu(9600);
    IR.begin(PINO_IR);
    IR.setLed(2,HIGH,180);
}


void loop() {  
    IR.update();

    struct leitura sensores = leitura_sensores();
    mostra_sensores_no_led(sensores);

    if (IR.available()) { /* quando o sensor tiver ativado */
        int cmd = IR.read(); // salva o número lido pelo sensor, estando ou não de 1 a 3
        if (cmd >= 4 && cmd <= 9) {
            estrategia = (enum estrategia)cmd; // substitui a estratégia atual por esse número
            mover(0,0); mostra_estrategia_no_led(estrategia); delay(100);
        }
    }

    if (IR.prepare()) { /* robô em preparação */
        Serial.println("Preparar");
        estado_atual = G_DIR;
        mover(0,0);

    } else if (IR.start()) {
        Serial.println("-> sumo start");

    } else if (IR.on()) {
        enum simbolo simb;

        // retorna o símbolo adequado de acordo com os sensores
        if      (sensores.frente_esq &&
                 sensores.frente_dir) simb = FRENTE;
        else if (sensores.esq)        simb = ESQ;
        else if (sensores.dir)        simb = DIR;
        else if (sensores.frente_esq) simb = FRENTE_ESQ;
        else if (sensores.frente_dir) simb = FRENTE_DIR;
        else                          simb = NADA;

        switch (estrategia) {
            case PIAO: {
                estado_atual = prox_estado(estado_atual, simb);
                acao_atual(estado_atual);
            } break;

            case MADMAX: { 
                Serial.println("FAZENDO MADMAX");
                mover(1023, 1023);
            } break;

            case ZIGZAG: {
                //! implementar
            } break;
        }

    } else if (IR.stop()){
        Serial.println("-> sumo stop");
        mover(0,0);
    }
}

// Define os próximos estados com base no estado atual e símbolo lido
estado prox_estado(estado e, simbolo s) {
    switch (e) {
        case G_DIR:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            } break;
        case RETO:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            } break;
        case G_ESQ:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_ESQ;
            } break;
        case G_FRENTE_ESQ:
            switch(s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_ESQ;
            } break;
        case G_FRENTE_DIR:
            switch(s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            } break;
    }

    return e; // valor padrão de segurança
}


void acao_atual(estado e) {
    switch (e) {
        case RETO: {
            Serial.println("EMPURRANDO");
            mover(1023,1023);
        } break;
        case G_ESQ: {
            Serial.println("GIRANDO PRA ESQUERDA");
            mover(-600,600);
        } break;
        case G_FRENTE_ESQ: {
            Serial.println("GIRANDO LEVE PARA ESQUERDA");
            mover(900, 1023);
        } break;
        case G_FRENTE_DIR: {
            Serial.println("GIRANDO LEVE PARA DIREITA");
            mover(1023, 900);
        } break;
        case G_DIR: {
            Serial.println("GIRANDO PRA DIREITA");
            mover(200,-200);
        } break;
    }
}