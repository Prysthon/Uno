#ifndef ARQUIVO_H
#define ARQUIVO_H

#include <stdio.h>
#include <string.h>
#include <locale.h>

int alterar_no_final(char const *msg) {
  FILE * arq = fopen("logs.txt", "a");
  if (!arq) return 0;
  fprintf(arq,"%s\n", msg);
  fclose(arq);
  return 1;
}

int limpar_arquivo() {
    FILE * arq = fopen("logs.txt", "w");
    if (!arq) {
        return 0; 
    }
    fclose(arq);
    return 1; 
}

#endif
