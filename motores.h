#include <Arduino.h>
#include <inttypes.h>

void motor(uint8_t m1, uint8_t m2, int16_t vel);

void init_motores(void) {
    pinMode(roda_esq_m1, OUTPUT);
    pinMode(roda_esq_m2, OUTPUT);
    pinMode(roda_dir_m1, OUTPUT);
    pinMode(roda_dir_m2, OUTPUT);
}

void mover(int16_t esq, int16_t dir) {
    motor(roda_esq_m1, roda_esq_m2, esq);
    motor(roda_dir_m1, roda_dir_m2, dir);
}

void motor(uint8_t m1, uint8_t m2, int16_t vel) {
    if (vel < 0) {
        analogWrite(m1, abs(vel));
        analogWrite(m2, 0);
    } else {
        analogWrite(m1, 0);
        analogWrite(m2, vel);
    }
}
