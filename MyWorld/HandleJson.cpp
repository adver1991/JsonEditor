#include <iostream>
#include <stdio.h>
#include <string>
#include <sstream>

#include "HandleJson.h"


HandleJson::HandleJson()
{
    m_treeView  = new TreeView();
}

HandleJson::~HandleJson()
{
    delete m_treeView; 
//    m_treeView->clear();
}

bool HandleJson::Null()
{
    m_treeView->createChildTree("null");
    return true;
}

bool HandleJson::Bool(bool b)
{
    if (true == b)
    {
        m_treeView->createChildTree("true");
    }
    else {
        m_treeView->createChildTree("false");
    }
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::Int(int i)
{
    std::stringstream tempInt;
    std::string tempIntStr;
    tempInt << i;
    tempInt >> tempIntStr;
    m_treeView->createChildTree(tempIntStr);
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::Uint(unsigned u){
    std::stringstream tempInt;
    std::string tempIntStr;
    tempInt << u;
    tempInt >> tempIntStr;
    m_treeView->createChildTree(tempIntStr);
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::Int64(int64_t i)
{
    std::stringstream tempInt;
    std::string tempIntStr;
    tempInt << i;
    tempInt >> tempIntStr;
    m_treeView->createChildTree(tempIntStr);
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::Uint64(uint64_t u)
{
    std::stringstream tempInt;
    std::string tempIntStr;
    tempInt << u;
    tempInt >> tempIntStr;
    m_treeView->createChildTree(tempIntStr);
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::Double(double d)
{
    std::stringstream tempDouble;
    std::string tempDoubleStr;
    tempDouble << d;
    tempDouble >> tempDoubleStr;
    m_treeView->createChildTree(tempDoubleStr);
    m_treeView->setValueTree();
    return true;
}

bool HandleJson::String(const char* str, rapidjson::SizeType length, bool copy)
{
    m_treeView->createChildTree(str);
    m_treeView->setValueTree();
    (void)(length);
     (void)(copy);
    return true;
}

bool HandleJson::StartObject()
{
    m_treeView->createParentTree("Object{}");
    return true;
}

bool HandleJson::Key(const char* str, rapidjson::SizeType length, bool copy)
{
    m_treeView->setKetStr(str);
    (void)(length);
     (void)(copy);
    return true;
}

bool HandleJson::EndObject(rapidjson::SizeType memberCount)
{
    m_treeView->takeLast();
    (void)(memberCount);
    return true;
}

bool HandleJson::StartArray()
{
    m_treeView->createParentTree("[]");
    return true;
}

bool HandleJson::EndArray(rapidjson::SizeType elementCount)
{
    m_treeView->takeLast();
    (void)(elementCount);
    return true;
}

bool HandleJson::RawNumber(const char *str, rapidjson::SizeType length, bool copy)
{
    (void)(str);
    (void)(length);
    (void)(copy);
    return true;
}

void HandleJson::show()
{
    m_treeView->show();
}

QTreeView* HandleJson::getTree()
{
    return m_treeView->getTree();
}

/* EOF */

