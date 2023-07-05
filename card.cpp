#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Integrantes: Mateus Alkmim, Tiago Prysthon, Guilherme Brasileiro, Tiago Vianna, Arthur Amorim

const char *card_type_names[] = {
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
  "cava_2", "pula", "reverso", "troca_cor", "cava_4", "capa",
};
const char *card_color_names[] = {
  "preto", "amarelo", "vermelho", "azul", "verde",
};
const char *card_ansi_code[] = {
  "\x1b[40m", // preto
  "\x1b[33m", // amarelo
  "\x1b[31m", // vermelho
  "\x1b[34m", // azul
  "\x1b[32m", // verde
};

// cria cada carta do baralho
Card *make_cards() {
  Card *cards = (Card *)malloc(CARDS_SIZE * sizeof(Card));
  size_t idx = 0;
  for (int color = CARD_YELLOW; color <= CARD_GREEN; color++) {
    cards[idx++] = (Card){CARD_0, (CardColor)color};
    for (int type = CARD_1; type <= CARD_REVERSE; type++) {
      for (int r = 0; r < 2; r++) {
        cards[idx++] = (Card){(CardType)type, (CardColor)color};
      }
    }
  }
  for (int r = 0; r < 4; r++) {
    cards[idx++] = (Card){CARD_WILD, CARD_BLACK};
    cards[idx++] = (Card){CARD_DRAW_4, CARD_BLACK};
  }

  // printf("%d\n", (int)idx);
  return cards;
}

void print_card(Card card) {
  // printf("%s %s", card_type_names[card.type], card_color_names[card.color]);
  printf("%s%s %s%s",
    card_ansi_code[card.color],
    card_type_names[card.type],
    card_color_names[card.color],
    "\033[0m");
}

// embaralha as cartas
void shuffle_cards(Card *cards) {
  for (int i = 0; i < CARDS_SIZE; i++) {
    int r = rand() % CARDS_SIZE;
    Card temp = cards[i]; // variavel vai guardar a carta i
    cards[i] = cards[r];  // vai colocar a carta r no lugar da i
    cards[r] = temp;      // vai colocar o i no lugar da r
  }
}

// verifica se pode colocar a carta A em cima da carta B
bool match_cards(Card a, Card b) {
    return a.type == b.type ||
        a.color == b.color ||
        a.color == CARD_BLACK;
}

int get_card_id(Card card) {
  if(card.type == CARD_WILD || card.type == CARD_DRAW_4) {
    return card.type - CARD_WILD;
  } else {
    return 2 + (card.type*4 + (card.color - CARD_YELLOW));
  }
}
Card get_card_from_id(int id) {
  if(id == 0) return (Card){CARD_WILD, CARD_BLACK};
  if(id == 1) return (Card){CARD_DRAW_4, CARD_BLACK};
  int type = (id-2)/4;
  int color = (id-2)%4;
  return (Card){(CardType)type, (CardColor)(color+1)};
}