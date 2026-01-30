#pragma once
#include "Segment.h"
#include "Food.h"
#include"Snake.h"
const int FOOD_COUNT = 5;
class Board
{
private:
    int BoardWidth;
    int BoardHeight;

public:
    Board();
    Board(int height, int width);

    int getWidth() const;
    int getHeight() const;

    bool IsWall(int x, int y) const;

    Segment GenerateFoodPos(
        const Segment* snakeHead,
        const Food foods[FOOD_COUNT],
        int foodCount
    ) const;
    void maintainFoods(
        Food foods[FOOD_COUNT],
        int& foodCount,
        const Snake& snake
        );
};


