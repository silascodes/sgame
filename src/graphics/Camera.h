#ifndef SGAME_CAMERA_H
#define SGAME_CAMERA_H

#include "util/Vector2.h"

namespace graphics {

    class Camera {
        public:
            Camera();

            void SetPosition(const util::Vector2 &pos);
            util::Vector2 GetPosition();

        private:
            util::Vector2 position;
    };

}

#endif //SGAME_CAMERA_H
