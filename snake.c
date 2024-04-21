#include "ripes_system.h"
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

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
//SNAKE"S BODY to be able to keep track of it's length
//Array that holds pointers
volatile unsigned int * snakeBody[MATRIX_HEIGHT * MATRIX_WIDTH / 4]; 
volatile unsigned int * snakeHead, * snakeTail;
volatile unsigned int * applePos;
unsigned int snakeLength = 0;
int apple = 0;
int randomAppleSeed = 0;
//Flag to see if snake has crashed
int hasCrashed = 0;

int lastMovement = 0;

void checkCollisions() {
    switch(lastMovement) {
        case 1:
            //up
            if(lastMovement == 1){
                //check to see if next led is blue or red when going up
                if(*(snakeHead - LED_MATRIX_0_WIDTH) == BLUE || *(snakeHead - LED_MATRIX_0_WIDTH) == RED){
                    hasCrashed = 1;
                }
            }
        break;
        case 2:
            //right
            if(lastMovement == 2){
                if(*(snakeHead + 1) == BLUE || *(snakeHead + 1) == RED){
                 hasCrashed = 1;
                }
            }
        break;
        case 3:
            //down
            if(lastMovement == 3){
                if(*(snakeHead + LED_MATRIX_0_WIDTH) == BLUE || *(snakeHead + LED_MATRIX_0_WIDTH) == RED){
                 hasCrashed = 1;
                }
            }
        break;
        case 4:
            //left
            if(lastMovement == 4){
                if(*(snakeHead - 1) == BLUE || *(snakeHead - 1) == RED){
                 hasCrashed = 1;
                }
            }
        break;
    }
}

void drawApple() {
    volatile unsigned int *applePtr = applePos;
    
    applePtr += LED_MATRIX_0_WIDTH + 1;
    *applePtr = GREEN;
    applePtr += LED_MATRIX_0_WIDTH;
    *applePtr = GREEN;
    applePtr++;
    *applePtr = GREEN;
    applePtr -= LED_MATRIX_0_WIDTH;
    *applePtr = GREEN;
}

void generateApple() {
    /*
    //set x and y coordinates randomly    
    apple = ((rand() % MATRIX_HEIGHT) + (rand() % MATRIX_WIDTH);
    
    for(int i = 0;i<snakeTail;i++){
        
    }
    */
    //srand is for the random seed generator
    srand(randomAppleSeed++);
    int appleX = rand() % ((MATRIX_WIDTH - 1));
    int appleY = rand() % ((MATRIX_HEIGHT - 1));
    
    applePos = led_base + appleY + LED_MATRIX_0_WIDTH + appleX;
    
    //check to see if the apple's coordinates are within the snake's head
    volatile unsigned int *ptr = snakeBody[0];
    while(ptr <= snakeTail) {
        if(ptr == applePos || ptr == applePos + 1 || 
        ptr == applePos + LED_MATRIX_0_WIDTH || ptr == applePos + LED_MATRIX_0_WIDTH - 1)
        {
            generateApple();
            return;
        }
        ptr++;
    }  
    drawApple();  
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
    /*
    led_base += LED_MATRIX_0_WIDTH + 1;
    *led_base = RED;
    led_base += LED_MATRIX_0_WIDTH;
    *led_base = RED;
    led_base++;
    *led_base = RED;
    led_base -= LED_MATRIX_0_WIDTH;
    *led_base = RED;
    */
    //Initialize pointer to the snakes body
    led_base += LED_MATRIX_0_WIDTH + 1;
    volatile unsigned int * ptr = led_base;
    snakeBody[snakeLength] = ptr;
    snakeLength++;
    *ptr++ = RED;
    *ptr++ = RED;
    *ptr++ = RED;
    *ptr = RED;
    
    //HEAD AND TAIL
    snakeHead = snakeBody[0];
    snakeTail = snakeBody[snakeLength - 1];
    
    //apple position
    generateApple();
    
    
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
    
    volatile unsigned int * previousTail = snakeTail;
    
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
    
    checkCollisions();
    
    if(hasCrashed){
        exit(0);
    }
    
    snakeHead = led_base;
    
    //im just shifting the snakes body here
    for (int i = 0; i < snakeLength; i++) {
        snakeBody[i] = (i == 0) ? snakeHead : snakeBody[i - 1];
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