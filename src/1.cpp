#include <SFML/Graphics.hpp>
#include <iostream>
#include <stack>
#include <queue>
#include "windows.h"
#include "Field.h"

#define PX_SIZE 20.f

void task();
sf::RectangleShape makePixel(sf::Vector2f position, sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue);
std::queue<std::pair<int, int>> simpleFill(int x, int y, Field *field);
std::queue<std::pair<int, int>> rowsFill(int x, int y, Field *field);

int main()
{
    task();
    return 0;
}

void task()
{
    Field field(800, 600);
    int mode = 1;
    sf::RenderWindow window(sf::VideoMode(field.getWidth(), field.getHeight()), "lab_3");
    std::queue<std::pair<int, int>> fill;

    sf::VertexArray vertical_lines(sf::Lines, 2 * field.getWidth() / PX_SIZE);
    for (int i = 0; i < 2 * field.getWidth() / PX_SIZE; i += 2)
    {
        vertical_lines[i].position = sf::Vector2f(i / 2 * PX_SIZE, 0);
        vertical_lines[i + 1].position = sf::Vector2f(i / 2 * PX_SIZE, field.getHeight());
        vertical_lines[i].color = {150, 150, 150};
        vertical_lines[i + 1].color = {150, 150, 150};
    }
    sf::VertexArray horizontal_lines(sf::Lines, 2 * field.getHeight() / PX_SIZE);
    for (int i = 0; i < 2 * field.getHeight() / PX_SIZE; i += 2)
    {
        horizontal_lines[i].position = sf::Vector2f(0, i / 2 * PX_SIZE);
        horizontal_lines[i + 1].position = sf::Vector2f(field.getWidth(), i / 2 * PX_SIZE);
        horizontal_lines[i].color = {150, 150, 150};
        horizontal_lines[i + 1].color = {150, 150, 150};
    }

    clock_t start = clock();
    clock_t end;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                int x = event.mouseButton.x / PX_SIZE;
                int y = event.mouseButton.y / PX_SIZE;

                field.placeBorder(x, y);
            }
            else if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            {
                std::queue<std::pair<int, int>> newFill;
                int x = event.mouseButton.x / PX_SIZE;
                int y = event.mouseButton.y / PX_SIZE;
                if (mode = 1)
                    newFill = simpleFill(x, y, &field);
                if (mode = 2)
                    newFill = rowsFill(x, y, &field);

                while (!newFill.empty())
                {
                    fill.push(newFill.front());
                    newFill.pop();
                }
                start = clock();
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Q)
            {
                mode = 1;
                std::cout << mode << std::endl;
            }
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::W)
            {
                mode = 2;
                std::cout << mode << std::endl;
            }
        }

        end = clock();
        double seconds = (double)(end - start) / CLOCKS_PER_SEC;
        if (seconds > 0.3 && !fill.empty())
        {
            start = end;
            field.placeFill(fill.front().first, fill.front().second);
            fill.pop();
        }

        for (int i = 0; i < field.getHeight(); i++)
            for (int j = 0; j < field.getWidth(); j++)
            {
                switch ((field.getPixel(i, j)))
                {
                case 1:
                    window.draw(makePixel({i * PX_SIZE, j * PX_SIZE}, 0, 120, 255));
                    break;

                case 2:
                    window.draw(makePixel({i * PX_SIZE, j * PX_SIZE}, 250, 0, 0));
                    break;

                case 3:
                    window.draw(makePixel({i * PX_SIZE, j * PX_SIZE}, 70, 0, 0));
                    break;

                default:
                    break;
                }
            }

        window.draw(vertical_lines);
        window.draw(horizontal_lines);
        window.display();
        window.clear();
    }
}

sf::RectangleShape makePixel(sf::Vector2f position, sf::Uint8 red, sf::Uint8 green, sf::Uint8 blue)
{
    sf::RectangleShape pixel;
    pixel.setSize({PX_SIZE - 1, PX_SIZE - 1});
    pixel.setFillColor({red, green, blue});
    pixel.setPosition(position);
    return pixel;
}

std::queue<std::pair<int, int>> simpleFill(int x, int y, Field *field)
{
    std::queue<std::pair<int, int>> fill;
    std::stack<std::pair<int, int>> stack;
    stack.push({x, y});
    while (!stack.empty())
    {
        std::pair<int, int> coor = stack.top();
        stack.pop();

        fill.push({coor.first, coor.second});
        field->placeHalfFill(coor.first, coor.second);

        std::pair<int, int> checks[4] = {{1, 0}, {0, -1}, {-1, 0}, {0, 1}};
        for (auto check : checks)
        {
            if (coor.first + check.first < 0 || coor.first + check.first >= (field->getHeight() - 1) ||
                coor.second + check.second < 0 || coor.second + check.second >= (field->getWidth() - 1))
                continue;
            if (field->getPixel(coor.first + check.first, coor.second + check.second) == 0)
            {
                stack.push({coor.first + check.first, coor.second + check.second});
            }
        }
    }
    return fill;
}

std::queue<std::pair<int, int>> rowsFill(int x, int y, Field *field)
{
    std::queue<std::pair<int, int>> fill;
    std::stack<std::pair<int, int>> stack;
    std::pair<int, int> coor;
    stack.push({x, y});
    int xLeft, xRight;

    while (!stack.empty())
    {
        coor = stack.top();
        stack.pop();
        xLeft = coor.first;
        xRight = coor.first;
        // fill.push({coor.first, coor.second});
        // field->placeHalfFill(coor.first, coor.second);

        while (field->getPixel(xLeft, coor.second) == 0)
        {
            fill.push({xLeft, coor.second});
            field->placeHalfFill(xLeft, coor.second);
            xLeft--;
        }
        while (field->getPixel(xRight++, coor.second) == 0)
        {
            fill.push({xRight, coor.second});
            field->placeHalfFill(xRight, coor.second);
            xRight++;
        }
        bool inStack = false;
        for (int i = xLeft; i <= xRight; i++)
        {
            if (field->getPixel(i, coor.second + 1) == 0 && !inStack)
            {
                stack.push({i, coor.second + 1});
                inStack = true;
            }
            else if (field->getPixel(i, coor.second + 1) == 1)
            {
                inStack = false;
            }
        }
        inStack = false;
        for (int i = xLeft; i <= xRight; i++)
        {
            if (field->getPixel(i, coor.second - 1) == 0 && !inStack)
            {
                stack.push({i, coor.second - 1});
                inStack = true;
            }
            else if (field->getPixel(i, coor.second - 1) == 1)
            {
                inStack = false;
            }
        }
    }

    return fill;
}