#include "fila.h"

/**
 * fila.c
 * Implementação das funções da fila.h
 * Descrições em fila.h
 **/

elemento_t * aloca_elemento (aviao_t * dado) {
    return (elemento_t *) malloc(sizeof(elemento_t));
}

void desaloca_elemento (elemento_t * elemento) {
    free(elemento);
}

fila_ordenada_t * criar_fila () {
    return (fila_ordenada_t *) malloc(sizeof(aviao_t) * TAMANHO_MAX_FILA_AVIOES);
}

void desaloca_fila (fila_ordenada_t * fila) {
    free(fila);
}

void inserir (fila_ordenada_t * fila, aviao_t * dado) {
    elemento_t *e = aloca_elemento(dado);
    if (dado->combustivel < 10) {
        e->proximo = fila->primeiro;
        fila->primeiro->anterior = dado;
    } else {
        e->anterior = fila->ultimo;
        fila->ultimo->proximo = e;
    }
}

aviao_t * remover (fila_ordenada_t * fila) {
    elemento_t e = fila->primeiro;
    aviao_t *a = e->dado;
    fila->primeiro = e->proximo;
    desaloca_elemento(e);
    
    return a;
}
