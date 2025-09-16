#include <Arduino.h>

#if defined(INVERTER_MOTOR_DIR_ESQ)
    #define roda_esq_m1 18
    #define roda_esq_m2 19
    #define roda_dir_m1 4
    #define roda_dir_m2 23
#else
    #define roda_esq_m1 4
    #define roda_esq_m2 23
    #define roda_dir_m1 18
    #define roda_dir_m2 19
#endif
#include "motores.h"

#if defined(INVERTER_SENSOR_DIR_ESQ)
    #define sensor_esq 32
    #define sensor_dir 27
    #define sensor_frente_esq 26
    #define sensor_frente_dir 33
#else
    #define sensor_esq 27
    #define sensor_dir 32
    #define sensor_frente_esq 33
    #define sensor_frente_dir 26
#endif
#include "sensores.h"

void init_edu(unsigned int baud = 9600) {
    init_motores();
    init_sensores();

    Serial.begin(baud);
}
