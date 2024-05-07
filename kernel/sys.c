#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <unistd.h>

extern void handler_syscall();

void init_syscall() {
  // ajout de la fonction de traitement de l'appel systeme
  add_syscall(NR_example, sys_example);
  add_syscall(NR_shutdown, sys_shutdown);
  add_syscall(NR_write, sys_write);

  // initialisation de l'IT soft qui gère les appels systeme
  init_idt_entry(0x80, (uint32_t)handler_syscall);
}

// code de la fonction de traitement de l'appel systeme example
int sys_example() {
  // on ne fait que retourner 1
  return 1;
}

int sys_shutdown(int n) {
  if (n == 1) {
    outw(0x2000, 0x604); // Poweroff qemu > 2.0
    return -1;
  }
  else
    return n;
}

int sys_write(char* s, int n) {
  console_putbytes(s, n);
  return n;
}