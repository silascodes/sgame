#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <json-c/json.h>

namespace util {

    char *FileToString(const std::string &path) {
        FILE *fp = fopen(path.c_str(), "r");
        if (!fp) {
            return NULL;
        }

        fseek(fp, 0, SEEK_END);
        int flen = ftell(fp);
        fseek(fp, 0, SEEK_SET);

        char *str = (char *) malloc(flen + 1);
        if (!str) {
            fclose(fp);
            return NULL;
        }
        memset(str, 0, flen + 1);

        int read = 0;
        while (flen - read > 0 && !feof(fp) && !ferror(fp)) {
            read += fread(str, 1, flen - read, fp);
        }

        return str;
    }

}

json_object *Json_GetObjectProp(json_object *object, const char *property) {
    // Check object and property are valid pointers
    if (!object || !property) {
        return NULL;
    }

    // Check object is infact an object
    if (json_object_get_type(object) != json_type_object) {
        return NULL;
    }

    // Check each property of object to see if has the name and type we're looking for
    json_object_object_foreach(object, key, val) {
        if (strcmp(key, property) == 0) {
            return val;
        }
    }

    // Didn't find property of that name
    return NULL;
}

json_object *Json_GetObjectProp_OfType(json_object *object, const char *property, json_type type) {
    // Try and find the requested property, if found check its type
    json_object *prop = Json_GetObjectProp(object, property);
    if (prop && json_object_get_type(prop) == type) {
        return prop;
    }

    // Not found, or incorrect type
    return NULL;
}

int Json_GetObjectProp_Int(json_object *object, const char *property, int d) {
    json_object *prop = Json_GetObjectProp_OfType(object, property, json_type_int);
    if (!prop) {
        return d;
    }

    return json_object_get_int(prop);
}

int Json_GetObjectProp_Bool(json_object *object, const char *property, int d) {
    json_object *prop = Json_GetObjectProp_OfType(object, property, json_type_boolean);
    if (!prop) {
        return d;
    }

    return (int) json_object_get_boolean(prop);
}

float Json_GetObjectProp_Float(json_object *object, const char *property, float d) {
    json_object *prop = Json_GetObjectProp_OfType(object, property, json_type_double);
    if (!prop) {
        return d;
    }

    return (float) json_object_get_double(prop);
}

const char *Json_GetObjectProp_String(json_object *object, const char *property, const char *d) {
    json_object *prop = Json_GetObjectProp_OfType(object, property, json_type_string);
    if (!prop) {
        return d;
    }

    return json_object_get_string(prop);
}

json_object *Json_GetObjectProp_Array(json_object *object, const char *property, json_object *d) {
    json_object *prop = Json_GetObjectProp_OfType(object, property, json_type_array);
    if (!prop) {
        return d;
    }

    return prop;
}
