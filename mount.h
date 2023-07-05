#include "card.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Integrantes: Mateus Alkmim, Tiago Prysthon, Guilherme Brasileiro, Tiago Vianna, Arthur Amorim

void inicializa_pilha(tp_pilha *p) { p->topo = -1; }

int pilha_vazia(tp_pilha *p) {
  if (p->topo == -1)
    return 1;
  else
    return 0;
}

int pilha_cheia(tp_pilha *p) {
  if (p->topo == MAX_PILHA - 1)
    return 1;
  else
    return 0;
}

int push(tp_pilha *p, tp_item_pilha e) {
  if (pilha_cheia(p))
    return 0;
  p->topo++;
  p->item[p->topo] = e;
  return 1;
}

int pop(tp_pilha *p, tp_item_pilha *e) {
  if (pilha_vazia(p))
    return 0;
  *e = p->item[p->topo];
  p->topo--;
  return 1;
}

int top(tp_pilha *p, tp_item_pilha *e) {
  if (pilha_vazia(p))
    return 0;

  *e = p->item[p->topo];
  return 1;
}

void imprime_pilha(tp_pilha p) {
  tp_item_pilha e;
  // printf("\n");
  while (!pilha_vazia(&p)) {
    pop(&p, &e);
    print_card(e);
    printf("\n");
  }
}

int altura_pilha(tp_pilha *p) { return p->topo + 1; }

int empilha_pilhas(tp_pilha *p1, tp_pilha *p2) {
  tp_item_pilha e;
  tp_pilha paux;
  inicializa_pilha(&paux);
  if (altura_pilha(p1) + altura_pilha(p2) > MAX_PILHA)
    return 0;
  while (!pilha_vazia(p2)) {
    pop(p2, &e);
    push(&paux, e);
  }
  while (!pilha_vazia(&paux)) {
    pop(&paux, &e);
    push(p1, e);
  }
  return 1;
}
