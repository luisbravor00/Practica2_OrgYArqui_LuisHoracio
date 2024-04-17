#include "ripes_system.h"
#include "stdio.h"

#define SW0 (0x01)
#define RED (0xFF0404)
#define BLACK (0x000000)
#define GREEN (0x12DB33)

volatile unsigned int * led_base = LED_MATRIX_0_BASE;
volatile unsigned int * dpad_up = D_PAD_0_UP;
volatile unsigned int * dpad_down = D_PAD_0_DOWN;
volatile unsigned int * dpad_left = D_PAD_0_LEFT;
volatile unsigned int * dpad_right = D_PAD_0_RIGHT;
volatile unsigned int * switch_base = SWITCHES_0_BASE;
int lastMovement = 0;

void initGame() {
    *led_base = RED;
    led_base++;
    *led_base = RED;
    led_base += LED_MATRIX_0_WIDTH - 1;
    *led_base = RED;
    led_base++;
    *led_base = RED;
    led_base -= LED_MATRIX_0_WIDTH;
}

void clearTail() {
    int * start = led_base;
    if (lastMovement == 2) {
        led_base--;
        *led_base = BLACK;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = BLACK;
    }
    if (lastMovement == 3) {
        led_base--;
        *led_base = BLACK;
        led_base++;
        *led_base = BLACK;
        led_base += LED_MATRIX_0_WIDTH;
    }
    led_base = start;
}

void moveSnake() {
    // int * start = led_base;
    
    if (lastMovement == 1) {
        led_base = 0;
    } else if (lastMovement == 2) {
        clearTail();
        led_base++;
        *led_base = RED;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = RED;
        led_base -= LED_MATRIX_0_WIDTH;
    } else if (lastMovement == 3) {
        clearTail();
        led_base += LED_MATRIX_0_WIDTH - 1;
        *led_base = RED;
        led_base ++;
        *led_base = RED;
        // Regresar puntero
    } else if (lastMovement == 4) {
        led_base = 0;
    }
    
    // led_base = start;
}

int detectMovement() {
    if (*dpad_up == 1) {
        lastMovement = 1;
        return 1;
    }
    if (*dpad_right == 1) {
        lastMovement = 2;
        return 2;
    }
    if (*dpad_down == 1) {
        lastMovement = 3;
        return 3;
    }
    if (*dpad_left == 1) {
        lastMovement = 4;
        return 4;
    }
    return 0;
}

void main() {
    int gameInitiated = 1;
    // 1 = UP, 2 = RIGHT, 3 = DOWN, 4 = LEFT
    int movement = 0;
    
    while (gameInitiated) {
        initGame();
        
        while (1) {
            movement = detectMovement();
            printf("Movement %i", movement);
            moveSnake();
            for(int i=0; i<10000; i++);
            
            if (*switch_base & SW0) {
                printf("Reiniciando juego...");
                break;
            }
        }
        // Delay
       
    }
}