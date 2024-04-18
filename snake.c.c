#include "ripes_system.h"
#include "stdio.h"

#define SW0 (0x01)
#define RED (0xFF0404)
#define BLACK (0x000000)
#define GREEN (0x12DB33)
#define BLUE (0x0000FF)
#define MATRIX_HEIGHT 24
#define MATRIX_WIDTH 34

volatile unsigned int * led_base = LED_MATRIX_0_BASE;
volatile unsigned int * dpad_up = D_PAD_0_UP;
volatile unsigned int * dpad_down = D_PAD_0_DOWN;
volatile unsigned int * dpad_left = D_PAD_0_LEFT;
volatile unsigned int * dpad_right = D_PAD_0_RIGHT;
volatile unsigned int * switch_base = SWITCHES_0_BASE;
int lastMovement = 0;

void checkCollisions() {
    switch(lastMovement) {
        case 1:
            
        break;
        case 2:
        
        break;
        case 3:
        
        break;
        case 4:
        
        break;
    }
}

void drawRectangle() {
    for (int i=0; i<MATRIX_WIDTH; i++) {
        *led_base = BLUE;
        led_base++;
    }
    for (int i=0; i<MATRIX_HEIGHT; i++) {
        *led_base = BLUE;
        led_base += LED_MATRIX_0_WIDTH;
    }
    for (int i=0; i<MATRIX_WIDTH; i++) {
        *led_base = BLUE;
        led_base--;
    }
    for (int i=0; i<MATRIX_HEIGHT; i++) {
        *led_base = BLUE;
        led_base -= LED_MATRIX_0_WIDTH;
    }
}


void initGame() {
    drawRectangle();
    led_base += LED_MATRIX_0_WIDTH + 1;
    *led_base = RED;
    led_base += LED_MATRIX_0_WIDTH;
    *led_base = RED;
    led_base++;
    *led_base = RED;
    led_base -= LED_MATRIX_0_WIDTH;
    *led_base = RED;
}

void clearTail() {
    if (lastMovement == 1) {
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = BLACK;
        led_base--;
        *led_base = BLACK;
        // Regresar puntero
        led_base++;
        led_base -= LED_MATRIX_0_WIDTH;
    }
    if (lastMovement == 2) {
        led_base--;
        *led_base = BLACK;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = BLACK;
        // Regresar puntero
        led_base -= LED_MATRIX_0_WIDTH - 1;
    }
    if (lastMovement == 3) {
        led_base--;
        *led_base = BLACK;
        // Regresar puntero
        led_base++;
        *led_base = BLACK;
    }
    if (lastMovement == 4) {
        *led_base = BLACK;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = BLACK;
        // Regresar puntero
        led_base -= LED_MATRIX_0_WIDTH;
        led_base--;
    }
}

void moveSnake() {
    clearTail();
    
    if (lastMovement == 1) {
        led_base -= LED_MATRIX_0_WIDTH;
        *led_base = RED;
        led_base--;
        *led_base = RED;
        // Regresar puntero
        led_base++;
        
    } else if (lastMovement == 2) {
        led_base++;
        *led_base = RED;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = RED;
        led_base -= LED_MATRIX_0_WIDTH;
        
    } else if (lastMovement == 3) {
        led_base += LED_MATRIX_0_WIDTH * 2;
        *led_base = RED;
        led_base--;
        *led_base = RED;
        // Regresar puntero
        led_base -= LED_MATRIX_0_WIDTH;
        led_base++;
        
    } else if (lastMovement == 4) {
        led_base--;
        *led_base = RED;
        led_base += LED_MATRIX_0_WIDTH;
        *led_base = RED;
        // Regresar puntero
        led_base -= LED_MATRIX_0_WIDTH;
        led_base++;
        
    }
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
        printf("Direccion inicial: %d", *led_base);
        while (1) {
            movement = detectMovement();
            printf("Movement %i - ", movement);
            moveSnake();
            for(int i=0; i<6500; i++);
            
            if (*switch_base & SW0) {
                printf("Reiniciando juego...");
                break;
            }
        }
    }
}