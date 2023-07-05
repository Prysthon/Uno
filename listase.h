#ifndef LISTASE_H
#define LISTASE_H

#include "card.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

tp_listase *inicializa_listase() { return NULL; }

int listase_vazia(tp_listase *lista) {
  if (lista == NULL)
    return 1;
  return 0;
}

tp_listase *aloca_listase() { // ela irá alocar um espaço na memória;
  tp_listase *novo_no;
  novo_no = (tp_listase *)malloc(sizeof(tp_listase));
  return novo_no;
}

/* Esse **l é um ponteiro que está apontando para outro ponteiro
para o endereço de memória original da listase*,
o que está apontando para o NULL */

int insere_listase_no_fim(tp_listase **l, tp_item_lista e) {
  tp_listase *novo_no, *atu;
  novo_no =
      aloca_listase(); // aqui está pegando o novo nó e apontando para o NULL;
  if (novo_no == NULL)
    return 0; // Não alocou memória
  // atribuir os valores para o novo nó;
  novo_no->info = e;
  novo_no->prox = NULL;
  // finaliza o encadeamento do nó
  if (listase_vazia(*l)) { // se for o primeiro nó entra aqui
    *l = novo_no;
  } else {
    atu = *l;
    while (atu->prox != NULL) {
      atu = atu->prox; // aponta para o ultimo nó
    }
    atu->prox = novo_no;
  }
  return 1;
}

tp_listase *obter_no_por_indice(tp_listase *lista, size_t indice) {
  if (indice < 0) {
    return NULL;
  }
  tp_listase *atu = lista;
  size_t i = 0;
  while (atu != NULL) {
    if (i++ == indice) {
      return atu;
    }
    atu = atu->prox;
  }
  return NULL;
}

bool obter_por_indice(tp_listase *lista, size_t indice, tp_item_lista *item) {
  tp_listase *no = obter_no_por_indice(lista, indice);
  if (no == NULL)
    return false;
  *item = no->info;
  return true;
}

bool remover_por_indice(tp_listase **lista, size_t indice,
                        tp_item_lista *item) {
  if (indice < 0 || lista == NULL || *lista == NULL)
    return false;
  if (indice == 0) {
    if (item != NULL) {
      *item = (*lista)->info;
    }
    *lista = (*lista)->prox;
    return *lista != NULL;
  }
  tp_listase *no_anterior = obter_no_por_indice(*lista, indice - 1);
  tp_listase *no_atual = no_anterior->prox;
  if (no_atual == NULL) {
    return false;
  }
  tp_listase *no_proximo = no_atual->prox;

  no_anterior->prox = no_proximo;
  if (item != NULL)
    *item = no_atual->info;
  free(no_atual);
  return true;
}

void imprime_listase(tp_listase *lista) {
  tp_listase *atu;
  atu = lista;
  while (atu != NULL) {
    print_card(atu->info);
    printf("\n");
    atu = atu->prox;
  }
}

void print_cards_with_pos(tp_listase *lista) {
  tp_listase *atu = lista;
  int pos = 1;
  while (atu != NULL) {
    printf("[%d] ", pos++);
    print_card(atu->info);
    printf("\n");
    atu = atu->prox;
  }
}

int tamanho_listase(tp_listase *lista) {
  int cont = 0;
  tp_listase *atu;
  atu = lista;
  while (atu != NULL) {
    cont++;
    atu = atu->prox;
  }
  return cont;
}

void destroi_listase(tp_listase **l) {
  tp_listase *atu;
  atu = *l;
  while (atu != NULL) {
    *l = atu->prox;
    free(atu);
    atu = *l;
  }
  *l = NULL;
}

#endif