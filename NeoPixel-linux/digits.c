#include "digits.h"
#include "utilities.h"

static char** head = NULL;
static char* letter = NULL;

void Digits_initialize(){

    head = malloc(sizeof(char*)*10);
    for(int i=0; i<10; i++){

        head[i] = malloc(sizeof(char)*8);
    }
    //init 0
    head[0][1] = 0b010;
    head[0][2] = 0b101;
    head[0][3] = 0b101;
    head[0][4] = 0b101;
    head[0][5] = 0b101;
    head[0][6] = 0b101;
    head[0][7] = 0b010;

    //init 1
    head[1][1] = 0b010;
    head[1][2] = 0b110;
    head[1][3] = 0b010;
    head[1][4] = 0b010;
    head[1][5] = 0b010;
    head[1][6] = 0b010;
    head[1][7] = 0b111;

    //init 2
    head[2][1] = 0b010;
    head[2][2] = 0b101;
    head[2][3] = 0b001;
    head[2][4] = 0b001;
    head[2][5] = 0b010;
    head[2][6] = 0b100;
    head[2][7] = 0b111;

    //init 3
    head[3][1] = 0b010;
    head[3][2] = 0b101;
    head[3][3] = 0b001;
    head[3][4] = 0b011;
    head[3][5] = 0b001;
    head[3][6] = 0b101;
    head[3][7] = 0b010;

    //init 4
    head[4][1] = 0b101;
    head[4][2] = 0b101;
    head[4][3] = 0b101;
    head[4][4] = 0b111;
    head[4][5] = 0b001;
    head[4][6] = 0b001;
    head[4][7] = 0b001;

    //init 5
    head[5][1] = 0b111;
    head[5][2] = 0b100;
    head[5][3] = 0b100;
    head[5][4] = 0b110;
    head[5][5] = 0b001;
    head[5][6] = 0b001;
    head[5][7] = 0b110;

    //init 6
    head[6][1] = 0b010;
    head[6][2] = 0b101;
    head[6][3] = 0b100;
    head[6][4] = 0b110;
    head[6][5] = 0b101;
    head[6][6] = 0b101;
    head[6][7] = 0b010;

    //init 7
    head[7][1] = 0b111;
    head[7][2] = 0b001;
    head[7][3] = 0b001;
    head[7][4] = 0b010;
    head[7][5] = 0b100;
    head[7][6] = 0b100;
    head[7][7] = 0b100;

    //init 8
    head[8][1] = 0b010;
    head[8][2] = 0b101;
    head[8][3] = 0b101;
    head[8][4] = 0b010;
    head[8][5] = 0b101;
    head[8][6] = 0b101;
    head[8][7] = 0b010;

    //init 9
    head[9][1] = 0b010;
    head[9][2] = 0b101;
    head[9][3] = 0b101;
    head[9][4] = 0b011;
    head[9][5] = 0b001;
    head[9][6] = 0b101;
    head[9][7] = 0b010;

    letter = malloc(sizeof(char)*8);

    letter[1] = 0b10001;
    letter[2] = 0b10001;
    letter[3] = 0b11011;
    letter[4] = 0b10001;
    letter[5] = 0b10101;
    letter[6] = 0b10001;
    letter[7] = 0b10001;

}

char** copy_head(){

    return head;
}

char* copy_M(){

    return letter;
}


void Digits_cleanup()
{
    for(int i=0; i<10; i++)
    {
        free(head[i]);
    }
    free(head);
    free(letter);
}