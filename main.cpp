#include <SFML/Graphics.hpp>
#include <optional>

int main()
{
    sf::RenderWindow window(sf::VideoMode({480u, 720u}), "Flappy Bird Step 1");
    window.setFramerateLimit(60);

    sf::Color skyBlue(135, 206, 235);

    sf::RectangleShape floor;
    floor.setSize({480.0f, 100.0f});
    floor.setFillColor(sf::Color(34, 139, 34));
    floor.setPosition({0.0f, 620.0f});

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

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

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

        window.clear(skyBlue);
        window.draw(floor);

        for (int i = 0; i < MAX_PIPES; i++)
        {
            window.draw(topPipes[i]);
            window.draw(bottomPipes[i]);
        }

        window.display();
    }

    return 0;
}
