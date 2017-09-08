#include <string>
#include "Deserializer.h"
#include "util/ISerializable.h"

namespace util {

    void Deserializer::Init(const std::string &file) {
        this->stream.open(file);
    }

    void Deserializer::Shutdown() {
        this->stream.close();
    }

    void Deserializer::Deserialize(ISerializable *object) {
        object->Deserialize(this);
    }

    void Deserializer::ReadString(std::string &str) {
        this->stream >> str;
    }

}
