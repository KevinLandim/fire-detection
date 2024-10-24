#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SIZE 30

void *sensor_node(void *arg);
void notify_neighbors(int x, int y);
void notify_control_center(int x, int y);
void notify(int x, int y);
void *fire_generator(void *arg);
void *control_center(void *arg);
void print_forest(); // Adicionado

#endif
