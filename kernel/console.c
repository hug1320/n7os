#include <n7OS/console.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <string.h>

uint16_t* scr_tab;
int cursor_pos;

void init_console() {
    scr_tab = (uint16_t*)SCREEN_ADDR;
    console_clear();
    init_topbar();
}

void console_scroll() {
    for (int i = VGA_WIDTH; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
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
    cursor_pos = VGA_WIDTH;
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
        if (cursor_pos > VGA_WIDTH) {
            cursor_pos--;
            scr_tab[cursor_pos] = CHAR_COLOR << 8 | ' ';
        }
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

void update_topbar_uptime() {
    char* buffer = format_timer();
    for (int i = VGA_WIDTH - 8; i < VGA_WIDTH; i++) {
        if (i < VGA_WIDTH - 8) {
            scr_tab[i] = (BLINK | GRAY << 4 | BLACK) << 8 | ' ';
        }
        else {
            scr_tab[i] = (BLINK | GRAY << 4 | RED) << 8 | buffer[i - VGA_WIDTH + 8];
        }
    }
}

void init_topbar() {
    // Topbar messages
    char* mail = "hugo@net7.dev";
    int mail_len = strlen(mail);
    char* buffer = "n7OS";
    int buffer_len = strlen(buffer);

    for (int i = 0; i < VGA_WIDTH; i++) {
        if (i >= (VGA_WIDTH - buffer_len) / 2 && i < (VGA_WIDTH + buffer_len) / 2) {
            scr_tab[i] = (BLINK | GRAY << 4 | RED) << 8 | buffer[i - 38];
        }
        else if (i < mail_len) {
            scr_tab[i] = (BLINK | GRAY << 4 | RED) << 8 | mail[i];
        }
        else {
            scr_tab[i] = (BLINK | GRAY << 4 | BLACK) << 8 | ' ';
        }
    }
    update_topbar_uptime();
}