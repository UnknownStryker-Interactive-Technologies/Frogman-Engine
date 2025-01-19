#ifndef ADVANCED_FROGMAN_ENGINE_HEADER_H
#define ADVANCED_FROGMAN_ENGINE_HEADER_H

/* The Apache License 2.0
   Copyright (c) 2025 by Frogman Dev Team, all rights reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software distributed
   under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied. See the License for the
   specific language governing permissions and limitations under the License.
*/

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <cmath>
#include <map>
#include <algorithm>
#include <chrono>

// Macros
#define MAX_ENTITIES 2000
#define LOG_ERROR(message) std::cerr << "ERROR: " << message << std::endl
#define LOG_DEBUG(message) std::cout << "DEBUG: " << message << std::endl

// Namespaces
namespace FrogmanEngine
{

    namespace Math
    {

        class Vector3
        {
            FE_CLASS(Vector3)
        public:
            FE_PROPERTY(x)
                float x;

           FE_PROPERTY(y)
                float y;

            FE_PROPERTY(z)
                float z;

            Vector3() : x(0), y(0), z(0) {}

            Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

            float length() const
            {
                return std::sqrt(x * x + y * y + z * z);
            }

            Vector3 normalized() const
            {
                float len = length();
                return len > 0 ? Vector3(x / len, y / len, z / len) : Vector3(0, 0, 0);
            }

            static float dot(const Vector3& v1, const Vector3& v2)
            {
                return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
            }

            static Vector3 cross(const Vector3& v1, const Vector3& v2)
            {
                return Vector3(
                    v1.y * v2.z - v1.z * v2.y,
                    v1.z * v2.x - v1.x * v2.z,
                    v1.x * v2.y - v1.y * v2.x
                );
            }
        };

    } // namespace Math

    enum class GameState
    {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        GAME_OVER,
        VICTORY
    };

    template<typename T>
    class Entity
    {
        FE_CLASS(Entity)
    public:
        Entity() : name("Unnamed Entity") {}

        Entity(const std::string& name) : name(name) {}

       FE_METHOD(update, void(float));
        virtual void update([[maybe_unused]] float deltaTime) {}

       FE_METHOD(render, void(void));
        virtual void render() {}

       // FE_METHOD(getComponent, T(void));
        T getComponent() const { return T(); }

        FE_METHOD(addComponent, void(const T&));
        void addComponent([[maybe_unused]] const T& component) {}

    protected:
      FE_PROPERTY(name);
        std::string name;

       FE_PROPERTY(components);
        std::vector<T> components;
    };

    class Player : public Entity<int>
    {
    public:
        Player() : Entity("Unnamed Player") {}

        Player(const std::string& playerName) : Entity(playerName) {}

        void update([[maybe_unused]] float deltaTime) override
        {
            // Custom update logic for player
        }

        void render() override
        {
            // Custom render logic for player
        }
    };

    namespace Rendering
    {

        class Renderer
        {
            FE_CLASS(Renderer)
        public:
            FE_METHOD(init, void(void));
            void init()
            {
                // Initialization code
            }

            FE_METHOD(render, void(void));
            void render()
            {
                // Render code
            }

            FE_METHOD(shutdown, void(void));
            void shutdown()
            {
                // Cleanup code
            }

            static Renderer& getInstance()
            {
                static Renderer instance;
                return instance;
            }

            Renderer() {} // Private constructor for singleton
        };

    } // namespace Rendering
    
    namespace Utility
    {

        class Timer
        {
            FE_CLASS(Timer)
        public:
            Timer() = default;
            ~Timer() = default;

            FE_METHOD(start, void(void));
            void start()
            {
                startTime = std::chrono::high_resolution_clock::now();
            }

            FE_METHOD(stop, void(void));
            void stop()
            {
                endTime = std::chrono::high_resolution_clock::now();
            }

            FE_METHOD(elapsed, double(void) const);
            double elapsed() const
            {
                return (double)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
            }

        private:
            std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
            std::chrono::time_point<std::chrono::high_resolution_clock> endTime;
        };

    } // namespace Utility

} // namespace FrogmanEngine

#endif // ADVANCED_FROGMAN_ENGINE_HEADER_H
