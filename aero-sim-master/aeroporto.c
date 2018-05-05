#include "aeroporto.h"

/**
 * aeroporto.c
 * Implementação das funções do aeroporto.h
 * Descrições em aeroporto.h
 **/

aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args) {
	aeroporto_t aeroporto;
	aeroporto.n_pistas = args[0];
	aeroporto.n_portoes = args[1];
	aeroporto.n_esteiras = args[2];
	aeroporto.n_max_avioes_esteira = args[3];

	aeroporto.t_pouso_decolagem = args[4];
	aeroporto.t_remover_bagagens = args[5];
	aeroporto.t_inserir_bagagens = args[6];
	aeroporto.t_bagagens_esteira = args[7];

	sem_init(&semaforo_pistas);
	sem_init(&semaforo_esteiras)

	return aeroporto;
}

void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {

}

void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {

}

int finalizar_aeroporto (aeroporto_t* aeroporto) {
	return 0;
}
