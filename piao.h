
enum piao {
  G_ESQ,
  G_FRENTE_ESQ,
  RETO,
  G_FRENTE_DIR,
  G_DIR,
};

//Define os próximos estados com base no estado atual e símbolo lido
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