#pragma once
#include "Segment.h"

enum Direction
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake
{
private:
    Segment* Head;
    Segment* Tail;
    Direction dir;
    int GrowingPoints;

public:
    //default constructor
    Snake(int startX, int startY);
    ~Snake();

    void Move();
    void grow(int points);
    bool SelfCollision() const;
    void ChangeDirection(Direction newDir);

    int getHeadX() const;
    int getHeadY() const;
    Segment* getHead() const;
};


