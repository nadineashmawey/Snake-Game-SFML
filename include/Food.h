
#pragma once
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;

enum class FoodType
{
    APPLE,
    BANANA,
    MEGA_STRAWBERRY,
    MEGA_WATERMELON
};

class Food
{
private:
    int FoodX;
    int FoodY;
    FoodType Prize;


public:
    Food();
    Food(FoodType type);
    void GenerateType();
    bool IsEaten(int headX, int headY) const;
    FoodType getType() const;

    int getPoints() const;
    int getX() const;
    int getY() const;
    void SetPos(int x, int y);




};

