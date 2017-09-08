#ifndef SGAME_ISERIALIZABLE_H
#define SGAME_ISERIALIZABLE_H

#include <iostream>

namespace util {

    class Serializer;
    class Deserializer;

    class ISerializable {
        public:
            virtual ~ISerializable() {}

            virtual void Serialize(Serializer *serializer) = 0;
            virtual void Deserialize(Deserializer *deserializer) = 0;
    };

}

#endif //SGAME_ISERIALIZABLE_H
