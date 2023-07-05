#include "arquivo.h"
#include "card.h"
#include "listase.h"
#include "mount.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Integrantes: Mateus Alkmim, Tiago Prysthon, Guilherme Brasileiro, Tiago Vianna, Arthur Amorim

// cavar carta da pilha para a mão de um jogador
Card draw_card_from_pile(tp_pilha *pile, tp_listase **player_hand) {
  Card card;
  pop(pile, &card);
  insere_listase_no_fim(player_hand, card);
  return card;
}

int modulo(int x, int y) { return (x % y + y) % y; }

#ifdef _WIN32
#define CLEAR "cls"
#else
#define CLEAR "clear"
#endif

int main() {
  srand(time(NULL));

  limpar_arquivo();

  bool cartas_lancadas[13 * 4 + 2];
  tp_listase *jogadas = inicializa_listase();
  tp_listase *jogadas_fim =
      jogadas; // para acessar o último nó sem ter que percorrer tudo

  // Número de jogadores
  int player_count;
  printf("Digite número de jogadores: ");
  scanf("%d", &player_count);
  printf("\n");

  // Inserindo nomes no vetor
  char player_names[player_count][50];
  for (int i = 0; i < player_count; i++) {
    printf("Digite o nome do jogador %d: ", i + 1);
    scanf(" %[^\n]s", player_names[i]);
    printf("\n");
  }

  printf("Boa sorte ");
  for (int i = 0; i < player_count - 2; i++) {
    printf("%s, ", player_names[i]);
  }
  printf("%s e %s!\n", player_names[player_count - 2],
         player_names[player_count - 1]);
  printf("\n\n");


  // pilha de cava
  Card *cards = make_cards();
  shuffle_cards(cards);
  tp_pilha draw_pile;
  inicializa_pilha(&draw_pile);
  for (int i = 0; i < CARDS_SIZE; i++) {
    push(&draw_pile, cards[i]);
  }

  tp_listase *player_hands[player_count];
  for (int i = 0; i < player_count; i++) {
    player_hands[i] = inicializa_listase();
    for (int j = 0; j < 7; j++) {
      Card card;
      pop(&draw_pile, &card);
      insere_listase_no_fim(&player_hands[i], card);
    }
  }

  // Pilha de descarte (mesa)
  tp_pilha discard_pile;
  Card card;
  inicializa_pilha(&discard_pile);
  do {
    pop(&draw_pile, &card);
    push(&discard_pile, card);
  } while (card.type == CARD_DRAW_2 || card.type == CARD_SKIP ||
           card.type == CARD_REVERSE || card.type == CARD_WILD ||
           card.type == CARD_DRAW_4 || card.type == CARD_BACKSIDE);

  // índice do jogador do turno atual
  int turn = 0;
  // direção de turno (1 ou -1)
  int turn_direction = 1;

  // Criando arquivo para logs do jogo
  char msg[100];
  alterar_no_final("INÍCIO DO JOGO\n\nJogadores:");

  for (int i = 0; i < player_count; i++) {
    alterar_no_final(player_names[i]);
  }

  alterar_no_final("\nJogadas: ");

  while (true) {
    // carta no topo da mesa
    Card top_card;
    top(&discard_pile, &top_card);
    char *name = player_names[turn];
    tp_listase **hand = &player_hands[turn];

    system(CLEAR);
    printf("===== Turno do Jogador %s =====\n\n", name);

    // verificar se o jogador tem alguma carta válida
    tp_listase *atu = *hand;
    bool alguma_valida = false;
    while (atu != NULL) {
      // carta atual
      Card current_card = atu->info;

      if (match_cards(current_card, top_card)) {
        alguma_valida = true;
        break;
      }

      atu = atu->prox;
    }
    if (!alguma_valida) {
      // senão, cavar até aparecer uma valida

      // 1. cavar carta
      // 2. checar se a nova carta é valida
      // 3. se for válida, parar.
      //    caso contrário voltar para a etapa 1.
      printf("Nenhuma carta valida na mao\n");
      Card card;
      do {
        card = draw_card_from_pile(&draw_pile, hand);
        printf("Cavou ");
        print_card(card);
        printf("\n");

        // Alterando arquivo
        sprintf(msg, " - %s cavou a carta %s %s", name,
                card_type_names[card.type], card_color_names[card.color]);
        alterar_no_final(msg);
      } while (!match_cards(card, top_card));
      printf("\n"); 
    }
    
    bool print_game_state = true;

    // deixar o jogador escolher a carta
    int escolha;
    do {
      if (print_game_state) {
        printf("Carta do topo: ");
        print_card(top_card);
        printf("\n\n");

        // imprimir a mão do jogador atual
        printf("Mao do jogador:\n");
        print_cards_with_pos(*hand);
        printf("\n");

        print_game_state = false;
      }

      printf("[0] Verificar se uma carta ja foi lancada\n\n");

      printf("Escolha uma opção: ");
      scanf("%d", &escolha);

      if (escolha == 0) {
        print_game_state = true;
        Card card_search;

        printf("[0-9] numeros\n"
               "[10] cava 2\n"
               "[11] pula\n"
               "[12] reverso\n"
               "[13] troca cor\n"
               "[14] cava 4\n");

        printf("Escolha um tipo de carta: ");
        scanf("%d", &card_search.type);

        if (card_search.type == 13 || card_search.type == 14) {
          card_search.color = CARD_BLACK;
        } else {
          printf("[0] amarelo\n"
                 "[1] vermelho\n"
                 "[2] azul\n"
                 "[3] verde\n");
          printf("Escolha uma cor: ");
          int c;
          scanf("%d", &c);
          card_search.color = (CardColor)(c + CARD_YELLOW);
        }

        int id = get_card_id(card_search);
        printf("\nA carta ");
        print_card(card_search);
        if (cartas_lancadas[id]) {
          printf(" ja foi lancada\n\n");
        } else {
          printf(" nao foi lancada ainda\n\n");
        }
        continue;
      }

      escolha -= 1;
      if (escolha < 0) {
        continue;
      }

      Card card;
      if (!obter_por_indice(*hand, escolha, &card)) {
        printf("Nao pode jogar esta carta\n");
        continue;
      }

      // verificar se a carta é válida
      if (match_cards(card, top_card)) {
        break;
      }

      printf("Nao pode jogar esta carta\n");
    } while (true);

    // jogar a carta
    Card card;
    remover_por_indice(hand, escolha, &card);
    push(&discard_pile, card);

    // Gravando no arquivo qual carta foi jogada
    sprintf(msg, "%s jogou a carta %s %s", name, card_type_names[card.type],
            card_color_names[card.color]);
    alterar_no_final(msg);

    if (jogadas_fim == NULL) {
      jogadas_fim = aloca_listase();
      jogadas_fim->info = card;
      jogadas_fim->prox = NULL;
      jogadas = jogadas_fim;
    } else {
      jogadas_fim->prox = aloca_listase();
      jogadas_fim = jogadas_fim->prox;
      jogadas_fim->info = card;
      jogadas_fim->prox = NULL;
    }

    cartas_lancadas[get_card_id(card)] = true;

    // fazer ação das cartas especiais
    if (card.type == CARD_DRAW_2 || card.type == CARD_DRAW_4) {
      int total = card.type == CARD_DRAW_2 ? 2 : 4;
      int next_player = modulo(turn + turn_direction, player_count);
      for (int i = 0; i < total; i++) {
        Card newCards =
            draw_card_from_pile(&draw_pile, &player_hands[next_player]);

        // Alterando arquivo
        sprintf(msg, " - %s cavou a carta %s %s", player_names[next_player],
                card_type_names[newCards.type],
                card_color_names[newCards.color]);
        alterar_no_final(msg);
      }
    }
    if (card.type == CARD_DRAW_2 || card.type == CARD_DRAW_4 ||
        card.type == CARD_SKIP) {
      turn = modulo(turn + turn_direction, player_count);
    } else if (card.type == CARD_REVERSE) {
      turn_direction *= -1;
    }
    if (card.type == CARD_WILD || card.type == CARD_DRAW_4) {
      while (true) {
        printf("[1] \x1b[33mAmarelo\x1b[0m\n"
               "[2] \x1b[31mVermelho\x1b[0m\n"
               "[3] \x1b[34mAzul\x1b[0m\n"
               "[4] \x1b[32mVerde\x1b[0m\n"
               "Escolha uma cor: ");
        int color;
        scanf("%d", &color);
        if (color >= 1 && color <= 4) {
          Card card;
          pop(&discard_pile, &card);
          card.color = (CardColor)color;
          push(&discard_pile, card);
          sprintf(msg, " - %s trocou para a cor %s", name,
                  card_color_names[color]);
          alterar_no_final(msg);
          break;
        }
      }
    }

    if (tamanho_listase(*hand) == 0) {
      printf("\n\n!!! Jogador %s Ganhou !!!\n", name);
      // Alterando arquivo
      sprintf(msg, "\n\n!!! Jogador %s Ganhou !!!\n", name);
      alterar_no_final(msg);
      break;
    }

    // passar o turno
    turn = modulo(turn + turn_direction, player_count);

    if(pilha_vazia(&draw_pile) == 0){
      shuffle_cards(cards);
      for (int i = 0; i < CARDS_SIZE; i++) {
        push(&draw_pile, cards[i]);
      }
    }
  }
}
