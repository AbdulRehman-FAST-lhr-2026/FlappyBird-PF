# FlappyBird-PF

A 2D arcade Flappy Bird clone developed in C++ using SFML 3 for our Programming Fundamentals (Section D) semester project.

## Features
* **Smooth Bird Physics & Wing Animation:** Composite 5-part bird (`CircleShape` & `ConvexShape`) with gravity, jump impulse, and dynamic wing-flapping motion.
* **Randomized Pipe Obstacles:** Infinite scrolling top and bottom pipes with randomized gap heights.
* **Dynamic Difficulty Scaling:** Game speed gradually increases as the player's score rises.
* **Scrolling Parallax Environment:** Moving sky clouds and scrolling ground grass stripes.
* **Live Score & Persistent High Score:** Real-time score tracking with automatic all-time high score saving/loading via `highscore.txt` (`<fstream>`).
* **Sound Effects:** Custom audio feedback for jumping, scoring, and collisions (`<SFML/Audio.hpp>`).
* **Complete Game States:** Start screen, active gameplay, collision detection, Game Over screen, and instant restart.

## Controls
* **Start Game / Flap (Jump):** `Spacebar` or `Up Arrow`
* **Restart After Game Over:** `R`
* **Quit Game:** Close Window (`X` button)

## How to Compile and Run (Windows / MinGW + SFML 3)
1. Ensure all asset files (`arial.ttf`, `jump.wav`, `score.wav`, `hit.wav`, `highscore.txt`) and SFML 3 `.dll` files (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-audio-3.dll`, `sfml-system-3.dll`) are in the same directory as `main.cpp`.
2. Open terminal in the project folder and compile using:
   ```bash
   g++ main.cpp -o main.exe -IC:\SFML\include -LC:\SFML\lib -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system