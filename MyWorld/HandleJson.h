/**
 * @file HandlerJson.h
 * @brief Declaration file of class HandlerJson.
 *
 * This file includes the declaration of class HandlerJson, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef HandlerJson_H
#define HandlerJson_H
#ifndef __cplusplus
#   error ERROR:This file requires C++ compliation (use a .cpp suffix)
#endif

#include "rapidjson/reader.h"
#include "TreeView.h"


/**
 *HandlerJson
 *
 *define class of HandlerJson
*/

class HandleJson
{
public:
    HandleJson();
    virtual ~HandleJson();

    virtual bool StartObject();
    virtual bool Null();
    virtual bool StartArray();
    virtual bool Bool(bool b);
    virtual bool Int(int i);
    virtual bool Uint(unsigned u);
    virtual bool Int64(int64_t i);
    virtual bool Uint64(uint64_t u);
    virtual bool Double(double d);
    virtual bool EndObject(rapidjson::SizeType memberCount);
    virtual bool EndArray(rapidjson::SizeType elementCount);
    virtual bool String(const char* str, rapidjson::SizeType length, bool copy);
    virtual bool Key(const char* str, rapidjson::SizeType length, bool copy);
    virtual bool RawNumber(const char* str, rapidjson::SizeType length, bool copy);

    void show();
    QTreeView* getTree();
private:
    TreeView* m_treeView;
    QStandardItem* m_parent;
    QStandardItem* m_child;
};

#endif /* HandlerJson_H */
/* EOF */
