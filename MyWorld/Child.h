/**
 * @file child.h
 * @brief Declaration file of class Child.
 *
 * This file includes the declaration of class CGDCGD, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef CHILD_H
#define CHILD_H
#ifndef __cplusplus
#   error ERROR:This file requires C++ compliation (use a .cpp suffix)
#endif

#include <QPlainTextEdit>
#include <QPaintEvent>

#include <QSize>

class QTreeView;
class QWidget;
class QLineEdit;
class QPushButton;
class QVBoxLayout;
class QDialog;
class QResizeEvent;
class LineNumberArea;
class JsonCoreOperation;
class HandleJson;

class Child : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit Child(QWidget *parent=0);
    virtual ~Child();
    void newFile();
    QString userFirendlyCurrentFile();
    QString currentFile() {return curFile;}
    bool loadFile(const QString& fileName);
    bool save();
    bool saveAs();
    bool saveFile(QString fileName);
    bool maybeSave();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    void lineNumberAreaPaintEvent(QPaintEvent* event);
    int lineNumberAreaWidth();

    void setJsonContent();
    bool checkJsonStr();

    bool formatJsonStr();
    bool compressJsonStr();
    bool modifyJsonStr();
    bool deletJsonStr();
    bool queryJsonStr();
    bool goToLine();
    bool parseJson(HandleJson* handle);
    bool jsonTree();

    void showFindDlg();

public:
    std::string m_result;
    QString m_Qresult;
    QString curFile;

protected:
    void closeEvent(QCloseEvent *);
    void resizeEvent(QResizeEvent *e);

private slots:
    void documentWasModified();
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect&, int);
    void highlightCurrentLine();
    void showForwardFindText();
    void showBackWardFindText();

private:
    QString strippedName(const QString &fullFileName);
    void setCurrentFile(const QString &fileName);

private:
    bool m_isUntitled;
    JsonCoreOperation* m_operationJson;
    QWidget* m_lineNumberArea;

private:
    QLineEdit* m_findLineEdit;
    QDialog* m_findDlg;
    QPushButton* m_FindFBtn;
    QPushButton* m_FindBBtn;
    QVBoxLayout* m_layout;
    int m_findType;
    HandleJson* m_handle;

private:
    QTreeView* m_treeView;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea (Child* editor)
        :QWidget(editor)
    {
        m_editor = editor;
    }

    QSize sizeHint() const Q_DECL_OVERRIDE
    {
        return QSize(m_editor->lineNumberAreaWidth(),0);
    }

protected:
    void paintEvent(QPaintEvent* event) Q_DECL_OVERRIDE
    {
        m_editor->lineNumberAreaPaintEvent(event);
    }
private:
    Child* m_editor;
};

#endif // CHILD_H
/* EOF */
