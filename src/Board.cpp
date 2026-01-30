#include "../include/Board.h"
Board :: Board()
    : BoardWidth(40), BoardHeight(30)
{
}
Board::Board(int height, int width)
    : BoardWidth(width), BoardHeight(height)
{
}

int Board::getWidth() const { return BoardWidth; }
int Board::getHeight() const { return BoardHeight; }

bool Board::IsWall(int x, int y) const
{
    return (x == 0 || y == 0 || x == BoardWidth - 1 || y == BoardHeight - 1);
}

Segment Board::GenerateFoodPos(
    const Segment* snakeHead,
    const Food foods[FOOD_COUNT],
    int foodCount
) const
{
    while (true)
    {
        int x = 1 + rand() % (BoardWidth - 2);
        int y = 1 + rand() % (BoardHeight - 2);

        bool collision = false;

        // Check collision with snake
        const Segment* current = snakeHead;
        while (current != nullptr)
        {
            if (current->x == x && current->y == y) //equivalent to (*current).x == x && (*current).y == y
            {
                collision = true;
                break;
            }
            current = current->next;
        }

        // Check collision with existing food
        if (!collision)
        {
            for (int i = 0; i < foodCount; i++)
            {
                if (foods[i].getX() == x &&
                    foods[i].getY() == y)
                {
                    collision = true;
                    break;
                }
            }
        }

        if (!collision)
            return Segment(x, y);
    }
}
void Board :: maintainFoods(
    Food foods[FOOD_COUNT],
    int& foodCount,
    const Snake& snake)
{
    while (foodCount < FOOD_COUNT)
    {
        Segment p = GenerateFoodPos(
            snake.getHead(),
            foods,
            FOOD_COUNT
        );

        foods[foodCount].SetPos(p.x, p.y);
        foods[foodCount].GenerateType();

        foodCount++;
    }
}

