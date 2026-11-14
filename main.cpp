#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({480u, 720u}), "Flappy Bird - PF Project");
    window.setFramerateLimit(60);

    sf::Color skyBlue(135, 206, 235);

    sf::RectangleShape floor;
    floor.setSize({480.0f, 100.0f});
    floor.setFillColor(sf::Color(34, 139, 34));
    floor.setPosition({0.0f, 620.0f});

    // --- Pipes Setup (AbdulRehman's Part) ---
    const int MAX_PIPES = 6;
    sf::RectangleShape topPipes[MAX_PIPES];
    sf::RectangleShape bottomPipes[MAX_PIPES];
    float pipeX[MAX_PIPES];
    float pipeTopHeight[MAX_PIPES];
    const float gapSize = 170.0f;
    const float pipeWidth = 60.0f;
    const float pipeSpeed = 2.0f;

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
    // WAQAR ALI'S CONTRIBUTION: BACKGROUND CLOUDS & BIRD SHAPE / PHYSICS
    // =========================================================================

    // 1. Background Clouds Setup (Using 1D Arrays)
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

    // 2. Bird Shape Setup (Body, Wing, Eye, Pupil & Beak)
    float birdX = 110.0f;
    float birdY = 280.0f;
    float birdVelocity = 0.0f;
    const float gravity = 0.45f;
    const float jumpStrength = -7.5f;

    sf::CircleShape birdBody(18.0f);
    birdBody.setFillColor(sf::Color(255, 215, 0)); // Yellow body

    sf::CircleShape birdWing(8.5f);
    birdWing.setFillColor(sf::Color(230, 150, 0)); // Orange-gold wing

    sf::CircleShape birdEye(4.5f);
    birdEye.setFillColor(sf::Color::White);

    sf::CircleShape birdPupil(2.0f);
    birdPupil.setFillColor(sf::Color::Black);

    sf::ConvexShape birdBeak;
    birdBeak.setPointCount(3);
    birdBeak.setPoint(0, {0.0f, 0.0f});
    birdBeak.setPoint(1, {12.0f, 5.0f});
    birdBeak.setPoint(2, {0.0f, 10.0f});
    birdBeak.setFillColor(sf::Color(255, 85, 0)); // Orange beak

    // =========================================================================

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

            // Waqar's Part: Spacebar ya Up Arrow dabane par Bird jump karegi
            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Space ||
                    keyPressed->code == sf::Keyboard::Key::Up)
                {
                    birdVelocity = jumpStrength;
                }
            }
        }

        // --- Pipes Movement Update ---
        for (int i = 0; i < MAX_PIPES; i++)
        {
            pipeX[i] -= pipeSpeed;

            if (pipeX[i] < -pipeWidth)
            {
                pipeX[i] = 500.0f + (MAX_PIPES - 1) * 180.0f;
            }

            topPipes[i].setPosition({pipeX[i], 0.0f});
            bottomPipes[i].setPosition({pipeX[i], pipeTopHeight[i] + gapSize});
        }

        // --- Waqar's Part: Background Clouds Movement Update ---
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

        // --- Waqar's Part: Bird Gravity & Position Update ---
        birdVelocity += gravity;
        birdY += birdVelocity;

        // Screen ke top aur floor se bahar jane se rokna
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
        birdWing.setPosition({birdX + 5.0f, birdY + 13.0f});
        birdEye.setPosition({birdX + 22.0f, birdY + 6.0f});
        birdPupil.setPosition({birdX + 25.0f, birdY + 7.5f});
        birdBeak.setPosition({birdX + 33.0f, birdY + 13.0f});

        // --- Drawing Everything ---
        window.clear(skyBlue);

        // Pehle clouds draw honge taake pipes ke peeche rahein
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

        // Bird ke saare parts draw karna
        window.draw(birdBody);
        window.draw(birdWing);
        window.draw(birdEye);
        window.draw(birdPupil);
        window.draw(birdBeak);

        window.display();
    }

    return 0;
}