#include <n7OS/console.h>
#include <n7OS/cpu.h>

uint16_t* scr_tab;
int cursor_pos;

void init_console() {
    scr_tab = (uint16_t*)SCREEN_ADDR;
    console_clear();
}

void console_scroll() {
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        scr_tab[i] = scr_tab[i + VGA_WIDTH];
    }
    for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        scr_tab[i] = CHAR_COLOR << 8 | ' ';
    }
    cursor_pos -= VGA_WIDTH;
}

void console_move_cursor() {
    outb(CMD_HIGH, PORT_CMD);
    outb((unsigned char)((cursor_pos >> 8) & 0xFF), PORT_DATA);
    outb(CMD_LOW, PORT_CMD);
    outb((unsigned char)(cursor_pos & 0xFF), PORT_DATA);
}

void console_clear() {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        scr_tab[i] = CHAR_COLOR << 8 | ' ';
    }
    cursor_pos = 0;
    console_move_cursor();
}

void console_putchar(const char c) {
    if (c > 31 && c < 127) {
        scr_tab[cursor_pos] = CHAR_COLOR << 8 | c;
        cursor_pos++;
    }
    else if (c == '\n') {
        cursor_pos += VGA_WIDTH - cursor_pos % VGA_WIDTH;
    }
    else if (c == '\t') {
        cursor_pos += 4 - cursor_pos % 4;
    }
    else if (c == '\b') {
        scr_tab[cursor_pos] = CHAR_COLOR << 8 | ' ';
        cursor_pos--;
    }
    if (cursor_pos >= VGA_WIDTH * VGA_HEIGHT) {
        console_scroll();
    }
    console_move_cursor();
}

void console_putbytes(const char* s, int len) {
    for (int i = 0; i < len; i++) {
        console_putchar(s[i]);
    }
}