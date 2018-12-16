#include <QTreeView>
#include <QStandardItemModel>
#include <QObject>
#include <QStringList>
#include <QString>

#include "TreeView.h"


TreeView::TreeView()
{
    m_keyStr = "";
    m_tree = new QTreeView();
    m_tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_model = new QStandardItemModel(m_tree);
    m_model->setHorizontalHeaderLabels(QStringList() << "Json Tree");
}

TreeView::~TreeView()
{
    m_Parentlist.clear();
    m_Childlist.clear();
}

void TreeView::createParentTree(std::string str)
{
    std::string tempStr;
    if (m_keyStr != "") {
        tempStr = m_keyStr + " : " + str;
    }
    else {
        tempStr = str;
    }

    QString s = QString::fromStdString(tempStr);
    QStandardItem* temp = new QStandardItem(s);

    if(!m_Parentlist.isEmpty()) {
        m_Parentlist.last()->appendRow(temp);
    }
    m_Parentlist.append(temp);

    m_keyStr = "";
}

void TreeView::createChildTree(std::string str)
{
    std::string tempStr;
    if (m_keyStr != "") {
        tempStr = m_keyStr + " : " + str;
    }
    else {
        tempStr = str;
    }

    QString s = QString::fromStdString(tempStr);
    m_Childlist.append(new QStandardItem(s));
    m_keyStr = "";
}

bool TreeView::setKeyTree()
{
    if (m_Parentlist.isEmpty() || m_Childlist.isEmpty()) {
        return false;
    }

    m_Parentlist.last()->setChild(m_Parentlist.last()->rowCount() ,0,m_Childlist.last());
    return true;
}

bool TreeView::setValueTree()
{
    if (m_Parentlist.isEmpty() || m_Childlist.isEmpty()) {
        return false;
    }

    m_Parentlist.last()->setChild(m_Parentlist.last()->rowCount(),0,m_Childlist.last());
    return true;
}

void TreeView::show()
{
    m_model->appendRow(m_Parentlist.first());
    m_tree->setModel(m_model);
    m_tree->show();
}

void TreeView::takeLast()
{
    if(m_Parentlist.size() == 1) {
        return;
    }
    m_Parentlist.takeLast();
}

void TreeView::setKetStr(std::string str)
{
    m_keyStr = str;
}

QTreeView* TreeView::getTree()
{
    m_model->appendRow(m_Parentlist.first());
    m_tree->setModel(m_model);
    return m_tree;
}

void TreeView::clear()
{
    for (int index =0 ; index < m_Parentlist.size(); index++)
    {
        delete m_Parentlist[index];
    }

    for (int index =0 ; index < m_Childlist.size(); index++)
    {
        delete m_Childlist[index];
    }

}

/* EOF */


