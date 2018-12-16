/**
 * @file CGDJson.h
 * @brief Declaration file of class CGDJson.
 *
 * This file includes the declaration of class CGDCGD, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef JSONCOREOPERATION_H
#define JSONCOREOPERATION_H
#ifndef __cplusplus
#   error ERROR:This file requires C++ compliation (use a .cpp suffix)
#endif

#include <string>

#include "rapidjson/document.h"


class HandleJson;

/**
 *JsonCoreOperation
 *
 *define class of CGDJson
*/
class JsonCoreOperation
{
public:
    JsonCoreOperation() {}
    virtual ~JsonCoreOperation() {}

    bool read(const char* str, const char* indentify, std::string& result);
    bool write(const char* fileName, const char* jsonStr);
    bool parse(const char* jsonStr,HandleJson* handler);
    bool check(const char* jsonStr, std::string &errorStr);
    bool createKey(rapidjson::Document& document, const char* key, const char* value);
    bool createKey(rapidjson::Document& document, const char* key, int value);
    bool createKey(rapidjson::Document& document, const char* key, double value);
    bool createKey(rapidjson::Document& document, const char* key, bool value);
    bool queryKey(rapidjson::Document& document, const char* key);
    bool deleteKey(rapidjson::Document& document, const char* key);

private:
    unsigned int errorLineNumber(const char* jsonStr,unsigned int offSet);
    bool checkKey(const char* key);

private:
    enum JSonType
    {
        Type_Null = 0,
        Type_False,
        Type_True,
        Type_Object,
        Type_Array,
        Type_String,
        Type_Number
    };
};
#endif // JSONCOREOPERATION_H
/* EOF */
