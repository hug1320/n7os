#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/handler_IT_50_C.h>
#include <n7OS/time.h>
#include <n7OS/keyboard.h>
#include <n7OS/kheap.h>
#include <n7OS/paging.h>
#include <n7OS/sys.h>
#include <n7OS/process.h>
#include <n7OS/mem.h>
#include <unistd.h>
#include <stdio.h>

void kernel_start(void) {
    init_console();
    // setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    init_kheap();
    initialise_paging();

    // Test segfault
    // int *p = (int *) 0xA0000000;
    // printf("%x\n", *p);
    // (*p)++;
    
    init_syscall();
    printf("Bienvenue dans n7OS\n");
    printf("\n          ,.  ,.\n          ||  ||\n         ,''--''.\n        : (.)(.) :\n       ,'        `.\n       :          :\n       :          :\n       `._m____m_,' \n\n");
    
    printf("> Initialisation de la console : OK\n");
    printf("> Initialisation du tas : OK\n");
    printf("> Initialisation de la pagination : OK\n");
    printf("> Initialisation des appels systeme : OK\n");

    // initialisation des interruptions
    init_IT_50();
    init_timer();
    init_keyboard();

    // lancement des interruptions
    sti();

    // Test interruption
    // __asm__("int $50"::);

    // Test appel systeme
    /*
    if (example() == 1) {
        printf("Appel systeme example ok\n");
    }
    */

    //shutdown(1);

    // Affichage de la mémoire virtuelle
    // print_mem();

    // Initialisation des processus
    init_process();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
