#ifndef SGAME_UTIL_H_H
#define SGAME_UTIL_H_H

#include <string>
#include <json-c/json.h>

/*bool AlmostEqualRelative(float A, float B,
                         float maxRelDiff = FLT_EPSILON)
{
    // Calculate the difference.
    float diff = fabs(A - B);
    A = fabs(A);
    B = fabs(B);
    // Find the largest
    float largest = (B > A) ? B : A;

    if (diff <= largest * maxRelDiff)
        return true;
    return false;
}*/

#define FLOAT_EQUAL(a, b) ( fabs(a - b) <= ((fabs(b) > fabs(a)) ? fabs(b) : fabs(a)) * 0.001 ? true : false )

namespace util {

    char *FileToString(const std::string &path);

}

json_object* Json_GetObjectProp(json_object* object, const char* property);
json_object* Json_GetObjectProp_OfType(json_object* object, const char* property, json_type type);
int Json_GetObjectProp_Int(json_object* object, const char* property, int d);
int Json_GetObjectProp_Bool(json_object* object, const char* property, int d);
float Json_GetObjectProp_Float(json_object* object, const char* property, float d);
const char* Json_GetObjectProp_String(json_object* object, const char* property, const char* d);
json_object* Json_GetObjectProp_Array(json_object* object, const char* property, json_object* d);

#endif //SGAME_UTIL_H_H
