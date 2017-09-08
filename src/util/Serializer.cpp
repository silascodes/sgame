#include <string>
#include "Serializer.h"
#include "util/ISerializable.h"

namespace util {

    void Serializer::Init(const std::string &file) {
        this->stream.open(file);
    }

    void Serializer::Shutdown() {
        this->stream.flush();
        this->stream.close();
    }

    void Serializer::Serialize(ISerializable *object) {
        object->Serialize(this);
    }

    void Serializer::WriteString(const std::string &str) {
        this->stream << str << '\0';
    }

}
