#include "faces.h"
#include "utilities.h"

int* getSmile()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b00000000;
    matrix[1] = 0b01100110;
    matrix[2] = 0b00100100;
    matrix[3] = 0b00000000;
    matrix[4] = 0b10000001;
    matrix[5] = 0b01000010;
    matrix[6] = 0b00111100;
    matrix[7] = 0b00000000;
    return matrix;
}
int* getAngry()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b00000000;
    matrix[1] = 0b01000010;
    matrix[2] = 0b01100110;
    matrix[3] = 0b00000000;
    matrix[4] = 0b00111100;
    matrix[5] = 0b01000010;
    matrix[6] = 0b01000010;
    matrix[7] = 0b00000000;
    return matrix;
}

int* getLoad1()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b10101010;
    matrix[1] = 0b00000001;
    matrix[2] = 0b10010100;
    matrix[3] = 0b00100001;
    matrix[4] = 0b10000100;
    matrix[5] = 0b00101001;
    matrix[6] = 0b10000000;
    matrix[7] = 0b01010101;
    return matrix;
}

int* getLoad2()
{
    int* matrix = malloc(8*sizeof(int));
    matrix[0] = 0b01010101;
    matrix[1] = 0b10000000;
    matrix[2] = 0b00101001;
    matrix[3] = 0b10000100;
    matrix[4] = 0b00100001;
    matrix[5] = 0b10010100;
    matrix[6] = 0b00000001;
    matrix[7] = 0b10101010;
    return matrix;
}