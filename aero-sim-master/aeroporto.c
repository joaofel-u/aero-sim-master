#include "aeroporto.h"

 /**
  * aeroporto.c
  * Implementação das funções do aeroporto.h
  * Descrições em aeroporto.h
  **/

 aeroporto_t* iniciar_aeroporto (size_t* args, size_t n_args) {

 	aeroporto_t *a = (aeroporto_t *) malloc(sizeof(aeroporto_t));
 	a->n_pistas = args[0];
 	a->n_portoes = args[1];
 	a->n_esteiras = args[2];
 	a->n_max_avioes_esteira = args[3];
 	a->t_pouso_decolagem = args[4];
 	a->t_remover_bagagens = args[5];
 	a->t_inserir_bagagens = args[6];
 	a->t_bagagens_esteira = args[7];
 	a->fila_pouso_decolagem = criar_fila();
 	a->fila_prioritaria = criar_fila();
 	sem_init(&a->pistas, 0, a->n_pistas);
 	sem_init(&a->portoes, 0, a->n_portoes);
 	sem_init(&a->esteiras, 0, (a->n_esteiras * a->n_max_avioes_esteira));
 	sem_init(&a->n_avioes, 0, 0);
 	return a;
 }

 void aproximacao_aeroporto (aeroporto_t* aeroporto, aviao_t* aviao) {
 	if (aviao->combustivel < 10)
 		inserir(aeroporto->fila_prioritaria, aviao);
 	else
 		inserir(aeroporto->fila_pouso_decolagem, aviao);

+	sem_post(&aeroporto->n_avioes);  // Sinaliza para a thread aeroporto que um aviao quer a pista
 	printf("Avião %d iniciou aproximação com %d%c de combustivel.\n", (int) aviao->id, (int) aviao->combustivel, 37);
 }

 void pousar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
 	sem_wait(&aviao->liberacao_pouso_decolagem);
    if (aviao->combustivel < 10)
        remover(aeroporto->fila_prioritaria);
    else
        remover(aeroporto->fila_pouso_decolagem);

 	usleep(aeroporto->t_pouso_decolagem * MICRO_TO_MILI);
 	printf("Avião %d aterrissou.\n", (int) aviao->id);
+	sem_post(&aeroporto->pistas);
 }

 void acoplar_portao (aeroporto_t* aeroporto, aviao_t* aviao) {
 	sem_post(&aeroporto->pistas);  // Inverter esta com a proxima se necessario ocupar para desocupar anterior
 	sem_wait(&aeroporto->portoes);
 	printf("Avião %d chegou a um portão de embarque/desembarque.\n", (int) aviao->id);
 }

 void transportar_bagagens (aeroporto_t* aeroporto, aviao_t* aviao) {
 	usleep(aeroporto->t_remover_bagagens * MICRO_TO_MILI);
 	printf("Avião %d foi descarregado.\n", (int) aviao->id);
 	adicionar_bagagens_esteira(aeroporto, aviao);  // Ver sobre retirar esta linha
 	usleep(aeroporto->t_inserir_bagagens * MICRO_TO_MILI);
 	printf("Avião %d foi carregado com novas bagagens.\n", (int) aviao->id);
 }

 void adicionar_bagagens_esteira (aeroporto_t* aeroporto, aviao_t* aviao) {
 	sem_wait(&aeroporto->esteiras);
 	printf("Bagagens do avião %d adicionadas à uma esteira.\n", (int) aviao->id);
 	usleep(aeroporto->t_bagagens_esteira * MICRO_TO_MILI);
 	sem_post(&aeroporto->esteiras);
 }

 void decolar_aviao (aeroporto_t* aeroporto, aviao_t* aviao) {
 	inserir(aeroporto->fila_pouso_decolagem, aviao);
    sem_post(&aeroporto->n_avioes);
    sem_post(&aeroporto->portoes);  // Inverter esta e a proxima se necessario desocupar para ocupar
 	sem_wait(&aviao->liberacao_pouso_decolagem);
 	remover(aeroporto->fila_pouso_decolagem);
 	usleep(aeroporto->t_pouso_decolagem * MICRO_TO_MILI);
 	printf("Avião %d decolou.\n", (int) aviao->id);
 	sem_post(&aeroporto->pistas);
 }

 int finalizar_aeroporto (aeroporto_t* aeroporto) {
    desaloca_fila(aeroporto->fila_prioritaria);
 	desaloca_fila(aeroporto->fila_pouso_decolagem);
 	sem_destroy(&(aeroporto->pistas));
 	sem_destroy(&(aeroporto->portoes));
 	sem_destroy(&(aeroporto->esteiras));
    sem_destroy(&aeroporto->n_avioes);
 	free(aeroporto);

 	return 1;
 }

 void libera_pouso_decolagem(aviao_t* aviao) {
     sem_post(&aviao->liberacao_pouso_decolagem);
}
