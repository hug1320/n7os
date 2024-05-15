#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>
#define SCANCODE_TABLE_SIZE 128

extern void handler_IT_33();

static const char char_map[SCANCODE_TABLE_SIZE] = {
    '\0', '\0', '&', '\0', '"', '\'', '(', '-', '\0', '_', '\0', '\0', ')', '=', '\b',
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '^', '$', '\n',
    '\0', 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '\0', '*',
    '\0', '\0', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '!', '\0',
    '*',
    '\0', // left alt
    ' ',
    '\0', // caps lock
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', // F1 - F10
    '\0', // num lock
    '\0', // scroll lock
    /* Keypad */
    '7', '8', '9', '-',
    '4', '5', '6', '+',
    '1', '2', '3', '0',
    '.',
    '\0', '\0', '\0',
    '\0', '\0', // F11 - F12
    '\0' // le reste
};

static const char char_map_maj[SCANCODE_TABLE_SIZE] = {
    '\0', '\0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '\0', '+', '\b',
    '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '"', '\0', '\n',
    '\0', 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '\0', '\0',
    '\0', '\0', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', '\0', '\0',
    '*',
    '\0', // left alt
    ' ',
    '\0', // caps lock
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', // F1 - F10
    '\0', // num lock
    '\0', // scroll lock
    /* Keypad */
    '7', '8', '9', '-',
    '4', '5', '6', '+',
    '1', '2', '3', '0',
    '.',
    '\0', '\0', '\0',
    '\0', '\0', // F11 - F12
    '\0' // le reste
};

int shift = 0;
int caps_lock = 0;
int alt = 0;
int alt_gr = 0;
int ctrl = 0;
int extended = 0;

void init_keyboard() {
    init_idt_entry(0x21, (uint32_t)handler_IT_33);
    outb(inb(0x21) & 0xfd, 0x21);
}

void handler_IT_33_C() {
    outb(0x20, 0x20);
    uint8_t scancode = inb(0x60);

    if (scancode & 0x80) {
        if (extended) {
            switch (scancode) {
            case 0x9D: // CTRL RELEASED
                ctrl -- ;
                break;

            case 0xB8: // ALT_GR RELEASED
                alt_gr = 0;
                break;

            default:
                break;
            }
        }
        else
        switch (scancode) {
        case 0xE0: // EXTENDED KEY RELEASED
            extended = 0;
            break;

        case 0xAA: // SHIFT RELEASED
        case 0xB6:
            shift -- ;
            break;

        case 0x9D: // CTRL RELEASED
            ctrl -- ;
            break;

        case 0xB8: // ALT RELEASED
            alt = 0;
            break;

        default:
            break;
        }
    }
    else {
        // printf("scancode: %x = ", scancode);
        if (extended) {
            switch (scancode) {
            case 0x1D: // CTRL PRESSED
                ctrl ++ ;
                break;

            case 0x38: // ALT_GR PRESSED
                alt_gr = 1;
                break;

            default:
                break;
            }
        }
        else {
            switch (scancode) {

            case 0xE0: // EXTENDED KEY
                extended = 1;
                break;

            case 0x2A: // SHIFT PRESSED
            case 0x36:
                shift ++ ;
                break;

            case 0x3A: // CAPS LOCK
                caps_lock = !caps_lock;
                break;

            case 0x1D: // CTRL PRESSED
                ctrl ++ ;
                break;

            case 0x38: // ALT PRESSED
                alt ++ ;
                break;

            default:
                if (shift > 0 || caps_lock) {
                    printf("%c", char_map_maj[scancode]);
                }
                else {
                    printf("%c", char_map[scancode]);
                }
                // printf("\ndebug: shift = %d ; caps_lock = %d; ctrl = %d; alt = %d; alt_gr = %d\n", shift, caps_lock, ctrl, alt, alt_gr);
                break;
            }

        }

    }

}