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
namespace FrogmanEngine 
{
    namespace Physics 
    {
        class Vector2 
        {
            FE_CLASS()
        public:
            FE_PROPERTY()
                float x;

            FE_PROPERTY()
                float y;

            Vector2(float _x = 0, float _y = 0)
                : x(_x), y(_y) 
            {}

            float length() const { return std::sqrt(x * x + y * y); }

            Vector2 normalized() const 
            {
                float len = length();
                return len > 0 ? Vector2(x / len, y / len) : Vector2(0, 0);
            }
        };
    }

    // This reflection macro code is incorrectly used.
    enum class GameState
    {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        GAME_OVER
    };

    // Template class for game entities
    template<typename T>
    class Entity 
    {
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

    namespace Rendering 
    {
        class Renderer
        {
            FE_CLASS()
        public:
            FE_METHOD()
                void render();
        };
    }
}
//
//// Function prototypes
//FE_STATIC_METHOD()
//void initializeGame();
//
//FE_STATIC_METHOD()
//void updateGame(float deltaTime);
//
//FE_STATIC_METHOD()
//void renderGame();
//
//FE_STATIC_METHOD()
//void shutdownGame();
//
//// Classes for game objects
//class GameObject {
//    FE_CLASS()
//public:
//    GameObject(const std::string& name);
//    FE_METHOD()
//        virtual void update(float deltaTime);
//    FE_METHOD()
//        virtual void render();
//    virtual ~GameObject() = default;
//
//private:
//    FE_PROPERTY()
//        std::string name;
//};
//
//class Player : public GameObject {
//    FE_CLASS_HAS_A_BASE();
//    FE_CLASS();
//public:
//    Player(const std::string& name, int health);
//    FE_METHOD()
//        void update(float deltaTime) override;
//    FE_METHOD()
//        void render() override;
//    FE_METHOD()
//        void takeDamage(int damage);
//
//private:
//    FE_PROPERTY()
//        int health;
//};
//
//// Template specialization for Entity<Physics::Vector2>
//template<>
//class FrogmanEngine::Entity<FrogmanEngine::Physics::Vector2> {
//    FE_CLASS();
//public:
//    Entity(const std::string& name);
//    FE_METHOD()
//        void update(float deltaTime);
//    FE_METHOD()
//        void render();
//    FE_METHOD()
//        FrogmanEngine::Physics::Vector2 getComponent();
//    FE_METHOD()
//        void addComponent(const FrogmanEngine::Physics::Vector2& component);
//
//private:
//    FE_PROPERTY()
//        std::string name;
//    FE_PROPERTY()
//        std::vector<FrogmanEngine::Physics::Vector2> components;
//};
//
//// Inline function for testing inline behavior
//inline void inlineTestFunction() {
//    std::cout << "This is an inline test function in game engine." << std::endl;
//}
//
//// Multiple inheritance example for complex objects
//class ComplexGameObject : public GameObject {
//    FE_CLASS_HAS_A_BASE();
//    FE_CLASS();
//public:
//    ComplexGameObject(const std::string& name);
//    FE_METHOD()
//        void complexUpdate();
//    FE_METHOD()
//        void render() override;
//};

#endif // FROGMAN_ENGINE_HEADER_H




//#ifndef FROGMAN_ENGINE_ADVANCED_H
//#define FROGMAN_ENGINE_ADVANCED_H
//
//#include <iostream>
//#include <vector>
//#include <string>
//#include <memory>
//#include <cmath>
//#include <map>
//#include <set>
//#include <unordered_map>
//#include <algorithm>
//#include <mutex>
//#include <thread>
//
///* The MIT License
//// Copyright and Permission Notice
//*/
//
//#define MAX_ENTITIES 1000
//#define LOG(message) std::cout << "LOG: " << message << std::endl
//#define DEBUG 1
//
//namespace FrogmanEngine {
//    // Forward declarations
//    class Engine;
//    namespace Physics {
//        class Vector2;
//        class Vector3;
//    }
//    namespace Rendering {
//        class Renderer;
//        class Texture;
//    }
//    namespace Audio {
//        class Sound;
//        class Music;
//    }
//
//    class Engine {
//        FE_CLASS()
//    public:
//        Engine();
//        ~Engine();
//        FE_METHOD()
//            void initialize();
//        FE_METHOD()
//            void update(float deltaTime);
//        FE_METHOD()
//            void render();
//        FE_METHOD()
//            void shutdown();
//
//    private:
//        FE_PROPERTY()
//            bool isRunning;
//        FE_PROPERTY()
//            std::unordered_map<std::string, std::shared_ptr<GameObject>> gameObjects;
//    };
//
//    namespace Physics {
//        class Vector2 {
//            FE_CLASS()
//        public:
//            FE_PROPERTY()
//                float x;
//            FE_PROPERTY()
//                float y;
//
//            Vector2(float _x = 0, float _y = 0) : x(_x), y(_y) {}
//            float length() const { return std::sqrt(x * x + y * y); }
//            Vector2 normalized() const { float len = length(); return len > 0 ? Vector2(x / len, y / len) : Vector2(0, 0); }
//
//            FE_METHOD()
//                static Vector2 add(const Vector2& v1, const Vector2& v2) { return Vector2(v1.x + v2.x, v1.y + v2.y); }
//        };
//
//        class Vector3 : public Vector2 {
//            FE_CLASS_HAS_A_BASE();
//            FE_CLASS()
//        public:
//            FE_PROPERTY()
//                float z;
//
//            Vector3(float _x = 0, float _y = 0, float _z = 0) : Vector2(_x, _y), z(_z) {}
//            float length() const { return std::sqrt(x * x + y * y + z * z); }
//            Vector3 normalized() const { float len = length(); return len > 0 ? Vector3(x / len, y / len, z / len) : Vector3(0, 0, 0); }
//
//            FE_METHOD()
//                static Vector3 add(const Vector3& v1, const Vector3& v2) { return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
//        };
//    }
//
//    enum class GameState {
//        MAIN_MENU,
//        PLAYING,
//        PAUSED,
//        GAME_OVER
//    };
//
//    template<typename T>
//    class Entity {
//        FE_CLASS()
//    public:
//        Entity(const std::string& name) : name(name) {}
//        FE_METHOD()
//            void update(float deltaTime);
//        FE_METHOD()
//            void render();
//        FE_METHOD()
//            T getComponent();
//        FE_METHOD()
//            void addComponent(const T& component);
//
//    private:
//        FE_PROPERTY()
//            std::string name;
//        FE_PROPERTY()
//            std::vector<T> components;
//    };
//
//    namespace Rendering {
//        class Renderer {
//            FE_CLASS()
//        public:
//            FE_METHOD()
//                void render();
//        };
//
//        class Texture {
//            FE_CLASS()
//        public:
//            Texture(const std::string& filePath);
//            FE_METHOD()
//                void load();
//            FE_METHOD()
//                void bind();
//            FE_METHOD()
//                void unbind();
//
//        private:
//            FE_PROPERTY()
//                std::string filePath;
//        };
//    }
//
//    namespace Audio {
//        class Sound {
//            FE_CLASS()
//        public:
//            Sound(const std::string& filePath);
//            FE_METHOD()
//                void play();
//            FE_METHOD()
//                void stop();
//
//        private:
//            FE_PROPERTY()
//                std::string filePath;
//        };
//
//        class Music : public Sound {
//            FE_CLASS_HAS_A_BASE();
//            FE_CLASS()
//        public:
//            Music(const std::string& filePath);
//            FE_METHOD()
//                void play() override;
//            FE_METHOD()
//                void pause();
//            FE_METHOD()
//                void stop() override;
//        };
//    }
//}
//
//void initializeGame();
//void updateGame(float deltaTime);
//void renderGame();
//void shutdownGame();
//
//class GameObject {
//    FE_CLASS()
//public:
//    GameObject(const std::string& name);
//    virtual ~GameObject() = default;
//    FE_METHOD()
//        virtual void update(float deltaTime);
//    FE_METHOD()
//        virtual void render();
//
//private:
//    FE_PROPERTY()
//        std::string name;
//};
//
//class Player : public GameObject {
//    FE_CLASS_HAS_A_BASE();
//    FE_CLASS()
//public:
//    Player(const std::string& name, int health);
//    FE_METHOD()
//        void update(float deltaTime) override;
//    FE_METHOD()
//        void render() override;
//    FE_METHOD()
//        void takeDamage(int damage);
//
//private:
//    FE_PROPERTY()
//        int health;
//};
//
//template<>
//class FrogmanEngine::Entity<FrogmanEngine::Physics::Vector2> {
//    FE_CLASS()
//public:
//    Entity(const std::string& name);
//    FE_METHOD()
//        void update(float deltaTime);
//    FE_METHOD()
//        void render();
//    FE_METHOD()
//        FrogmanEngine::Physics::Vector2 getComponent();
//    FE_METHOD()
//        void addComponent(const FrogmanEngine::Physics::Vector2& component);
//
//private:
//    FE_PROPERTY()
//        std::string name;
//    FE_PROPERTY()
//        std::vector<FrogmanEngine::Physics::Vector2> components;
//};
//
//template<>
//class FrogmanEngine::Entity<FrogmanEngine::Physics::Vector3> {
//    FE_CLASS()
//public:
//    Entity(const std::string& name);
//    FE_METHOD()
//        void update(float deltaTime);
//    FE_METHOD()
//        void render();
//    FE_METHOD()
//        FrogmanEngine::Physics::Vector3 getComponent();
//    FE_METHOD()
//        void addComponent(const FrogmanEngine::Physics::Vector3& component);
//
//private:
//    FE_PROPERTY()
//        std::string name;
//    FE_PROPERTY()
//        std::vector<FrogmanEngine::Physics::Vector3> components;
//};
//
//inline void inlineTestFunction() {
//    std::cout << "This is an inline test function in game engine." << std::endl;
//}
//
//class ComplexGameObject : public GameObject {
//    FE_CLASS_HAS_A_BASE();
//    FE_CLASS()
//public:
//    ComplexGameObject(const std::string& name);
//    FE_METHOD()
//        void complexUpdate();
//    FE_METHOD()
//        void render() override;
//};
//
//class NPC : public GameObject {
//    FE_CLASS_HAS_A_BASE();
//    FE_CLASS()
//public:
//    NPC(const std::string& name);
//    FE_METHOD()
//        void update(float deltaTime) override;
//    FE_METHOD()
//        void render() override;
//    FE_METHOD()
//        void interact();
//
//private:
//    FE_PROPERTY()
//        std::string dialog;
//    FE_PROPERTY()
//        bool isFriendly;
//};
//
//#endif // FROGMAN_ENGINE_ADVANCED_H
