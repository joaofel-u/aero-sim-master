#include "stdlib.h"
#include "aviao.h"

/**
 * aviao.c
 * Implementação das funções do aviao.h
 * Descrições em aviao.h
 **/

aviao_t * aloca_aviao (size_t combustivel, size_t id) {
    aviao_t *a = (aviao_t *) malloc(sizeof(aviao_t));
    a->combustivel = combustivel;
    a->id = id;

    return a;
}

void desaloca_aviao(aviao_t* aviao) {
    pthread_exit(&(aviao->thread));
    free(aviao);
}
