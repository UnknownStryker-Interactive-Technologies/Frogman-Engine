#ifndef FROGMAN_ENGINE_HEADER_H
#define FROGMAN_ENGINE_HEADER_H
/*
The Apache License 2.0

Copyright (c) <year> by <author name>, all rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

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
            FE_CLASS(Vector2)
        public:
            FE_PROPERTY(x)
                float x;

            FE_PROPERTY(y)
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
        FE_CLASS(Entity)
    public:
        Entity(const std::string& name) : name(name) {}

        FE_METHOD(update, void (float))
            void update(float deltaTime) {}

        FE_METHOD(render, void (void))
            void render() {}

        FE_METHOD(getComponent, T (void))
            T getComponent() {}

        FE_METHOD(addComponent, void (const T&))
            void addComponent(const T& component) {}

    private:
        FE_PROPERTY(name)
            std::string name;
        FE_PROPERTY(components)
            std::vector<T> components;
    };

    namespace Rendering 
    {
        class Renderer
        {
            FE_CLASS(Renderer)
        public:
            FE_METHOD(render, void (void))
                void render() {}
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
