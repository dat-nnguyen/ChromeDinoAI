# 🦖 Chrome Dino AI - Genetic Algorithm in C++

![Language](https://img.shields.io/badge/language-C%2B%2B17-blue.svg)
![Library](https://img.shields.io/badge/library-SDL2-orange.svg)
![AI](https://img.shields.io/badge/AI-Neural%20Network-green.svg)

A high-performance simulation of the classic Chrome Dino game, played automatically by an AI.

Unlike standard AI projects that use Python libraries (TensorFlow/PyTorch), this project implements a **Feed-Forward Neural Network** and **Genetic Algorithm** entirely from scratch in C++. It demonstrates memory management, game engine architecture, and evolutionary computing concepts.

---

## 🧠 How It Works

<img width="755" height="629" alt="Image" src="https://github.com/user-attachments/assets/40cfb924-79cb-4f25-be93-4059430bfae8" />

The AI does not know the rules of the game. It learns to play through **Natural Selection**.

### 1. The Neural Network (The Brain)
Each Dino has a custom "Brain" consisting of:
* **Input Layer (4 Neurons):**
  1. Distance to next obstacle
  2. Speed of the game
  3. Width of obstacle
  4. Height of obstacle 
* **Hidden Layer:** 6 Neurons with Sigmoid activation functions.
* **Output Layer (2 Neurons):**
  1. Jump Probability
  2. Duck Probability

### 2. The Genetic Algorithm (The Evolution)
1.  **Population:** The game spawns **50 Dinos** simultaneously per generation.
2.  **Selection:** Dinos that survive longer get a higher "Fitness" score.
3.  **Crossover & Mutation:** When the entire population dies, the best-performing brain is saved. The next generation is created by cloning this champion and applying random **mutations** (shifting weights by +/- 10%) to the offspring.
4.  **Result:** Over 10-20 generations, the AI evolves from random jumping to near-perfect gameplay.

---

## 🛠️ Tech Stack

* **Language:** C++17
* **Graphics/Audio:** SDL2, SDL2_Image, SDL2_TTF, SDL2_Mixer
* **IDE/Build:** Clang++, Make (or CMake)

---

## 📂 Project Structure

```text
├── assets/          # Sprites, Fonts, and Audio files
├── header/          # Header files (.h) including Brain.h, Dino.h
├── src/             # Implementation files (.cpp) including Brain.cpp, Genetic Logic
├── main.cpp         # Main Game Loop and Population Manager
└── README.md
```
## Preview
Here is how it will look when rendered on GitHub or GitLab:

## 🚀 How to Build & Run

### Prerequisites
You need **SDL2** installed on your machine.

* **Mac (Homebrew):** `brew install sdl2 sdl2_image sdl2_ttf sdl2_mixer`
* **Linux:** `sudo apt-get install libsdl2-dev`
* **Windows:** Install SDL2 development libraries via MinGW or Visual Studio.

### Compile Command (Terminal)
Run this command from the project root directory:

```bash
clang++ -std=c++17 main.cpp src/Dino.cpp src/Ground.cpp src/Obstacles.cpp src/Scores.cpp src/RestartButton.cpp src/SoundManager.cpp src/Constants.cpp src/Brain.cpp -o dino -Iheader -I/usr/local/include -L/usr/local/lib -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer
```
### Run the Simulation
```bash
./dino
```

## 🎮 Controls

* **Watch:** Sit back and watch the AI evolve!
* **Quit:** Close the window to exit the simulation.

> **Note:** Manual keyboard controls have been disabled to prioritize AI processing speed.

## 📈 Key Learnings / Implementation Details

* **Memory Management:** Handling a population of 50 objects requires careful pointer management and cleanup in destructors to prevent memory leaks during generation resets.
* **Math Implementation:** Implemented Matrix multiplication and Sigmoid activation functions manually for the Neural Network.
* **Game Physics:** Custom collision detection (AABB) and gravity logic.

**Author:** Nguyen Tuan Dat

**License:** MIT

