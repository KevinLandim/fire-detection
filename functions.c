#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

extern char forest[SIZE][SIZE];
extern pthread_mutex_t mutex_forest;

void *sensor_node(void *arg) {
    long index = (long)arg;
    int x = index / SIZE;
    int y = index % SIZE;

    while (1) {
        pthread_mutex_lock(&mutex_forest);
        if (forest[x][y] == '@') { // Fogo detectado
            // Notificar nós vizinhos
            notify_neighbors(x, y);
        }
        pthread_mutex_unlock(&mutex_forest);

        sleep(1); // Aguarda um ciclo
    }

    return NULL;
}

void notify_neighbors(int x, int y) {
    // Propaga a mensagem para os vizinhos
    if (x > 0 && forest[x-1][y] == 'T') notify(x-1, y); // Célula acima
    if (x < SIZE-1 && forest[x+1][y] == 'T') notify(x+1, y); // Célula abaixo
    if (y > 0 && forest[x][y-1] == 'T') notify(x, y-1); // Célula à esquerda
    if (y < SIZE-1 && forest[x][y+1] == 'T') notify(x, y+1); // Célula à direita

    if (x == 0 || x == SIZE-1 || y == 0 || y == SIZE-1) {
        // Notifica a central de controle se estiver na borda
        notify_control_center(x, y);
    }
}

void notify(int x, int y) {
    pthread_mutex_lock(&mutex_forest);
    if (forest[x][y] == 'T') {
        forest[x][y] = '@'; // Marca o nó vizinho em chamas
    }
    pthread_mutex_unlock(&mutex_forest);
}

void notify_control_center(int x, int y) {
    printf("Incêndio detectado na borda (%d, %d). Notificando central de controle.\n", x, y);
}

void *fire_generator(void *arg) {
    while (1) {
        int x = rand() % SIZE;
        int y = rand() % SIZE;

        pthread_mutex_lock(&mutex_forest);
        if (forest[x][y] == 'T') {
            forest[x][y] = '@'; // Incêndio iniciado
        }
        pthread_mutex_unlock(&mutex_forest);

        sleep(3); // Gera um novo incêndio a cada 3 segundos
    }

    return NULL;
}

void *control_center(void *arg) {
    while (1) {
        pthread_mutex_lock(&mutex_forest);
        // Ações da central de controle para combater incêndios
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (forest[i][j] == '@') {
                    forest[i][j] = '/'; // Combate o fogo
                }
            }
        }
        pthread_mutex_unlock(&mutex_forest);

        print_forest(); // Imprime a floresta atualizada
        sleep(1); // Ciclo da central de controle
    }

    return NULL;
}

void print_forest() {
    pthread_mutex_lock(&mutex_forest);
    system("clear");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", forest[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&mutex_forest);
}
