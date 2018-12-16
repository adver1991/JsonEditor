#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

#include "rapidjson/reader.h"
#include "rapidjson/schema.h"
#include "rapidjson/error/en.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/pointer.h"

#include "HandleJson.h"
#include "JsonCoreOperation.h"


#define IS_EQUAL 0

bool JsonCoreOperation::read(const char* str, const char* indentify, std::string& result)
{
    if((NULL == str) || (NULL == indentify)){
        return false;
    }
    if(IS_EQUAL == std::strcmp(indentify,"File")){
        std::fstream in(str);
        if(!in){
           return false;
        }
        else{
            in.seekg(0,std::ios::end);
            result.resize(in.tellg());
            in.seekg(0,std::ios::beg);
            in.read(&result[0],result.size());
            in.close();
            return true;
        }
    }
    else if(IS_EQUAL == std::strcmp(indentify,"String")){
        result = str;
    }
    else{
        return false;
    }
    return true;
}

bool JsonCoreOperation::write(const char* fileName, const char* jsonStr)
{
    if((NULL == fileName) || (NULL == jsonStr)){
        return false;
    }
    std::ofstream outFile;
    outFile.open(fileName);
    if(outFile.is_open()){
        outFile<<jsonStr;
    }
    else{
        return false;
    }
    outFile.close();
    return true;
}

bool JsonCoreOperation::parse(const char* jsonStr, HandleJson* handler)
{
    if((NULL == jsonStr) || (NULL == handler)){
        return false;
    }
    rapidjson::StringStream s(jsonStr);
    rapidjson::Reader reader;
    reader.Parse(s,*(handler));
    return true;
}

bool JsonCoreOperation::check(const char* jsonStr, std::string &errorStr)
{
    if(IS_EQUAL == std::strcmp(jsonStr,"null")){
        return false;
    }

    std::string tempErrorLine;
    std::stringstream temp;

    rapidjson::Document document;
    rapidjson::SchemaDocument sd(document);
    rapidjson::SchemaValidator validator(sd);
    rapidjson::Reader reader;
    rapidjson::StringStream ss(jsonStr);
    validator.Reset();
    if(!reader.Parse(ss,validator)
        && reader.GetParseErrorCode() != rapidjson::kParseErrorTermination){
        unsigned int errorNumber = errorLineNumber(jsonStr,
            static_cast<unsigned>(reader.GetErrorOffset()));
       temp <<errorNumber;
       temp >> tempErrorLine;
       errorStr = "Error: Parse error on line " + tempErrorLine + " : " +GetParseError_En(reader.GetParseErrorCode());
       return false;
    }
    else{
        return true;
    }
}

bool JsonCoreOperation::createKey(rapidjson::Document& document, const char* key, const char* value)
{
    if((NULL == key) || (NULL ==value)){
        return false;
    }
            if(false == checkKey(key)){
            return false;
    }
    if(IS_EQUAL == std::strcmp(value,"null")){
        rapidjson::Pointer(key).Erase(document);
        rapidjson::Pointer(key).Create(document);
    }
    else{
        rapidjson::Pointer(key).Set(document,value);
    }
    return true;
}

bool JsonCoreOperation::createKey(rapidjson::Document& document, const char* key, int value)
{
    if(NULL == key){
        return false;
    }
    if(false == checkKey(key)){
        return false;
    }
    rapidjson::Pointer(key).Set(document,value);
    return true;
}


bool JsonCoreOperation::createKey(rapidjson::Document& document, const char* key, double value)
{
    if(NULL == key){
        return false;
    }
    if(false == checkKey(key)){
        return false;
    }
    rapidjson::Pointer(key).Set(document,value);
    return true;
}

bool JsonCoreOperation::createKey(rapidjson::Document& document, const char* key, bool value)
{
    if(NULL == key){
        return false;
    }
    if(false == checkKey(key)){
        return false;
    }
    rapidjson::Pointer(key).Set(document,value);
    return true;
}

bool JsonCoreOperation::queryKey(rapidjson::Document& document, const char* key)
{
    if(false == checkKey(key)){
        return false;
    }
    if(rapidjson::Value* value = rapidjson::Pointer(key).Get(document)){
        std::cout<<key<<" : ";
        switch(value->GetType()){
        case Type_Null:
            {
                std::cout<<"NULL"<<std::endl;
            }
            break;
        case Type_False:
            {
                std::cout<<"False"<<std::endl;
            }
            break;
        case Type_True:
            {
                std::cout<<"True"<<std::endl;
            }
            break;
        case Type_Object:
            {
                std::cout<<"Object{}"<<std::endl;
            }
            break;
        case Type_Array:
            {
                std::cout<<"Array[]"<<std::endl;
            }
            break;
        case Type_String:
            {
                std::cout<<value->GetString()<<std::endl;
            }
            break;
        case Type_Number:
            {
                std::cout<<value->GetDouble()<<std::endl;
            }
            break;
        default:
            {
                std::cout<<"Error"<<std::endl;
            }
            break;
        }
        return true;
    }
    else{
        return false;
    }
}

bool JsonCoreOperation::deleteKey(rapidjson::Document& document, const char* key)
{
    if(NULL == key){
        return false;
    }
    if(false == checkKey(key)){
        return false;
    }
    if(false == (rapidjson::Pointer(key).Erase(document))){
        return false;
    }
    return true;
}

unsigned int JsonCoreOperation::errorLineNumber(const char* json, unsigned int offSet)
{
    if((NULL == json) || (0 == offSet)){
        return false;
    }
    unsigned int lineNumber = 0;
    unsigned int strNumber = 0;
    std::string str="";
    std::ofstream outFile;
    outFile.open("TempForCheck.json");
    if(outFile.is_open()){
        outFile<<json;
    }
    outFile.close();
    std::ifstream in("TempForCheck.json");
    while (strNumber < offSet) {
        getline(in,str);
        unsigned int tempNumber = (unsigned int)str.length()+1;
        strNumber += tempNumber;
        ++lineNumber;
    }
    std::remove("TempForCheck.json");
    return lineNumber;
}

bool JsonCoreOperation::checkKey(const char* key)
{
    if(NULL == key){
        return false;
    }
    if(rapidjson::Pointer(key).IsValid()){
        return true;
    }
    else{
        return false;
    }
}

/* EOF */
