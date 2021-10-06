#include "Field.h"

void Field::clear()
{
    for (int i = 0; i < height; i++)
        for (int j = 0; j < width; j++)
        {
            field[i][j] = 0;
        }
}

void Field::placeBorder(int x, int y)
{
    if (x < width && y < height)
        field[y][x] = (field[y][x] + 1) % 2;
}

void Field::placeFill(int x, int y)
{
    if (x < width && y < height)
        field[y][x] = 2;
}

void Field::placeHalfFill(int x, int y)
{
    if (x < width && y < height)
        field[y][x] = 3;
}

int Field::getWidth()
{
    return width;
}

int Field::getHeight()
{
    return height;
}

int Field::getPixel(int x, int y)
{
    if (x < width && y < height)
        return field[y][x];
}

void Field::resize(int x, int y)
{
    width = x;
    height = y;
}