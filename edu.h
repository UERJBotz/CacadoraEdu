#ifndef _EDU_H_
#define _EDU_H_

#include <inttypes.h>

void init_edu(unsigned int baud);

bool dist_frente_esq(void);
bool dist_frente_dir(void);
bool dist_esq(void);
bool dist_dir(void);

struct leitura {
  bool esq, frente_esq, frente_dir, dir;
};

leitura leitura_sensores() {
  return {
    .esq        = dist_esq(),
    .frente_esq = dist_frente_esq(),
    .frente_dir = dist_frente_dir(),
    .dir        = dist_dir(),
  };
}

void mover(int16_t esq, int16_t dir);

#endif//_EDU_H_