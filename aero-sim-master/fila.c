#include "fila.h"

/**
 * fila.c
 * Implementação das funções da fila.h
 * Descrições em fila.h
 **/

elemento_t * aloca_elemento (aviao_t * dado) {
    elemento_t *e = (elemento_t *) malloc(sizeof(elemento_t));
    e->dado = dado;

    return e;
}

void desaloca_elemento (elemento_t * elemento) {
    free(elemento);
}

fila_ordenada_t * criar_fila () {
    fila_ordenada_t *f = (fila_ordenada_t *) malloc(sizeof(elemento_t) * TAMANHO_MAX_FILA_AVIOES);
    f->n_elementos = 0;
    pthread_mutex_init(&f->mutex, NULL);

    return f;
}

void desaloca_fila (fila_ordenada_t * fila) {
    while(fila->n_elementos > 0) {
        elemento_t *aux = fila->ultimo;
        fila->ultimo = aux->anterior;
        free(aux);
    }
    pthread_mutex_destroy(&fila->mutex);
    free(fila);
}

void inserir (fila_ordenada_t * fila, aviao_t * dado) {
    pthread_mutex_lock(&fila->mutex);
    if (fila->n_elementos < TAMANHO_MAX_FILA_AVIOES) {
        elemento_t *e = aloca_elemento(dado);
        if (fila->n_elementos == 0) {
            fila->primeiro = e;
            fila->ultimo = e;
        } else {
            if (dado->combustivel < 10) {
                e->proximo = fila->primeiro;
                fila->primeiro->anterior = e;
                fila->primeiro = e;
            } else {
                e->anterior = fila->ultimo;
                fila->ultimo->proximo = e;
                fila->ultimo = e;
            }
        }
        fila->n_elementos++;
    }
    pthread_mutex_unlock(&fila->mutex);
}

aviao_t * remover (fila_ordenada_t * fila) {
    pthread_mutex_lock(&fila->mutex);
    if (fila->n_elementos > 0) {
        elemento_t *e = fila->primeiro;
        aviao_t *a = e->dado;
        fila->primeiro = e->proximo;
        desaloca_elemento(e);
        fila->n_elementos--;
        pthread_mutex_unlock(&fila->mutex);

        return a;
    } else {
        pthread_mutex_unlock(&fila->mutex);
        return NULL;
    }
}
