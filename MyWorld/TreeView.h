/**
 * @file treeview.h
 * @brief Declaration file of class TreeView.
 *
 * This file includes the declaration of class CGDCGD, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef TREEVIEW_H
#define TREEVIEW_H

#include <string>
#include <QList>

class QTreeView;
class QStandardItemModel;
class QStandardItem;

class TreeView
{
public:
    TreeView();
    virtual ~TreeView();
    void createParentTree(std::string str);
    void createChildTree(std::string str);
    bool setKeyTree();
    bool setValueTree();
    void show();
    void takeLast();
    void setKetStr(std::string str);
    QTreeView* getTree();
    void clear();
private:
    std::string m_keyStr;
    QTreeView* m_tree;
    QStandardItemModel* m_model;
    QList<QStandardItem*> m_Parentlist;
    QList<QStandardItem*> m_Childlist;
};

#endif // TREEVIEW_H
/* EOF */
