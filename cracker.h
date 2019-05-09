#ifndef cracker2_H
#define cracker2_H

#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "src/reverse.h"
#include "src/sha256.h"

extern int NTHREAD = 1;
extern int CONS = 0;
extern int FILEOUT = 0;
extern sem_t number_of_empty;
extern sem_t number_of_full;
extern pthread_mutex_t mutex1;
extern pthread_mutex_t mutex2;
extern const char filename = 'c';
extern int FINISH = 0;
extern uint8_t **tab1;
extern char **tab2;
extern int COMPTEUR = 1;
extern int COMPTEUR2 = 1;
extern int sizetab;
extern char voyelles[6] = {'a','e','i','o','u','y'};

void *traduction(void *arg);
int readFile(int file);
int main(int argc, char const *argv[]);
int readFile(int file);
int isvoyelle(char c);
int isconsonne(char c);
int countl(int cons, char *str);
node_t *init_node(char *value);
int add_node(list_t *list, char *value);
void *compare(void *arg);
bool isTab1Empty(uint8_t **tab);
bool isTab2Empty(char **tab);

#endif