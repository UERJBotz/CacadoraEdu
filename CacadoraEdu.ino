#include "edu.h"
#include "SumoIR.h"
#include "LEDFX.h"

#define PINO_IR 15
SumoIR IR;
unsigned long tempo_anterior = 0;
 
enum simbolo {
    NADA = 0,
    FRENTE,
    ESQ,
    DIR,
    FRENTE_ESQ,
    FRENTE_DIR,
    TEMP0,
    TEMP1,
    TEMP2,
    TEMP3,
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
    MADMAX = 5,
    MADMAX_ESQUERDA = 6,
    ZIGZAG = 7, 
};

enum estado estado_atual;
enum estrategia estrategia = PIAO;
enum estrategia estrategia2   = ZIGZAG;

void setup() { 
    init_edu(9600);
    IR.begin(PINO_IR);
    IR.setLed(2,HIGH,180);
}


void loop() {  
    IR.update();
    unsigned long dt = millis() - tempo_anterior;
    struct leitura sensores = leitura_sensores();
    mostra_sensores_no_led(sensores);

    // Recebe comandos via controle IR
    if (IR.available()) {
        int cmd = IR.read();
        if (cmd >= 4 && cmd <= 9) {
            estrategia = (enum estrategia)cmd;
            mover(0, 0);
            mostra_estrategia_no_led(estrategia);
            delay(100);
        }
    }

    // Estado de preparação
    if (IR.prepare()) {
        Serial.println("Preparar");
        estado_atual = G_DIR;
        mover(0, 0);
    } 

    // Início da luta
    else if (IR.start()) {
        Serial.println("-> sumo start");
    } 

    // Robô ligado e em combate
    else if (IR.on()) {
        enum simbolo simb; // visível em todo o bloco abaixo

        if      (sensores.frente_esq &&
                 sensores.frente_dir) simb = FRENTE;
        else if (sensores.esq)        simb = ESQ;
        else if (sensores.dir)        simb = DIR;
        else if (sensores.frente_esq) simb = FRENTE_ESQ;
        else if (sensores.frente_dir) simb = FRENTE_DIR;
        else if (dt < 3000)           simb = TEMP0;
        else if (dt < 5000)           simb = TEMP1;
        else if (dt < 8000)           simb = TEMP2;
        else if (dt < 10000)          simb = TEMP3;
        else                          simb = NADA;

        switch (estrategia) {
            case PIAO:
                estado_atual = prox_estado(estado_atual, simb);
                acao_atual(estado_atual);
                break;

            case MADMAX:
                Serial.println("FAZENDO MADMAX");
                mover(1023, 1023);
                break;

            case MADMAX_ESQUERDA:
                mover(-1023, 1023);
                mostra_estrategia_no_led(9);
                break;

            case ZIGZAG:
                estado_atual = estado_zigzag(estado_atual, simb);
                acao_atual(estado_atual);
                break;
        }
    }

    // Parada
    else if (IR.stop()) {
        Serial.println("-> sumo stop");
        mover(0, 0);
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


    estado estado_zigzag(estado f, simbolo s) {
        switch (f) {
            case TEMP0:
                switch (s) {
                    case TEMP0:      return G_DIR;
                    case TEMP1:      return RETO;
                    case TEMP2:      return G_ESQ;
                    case TEMP3:      return RETO;
                    case FRENTE:     return RETO;
                    case ESQ:        return G_ESQ;
                    case FRENTE_ESQ: return G_FRENTE_ESQ;
                    case FRENTE_DIR: return G_FRENTE_DIR;
                    case DIR:        return G_DIR;
                    case NADA:       return G_DIR;
                }
             break;

            case TEMP1:
                switch (s) {
                    case TEMP0:      return G_DIR;
                    case TEMP1:      return RETO;
                    case TEMP2:      return G_ESQ;
                    case TEMP3:      return RETO;
                    case FRENTE:     return RETO;
                    case ESQ:        return G_ESQ;
                    case FRENTE_ESQ: return G_FRENTE_ESQ;
                    case FRENTE_DIR: return G_FRENTE_DIR;
                    case DIR:        return G_DIR;
                    case NADA:       return G_DIR;
                }
                break;

            case TEMP2:
                switch (s) {
                    case TEMP0:      return G_DIR;
                    case TEMP1:      return RETO;
                    case TEMP2:      return G_ESQ;
                    case TEMP3:      return RETO;
                    case FRENTE:     return RETO;
                    case ESQ:        return G_ESQ;
                    case FRENTE_ESQ: return G_FRENTE_ESQ;
                    case FRENTE_DIR: return G_FRENTE_DIR;
                    case DIR:        return G_DIR;
                    case NADA:       return G_DIR;
            }
            break;

        case TEMP3:
            switch (s) {
                case TEMP0:      return G_DIR;
                case TEMP1:      return RETO;
                case TEMP2:      return G_ESQ;
                case TEMP3:      return RETO;
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            }
            break;
        case RETO:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            }
            break;

        case G_ESQ:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_ESQ;
            }
            break;

        case G_FRENTE_ESQ:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_ESQ;
            }
            break;

        case G_FRENTE_DIR:
            switch (s) {
                case FRENTE:     return RETO;
                case ESQ:        return G_ESQ;
                case FRENTE_ESQ: return G_FRENTE_ESQ;
                case FRENTE_DIR: return G_FRENTE_DIR;
                case DIR:        return G_DIR;
                case NADA:       return G_DIR;
            }
            break;
    }

    return f;  // retorno seguro
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


