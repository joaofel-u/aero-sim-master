#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "aeroporto.h"
#include "aviao.h"

#define NOVO_AVIAO_MIN 30
#define NOVO_AVIAO_MAX 120
#define COMBUSTIVEL_MIN 1
#define COMBUSTIVEL_MAX 100
#define TEMPO_POUSO_DECOLAGEM 40
#define TEMPO_REMOVER_BAGAGENS 90
#define TEMPO_INSERIR_BAGAGENS 110
#define TEMPO_BAGAGENS_ESTEIRA 200
#define TEMPO_SIMULACAO 10000

/*
	COMANDO PARA COMPILAR:
	gcc -o build aeroporto.c aeroporto.h aviao.c aviao.h fila.c fila.h main.c -lpthread
*/

// Cuidar com o seg
pthread_t threads_avioes[TEMPO_SIMULACAO];
int contador_threads_avioes = 0;

aeroporto_t* meu_aeroporto;  // Ver necessidade de ser global
int quit_thread_aeroporto = 0;  // Sinaliza para a thread aeroporto finalizar

void * thread_aviao_func(void* arg);  // Funcao de uma thread que representa um aviao

void * thread_aeroporto_func(void* arg);

int main (int argc, char** argv) {

	// Variáveis temporais (inicio t_)
	size_t t_novo_aviao_min, t_novo_aviao_max;
	size_t t_pouso_decolagem;
	size_t t_remover_bagagens, t_inserir_bagagens;
	size_t t_bagagens_esteira, t_simulacao;

	// Variáveis discretas (inicio n_)
	size_t n_pistas, n_portoes;
	size_t n_max_avioes_esteira, n_esteiras;
	size_t n_args;

	// Variáveis de prioridade (inicio p_)
	size_t p_combustivel_min, p_combustivel_max;

	if (argc == 5) { // Argumentos sem tempos de execução
		t_novo_aviao_min = NOVO_AVIAO_MIN;
		t_novo_aviao_max = NOVO_AVIAO_MAX;
		t_pouso_decolagem = TEMPO_POUSO_DECOLAGEM;
		t_remover_bagagens = TEMPO_REMOVER_BAGAGENS;
		t_inserir_bagagens = TEMPO_INSERIR_BAGAGENS;
		t_bagagens_esteira = TEMPO_BAGAGENS_ESTEIRA;
		t_simulacao = TEMPO_SIMULACAO;
		p_combustivel_min = COMBUSTIVEL_MIN;
		p_combustivel_max = COMBUSTIVEL_MAX;
		n_pistas = atoi(argv[1]);
		n_portoes = atoi(argv[2]);
		n_max_avioes_esteira = atoi(argv[3]);
		n_esteiras = atoi(argv[4]);

	} else if (argc == 14) { // Argumentos com tempos de execução
		int i = 0; // Este contador será incrementado antes de coletar um argumento
		t_novo_aviao_min = atoi(argv[++i]);
		t_novo_aviao_max = atoi(argv[++i]);
		p_combustivel_min = atoi(argv[++i]);
		p_combustivel_max = atoi(argv[++i]);
		t_pouso_decolagem = atoi(argv[++i]);
		n_pistas = atoi(argv[++i]);
		t_remover_bagagens = atoi(argv[++i]);
		t_inserir_bagagens = atoi(argv[++i]);
		n_portoes = atoi(argv[++i]);
		n_max_avioes_esteira = atoi(argv[++i]);
		t_bagagens_esteira = atoi(argv[++i]);
		n_esteiras = atoi(argv[++i]);
		t_simulacao = atoi(argv[++i]);

	} else { // Número incorreto de argumentos
		printf("Todas as entradas são inteiros positivos!!\nUso:\n");
		printf("./aeroporto  NOVO_AVIAO_MIN  NOVO_AVIAO_MAX\n");
		printf("COMBUSTIVEL_MIN COMBUSTIVEL_MAX\n");
		printf("TEMPO_POUSO_DECOLAGEM  NUMERO_PISTAS  TEMPO_REMOVER_BAGAGENS\n");
		printf("TEMPO_INSERIR_BAGAGENS  NUMERO_PORTOES  MAXIMO_AVIOES_ESTEIRA\n");
		printf("TEMPO_BAGAGENS_ESTEIRA  NUMERO_ESTEIRAS  TEMPO_SIMULACAO\n");
		printf("----------OU----------\n");
		printf("./airport  NUMERO_PISTAS  NUMERO_PORTOES  MAXIMO_AVIOES_ESTEIRA  NUMERO_ESTEIRAS\n");
		return 0;
	}

	// Impressão com os parâmetros selecionados para simulação
	printf("Simulação iniciada com tempo total: %lu\n", t_simulacao);
	printf("Tempo para criação de aviões: %lu - %lu\n", t_novo_aviao_min, t_novo_aviao_max);
	printf("Número de pistas de pouso: %lu\n", n_pistas);
	printf("Tempo de pouso e decolagem: %lu\n", t_pouso_decolagem);
	printf("Número de portões de embarque: %lu\n", n_portoes);
	printf("Tempo de inserção (%lu) e remoção (%lu) de bagagens\n", t_inserir_bagagens, t_remover_bagagens);
	printf("Número de esteiras: %lu, com %lu aviões por esteira\n", n_esteiras, n_max_avioes_esteira);
	printf("Tempo das bagagens nas esteiras: %lu\n", t_bagagens_esteira);

	// Inicialização do aeroporto
	n_args = 8;
	size_t args[8] = {n_pistas, n_portoes, n_esteiras,
				n_max_avioes_esteira,
				t_pouso_decolagem, t_remover_bagagens,
				t_inserir_bagagens, t_bagagens_esteira};

	meu_aeroporto = iniciar_aeroporto(args, n_args);

	// Descreve aqui sua simulação usando as funções definidas no arquivo "aeroporto.h"
	// Lembre-se de implementá-las num novo arquivo "aeroporto.c"

	int proximo_horario = 0;
	int n_avioes_dia = 0;  // Auxilia no controle do id unico de cada aviao
	srand(time(NULL));  // Inicia a semente do rand

	pthread_t thread_aeroporto;
	pthread_create(&thread_aeroporto, NULL, thread_aeroporto_func, (void *) meu_aeroporto);

	for (int i = 0; i < TEMPO_SIMULACAO; i++) {
		if (i == proximo_horario) {
			// Calcula horario de chegada do proximo aviao
			proximo_horario = i + (rand() % 90) + 30;

			// Cria aviao
			int combustivel_novo_aviao = (rand() % 100) + 1;
			aviao_t *a = aloca_aviao(combustivel_novo_aviao, ++n_avioes_dia);

			// Atribui uma thread responsavel pelo aviao criado
			pthread_t thread;
			pthread_create(&thread, NULL, thread_aviao_func, (void *) a);
			threads_avioes[contador_threads_avioes] = thread;
			contador_threads_avioes++;
			a->thread = thread;  // VER
		}
		usleep(1000);
	}

	for (int i = 0; i < contador_threads_avioes; i++)
    	pthread_join(threads_avioes[i], NULL);

	//sleep(1000);  // REVER MODELO (esperar todos terminarem)
	quit_thread_aeroporto = 1;
    pthread_join(thread_aeroporto, NULL);
	finalizar_aeroporto(meu_aeroporto);
	return 1;
}

void * thread_aviao_func(void* arg) {  // Arg = aviao da thread
	aviao_t* aviao = (aviao_t *) arg;

	aproximacao_aeroporto(meu_aeroporto, aviao);

	pousar_aviao(meu_aeroporto, aviao);

	acoplar_portao(meu_aeroporto, aviao);

	transportar_bagagens(meu_aeroporto, aviao);

	decolar_aviao(meu_aeroporto, aviao);

	pthread_exit(NULL);
	return NULL;
}

void * thread_aeroporto_func(void* arg) {
	aeroporto_t* aeroporto = (aeroporto_t*) arg;

	while (!quit_thread_aeroporto) {
		sem_wait(&aeroporto->pistas);
	}

	pthread_exit(NULL);
	return NULL;
}
