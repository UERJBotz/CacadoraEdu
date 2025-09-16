#include "digitalWriteFast.h"
#include <Arduino.h>
#include <inttypes.h>
#include <math.h>

void init_sensores(void) {
    pinMode(sensor_esq, INPUT);
    pinMode(sensor_dir, INPUT);

    pinMode(sensor_frente_esq, INPUT);
    pinMode(sensor_frente_dir, INPUT);
}

bool dist_frente_esq(void) { return digitalRead(sensor_frente_esq); }
bool dist_frente_dir(void) { return digitalRead(sensor_frente_dir); }
bool dist_esq(void)        { return digitalRead(sensor_esq); }
bool dist_dir(void)        { return digitalRead(sensor_dir); }

