#ifndef SGAME_DESERIALIZER_H
#define SGAME_DESERIALIZER_H

#include <string>
#include <fstream>

namespace util {

    class ISerializable;

    class Deserializer {
        public:
            void Init(const std::string &file);
            void Shutdown();

            void Deserialize(ISerializable *object);

            void ReadString(std::string &str);

        private:
            std::ifstream stream;
    };

}

#endif //SGAME_DESERIALIZER_H
