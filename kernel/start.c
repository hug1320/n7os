#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>

void kernel_start(void) {
    init_console();
    setup_base(0 /* la memoire virtuelle n'est pas encore definie */);
    printf("Bienvenue dans le kernel de n7OS\n");
    printf("\n          ,.  ,.\n          ||  ||\n         ,''--''.\n        : (.)(.) :\n       ,'        `.\n       :          :\n       :          :\n       `._m____m_,' \n\n");
    
    // lancement des interruptions
    sti();

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
