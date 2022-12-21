#include "fullfillment.h"
#include "utilities.h"

bool** matrix;

void fillFirstLevel()
{
    for(int y=0; y<8; y++)
    {
        matrix[0][y] = 1;
        matrix[7][y] = 1;
    }
    for(int x=1; x<7; x++)
    {
        matrix[x][0] = 1;
        matrix[x][7] = 1;
    }
}

void fillSecondLevel()
{
    for(int y=1; y<7; y++)
    {
        matrix[1][y] = 1;
        matrix[6][y] = 1;
    }
    for(int x=2; x<6; x++)
    {
        matrix[x][1] = 1;
        matrix[x][6] = 1;
    }
}

void fillThirdLevel()
{
    for(int y=2; y<6; y++)
    {
        matrix[2][y] = 1;
        matrix[5][y] = 1;
    }
    for(int x=3; x<5; x++)
    {
        matrix[x][2] = 1;
        matrix[x][5] = 1;
    }
}

void fillAllLevel()
{
    for(int y=3; y<5; y++)
    {
        matrix[3][y] = 1;
        matrix[4][y] = 1;
    }
}

bool** fillLevels(int level)
{
    matrix = malloc(8*sizeof(bool*));
    for(int i=0; i<8; i++)
    {
        matrix[i] = calloc(8, 1);
    }
    if(level>0)
    {
        fillFirstLevel();
    }
    if(level>1)
    {
        fillSecondLevel();
    }
    if(level>2)
    {
        fillThirdLevel();
    }
    if(level>3)
    {
        fillAllLevel();
    }


    return matrix;
}