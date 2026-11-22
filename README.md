# FlappyBird-PF

A 2D Flappy Bird game developed in C++ using SFML for our Programming Fundamentals semester project.

## Group Members & Contributions

* **AbdulRehman** (`AbdulRehman-FAST-lhr-2026`):
  * SFML Window Setup (`sf::RenderWindow`) & Main 60 FPS Game Loop
  * Base Ground Floor Rendering (`sf::RectangleShape`)
  * Moving Pipe Obstacles & Gap Generation (1D Arrays)
  * Randomized Pipe Heights (`rand()` & `srand()`)
  * Pipe & Ground Collision Detection Logic
  * Game States Management (Start Screen, Active Gameplay, Game Over & Restart)

* **Waqar Ali** (`waqarali010619-stack`):
  * Scrolling Background Clouds (1D Arrays & `sf::CircleShape`)
  * Scrolling Ground Grass Animation (`sf::RectangleShape`)
  * Composite 5-Part Bird Shape with Gravity, Jump Physics & Wing Flap Animation
  * Live Score Tracking & Persistent High Score File Handling (`highscore.txt` via `<fstream>`)
  * SFML Audio Sound Effects (`jump.wav`, `score.wav`, `hit.wav` via `<SFML/Audio.hpp>`)
  * Dynamic Difficulty Scaling (Progressive Game Speed with Score)