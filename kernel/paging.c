#include <n7OS/processor_structs.h>
#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include <stddef.h> // nécessaire pour NULL

PageDirectory pageDir;

void initialise_paging() {

    uint32_t index = 0;

    init_mem();

    pageDir = (PageDirectory)kmalloc_a(sizeof(PDE) * 1024);

    for (int i = 0; i < 1024; ++i) {
        PageTable new_page_table = (PageTable)kmalloc_a(sizeof(PTE) * 1024);
        pageDir[i].page_dir_entry.page = (uint32_t)new_page_table >> 12;
        pageDir[i].page_dir_entry.present = 1;
        pageDir[i].page_dir_entry.writeable = 1;
        pageDir[i].page_dir_entry.user = 0;
        index = (uint32_t)new_page_table + sizeof(PTE) * 1024;
    }

    for (uint32_t i = 0; i < index; i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 1);
    }
    /*
    for (int i = mem_heap; i < mem_end; i + PAGE_SIZE) {
        idx_pdr;
        idx_ptb;
        pdr[idx_pdr].ptb[idx_ptb].page_entry.page = i;
        // vérifier les conditions sur idx pour bien swwitcher qd il faut
    }
    */

    // On initialise le répertoire de page
    __asm__ __volatile__("mov %0, %%cr3"::"r"(pageDir));
    // On active la pagination
    uint32_t cr0;
    __asm__ __volatile__("mov %%cr0, %0":"=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ __volatile__("mov %0, %%cr0"::"r"(cr0));

    // On initialise le pointeur de pile
    setup_base((uint32_t)pageDir);
}

void setPageEntry(PTE* page_table_entry, uint32_t new_page, int is_writeable, int is_kernel) {
    page_table_entry->page_entry.present = 1;
    page_table_entry->page_entry.accessed = 0;
    page_table_entry->page_entry.dirty = 0;
    page_table_entry->page_entry.writeable = is_writeable;
    page_table_entry->page_entry.user = ~is_kernel;
    page_table_entry->page_entry.page = new_page >> 12;
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // address = adresse virtuelle à allouer 
    // address = idx_PDE | idx_PTE | offset
    //             10    |    10   |   12

    // calcul de la position de la table de page dans le répertoire de page
    int idx_pagedir = address >> 22;
    PageTable page_table;

    // on recupere l'entree dans le répertoire de page
    // une entree contient l'adresse de la table de page + bits de controle
    PDE page_dir_entry = pageDir[idx_pagedir];

    // on recupere l'adresse de la page de table dans le répertoire de page
    page_table = (PageTable)(page_dir_entry.page_dir_entry.page << 12);

    // recherche d'une page libre dans la memoire physique
    //uint32_t phy_page = findfreePage();

    // calcul de la position de la page dans la table de page
    int idx_pagetab = (address >> 12) & 0x3FF;

    // mise a jour de l'entree dans la page de table
    setPageEntry(&page_table[idx_pagetab], address, is_writeable, is_kernel);
    return page_table;
}
