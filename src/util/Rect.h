#ifndef SGAME_RECT_H
#define SGAME_RECT_H

#include <util/Vector2.h>

namespace util {

    class Rect {
        public:
            Rect() {
                this->x = 0;
                this->y = 0;
                this->width = 0;
                this->height = 0;
            }

            Rect(float x, float y, float width, float height) {
                this->x = x;
                this->y = y;
                this->width = width;
                this->height = height;
            }

            bool OverlapsRect(const Rect &box) const {
                if(
                    box.ContainsPoint(util::Vector2(this->x, this->y)) ||
                    box.ContainsPoint(util::Vector2(this->x + this->width, this->y)) ||
                    box.ContainsPoint(util::Vector2(this->x, this->y + this->height)) ||
                    box.ContainsPoint(util::Vector2(this->x + this->width, this->y + this->height)) ||
                    this->ContainsPoint(util::Vector2(box.x, box.y)) ||
                    this->ContainsPoint(util::Vector2(box.x + box.width, box.y)) ||
                    this->ContainsPoint(util::Vector2(box.x, box.y + box.height)) ||
                    this->ContainsPoint(util::Vector2(box.x + box.width, box.y + box.height))
                    ) {
                    return true;
                }
                return false;
            }

            bool ContainsPoint(const util::Vector2 &point) const {
                if(point.x >= this->x && point.x <= this->x + this->width && point.y >= this->y && point.y <= this->y + this->height) {
                    return true;
                }
                return false;
            }

            float x, y;
            float width, height;
    };

}

#endif //SGAME_RECT_H
