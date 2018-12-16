#include <QWidget>
#include <QtWidgets>
#include <QPrinter>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QTreeView>
#include <QDialog>
#include <QResizeEvent>

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/pointer.h"

#include "JsonCoreOperation.h"
#include "HandleJson.h"

#include "Child.h"

Child::Child(QWidget* parent)
    :QPlainTextEdit(parent)
{

    m_lineNumberArea = new LineNumberArea(this);
    m_operationJson = new JsonCoreOperation();
    m_treeView = NULL;
    m_isUntitled = true;

    setAttribute(Qt::WA_DeleteOnClose);
    setWordWrapMode(QTextOption::NoWrap);

    m_findDlg = new QDialog(this);
    m_findDlg->setWindowTitle("查找");
    m_findLineEdit = new QLineEdit(m_findDlg);
    m_FindFBtn = new QPushButton(tr("向前查找"),m_findDlg);
    m_FindBBtn = new QPushButton(tr("向后查找"),m_findDlg);
    m_layout = new QVBoxLayout(m_findDlg);
    m_layout->addWidget(m_findLineEdit);
    m_layout->addWidget(m_FindFBtn);
    m_layout->addWidget(m_FindBBtn);
    m_findType = 0;

    connect(m_FindFBtn,SIGNAL(clicked()), this, SLOT(showForwardFindText()));
    connect(m_FindBBtn, SIGNAL(clicked()), this, SLOT(showBackWardFindText()));

    //高亮+行号
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth(0);
}

Child::~Child()
{
    delete m_lineNumberArea;
    m_lineNumberArea = NULL;

    delete m_operationJson;
    m_operationJson = NULL;

    document()->clearUndoRedoStacks();
}

void Child::newFile()
{
    static int sequenceNumber = 1;
    m_isUntitled = true;
    curFile = tr("文档 %1").arg(sequenceNumber++);
    setWindowTitle(curFile + "[*]");
    connect(document(),SIGNAL(contentsChanged()),this,SLOT(documentWasModified()));
}

void Child::documentWasModified()
{
    setWindowModified(document()->isModified());
}

QString Child::userFirendlyCurrentFile()
{
    return strippedName(curFile);
}

QString Child::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void Child::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

bool Child::loadFile(const QString &fileName)
{
    if (!fileName.isEmpty()) {
        if (!QFile::exists(fileName)) {
             return false;
        }

        QFile file(fileName);
        if (!file.open(QFile::ReadOnly)) {
            return false;
        }

        QTextStream stream(&file);
        stream.setCodec(QTextCodec::codecForName("UTF-8"));
        this->setPlainText(stream.readAll());
    }

   setCurrentFile(fileName);
   setJsonContent();

   connect(document(), SIGNAL(contentsChanged()),this,SLOT(documentWasModified()));

   return true;
}

void Child::setCurrentFile(const QString &fileName)
{
    curFile = QFileInfo(fileName).canonicalFilePath();
    m_isUntitled = false;
    document()->setModified(false);
    setWindowModified(false);
    setWindowTitle(userFirendlyCurrentFile() + "[*]");
}

bool Child::save()
{

    if (false == checkJsonStr()) {
       return false;
    }

    if (m_isUntitled) {
        return saveAs();
    }
    else {
        return saveFile(curFile);
    }
}

bool Child::saveAs()
{
    if (false == checkJsonStr()) {
       return false;
    }

    QString fileName = QFileDialog::getSaveFileName(this, tr("另存为"),curFile,tr("All Files(*.*)"));

    if (fileName.isEmpty()) {
        return false;
    }

    return saveFile(fileName);
}

//写文件
bool Child::saveFile(QString fileName)
{

       QFile writer(fileName);
       if (writer.open(QFile::WriteOnly | QFile::Truncate)) {
          QTextStream out(&writer);
          out<<this->document()->toPlainText();
       }

       setCurrentFile(fileName);

       return true;
}

bool Child::maybeSave()
{
    if (!document()->isModified()) {
        return true;
    }

    QMessageBox::StandardButton ret;
    ret = QMessageBox::warning(this, tr("Word"), tr("文档 '%1' 已修改，保存?").arg(userFirendlyCurrentFile()),
             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    if (ret == QMessageBox::Save) {
        return save();
    }
    else if (ret == QMessageBox::Cancel) {
        return false;
    }

    return true;
}

void Child::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = this->textCursor();
    if (!cursor.hasSelection()) {
        cursor.select(QTextCursor::WordUnderCursor);
    }

    cursor.mergeCharFormat(format);

    this->mergeCurrentCharFormat(format);

}

int Child::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1,blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 3 + fontMetrics().width(QLatin1Char('9'))*digits;

    return space;
}

void Child::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy) {
         m_lineNumberArea->scroll(0, dy);
    }
    else {
        m_lineNumberArea->update(0, rect.y(),m_lineNumberArea->width(), rect.height());
    }

    if (rect.contains(viewport()->rect())) {
        updateLineNumberAreaWidth(0);
    }
}

void Child::updateLineNumberAreaWidth(int /*newBlockCount*/)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

void Child::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr= contentsRect();
    m_lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void Child::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(m_lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(-2,top,m_lineNumberArea->width(),fontMetrics().height(),Qt::AlignRight,number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}

void Child::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    QTextEdit::ExtraSelection selection;
    QColor lineColor = QColor(Qt::green).lighter(160);

    selection.format.setBackground(lineColor);
    selection.format.setProperty(QTextFormat::FullWidthSelection,true);
    selection.cursor = textCursor();
    selection.cursor.clearSelection();

    extraSelections.append(selection);
    setExtraSelections(extraSelections);
}

void Child::setJsonContent()
{
    m_result = this->document()->toPlainText().toStdString();
}

bool Child::checkJsonStr()
{
    setJsonContent();
    std::string test;
    if (true == m_operationJson->check(m_result.c_str(),test)) {

        return true;
    }

    QMessageBox::warning(this,tr("Parse Error"),tr(test.c_str()),QMessageBox::Ok);

    return false;
}

bool Child::formatJsonStr()
{
    if(false == checkJsonStr()) {
        return false;
    }

    rapidjson::Document d;
    d.Parse(m_result.c_str());
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    m_result = buffer.GetString();
    QString tempQResult = QString::fromStdString(m_result);
    document()->setPlainText(tempQResult);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

bool Child::compressJsonStr()
{
    if(false == checkJsonStr()) {
        return false;
    }

    document()->setPlainText(this->toPlainText().remove(QRegExp("\\s")));
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;

#if 0
    rapidjson::Document d;
    d.Parse(m_result.c_str());
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    m_result = buffer.GetString();
    QString tempQResult = QString::fromStdString(m_result);
    document()->setPlainText(tempQResult);
    connect(document(), SIGNAL(contentsChanged()),this,SLOT(documentWasModified()));
    return true;
#endif
}

bool Child::modifyJsonStr()
{
    if(false == checkJsonStr()) {
        return false;
    }

    QString keyStr;
    QString typeStr;
    QString valueStr;
    int valueInt = 0;
    double valueDouble = 0;
    bool valueBool = true;
    int selectIndex = 0;

    QStringList valueTypeItems;
    valueTypeItems.append(tr("String"));
    valueTypeItems.append(tr("Int"));
    valueTypeItems.append(tr("Double"));
    valueTypeItems.append(tr("True"));
     valueTypeItems.append(tr("False"));
    valueTypeItems.append(tr("Null"));

    bool ok= false;
    keyStr = QInputDialog::getText(this, tr("Input"), tr("Input key string"), QLineEdit::Normal, tr("/new"), &ok);
    if(!ok || keyStr.isEmpty()) {
        return false;
    }
    else{
        typeStr = QInputDialog::getItem(this, tr("Input"),tr("Select value type"), valueTypeItems,0,false,&ok);

        if(!ok || typeStr.isEmpty()) {
            return false;
        }

        if ("String" == typeStr ) {
             valueStr = QInputDialog::getText(this, tr("Input"),tr("Input string"), QLineEdit::Normal, tr("newvalue"), &ok);
             selectIndex = 0;
             if (!ok || valueStr.isEmpty()) {
                return false;
             }

        }
        else if ("Int" == typeStr) {
            valueInt = QInputDialog::getInt(this, tr("Input"), tr("Input int"), 0, -2147483647, 2147483647, 1,  &ok);
            selectIndex = 1;
            if (!ok) {
               return false;
            }
        }
        else if ("Double" == typeStr) {
           selectIndex = 2;
            valueDouble = QInputDialog::getDouble(this, tr("Input"), tr("Input double"), 0, -2147483647,2147483647, 1, &ok);
            if (!ok) {
               return false;
            }
        }
        else if ("True" == typeStr) {
            selectIndex = 3;
            valueBool = true;
        }
        else if ("False" == typeStr) {
            selectIndex = 3;
            valueBool = false;
        }
        else if("Null" == typeStr) {
             selectIndex = 4;
        }
        else {
            return false;
        }
    }

    setJsonContent();
    rapidjson::Document d;
    d.Parse(m_result.c_str());

    switch (selectIndex) {
    case 0:
        {
            if(false == m_operationJson->createKey(d, keyStr.toStdString().c_str(), valueStr.toStdString().c_str())) {
                return false;
            }
            break;
        }
    case 1:
        {
            if(false == m_operationJson->createKey(d, keyStr.toStdString().c_str(), valueInt)) {
                return false;
            }
            break;
        }
    case 2:
        {
            if(false == m_operationJson->createKey(d, keyStr.toStdString().c_str(), valueDouble)) {
                return false;
            }
            break;
        }
    case 3:
        {
            if(false == m_operationJson->createKey(d, keyStr.toStdString().c_str(), valueBool)) {
                return false;
            }
            break;
        }
    case 4:
        {
            if(false == m_operationJson->createKey(d, keyStr.toStdString().c_str(), "null")) {
                return false;
            }
            break;
        }
    default:
        break;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    m_result = buffer.GetString();
    QString tempQResult = QString::fromStdString(m_result);

    document()->setPlainText(tempQResult);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

bool Child::deletJsonStr()
{
    bool ok= false;
    QString keyStr = QInputDialog::getText(this, tr("Delete"), tr("Key string"), QLineEdit::Normal,tr("/new"),&ok);
    if(!ok || keyStr.isEmpty()) {
        return false;
    }

    setJsonContent();
    rapidjson::Document d;
    d.Parse(m_result.c_str());

    if (false == m_operationJson->deleteKey(d, keyStr.toStdString().c_str())) {
       return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    m_result = buffer.GetString();
    QString tempQResult = QString::fromStdString(m_result);
//    document()->clear();
    document()->setPlainText(tempQResult);
    connect(document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    return true;
}

bool Child::queryJsonStr()
{
    bool ok= false;

    QString keyStr = QInputDialog::getText(this, tr("Find"), tr("Key string"), QLineEdit::Normal, tr("/new"), &ok);
    if(!ok || keyStr.isEmpty()) {
        return false;
    }
    return true;
}

void Child::showFindDlg()
{
    m_findDlg->show();
}

void Child::showForwardFindText()
{
    QString str = m_findLineEdit->text();
    if (this->find(str)) {
       QPalette palette = this->palette();
       palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
       this->setPalette(palette);
    }
    else {
        QMessageBox::information(this, tr("Find"), tr("No Content"),QMessageBox::Ok);
    }
}

void Child::showBackWardFindText()
{
    QString str = m_findLineEdit->text();
    if (this->find(str,QTextDocument::FindBackward)) {
       QPalette palette = this->palette();
       palette.setColor(QPalette::Highlight, palette.color(QPalette::Active, QPalette::Highlight));
       this->setPalette(palette);
    }
    else {
        QMessageBox::information(this,tr("Find"),tr("NO Content"),QMessageBox::Ok);
    }
}

bool Child::goToLine()
{
    bool ok = false;
    int blockNumbers = this->document()->blockCount();
    int toLineNumber = QInputDialog::getInt(this, tr("跳转到指定行"),tr("Line Number:"), 1,1,blockNumbers,1, &ok);
    if (!ok) {
      return false;
    }

    QTextCursor tc = this->textCursor();
    int toPost = this->document()->findBlockByLineNumber(toLineNumber - 1).position();
    tc.setPosition(toPost, QTextCursor::MoveAnchor);
    this->setTextCursor(tc);
    this->centerCursor();
    return true;
}

bool Child::parseJson(HandleJson *handle)
{
    if (false == checkJsonStr()) {
        return false;
    }

    if ( false == m_operationJson->parse(m_result.c_str(), handle)) {
        return false;
    }

    return true;
}

bool Child::jsonTree()
{
    HandleJson* handle = new HandleJson();
    if (false == parseJson(handle)) {
        return false;
    }

    if (m_treeView != NULL) {
        m_treeView->close();
        delete m_treeView;
        m_treeView = NULL;
    }

    m_treeView = handle->getTree();
    m_treeView->setParent(this,Qt::Window);
    m_treeView->setWindowTitle(userFirendlyCurrentFile());
    m_treeView->setColumnWidth(0, 1000);
    m_treeView->move(500, 500);
    m_treeView->show();

    delete handle;

    return true;
}


/* EOF */
