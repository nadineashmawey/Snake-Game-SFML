#include <SFML/Graphics.hpp>
#include <ctime>
#include <fstream>
#include <iostream>
#include "../include/Snake.h"
#include "../include/Board.h"
#include "../include/Food.h"
using namespace sf;
// ================= CONSTANTS =================
const int CELL_SIZE = 30;
const int BOARD_WIDTH = 40;
const int BOARD_HEIGHT = 30;


// ================= MAIN =================
int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    // ================= LOAD HIGH SCORE =================
    int highScore = 0;
    std::ifstream in("highscore.txt");
    if (in.is_open()) {
        in >> highScore;
        in.close();
    }


    // ================= ADMIN WINDOW =================
     RenderWindow admin( VideoMode(500, 700), "Snake");
    admin.setFramerateLimit(60);

    // ---------- SNAKE PREVIEW ----------
     RectangleShape snakeBody({ 200, 30 });
    snakeBody.setFillColor( Color(90, 130, 255));
    snakeBody.setPosition(120, 250);

     CircleShape snakeHead(18);
    snakeHead.setFillColor( Color(90, 130, 255));
    snakeHead.setPosition(300, 246);

     CircleShape eye(4);
    eye.setFillColor( Color::White);


     Font font;
    font.loadFromFile("Resources/arial.ttf");

    // Colors
     Color bgColor(46, 74, 45);
     Color cardColor(98, 196, 233);
     Color playBlue(27, 98, 208);
     Color grid1(158, 214, 91);
     Color grid2(142, 200, 80);

    // Background and card
     RectangleShape background({ 500, 700 });
    background.setFillColor(bgColor);
     RectangleShape card({ 360, 460 });
    card.setFillColor(cardColor);
    card.setPosition(70, 60);
     RectangleShape preview({ 360, 280 });
    preview.setFillColor(cardColor);
    preview.setPosition(70, 60);

    // Play button
     RectangleShape playButton({ 360, 60 });
    playButton.setFillColor(playBlue);
    playButton.setPosition(70, 540);
     Text playText("Play", font, 22);
    playText.setFillColor( Color::White);
    playText.setPosition(220, 555);


    // ---------------- HIGH SCORE DISPLAY ----------------
     Text highScoreTitle("HIGH SCORE", font, 20);
    highScoreTitle.setStyle( Text::Bold);
    highScoreTitle.setFillColor( Color::Black);

    // Center position 
    float centerX = 240.f;
    float centerY = 440.f;

    // Get text bounds and center it
     FloatRect textBounds = highScoreTitle.getLocalBounds();
    highScoreTitle.setOrigin(textBounds.left + textBounds.width / 2.f,
        textBounds.top + textBounds.height / 2.f);
    highScoreTitle.setPosition(centerX, centerY);

    // Trophy sprite
     Texture trophyTexture;
    if (!trophyTexture.loadFromFile("resources/trophy.png")) {
        std::cerr << "Failed to load trophy image\n";
    }
     Sprite trophySprite(trophyTexture);
    trophySprite.setScale(0.4f, 0.4f);
     FloatRect trophyBounds = trophySprite.getLocalBounds();
    float trophyMargin = 10.f;
    trophySprite.setOrigin(trophyBounds.width / 2.f, trophyBounds.height / 2.f);

    float yOffset = 10.f;
    trophySprite.setPosition(centerX - textBounds.width / 2.f - trophyBounds.width / 2.f + trophyMargin, centerY + yOffset);

    // High score value
     Text highScoreValue(std::to_string(highScore), font, 23);
    highScoreTitle.setStyle( Text::Bold);
    highScoreValue.setFillColor( Color::Black);

     FloatRect scoreBounds = highScoreValue.getLocalBounds();
    highScoreValue.setOrigin(scoreBounds.left + scoreBounds.width / 2.f,
        scoreBounds.top + scoreBounds.height / 2.f);
    highScoreValue.setPosition(centerX + textBounds.width / 2.f + scoreBounds.width / 2.f + trophyMargin, centerY);

    bool startGame = false;
    while (admin.isOpen()) {
         Event e;
        while (admin.pollEvent(e)) {
            if (e.type ==  Event::Closed) return 0;
            if (e.type ==  Event::MouseButtonPressed) {
                 Vector2f m(e.mouseButton.x, e.mouseButton.y);
                if (playButton.getGlobalBounds().contains(m)) {
                    startGame = true;
                    admin.close();
                }
            }
        }

        // Hover effect for play button
         Vector2i m =  Mouse::getPosition(admin);
        playButton.setFillColor(
            playButton.getGlobalBounds().contains(( Vector2f)m) ?  Color(40, 120, 240) : playBlue
        );

        // ---------- DRAW ----------
        admin.clear();
        admin.draw(background);
        admin.draw(card);
        admin.draw(preview);

        // Title
         Text titleText("SNAKE GAME", font, 40);
        titleText.setFillColor( Color::Black);
        titleText.setStyle( Text::Bold);
        titleText.setPosition(100.f, 110.f);
        admin.draw(titleText);

        // Draw high score group
        admin.draw(trophySprite);
        admin.draw(highScoreTitle);
        admin.draw(highScoreValue);

        // Grid preview
        for (int y = 0; y < 3; y++)
            for (int x = 0; x < 9; x++) {
                 RectangleShape cell({ 40, 40 });
                cell.setPosition(70 + x * 40, 300 + y * 40);
                cell.setFillColor((x + y) % 2 ? grid1 : grid2);
                admin.draw(cell);
            }

        admin.draw(snakeBody);
        admin.draw(snakeHead);
        eye.setPosition(315, 255); admin.draw(eye);
        eye.setPosition(330, 255); admin.draw(eye);

        

        admin.draw(playButton);
        admin.draw(playText);
        admin.display();
    }

    // ================= GAME LOOP WITH RESTART =================
    bool playAgain = true;
    while (playAgain) {
        playAgain = false; 

         RenderWindow window(
             VideoMode(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE),
            "Snake Game"
        );
        window.setFramerateLimit(60);

        Board board(BOARD_HEIGHT, BOARD_WIDTH);
        Snake snake(BOARD_WIDTH / 2, BOARD_HEIGHT / 2);
        int score = 0;
        Food foods[FOOD_COUNT];
        int foodCount = 0;
        board.maintainFoods(foods, foodCount, snake);

         Clock clock;
        float moveDelay = 0.2f;
        bool gameOver = false;
        bool paused = false;
         Font font;
        font.loadFromFile("Resources/arial.ttf");
         Text scoreText("", font, 24);
        scoreText.setFillColor( Color::Black);
        scoreText.setPosition(10.f, 5.f);

        // Pause button (top right corner)
         RectangleShape pauseButton( Vector2f(120, 40));
        pauseButton.setFillColor( Color(200, 200, 200));
        pauseButton.setPosition(BOARD_WIDTH* CELL_SIZE - 130, 10.f);
         Text pauseText("Pause", font, 20);
        pauseText.setFillColor( Color::Black);
        pauseText.setPosition(pauseButton.getPosition().x + 15, pauseButton.getPosition().y + 5);

        // Pause overlay
         RectangleShape pauseOverlay( Vector2f(BOARD_WIDTH* CELL_SIZE, BOARD_HEIGHT* CELL_SIZE));
        pauseOverlay.setFillColor( Color(0, 0, 0, 200));

        // Continue button
         RectangleShape continueButton( Vector2f(150, 50));
        continueButton.setFillColor( Color(0, 200, 0));
        continueButton.setPosition((BOARD_WIDTH* CELL_SIZE - 150) / 2.f, (BOARD_HEIGHT* CELL_SIZE - 50) / 2.f);

         Text continueText("Resume", font, 24);
        continueText.setFillColor( Color::Red);
        continueText.setPosition(continueButton.getPosition().x + 15, continueButton.getPosition().y + 10);

        // Paused text
         Text pausedText("Game Paused", font, 30);
        pausedText.setFillColor( Color::White);
        pausedText.setPosition((BOARD_WIDTH* CELL_SIZE - pausedText.getLocalBounds().width) / 2.f, continueButton.getPosition().y - 60);



        while (window.isOpen() && !gameOver) {
             Event e;
            while (window.pollEvent(e)) {
                if (e.type ==  Event::KeyPressed && !paused) {
                    if (e.key.code ==  Keyboard::Up) snake.ChangeDirection(UP);
                    if (e.key.code ==  Keyboard::Down) snake.ChangeDirection(DOWN);
                    if (e.key.code ==  Keyboard::Left) snake.ChangeDirection(LEFT);
                    if (e.key.code ==  Keyboard::Right) snake.ChangeDirection(RIGHT);
                }

                // Mouse clicks
                if (e.type ==  Event::MouseButtonPressed) {
                     Vector2f mouse(e.mouseButton.x, e.mouseButton.y);

                    // Pause button click
                    if (pauseButton.getGlobalBounds().contains(mouse)) {
                        paused = true;
                    }

                    // Continue button click
                    if (paused && continueButton.getGlobalBounds().contains(mouse)) {
                        paused = false;
                    }
                }
            }

            if (!paused && clock.getElapsedTime().asSeconds() >= moveDelay) {
                snake.Move();
                clock.restart();

                if (board.IsWall(snake.getHeadX(), snake.getHeadY()) || snake.SelfCollision())
                    gameOver = true;

                for (int i = 0; i < foodCount; i++) {
                    Food& f = foods[i];
                    if (f.IsEaten(snake.getHeadX(), snake.getHeadY())) {
                        score += f.getPoints();
                        snake.grow(f.getPoints());
                        if (score > highScore) highScore = score;

                        Segment p = board.GenerateFoodPos(snake.getHead(), foods,FOOD_COUNT);
                        f.SetPos(p.x, p.y);
                        f.GenerateType();
                    }
                }
                board.maintainFoods(foods,foodCount, snake);
            }

            // Draw snake and food
            window.clear();
             Color grid1(158, 214, 91);  // light green
             Color grid2(142, 200, 80);  // dark green


            for (int y = 0; y < BOARD_HEIGHT; y++) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                     RectangleShape cell( Vector2f(CELL_SIZE, CELL_SIZE));
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cell.setFillColor((x + y) % 2 == 0 ? grid1 : grid2); // checkerboard effect
                    window.draw(cell);
                }
            }

            Segment* cur = snake.getHead();
            float r = CELL_SIZE / 2.f;
            while (cur) {
                 CircleShape c(r);
                c.setFillColor(cur == snake.getHead() ?  Color::Blue :  Color(100, 150, 255));
                c.setPosition(cur->x * CELL_SIZE + r, cur->y * CELL_SIZE + r);
                c.setOrigin(r, r);
                window.draw(c);
                cur = cur->next;
            }
            for (int i = 0; i < foodCount; i++)
            {
                Food& f = foods[i];
                float r;          
                 Color color; 

                // Set radius and color depending on the food type
                switch (f.getType())
                {
                case FoodType::APPLE:
                    r = CELL_SIZE * 0.4f;
                    color =  Color::Red;
                    break;
                case FoodType::MEGA_STRAWBERRY:
                    r = CELL_SIZE * 0.5f;
                    color =  Color(255, 0, 255);
                    break;
                case FoodType::MEGA_WATERMELON:
                    r = CELL_SIZE * 0.5f;
                    color =  Color::Green;
                    break;
                default:
                    r = CELL_SIZE * 0.4f;
                    color =  Color::Yellow;
                    break;
                }

                // Create the circle with the correct radius
                 CircleShape foodCircle(r);
                foodCircle.setOrigin(r, r); // center the circle
                foodCircle.setPosition(f.getX() * CELL_SIZE + CELL_SIZE / 2.f,
                    f.getY() * CELL_SIZE + CELL_SIZE / 2.f);
                foodCircle.setFillColor(color);

                window.draw(foodCircle);
            }



            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            window.draw(pauseButton);
            window.draw(pauseText);

            if (paused) {
                window.draw(pauseOverlay);
                window.draw(pausedText);
                window.draw(continueButton);
                window.draw(continueText);
            }

            window.display();
        }

        // ================= GAME OVER SCREEN =================
        if (gameOver) {
             RectangleShape overlay( Vector2f(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE));
            overlay.setFillColor( Color(0, 0, 0, 150));

             Text gameOverText("Game Over", font, 50);
            gameOverText.setFillColor( Color::Red);
            gameOverText.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 130, BOARD_HEIGHT * CELL_SIZE / 2 - 100);

            // Play Again button
             RectangleShape playAgainButton( Vector2f(200, 50));
            playAgainButton.setFillColor( Color::Blue);
            playAgainButton.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 100, BOARD_HEIGHT * CELL_SIZE / 2);

             Text playAgainText("Play Again", font, 24);
            playAgainText.setFillColor( Color::White);
            playAgainText.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 70, BOARD_HEIGHT * CELL_SIZE / 2 + 10);

            // Quit button
             RectangleShape quitButton( Vector2f(200, 50));
            quitButton.setFillColor( Color::Red);
            quitButton.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 100, BOARD_HEIGHT * CELL_SIZE / 2 + 70);

             Text quitText("Quit", font, 24);
            quitText.setFillColor( Color::White);
            quitText.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 30, BOARD_HEIGHT * CELL_SIZE / 2 + 80);

            bool waiting = true;
            while (waiting && window.isOpen()) {
                 Event e;
                while (window.pollEvent(e)) {
                    if (e.type ==  Event::Closed) window.close();
                    if (e.type ==  Event::MouseButtonPressed) {
                         Vector2f mouse(e.mouseButton.x, e.mouseButton.y);
                        if (playAgainButton.getGlobalBounds().contains(mouse)) {
                            waiting = false;
                            playAgain = true;
                        }
                        if (quitButton.getGlobalBounds().contains(mouse)) {
                            waiting = false;
                            window.close();
                        }
                    }
                }

                // Hover effects
                 Vector2i m =  Mouse::getPosition(window);
                playAgainButton.setFillColor(playAgainButton.getGlobalBounds().contains(( Vector2f)m)
                    ?  Color(100, 200, 255) :  Color::Blue);
                quitButton.setFillColor(quitButton.getGlobalBounds().contains(( Vector2f)m)
                    ?  Color(255, 100, 100) :  Color::Red);

                window.clear();
                window.draw(overlay);
                window.draw(gameOverText);
                window.draw(playAgainButton);
                window.draw(playAgainText);
                window.draw(quitButton);
                window.draw(quitText);
                window.display();
            }
        }
    }

    // ================= SAVE HIGH SCORE =================
    std::ofstream out("highscore.txt");
    out << highScore;
    out.close();

    return 0;
}