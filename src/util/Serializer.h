#ifndef SGAME_SERIALIZER_H
#define SGAME_SERIALIZER_H

#include <string>
#include <fstream>

namespace util {

    class ISerializable;

    class Serializer {
        public:
            void Init(const std::string &file);
            void Shutdown();

            void Serialize(ISerializable *object);

            void WriteString(const std::string &str);

        private:
            std::ofstream stream;
    };

}

#endif //SGAME_SERIALIZER_H
