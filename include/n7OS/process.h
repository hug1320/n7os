#ifndef _PROCESS_H
#define _PROCESS_H

#define MAX_PROCESS 32
#define STACK_SIZE 1024
#define NUM_REGS 5

#include <inttypes.h>

// Enumération des états d'un processus
typedef enum process_state_t {
    ELU,
    PRET,
    BLOQUE,
} process_state_t;

// Enumération des registres
typedef enum reg_t {
    EBX,
    ESP,
    EBP,
    ESI,
    EDI
} reg_t;

// Type pid_t
typedef int32_t pid_t;

// Type fonction
typedef void (*fnptr)();

// Structure d'un processus
typedef struct process_t {
    uint32_t regs[NUM_REGS];
    uint32_t stack[STACK_SIZE];
    process_state_t state;
    pid_t pid;
    char name[32];

} process_t;

// initialisation des processus
void init_process();

// Initialisation de la table des processus
void intialize_process_table();

// Création d'un processus
pid_t new_process(const char *name, fnptr function);

// Récupération du pid du processus courant
pid_t get_current_pid();

// Fin du processus courant
int exit_current();

// Mise en sommeil du processus courant
int sleep_current(int seconds);

// Initialisation de la table des processus
void wake_up(pid_t pid);

// Scheduleur
void schedule();

// Affichage des processus actifs
void print_active_processes();

#endif