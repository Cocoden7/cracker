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
#include "reverse.h"
#include "sha256.h"

int NTHREAD = 1;
int CONS = 0;
int FILEOUT = 0;
sem_t number_of_empty;
sem_t number_of_full;
pthread_mutex_t mutex2;
const char filename = 'c';
int FINISH = 0;
uint8_t **tab1;
char **tab2;
int COMPTEUR = 1;
int COMPTEUR2 = 1;
int sizetab;
char voyelles[6] = {'a','e','i','o','u','y'};
typedef struct node
{
	struct node *next;
	char* mdp;
}node_t;

typedef struct list {
  struct node *first;
  int size;
} list_t;

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