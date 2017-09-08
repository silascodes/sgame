#ifndef SGAME_VECTOR2_H
#define SGAME_VECTOR2_H

#include <Box2D/Box2D.h>

namespace util {

    class Vector2 {
        public:
            Vector2() {
                this->x = 0;
                this->y = 0;
            }

            Vector2(float x, float y) {
                this->x = x;
                this->y = y;
            }

            Vector2(b2Vec2 vec) {
                this->x = vec.x;
                this->y = vec.y;
            }

            b2Vec2 GetBox2D() {
                return b2Vec2(this->x, this->y);
            }

            float Magnitude() {
                return sqrt(this->x * this->x + this->y * this->y);
            }

            void Normalize() {
                float mag = this->Magnitude();
                this->x /= mag;
                this->y /= mag;
            }

            void operator=(float s) {
                this->x = s;
                this->y = s;
            }

            void operator=(const Vector2 &vec) {
                this->x = vec.x;
                this->y = vec.y;
            }

            Vector2 operator-() {
                return Vector2(-this->x, -this->y);
            }

            Vector2 operator-(float s) {
                return Vector2(this->x - s, this->y - s);
            }

            Vector2 operator-(const Vector2 &vec) {
                return Vector2(this->x - vec.x, this->y - vec.y);
            }

            void operator -=(const Vector2 &vec) {
                this->x -= vec.x;
                this->y -= vec.y;
            }

            Vector2 operator+(float s) {
                return Vector2(this->x + s, this->y + s);
            }

            Vector2 operator+(const Vector2 &vec) {
                return Vector2(this->x + vec.x, this->y + vec.y);
            }

            Vector2 operator*(float s) {
                return Vector2(this->x * s, this->y * s);
            }

            Vector2 operator*(const Vector2 &vec) {
                return Vector2(this->x * vec.x, this->y * vec.y);
            }

            float x, y;
    };

}

#endif //SGAME_VECTOR2_H
