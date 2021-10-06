#pragma once

class Field
{
private:
    int width, height;
    int **field;

public:
    Field(int width_p, int height_p)
    {
        width = width_p;
        height = height_p;
        field = new int *[height];
        for (int i = 0; i < height; i++)
        {
            field[i] = new int[width];
        }

        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                field[i][j] = 0;
            }
    }
    ~Field()
    {
        for (int i = 0; i < height; ++i)
            delete[] field[i];
        delete[] field;
    }

    void resize(int x, int y);
    void clear();
    void placeBorder(int x, int y);
    void placeFill(int x, int y);
    void placeHalfFill(int x, int y);
    int getWidth();
    int getHeight();
    int getPixel(int x, int y);
};