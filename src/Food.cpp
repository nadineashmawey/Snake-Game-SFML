#include "../include/Food.h"
#include <iostream>
using namespace std;
Food::Food()
    : FoodX(0), FoodY(0), Prize(FoodType::APPLE)
{
}

Food::Food(FoodType type)
    : FoodX(0), FoodY(0), Prize(type)
{
}


void Food::GenerateType()
{
    int r = rand() % 4; //choose number between 0...3
    Prize = static_cast<FoodType>(r);

}

bool Food::IsEaten(int headX, int headY) const
{
    return (FoodX == headX && FoodY == headY);
}

FoodType Food::getType() const
{
    return Prize;
}


int Food::getPoints() const
{
    if (Prize == FoodType::MEGA_STRAWBERRY ||
        Prize == FoodType::MEGA_WATERMELON)
        return 3;

    return 1;
}

int Food::getX() const { return FoodX; }
int Food::getY() const { return FoodY; }


void Food::SetPos(int x, int y)
{
    FoodX = x;
    FoodY = y;
}

