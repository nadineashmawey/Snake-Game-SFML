#include "../include/Snake.h"

Snake::Snake(int startX, int startY)
{
    Head = new Segment(startX, startY);
    Tail = Head;
    dir = RIGHT;
    GrowingPoints = 0;
}

Snake::~Snake()
{
    Segment* next;
    while (Head != nullptr)
    {
        next = Head->next;
        delete Head;
        Head = next;
    }
}

void Snake::Move()
{
    int headX = Head->x;
    int headY = Head->y;

    switch (dir)
    {
    case UP:    headY--; break;
    case DOWN:  headY++; break;
    case LEFT:  headX--; break;
    case RIGHT: headX++; break;
    }

    Segment* newHead = new Segment(headX, headY);
    newHead->next = Head;
    Head = newHead;

    if (GrowingPoints > 0)
    {
        GrowingPoints--;
        return;
    }

    if (Head == Tail)
        return;

    Segment* current = Head;
    while (current->next != Tail)
    {
        current = current->next;
    }

    delete Tail;
    Tail = current;
    Tail->next = nullptr;
}

void Snake::grow(int points)
{
    GrowingPoints += points;
}

bool Snake::SelfCollision() const
{
    Segment* current = Head->next;
    while (current!=nullptr)
    {
        if (current->x == Head->x && current->y == Head->y)
            return true;
        current = current->next;
    }
    return false;
}

void Snake::ChangeDirection(Direction newDir)
{
    if ((dir == UP && newDir == DOWN) ||
        (dir == DOWN && newDir == UP) ||
        (dir == LEFT && newDir == RIGHT) ||
        (dir == RIGHT && newDir == LEFT))
        return;

    dir = newDir;
}

int Snake::getHeadX() const { return Head->x; }
int Snake::getHeadY() const { return Head->y; }
Segment* Snake::getHead() const { return Head; }
