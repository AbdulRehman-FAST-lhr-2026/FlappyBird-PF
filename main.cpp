#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    // ==================== ABDULREHMAN'S PART ====================
    // create the main game window and lock it to 60 fps
    sf::RenderWindow window(sf::VideoMode({480u, 720u}), "Flappy Bird - PF Final Project");
    window.setFramerateLimit(60);

    // sky background color and the green ground floor at the bottom
    sf::Color skyBlue(135, 206, 235);

    sf::RectangleShape floor;
    floor.setSize({480.0f, 100.0f});
    floor.setFillColor(sf::Color(34, 139, 34));
    floor.setPosition({0.0f, 620.0f});

    // setup arrays for 6 pairs of top and bottom pipes
    const int MAX_PIPES = 6;
    sf::RectangleShape topPipes[MAX_PIPES];
    sf::RectangleShape bottomPipes[MAX_PIPES];
    float pipeX[MAX_PIPES];
    float pipeTopHeight[MAX_PIPES];
    const float gapSize = 170.0f;
    const float pipeWidth = 60.0f;
    float pipeSpeed = 2.0f;

    // give each pipe its starting height, gap, and horizontal spacing
    for (int i = 0; i < MAX_PIPES; i++)
    {
        pipeTopHeight[i] = 120.0f + (i * 30) % 180;
        pipeX[i] = 500.0f + i * 180.0f;

        topPipes[i].setSize({pipeWidth, pipeTopHeight[i]});
        topPipes[i].setFillColor(sf::Color(0, 150, 0));
        topPipes[i].setPosition({pipeX[i], 0.0f});

        float bottomHeight = 720.0f - 100.0f - pipeTopHeight[i] - gapSize;
        bottomPipes[i].setSize({pipeWidth, bottomHeight});
        bottomPipes[i].setFillColor(sf::Color(0, 150, 0));
        bottomPipes[i].setPosition({pipeX[i], pipeTopHeight[i] + gapSize});
    }

    // load arial font for all text on screen
    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        return -1;
    }

    // track frames for random pipes and current game state (0 = start, 1 = playing, 2 = game over)
    int frameCount = 0;
    int gameState = 0;

    // text shown on the start screen
    sf::Text startText(font, "Press SPACE to Start", 34);
    startText.setFillColor(sf::Color::White);
    startText.setOutlineColor(sf::Color::Black);
    startText.setOutlineThickness(2.0f);
    startText.setPosition({70.0f, 300.0f});

    // text shown when the player loses
    sf::Text gameOverText(font, "Game Over!\nPress R to Restart", 34);
    gameOverText.setFillColor(sf::Color::White);
    gameOverText.setOutlineColor(sf::Color::Black);
    gameOverText.setOutlineThickness(2.0f);
    gameOverText.setPosition({70.0f, 260.0f});

    // ==================== WAQAR ALI'S PART ====================
    // setup 4 clouds in the sky, each made of 3 overlapping circles
    const int MAX_CLOUDS = 4;
    sf::CircleShape cloudLeft[MAX_CLOUDS];
    sf::CircleShape cloudMid[MAX_CLOUDS];
    sf::CircleShape cloudRight[MAX_CLOUDS];
    float cloudX[MAX_CLOUDS] = {60.0f, 210.0f, 360.0f, 510.0f};
    float cloudY[MAX_CLOUDS] = {85.0f, 155.0f, 65.0f, 135.0f};
    const float cloudSpeed = 0.6f;

    for (int i = 0; i < MAX_CLOUDS; i++)
    {
        cloudLeft[i].setRadius(18.0f);
        cloudLeft[i].setFillColor(sf::Color(255, 255, 255, 225));

        cloudMid[i].setRadius(24.0f);
        cloudMid[i].setFillColor(sf::Color(255, 255, 255, 225));

        cloudRight[i].setRadius(18.0f);
        cloudRight[i].setFillColor(sf::Color(255, 255, 255, 225));
    }

    // small green grass stripes on top of the floor so the ground looks like its moving
    const int MAX_STRIPES = 14;
    sf::RectangleShape groundStripes[MAX_STRIPES];
    float stripeX[MAX_STRIPES];
    for (int i = 0; i < MAX_STRIPES; i++)
    {
        stripeX[i] = i * 40.0f;
        groundStripes[i].setSize({20.0f, 14.0f});
        groundStripes[i].setFillColor(sf::Color(55, 185, 55));
        groundStripes[i].setPosition({stripeX[i], 620.0f});
    }

    // bird starting position, gravity, and jump strength
    float birdX = 110.0f;
    float birdY = 280.0f;
    float birdVelocity = 0.0f;
    float wingOffsetY = 13.0f;
    const float gravity = 0.45f;
    const float jumpStrength = -7.5f;

    // build the bird using 5 shapes (body, wing, eye, pupil, and triangle beak)
    sf::CircleShape birdBody(18.0f);
    birdBody.setFillColor(sf::Color(255, 215, 0));

    sf::CircleShape birdWing(8.5f);
    birdWing.setFillColor(sf::Color(230, 150, 0));

    sf::CircleShape birdEye(4.5f);
    birdEye.setFillColor(sf::Color::White);

    sf::CircleShape birdPupil(2.0f);
    birdPupil.setFillColor(sf::Color::Black);

    sf::ConvexShape birdBeak;
    birdBeak.setPointCount(3);
    birdBeak.setPoint(0, {0.0f, 0.0f});
    birdBeak.setPoint(1, {12.0f, 5.0f});
    birdBeak.setPoint(2, {0.0f, 10.0f});
    birdBeak.setFillColor(sf::Color(255, 85, 0));

    // variables to track current score, high score, and which pipes are already scored
    int score = 0;
    int highScore = 0;
    bool pipePassed[MAX_PIPES] = {false, false, false, false, false, false};

    // read the saved high score from highscore.txt if the file exists
    ifstream inFile("highscore.txt");
    if (inFile.is_open())
    {
        inFile >> highScore;
        inFile.close();
    }

    // setup live score and best score text at the top corners
    sf::Text scoreText(font, "Score: 0", 26);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.0f);
    scoreText.setPosition({18.0f, 15.0f});

    sf::Text highScoreText(font, "Best: " + to_string(highScore), 26);
    highScoreText.setFillColor(sf::Color(255, 230, 80));
    highScoreText.setOutlineColor(sf::Color::Black);
    highScoreText.setOutlineThickness(2.0f);
    highScoreText.setPosition({335.0f, 15.0f});

    // load sound files for jump, scoring a point, and hitting a pipe
    sf::SoundBuffer jumpBuffer, scoreBuffer, hitBuffer;
    if (!jumpBuffer.loadFromFile("jump.wav") ||
        !scoreBuffer.loadFromFile("score.wav") ||
        !hitBuffer.loadFromFile("hit.wav"))
    {
        return -1;
    }

    sf::Sound jumpSound(jumpBuffer);
    sf::Sound scoreSound(scoreBuffer);
    sf::Sound hitSound(hitBuffer);

    while (window.isOpen())
    {
        // ==================== ABDULREHMAN'S PART ====================
        frameCount++;

        // check for window close and keyboard button presses
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                // flap the bird up and play jump sound when space or up arrow is pressed
                if (gameState != 2 && (keyPressed->code == sf::Keyboard::Key::Space ||
                                       keyPressed->code == sf::Keyboard::Key::Up))
                {
                    birdVelocity = jumpStrength;
                    wingOffsetY = 6.0f;
                    jumpSound.play();
                }

                // switch from start screen to active gameplay
                if (gameState == 0 && (keyPressed->code == sf::Keyboard::Key::Space ||
                                       keyPressed->code == sf::Keyboard::Key::Up))
                {
                    gameState = 1;
                }

                // reset score, speed, bird, and pipes when R is pressed after game over
                if (gameState == 2 && keyPressed->code == sf::Keyboard::Key::R)
                {
                    score = 0;
                    pipeSpeed = 2.0f;
                    birdY = 280.0f;
                    birdVelocity = 0.0f;
                    wingOffsetY = 13.0f;

                    for (int i = 0; i < MAX_PIPES; i++)
                    {
                        pipePassed[i] = false;
                        pipeX[i] = 500.0f + i * 180.0f;
                        topPipes[i].setPosition({pipeX[i], 0.0f});
                        bottomPipes[i].setPosition({pipeX[i], pipeTopHeight[i] + gapSize});
                    }

                    scoreText.setString("Score: 0");
                    gameState = 0;
                }
            }
        }

        // move pipes left and give them a new random height when they go off screen
        if (gameState == 1)
        {
            for (int i = 0; i < MAX_PIPES; i++)
            {
                pipeX[i] -= pipeSpeed;

                if (pipeX[i] < -pipeWidth)
                {
                    pipeX[i] = 500.0f + (MAX_PIPES - 1) * 180.0f;
                    pipeTopHeight[i] = 90.0f + static_cast<float>((frameCount * 37 + i * 73) % 240);
                    float newBottomHeight = 720.0f - 100.0f - pipeTopHeight[i] - gapSize;
                    topPipes[i].setSize({pipeWidth, pipeTopHeight[i]});
                    bottomPipes[i].setSize({pipeWidth, newBottomHeight});
                    pipePassed[i] = false;
                }

                topPipes[i].setPosition({pipeX[i], 0.0f});
                bottomPipes[i].setPosition({pipeX[i], pipeTopHeight[i] + gapSize});
            }

            // check if the bird hit the ground or any top/bottom pipe
            bool collided = false;
            if (birdY >= 620.0f - 36.0f)
            {
                collided = true;
            }

            for (int i = 0; i < MAX_PIPES; i++)
            {
                if (birdBody.getGlobalBounds().findIntersection(topPipes[i].getGlobalBounds()) ||
                    birdBody.getGlobalBounds().findIntersection(bottomPipes[i].getGlobalBounds()))
                {
                    collided = true;
                    break;
                }
            }

            // play hit sound and end the game on collision
            if (collided)
            {
                hitSound.play();
                gameState = 2;
            }
        }
        else if (gameState == 0)
        {
            // keep the bird hovering in the middle before the game starts
            birdY = 280.0f;
            birdVelocity = 0.0f;
            wingOffsetY = 13.0f;
        }
        else if (gameState == 2)
        {
            // stop bird movement when game is over
            birdVelocity = 0.0f;
        }

        // ==================== WAQAR ALI'S PART ====================
        if (gameState == 1)
        {
            // increase score when bird passes a pipe and slowly make the game faster
            for (int i = 0; i < MAX_PIPES; i++)
            {
                if (!pipePassed[i] && pipeX[i] + pipeWidth < birdX)
                {
                    pipePassed[i] = true;
                    score++;
                    scoreSound.play();

                    if (pipeSpeed < 5.5f)
                    {
                        pipeSpeed += 0.12f;
                    }

                    // save new high score to file if current score beats the old record
                    if (score > highScore)
                    {
                        highScore = score;
                        ofstream outFile("highscore.txt");
                        if (outFile.is_open())
                        {
                            outFile << highScore;
                            outFile.close();
                        }
                    }

                    scoreText.setString("Score: " + to_string(score));
                    highScoreText.setString("Best: " + to_string(highScore));
                }
            }

            // scroll the green grass stripes at the same speed as the pipes
            for (int i = 0; i < MAX_STRIPES; i++)
            {
                stripeX[i] -= pipeSpeed;
                if (stripeX[i] < -40.0f)
                {
                    stripeX[i] += MAX_STRIPES * 40.0f;
                }
                groundStripes[i].setPosition({stripeX[i], 620.0f});
            }

            // pull the bird down with gravity and slowly bring the wing back down
            birdVelocity += gravity;
            birdY += birdVelocity;

            if (wingOffsetY < 14.0f)
            {
                wingOffsetY += 0.4f;
            }

            // prevent the bird from going above the top or below the floor
            if (birdY < 0.0f)
            {
                birdY = 0.0f;
                birdVelocity = 0.0f;
            }
            if (birdY > 620.0f - 36.0f)
            {
                birdY = 620.0f - 36.0f;
                birdVelocity = 0.0f;
            }
        }

        // slowly move clouds across the background sky
        for (int i = 0; i < MAX_CLOUDS; i++)
        {
            cloudX[i] -= cloudSpeed;
            if (cloudX[i] < -80.0f)
            {
                cloudX[i] = 500.0f;
            }

            cloudLeft[i].setPosition({cloudX[i], cloudY[i] + 8.0f});
            cloudMid[i].setPosition({cloudX[i] + 16.0f, cloudY[i]});
            cloudRight[i].setPosition({cloudX[i] + 38.0f, cloudY[i] + 8.0f});
        }

        // update positions of all 5 bird shapes together
        birdBody.setPosition({birdX, birdY});
        birdWing.setPosition({birdX + 5.0f, birdY + wingOffsetY});
        birdEye.setPosition({birdX + 22.0f, birdY + 6.0f});
        birdPupil.setPosition({birdX + 25.0f, birdY + 7.5f});
        birdBeak.setPosition({birdX + 33.0f, birdY + 13.0f});

        // ==================== RENDERING ====================
        // clear the previous frame and draw everything from back to front
        window.clear(skyBlue);

        for (int i = 0; i < MAX_CLOUDS; i++)
        {
            window.draw(cloudLeft[i]);
            window.draw(cloudMid[i]);
            window.draw(cloudRight[i]);
        }

        for (int i = 0; i < MAX_PIPES; i++)
        {
            window.draw(topPipes[i]);
            window.draw(bottomPipes[i]);
        }
        window.draw(floor);

        for (int i = 0; i < MAX_STRIPES; i++)
        {
            window.draw(groundStripes[i]);
        }

        window.draw(birdBody);
        window.draw(birdWing);
        window.draw(birdEye);
        window.draw(birdPupil);
        window.draw(birdBeak);

        window.draw(scoreText);
        window.draw(highScoreText);

        if (gameState == 0)
        {
            window.draw(startText);
        }
        else if (gameState == 2)
        {
            window.draw(gameOverText);
        }

        window.display();
    }

    return 0;
}