#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <n7OS/handler_IT_50_C.h>
#include <n7OS/time.h>

void kernel_start(void) {
    init_console();
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    printf("Bienvenue dans le kernel de n7OS\n");
    printf("\n          ,.  ,.\n          ||  ||\n         ,''--''.\n        : (.)(.) :\n       ,'        `.\n       :          :\n       :          :\n       `._m____m_,' \n\n");
    
    // initialisation des interruptions
    init_IT_50();
    init_timer();

    // lancement des interruptions
    sti();

    __asm__("int $50"::);
    

    // on ne doit jamais sortir de kernel_start
    while (1) {
        print_timer();
        // cette fonction arrete le processeur
        //hlt();
    }
}
