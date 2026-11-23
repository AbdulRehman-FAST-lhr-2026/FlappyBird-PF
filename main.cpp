#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    // --- [ABDULREHMAN] Window & Ground Base Setup ---
    sf::RenderWindow window(sf::VideoMode({480u, 720u}), "Flappy Bird - PF Final Project");
    window.setFramerateLimit(60);

    sf::Color skyBlue(135, 206, 235);

    sf::RectangleShape floor;
    floor.setSize({480.0f, 100.0f});
    floor.setFillColor(sf::Color(34, 139, 34));
    floor.setPosition({0.0f, 620.0f});

    // --- [ABDULREHMAN] Pipe Obstacles Setup ---
    const int MAX_PIPES = 6;
    sf::RectangleShape topPipes[MAX_PIPES];
    sf::RectangleShape bottomPipes[MAX_PIPES];
    float pipeX[MAX_PIPES];
    float pipeTopHeight[MAX_PIPES];
    const float gapSize = 170.0f;
    const float pipeWidth = 60.0f;
    float pipeSpeed = 2.0f; // [WAQAR ALI] Non-const so speed increases with score

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

    // =========================================================================
    // [WAQAR ALI - PART 1] Background Clouds Setup (1D Arrays & CircleShapes)
    // =========================================================================
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

    // =========================================================================
    // [WAQAR ALI - PART 2] Ground Scrolling Grass Stripes Setup
    // =========================================================================
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

    // =========================================================================
    // [WAQAR ALI - PART 3] Composite Bird Shape, Physics & Wing Animation Setup
    // =========================================================================
    float birdX = 110.0f;
    float birdY = 280.0f;
    float birdVelocity = 0.0f;
    float wingOffsetY = 13.0f;
    const float gravity = 0.45f;
    const float jumpStrength = -7.5f;

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

    // =========================================================================
    // [WAQAR ALI - PART 4] Score & High Score File Handling (highscore.txt)
    // =========================================================================
    int score = 0;
    int highScore = 0;
    bool pipePassed[MAX_PIPES] = {false, false, false, false, false, false};

    ifstream inFile("highscore.txt");
    if (inFile.is_open())
    {
        inFile >> highScore;
        inFile.close();
    }

    sf::Font font;
    if (!font.openFromFile("arial.ttf"))
    {
        return -1;
    }

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

    // =========================================================================
    // [WAQAR ALI - PART 5] SFML Audio Sound Effects Setup
    // =========================================================================
    sf::SoundBuffer jumpBuffer, scoreBuffer, hitBuffer;
    if (!jumpBuffer.loadFromFile("jump.wav") ||
        !scoreBuffer.loadFromFile("score.wav") ||
        !hitBuffer.loadFromFile("hit.wav"))
    {
        return -1;
    }

    sf::Sound jumpSound(jumpBuffer);
    sf::Sound scoreSound(scoreBuffer);
    sf::Sound hitSound(hitBuffer); // [ABDULREHMAN] Call hitSound.play() on pipe/floor collision

    // =========================================================================
    // MAIN GAME LOOP
    // =========================================================================
    while (window.isOpen())
    {
        while (const auto event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // --- [WAQAR ALI] Bird Jump Controls, Wing Flap & Jump Sound ---
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Space ||
                    keyPressed->code == sf::Keyboard::Key::Up)
                {
                    birdVelocity = jumpStrength;
                    wingOffsetY = 6.0f;
                    jumpSound.play();
                }
            }
        }

        // --- [ABDULREHMAN] Pipes Movement ---
        for (int i = 0; i < MAX_PIPES; i++)
        {
            pipeX[i] -= pipeSpeed;

            // --- [WAQAR ALI - PART 6] Score, Progressive Speed & highscore.txt Saving ---
            if (!pipePassed[i] && pipeX[i] + pipeWidth < birdX)
            {
                pipePassed[i] = true;
                score++;
                scoreSound.play();

                // Gradually increase difficulty as score increases (max cap: 5.5f)
                if (pipeSpeed < 5.5f)
                {
                    pipeSpeed += 0.12f;
                }

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

            if (pipeX[i] < -pipeWidth)
            {
                pipeX[i] = 500.0f + (MAX_PIPES - 1) * 180.0f;
                pipePassed[i] = false; // [WAQAR ALI] Reset score flag when pipe recycles
            }

            topPipes[i].setPosition({pipeX[i], 0.0f});
            bottomPipes[i].setPosition({pipeX[i], pipeTopHeight[i] + gapSize});
        }

        // --- [WAQAR ALI] Background Clouds Scrolling Logic ---
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

        // --- [WAQAR ALI] Ground Grass Stripes Scrolling Logic ---
        for (int i = 0; i < MAX_STRIPES; i++)
        {
            stripeX[i] -= pipeSpeed;
            if (stripeX[i] < -40.0f)
            {
                stripeX[i] += MAX_STRIPES * 40.0f;
            }
            groundStripes[i].setPosition({stripeX[i], 620.0f});
        }

        // --- [WAQAR ALI] Bird Gravity, Boundaries & Wing Animation ---
        birdVelocity += gravity;
        birdY += birdVelocity;

        if (wingOffsetY < 14.0f)
        {
            wingOffsetY += 0.4f;
        }

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

        birdBody.setPosition({birdX, birdY});
        birdWing.setPosition({birdX + 5.0f, birdY + wingOffsetY});
        birdEye.setPosition({birdX + 22.0f, birdY + 6.0f});
        birdPupil.setPosition({birdX + 25.0f, birdY + 7.5f});
        birdBeak.setPosition({birdX + 33.0f, birdY + 13.0f});

        // --- [ABDULREHMAN'S REMAINING PART] Add Random Pipe Heights, Collision & Game Over Here ---

        // =====================================================================
        // RENDERING / DRAWING
        // =====================================================================
        window.clear(skyBlue);

        // [WAQAR ALI] Draw Background Clouds
        for (int i = 0; i < MAX_CLOUDS; i++)
        {
            window.draw(cloudLeft[i]);
            window.draw(cloudMid[i]);
            window.draw(cloudRight[i]);
        }

        // [ABDULREHMAN] Draw Pipes & Base Floor
        for (int i = 0; i < MAX_PIPES; i++)
        {
            window.draw(topPipes[i]);
            window.draw(bottomPipes[i]);
        }
        window.draw(floor);

        // [WAQAR ALI] Draw Ground Grass Stripes
        for (int i = 0; i < MAX_STRIPES; i++)
        {
            window.draw(groundStripes[i]);
        }

        // [WAQAR ALI] Draw Composite Bird (5 Parts)
        window.draw(birdBody);
        window.draw(birdWing);
        window.draw(birdEye);
        window.draw(birdPupil);
        window.draw(birdBeak);

        // [WAQAR ALI] Draw Live Score & High Score UI
        window.draw(scoreText);
        window.draw(highScoreText);

        window.display();
    }

    return 0;
}