/**
 * @file World.h
 * @brief Declaration file of class World.
 *
 * This file includes the declaration of class CGDCGD, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef WORLD_H
#define WORLD_H
#ifndef __cplusplus
#   error ERROR:This file requires C++ compliation (use a .cpp suffix)
#endif

#include <QMainWindow>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrintPreviewDialog>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/writer.h"

class QMdiArea;
class QAction;
class QMenu;
class QComboBox;
class Child;
class QMdiSubWindow;
class QSignalMapper;
class QTreeView;

class World : public QMainWindow
{
    Q_OBJECT

public:
    World(QWidget *parent = 0);
    virtual ~World();

protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void updateWindowMenu(); //更新窗口菜单
    void about();
    void fileNew();
    void fileOpen();
    void fileSave();
    void fileSaveAs();
    void updateMenus();
    void setActiveSubWindow(QWidget* window);
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void textSize(const QString &p);
    void filePrint();
    void filePrintPreview();
    void printPreview(QPrinter *printer);

    void prettyJson();
    void compressJson();
    void modifyJson();
    void deleteJson();
    void queryJson();
    void checkJson();
    void goToLineJson();
    void jsonTree();

private:
    Child* createChild();
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    Child* activeChild();
    QMdiSubWindow* findChild(const QString &fileName);

private:
    QMdiArea* m_mdiArea;
    QSignalMapper* m_windowMapper;

private:
    QMenu* m_editMenu;
    QAction* m_undoAct;
    QAction* m_redoAct;
    QAction* m_cutAct;
    QAction* m_copyAct;
    QAction* m_pastAct;

private:
    QMenu* m_fileMenu;
    QAction* m_newAct;
    QAction* m_openAct;
    QAction* m_saveAct;
    QAction* m_saveAsAct;
    QAction* m_printAct;
    QAction* m_printPreviewAct;
    QAction* m_exitAct;

private:
    QMenu* m_windowMenu;
    QAction* m_closeAct;
    QAction* m_closeAllAct;
    QAction* m_tileAct;
    QAction* m_cascadeAct;
    QAction* m_nextAct;
    QAction* m_previousAct;
    QAction* m_separatorAct;

private:
    QMenu* m_helpMenu;
    QAction* m_aboutAct;
    QAction* m_aboutQtAct;

private:
    QMenu* m_formatMenu;
    QAction* m_prettyJsonAct;
    QAction* m_compressJsonAct;
    QAction* m_checkJsonAct;
    QAction* m_jsonTreeAct;

private:
    QToolBar* m_fileToolBar;
    QToolBar* m_editToolBar;
    QToolBar* m_formatToolBar;
    QComboBox* m_comboSize;   //字号选择框

private:
    QAction* m_modifyJsonAct;
    QAction* m_deleteJsonAct;
    QAction* m_queryJsonAct;
    QAction* m_goToLineJsonAct;
};

#endif // WORLD_H
