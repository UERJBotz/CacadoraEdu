#include <Arduino.h>

#include "placa.h"

#define roda_esq_m1 MA1
#define roda_esq_m2 MA2
#define roda_dir_m1 MB2
#define roda_dir_m2 MB1
#include "motores.h"

#define sensor_esq BTN2
#define sensor_dir S8
#define sensor_frente_esq S5
#define sensor_frente_dir S4
#include "sensores.h"

void init_edu(unsigned int baud = 9600) {
    init_motores();
    init_sensores();

    Serial.begin(baud);
}
