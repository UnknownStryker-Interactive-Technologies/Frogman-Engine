#ifndef FROGMAN_ENGINE_HEADER_H
#define FROGMAN_ENGINE_HEADER_H
/*
The MIT License

Copyright (c) <2077> <Uniform Sierra>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>

// Macros
#define MAX_ENTITIES 1000
#define LOG(message) std::cout << "LOG: " << message << std::endl

// Namespaces
namespace FrogmanEngine {
    namespace Physics {
        class Vector2 {
            FE_CLASS()
        public:
            FE_PROPERTY()
                float x, y;
            Vector2(float _x = 0, float _y = 0) : x(_x), y(_y) {}
            float length() const { return std::sqrt(x * x + y * y); }
            Vector2 normalized() const {
                float len = length();
                return len > 0 ? Vector2(x / len, y / len) : Vector2(0, 0);
            }
        };
    }

    namespace Rendering {
        class Renderer {
            FE_CLASS()
        public:
            FE_METHOD()
                void render();
        };
    }

    // Template class for game entities
    template<typename T>
    class Entity {
        FE_CLASS()
    public:
        Entity(const std::string& name) : name(name) {}

        FE_METHOD()
            void update(float deltaTime);

        FE_METHOD()
            void render();

        FE_METHOD()
            T getComponent();

        FE_METHOD()
            void addComponent(const T& component);

    private:
        FE_PROPERTY()
            std::string name;
        FE_PROPERTY()
            std::vector<T> components;
    };

    // Enumeration for game states
    enum class GameState {
        FE_CLASS()
        MAIN_MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };
}

// Function prototypes
FE_STATIC_METHOD()
void initializeGame();

FE_STATIC_METHOD()
void updateGame(float deltaTime);

FE_STATIC_METHOD()
void renderGame();

FE_STATIC_METHOD()
void shutdownGame();

// Classes for game objects
class GameObject {
    FE_CLASS()
public:
    GameObject(const std::string& name);
    FE_METHOD()
        virtual void update(float deltaTime);
    FE_METHOD()
        virtual void render();
    virtual ~GameObject() = default;

private:
    FE_PROPERTY()
        std::string name;
};

class Player : public GameObject {
    FE_CLASS_HAS_A_BASE()
public:
    Player(const std::string& name, int health);
    FE_METHOD()
        void update(float deltaTime) override;
    FE_METHOD()
        void render() override;
    FE_METHOD()
        void takeDamage(int damage);

private:
    FE_PROPERTY()
        int health;
};

// Template specialization for Entity<Physics::Vector2>
template<>
class FrogmanEngine::Entity<FrogmanEngine::Physics::Vector2> {
    FE_CLASS()
public:
    Entity(const std::string& name);
    FE_METHOD()
        void update(float deltaTime);
    FE_METHOD()
        void render();
    FE_METHOD()
        FrogmanEngine::Physics::Vector2 getComponent();
    FE_METHOD()
        void addComponent(const FrogmanEngine::Physics::Vector2& component);

private:
    FE_PROPERTY()
        std::string name;
    FE_PROPERTY()
        std::vector<FrogmanEngine::Physics::Vector2> components;
};

// Inline function for testing inline behavior
inline void inlineTestFunction() {
    std::cout << "This is an inline test function in game engine." << std::endl;
}

// Multiple inheritance example for complex objects
class ComplexGameObject : public GameObject, public FrogmanEngine::Rendering::Renderer {
    FE_CLASS_HAS_A_BASE()
public:
    ComplexGameObject(const std::string& name);
    FE_METHOD()
        void complexUpdate();
    FE_METHOD()
        void render() override;
};

#endif // FROGMAN_ENGINE_HEADER_H
