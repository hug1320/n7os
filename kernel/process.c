#include <n7OS/process.h>
#include <n7OS/time.h>
#include <n7OS/cpu.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

extern void ctx_sw(uint32_t* old, uint32_t* new);
extern void idle();
extern void processus1();

// table des processus
static process_t process_table[MAX_PROCESS];

void init_process() {
    intialize_process_table();
    
    process_table[new_process("Idle", idle)].state = ELU;
    new_process("Processus 1", processus1);
    new_process("Processus 2", processus1);
    idle();
}

void intialize_process_table() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        process_table[i].state = BLOQUE;
        process_table[i].pid = -1;
    }
}

pid_t new_process(const char* name, fnptr function) {
    // recherche d'une entrée libre dans la table des processus
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].state == BLOQUE && process_table[i].pid == -1) {

            // initialisation de la structure processus
            process_table[i].regs[EBX] = 0;
            process_table[i].regs[ESP] = (uint32_t) &process_table[i].stack[STACK_SIZE - 1];
            process_table[i].regs[EBP] = 0;
            process_table[i].regs[ESI] = 0;
            process_table[i].regs[EDI] = 0;
            process_table[i].state = PRET;
            process_table[i].pid = i;
            strncpy(process_table[i].name, name, 32);

            // initialisation de la pile
            process_table[i].stack[STACK_SIZE - 1] = (uint32_t)function;

            printf("Nouveau processus %d : %s\n", i, name);

            return i;
        }
    }
    return -1;
}

pid_t get_current_pid() {
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].state == ELU) {
            return process_table[i].pid;
        }
    }
    return -1;
}

int exit_current() {
    int32_t current_pid = getpid();
    process_table[current_pid].state = BLOQUE;
    process_table[current_pid].pid = -1;
    process_table[current_pid].name[0] = '\0';
    process_table[current_pid].stack[STACK_SIZE - 1] = 0;
    schedule();
    return 0;
}

int sleep_current(int seconds) {
    int32_t current_pid = getpid();
    process_table[current_pid].state = BLOQUE;
    add_sleeping_process(current_pid, seconds);
    for (int32_t i = current_pid + 1; i < current_pid + MAX_PROCESS; i++) {
        if (process_table[i % MAX_PROCESS].state == PRET) {
            process_table[i % MAX_PROCESS].state = ELU;
            printf("Switching to process %d : %s\n", i % MAX_PROCESS, process_table[i % MAX_PROCESS].name);
            sti();
            ctx_sw(process_table[current_pid].regs, process_table[i % MAX_PROCESS].regs);
            return 0;
        }
    }
    return 0;
}

void wake_up(pid_t pid) {
    process_table[pid].state = PRET;
}

void schedule() {
    int32_t current_pid = getpid();
    for (int32_t i = current_pid + 1; i < current_pid + MAX_PROCESS; i++) {
        if (process_table[i % MAX_PROCESS].state == PRET) {
            process_table[current_pid].state = PRET;
            process_table[i % MAX_PROCESS].state = ELU;
            printf("Switching to process %d : %s\n", i % MAX_PROCESS, process_table[i % MAX_PROCESS].name);
            sti();
            ctx_sw(process_table[current_pid].regs, process_table[i % MAX_PROCESS].regs);
            return;
        }
    }
}

void print_active_processes() {
    printf("Processus actifs : ");
    for (int i = 0; i < MAX_PROCESS; i++) {
        if (process_table[i].state != BLOQUE) {
            printf("%s, ", process_table[i].name);
        }
    }
    printf("\n");
}
