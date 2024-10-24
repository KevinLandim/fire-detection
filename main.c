#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h> // Incluído para usar time()
#include "functions.h"

#define SIZE 30

char forest[SIZE][SIZE];
pthread_mutex_t mutex_forest;

void initialize_forest() {
    pthread_mutex_lock(&mutex_forest);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            forest[i][j] = 'T'; // Todas as células inicialmente com sensores
        }
    }
    pthread_mutex_unlock(&mutex_forest);
}

int main() {
    pthread_t sensor_threads[SIZE][SIZE];
    pthread_t fire_thread, control_thread;
    pthread_mutex_init(&mutex_forest, NULL);

    initialize_forest();

    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Criar threads para os nós sensores
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pthread_create(&sensor_threads[i][j], NULL, sensor_node, (void *)((long)(i * SIZE + j)));
        }
    }

    // Criar thread para gerar incêndios
    pthread_create(&fire_thread, NULL, fire_generator, NULL);

    // Criar thread para central de controle
    pthread_create(&control_thread, NULL, control_center, NULL);

    // Opcional: Aguardar a finalização das threads (neste caso, elas rodam indefinidamente)
    pthread_join(fire_thread, NULL);
    pthread_join(control_thread, NULL);

    // Liberar recursos
    pthread_mutex_destroy(&mutex_forest);

    return 0;
}
