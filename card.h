#pragma once

// Integrantes: Mateus Alkmim, Tiago Prysthon, Guilherme Brasileiro, Tiago
// Vianna, Arthur Amorim

// Numero de cartas num baralho
#define CARDS_SIZE 108

// Tipo da carta
typedef enum CardType {
  CARD_0,
  CARD_1,
  CARD_2,
  CARD_3,
  CARD_4,
  CARD_5,
  CARD_6,
  CARD_7,
  CARD_8,
  CARD_9,
  CARD_DRAW_2,   // Cava 2
  CARD_SKIP,     // Pular
  CARD_REVERSE,  // Reverso
  CARD_WILD,     // Troca cor
  CARD_DRAW_4,   // Cava 4
  CARD_BACKSIDE, // Capa
} CardType;

// Cor da carta
typedef enum CardColor {
  CARD_BLACK,  // Preto
  CARD_YELLOW, // Amarelo
  CARD_RED,    // Vermelho
  CARD_BLUE,   // Azul
  CARD_GREEN,  // Verde
} CardColor;

typedef struct Card {
  CardType type;
  CardColor color;
} Card;

// Mapeia os valores do enum CardType para uma string
extern const char *card_type_names[];
// Mapeia os valores do enum CardColor para uma string
extern const char *card_color_names[];

extern const char *card_ansi_code[];

// TESTE
static const int colors[] = {
    16, 6, 4, 1, 2,
};
// FIM DO TESTE

// Definindo estruturas da pilha e lista
typedef Card tp_item_lista;

typedef struct tp_no {
  tp_item_lista info;
  struct tp_no *prox;
} tp_listase;

#define MAX_PILHA 200

typedef Card tp_item_pilha;

typedef struct {
  int topo;
  tp_item_pilha item[MAX_PILHA];
} tp_pilha;

Card *make_cards();
void shuffle_cards(Card *cards);
void print_card(Card card);
bool match_cards(Card a, Card b);
int get_card_id(Card card);
Card get_card_from_id(int id);
