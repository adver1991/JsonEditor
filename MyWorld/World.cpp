#include <QtWidgets>
#include <QMenu>
#include <QMenuBar>

#include "HandleJson.h"

#include "World.h"
#include "Child.h"

const QString rsrcPath = ":/images";

World::World(QWidget *parent)
: QMainWindow(parent)
{
    m_mdiArea = new QMdiArea;
    m_mdiArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_mdiArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setCentralWidget(m_mdiArea);
    connect(m_mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow*)), this,
            SLOT(updateMenus()));

    m_windowMapper = new QSignalMapper(this);
    connect(m_windowMapper, SIGNAL(mapped(QWidget*)), this, SLOT(setActiveSubWindow(QWidget*)));

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    updateMenus();

    move(200,150);
    resize(800,800);

    setWindowTitle(tr("Json Editor"));
}

World::~World()
{
}

void World::createActions()
{
    /*文件*/
    m_newAct = new QAction(QIcon(rsrcPath + "/filenew.png"), tr("New File(&N)"),this);
    m_newAct->setShortcuts(QKeySequence::New);
    m_newAct->setToolTip("新建");
    m_newAct->setStatusTip(tr("创建一个新文档"));
    connect(m_newAct,SIGNAL(triggered()), this, SLOT(fileNew()));

    m_openAct = new QAction(QIcon(rsrcPath + "/fileopen.png"), tr("Open File(&O)..."), this);
    m_openAct->setShortcuts(QKeySequence::Open);
    m_openAct->setToolTip("打开");
    m_openAct->setStatusTip(tr("打开已存在的文档"));
    connect(m_openAct,SIGNAL(triggered()), this, SLOT(fileOpen()));

    m_saveAct = new QAction(QIcon(rsrcPath + "/filesave.png"), tr("Save(&S)"),this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setToolTip("保存");
    m_saveAct->setStatusTip(tr("将当前文档保存"));
    connect(m_saveAct,SIGNAL(triggered()), this, SLOT(fileSave()));

    m_saveAsAct = new QAction(tr("Save As(&A)"), this);
    m_saveAsAct->setShortcuts(QKeySequence::SaveAs);
    m_saveAsAct->setStatusTip(tr("以新的名字保存文档"));
    connect(m_saveAsAct,SIGNAL(triggered()), this, SLOT(fileSaveAs()));

    m_printAct = new QAction(QIcon(rsrcPath + "/fileprint.png"), tr("Print(&P)"), this);
    m_printAct->setShortcuts(QKeySequence::Print);
    m_printAct->setToolTip("打印");
    m_printAct->setStatusTip(tr("打印文档"));
    connect(m_printAct,SIGNAL(triggered()), this, SLOT(filePrint()));

    m_printPreviewAct = new QAction(tr("Print Preview..."), this);
    m_printPreviewAct->setStatusTip(tr("预览打印效果"));
    connect(m_printPreviewAct,SIGNAL(triggered()), this, SLOT(filePrintPreview()));

    m_exitAct = new QAction(tr("Exit(&X)"), this);
    m_exitAct->setShortcuts(QKeySequence::Quit);
    m_exitAct->setStatusTip(tr("退出应用程序"));
    connect(m_exitAct, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));


    /*编辑*/
    m_undoAct = new QAction(QIcon(rsrcPath + "/editundo.png"), tr("Undo(&U)"), this);
    m_undoAct->setShortcut(QKeySequence::Undo);
    m_undoAct->setToolTip("撤销");
    m_undoAct->setStatusTip(tr("撤销当前操作"));
    connect(m_undoAct,SIGNAL(triggered()), this ,SLOT(undo()));

    m_redoAct = new QAction(QIcon(rsrcPath + "/editredo.png"), tr("Redo(&R)"), this);
    m_redoAct->setShortcut(QKeySequence::Redo);
    m_redoAct->setToolTip("重做");
    m_redoAct->setStatusTip(tr("恢复之前操作"));
    connect(m_redoAct,SIGNAL(triggered()), this ,SLOT(redo()));

    m_cutAct = new QAction(QIcon(rsrcPath + "/editcut.png"), tr("Cut(&T)"), this);
    m_cutAct->setShortcut(QKeySequence::Cut);
    m_cutAct->setToolTip("剪切");
    m_cutAct->setStatusTip(tr("剪切所选内容"));
    connect(m_cutAct, SIGNAL(triggered()), this, SLOT(cut()));

    m_copyAct = new QAction(QIcon(rsrcPath + "/editcopy.png"), tr("Copy(&C)"), this);
    m_copyAct->setShortcut(QKeySequence::Copy);
    m_copyAct->setToolTip("复制");
    m_copyAct->setStatusTip(tr("复制所选内容"));
    connect(m_copyAct, SIGNAL(triggered()), this, SLOT(copy()));

    m_pastAct = new QAction(QIcon(rsrcPath + "/editpaste.png"), tr("Paste(&P)"), this);
    m_pastAct->setShortcut(QKeySequence::Paste);
    m_pastAct->setToolTip("粘贴");
    m_pastAct->setStatusTip(tr("粘贴剪贴板内容到文档"));
    connect(m_pastAct, SIGNAL(triggered()), this, SLOT(paste()));

    /*窗口*/
    m_closeAct = new QAction(tr("Close(&O)"), this);
    m_closeAct->setStatusTip(tr("关闭活动文档子窗口"));
    m_closeAct->setShortcut(QKeySequence::Close);
    connect(m_closeAct, SIGNAL(triggered()), m_mdiArea, SLOT(closeActiveSubWindow()));

    m_closeAllAct = new QAction(tr("Close All(&A)"),this);
    m_closeAllAct->setStatusTip(tr("关闭所有子窗口"));
    connect(m_closeAllAct, SIGNAL(triggered()), m_mdiArea, SLOT(closeAllSubWindows()));

    m_tileAct = new QAction(QIcon(rsrcPath + "/texttile.png"), tr("Tile(&T)"),this);
    m_tileAct->setStatusTip(tr("平铺所有子窗口"));
    connect(m_tileAct, SIGNAL(triggered()), m_mdiArea, SLOT(tileSubWindows()));

    m_cascadeAct = new QAction(QIcon(rsrcPath + "/textstack.png"),tr("Stack(&C)"),this);
    m_cascadeAct->setStatusTip(tr("层叠子窗口"));
    connect(m_cascadeAct, SIGNAL(triggered()), m_mdiArea, SLOT(cascadeSubWindows()));

    m_nextAct = new QAction(tr("Next(&X)"),this);
    m_nextAct->setShortcut(QKeySequence::NextChild);
    m_nextAct->setStatusTip(tr("移动焦点到下一个子窗口"));
    connect(m_nextAct, SIGNAL(triggered()), m_mdiArea, SLOT(activateNextSubWindow()));

    m_separatorAct = new QAction(this);
    m_separatorAct->setSeparator(true);

    /*帮助*/
    m_aboutAct = new QAction(tr("About(&A)"), this);
    m_aboutAct->setStatusTip(tr("关于程序"));
    connect(m_aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    m_aboutQtAct = new QAction(tr("About QT(&Q)"), this);
    m_aboutQtAct->setStatusTip(tr("关于QT库"));
    connect(m_aboutQtAct,SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    //格式化Json和压缩Json
    m_prettyJsonAct = new QAction(QIcon(rsrcPath + "/textformat.png"), tr("Format Json(&P)"), this);
    m_prettyJsonAct->setToolTip(tr("格式化"));
    m_prettyJsonAct->setStatusTip(tr("格式化Json数据"));
    m_prettyJsonAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_P);
    connect(m_prettyJsonAct,SIGNAL(triggered()), this, SLOT(prettyJson()));

    m_compressJsonAct = new QAction(QIcon(rsrcPath + "/textcompress.png"), tr("Compress Json(&C)"), this);
    m_compressJsonAct->setToolTip(tr("压缩数据"));
    m_compressJsonAct->setStatusTip(tr("压缩JSON数据，清除所有空白符"));
    m_compressJsonAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_C);
    connect(m_compressJsonAct, SIGNAL(triggered()), this, SLOT(compressJson()));

    m_checkJsonAct = new QAction(QIcon(rsrcPath + "/textcheck.png"), tr("Check Json(&K)"), this);
    m_checkJsonAct->setToolTip(tr("校验数据"));
    m_checkJsonAct->setStatusTip(tr("校验JSON数据，提示错误行"));
    m_checkJsonAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_K);
    connect(m_checkJsonAct,SIGNAL(triggered()), this, SLOT(checkJson()));

    m_jsonTreeAct = new QAction(QIcon(rsrcPath + "/texttree.png"), tr("Json Tree(&K)"), this);
    m_jsonTreeAct->setToolTip(tr("Json Tree"));
    m_jsonTreeAct->setStatusTip(tr("生成Json树形结构，仅供参考"));
    m_jsonTreeAct->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_T);
    connect(m_jsonTreeAct,SIGNAL(triggered()), this, SLOT(jsonTree()));

    //动态修改、删除、查询
    m_modifyJsonAct = new QAction(QIcon(rsrcPath + "/textmodify.png"), tr("Modify Json(&M)"), this);
    m_modifyJsonAct->setToolTip(tr("修改数据"));
    m_modifyJsonAct->setShortcut(Qt::CTRL + Qt::Key_M);
    m_modifyJsonAct->setStatusTip(tr("修改一条Json数据，若不存在则新增数据"));
    connect(m_modifyJsonAct, SIGNAL(triggered()), this, SLOT(modifyJson()));

    m_deleteJsonAct = new QAction(QIcon(rsrcPath + "/textdelete.png"),tr("Delete Json(&D)"), this);
    m_deleteJsonAct->setToolTip(tr("删除数据"));
    m_deleteJsonAct->setShortcut(Qt::CTRL + Qt::Key_D);
    m_deleteJsonAct->setStatusTip(tr("删除一条Json数据"));
    connect(m_deleteJsonAct, SIGNAL(triggered()), this, SLOT(deleteJson()));

    m_queryJsonAct = new QAction(QIcon(rsrcPath + "/textquery.png"), tr("Query Json(&F)"), this);
    m_queryJsonAct->setToolTip(tr("查询数据"));
    m_queryJsonAct->setShortcut(QKeySequence::Find);
    m_queryJsonAct->setStatusTip(tr("查询JSON数据"));
    connect(m_queryJsonAct, SIGNAL(triggered()), this,SLOT(queryJson()));

    m_goToLineJsonAct = new QAction(QIcon(rsrcPath + "/textgo.png"), tr("Go To Line(&G)"), this);
    m_goToLineJsonAct->setToolTip(tr("行定位"));
    m_goToLineJsonAct->setShortcut(Qt::CTRL + Qt::Key_G);
    m_goToLineJsonAct->setStatusTip(tr("定位到指定行"));
    connect(m_goToLineJsonAct, SIGNAL(triggered()), this, SLOT(goToLineJson()));
}

void World::createMenus()
{
    /*文件*/
    m_fileMenu = menuBar()->addMenu(tr("File"));
    m_fileMenu->addAction(m_newAct);
    m_fileMenu->addAction(m_openAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_saveAct);
    m_fileMenu->addAction(m_saveAsAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_printAct);
    m_fileMenu->addAction(m_printPreviewAct);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(m_exitAct);

    /*编辑*/
    m_editMenu = menuBar()->addMenu(tr("Edit"));
    m_editMenu->addAction(m_undoAct);
    m_editMenu->addAction(m_redoAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_cutAct);
    m_editMenu->addAction(m_copyAct);
    m_editMenu->addAction(m_pastAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_modifyJsonAct);
    m_editMenu->addAction(m_deleteJsonAct);
    m_editMenu->addAction(m_queryJsonAct);
    m_editMenu->addSeparator();
    m_editMenu->addAction(m_goToLineJsonAct);

    /*窗口*/
    m_windowMenu = menuBar()->addMenu(tr("Window"));
    updateWindowMenu();
    connect(m_windowMenu,SIGNAL(aboutToShow()),this,SLOT(updateWindowMenu()));
    menuBar()->addSeparator();

    /*格式*/
    m_formatMenu = menuBar()->addMenu(tr("Format"));
    m_formatMenu->addAction(m_prettyJsonAct);
    m_formatMenu->addAction(m_compressJsonAct);
    m_formatMenu->addSeparator();
    m_formatMenu->addAction(m_checkJsonAct);
    m_formatMenu->addAction(m_jsonTreeAct);

    /*帮助*/
    m_helpMenu = menuBar()->addMenu(tr("Help"));
    m_helpMenu->addAction(m_aboutAct);
    m_helpMenu->addSeparator();
    m_helpMenu->addAction(m_aboutQtAct);
}

void World::updateWindowMenu()
{
    m_windowMenu->clear();
    m_windowMenu->addAction(m_closeAct);
    m_windowMenu->addAction(m_closeAllAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_nextAct);
    m_windowMenu->addSeparator();
    m_windowMenu->addAction(m_tileAct);
    m_windowMenu->addAction(m_cascadeAct);
    m_windowMenu->addAction(m_separatorAct);

    QList<QMdiSubWindow*> windows = m_mdiArea->subWindowList();
    m_separatorAct->setVisible(!windows.isEmpty());

    for (int i = 0; i < windows.size(); ++i) {
        Child* child = qobject_cast<Child*>(windows.at(i)->widget());
        QString text;
        if (i < 9) {
            text = tr("&%1 %2").arg(i+1).arg(child->userFirendlyCurrentFile());
        }
        else {
            text = tr("%1 %2").arg(i+1).arg(child->userFirendlyCurrentFile());
        }

        QAction* action = m_windowMenu->addAction(text);
        action->setCheckable(true);
        action->setChecked(child == activeChild());
        connect(action,SIGNAL(triggered()),m_windowMapper,SLOT(map()));
        m_windowMapper->setMapping(action,windows.at(i));
    }
}

void World::createToolBars()
{
    //文件工具栏
    m_fileToolBar = addToolBar(tr("File"));
    m_fileToolBar->addAction(m_newAct);
    m_fileToolBar->addAction(m_openAct);
    m_fileToolBar->addAction(m_saveAct);
    m_fileToolBar->addSeparator();
    m_fileToolBar->addAction(m_tileAct);
    m_fileToolBar->addAction(m_cascadeAct);

    //编辑工具栏
    m_editToolBar = addToolBar(tr("Edit"));
    m_editToolBar->addAction(m_undoAct);
    m_editToolBar->addAction(m_redoAct);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_cutAct);
    m_editToolBar->addAction(m_copyAct);
    m_editToolBar->addAction(m_pastAct);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_modifyJsonAct);
    m_editToolBar->addAction(m_deleteJsonAct);
    m_editToolBar->addAction(m_queryJsonAct);
    m_editToolBar->addSeparator();
    m_editToolBar->addAction(m_goToLineJsonAct);

    //格式工具栏
    m_formatToolBar = addToolBar(tr("Format"));
    m_formatToolBar->addAction(m_prettyJsonAct);
    m_formatToolBar->addAction(m_compressJsonAct);
    m_formatToolBar->addSeparator();
    m_formatToolBar->addAction(m_checkJsonAct);
    m_formatToolBar->addAction(m_jsonTreeAct);
    m_comboSize = new QComboBox();
    m_formatToolBar->addWidget(m_comboSize);
    m_comboSize->setEditable(true);
    m_comboSize->setStatusTip("更改字号");

    QFontDatabase db;
    foreach (int size, db.standardSizes()) {
        m_comboSize->addItem(QString::number(size));
        connect(m_comboSize, SIGNAL(activated(QString)),this,SLOT(textSize(QString)));
        m_comboSize->setCurrentIndex(m_comboSize->findText(QString::number(QApplication::font().pointSize())));
    }
}

void World::about()
{
    QMessageBox::about(this, tr("About"),tr("基于Rapidjosn实现的Json编辑工具\nAuthor: 林多\nCSDN：http://blog.csdn.net/zxc024000\n"
                                            "软件版权归林多所有，仅用于学习和研究使用，未经授权不得用于任何商业用途。"));
    if (!QFile::exists("Help.odt")) {
        statusBar()->showMessage(tr("帮助文档不存在"), 0);
        return;
    }
    QProcess::execute("/usr/bin/libreoffice",QStringList()<<"Help.odt");
}


Child* World::createChild()
{
    Child* child = new Child(this);
    m_mdiArea->addSubWindow(child);
    connect(child, SIGNAL(copyAvailable(bool)), m_cutAct, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), m_copyAct, SLOT(setEnabled(bool)));
    return child;
}

void World::fileNew()
{
    Child* child = createChild();
    child->newFile();
    child->show();
//    enabledText();
}

void World::updateMenus()
{
    bool hasChild = (activeChild() != 0);

    m_saveAct->setEnabled(hasChild);
    m_saveAsAct->setEnabled(hasChild);
    m_printAct->setEnabled(hasChild);
    m_printPreviewAct->setEnabled(hasChild);
    m_pastAct->setEnabled(hasChild);
    m_undoAct->setEnabled(hasChild);
    m_redoAct->setEnabled(hasChild);

    m_closeAct->setEnabled(hasChild);
    m_closeAllAct->setEnabled(hasChild);
    m_tileAct->setEnabled(hasChild);
    m_cascadeAct->setEnabled(hasChild);
    m_nextAct->setEnabled(hasChild);

    m_separatorAct->setVisible(hasChild);

//    bool hasSelection = (activeChild() && activeChild()->textCursor().hasSelection());

    m_cutAct->setEnabled(hasChild);
    m_copyAct->setEnabled(hasChild);

    m_prettyJsonAct->setEnabled(hasChild);
    m_compressJsonAct->setEnabled(hasChild);
    m_checkJsonAct->setEnabled(hasChild);
    m_jsonTreeAct->setEnabled(hasChild);
    m_goToLineJsonAct->setEnabled(hasChild);

    m_modifyJsonAct->setEnabled(hasChild);
    m_queryJsonAct->setEnabled(hasChild);
    m_deleteJsonAct->setEnabled(hasChild);
}

Child* World::activeChild()
{
    if (QMdiSubWindow* activeSubWindow = m_mdiArea->activeSubWindow()) {
        return qobject_cast<Child*>(activeSubWindow->widget());
    }
    return 0;
}

void World::setActiveSubWindow(QWidget *window)
{
    if (!window) {
        return;
    }
    m_mdiArea->setActiveSubWindow(qobject_cast<QMdiSubWindow*>(window));
}

void World::closeEvent(QCloseEvent *event)
{
    m_mdiArea->closeAllSubWindows();
    if (m_mdiArea->currentSubWindow()) {
        event->ignore();
    }
    else {
        event->accept();
    }
}

void World::createStatusBar()
{
    statusBar()->showMessage(tr("就绪"));
}

void World::fileOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开"), QString(), tr("所有文件(*.*)"));
    if (!fileName.isEmpty()) {
        QMdiSubWindow* existing = findChild(fileName);
        if (existing) {
            m_mdiArea->setActiveSubWindow(existing);
            return;
        }

        Child* child =createChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("文件已载入"), 0);
            child->show();
        }
        else {
            child->close();
        }
    }

}

QMdiSubWindow* World::findChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow* window, m_mdiArea->subWindowList()) {
        Child* child = qobject_cast<Child*>(window->widget());
        if (child->currentFile() == canonicalFilePath) {
            return window;
        }
    }

    return 0;
}

void World::fileSave()
{
    if (activeChild() && activeChild()->save()) {
       statusBar()->showMessage(tr("保存成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("保存失败"), 0);
    }
}

void World::fileSaveAs()
{
    if (activeChild() && activeChild()->saveAs()) {
        statusBar()->showMessage(tr("保存成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("保存失败"), 0);
    }
}

void World::undo()
{
    if (activeChild()) {
        activeChild()->document()->undo();
    }
}

void World::redo()
{
    if (activeChild()) {
        activeChild()->redo();
    }
}

void World::cut()
{
    if (activeChild()) {
        activeChild()->cut();
    }
}

void World::copy()
{
    if (activeChild()) {
        activeChild()->copy();
    }
}

void World::paste()
{
    if (activeChild()) {
        activeChild()->paste();
    }
}

void World::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat()) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(pointSize);
        if(activeChild())
            activeChild()->mergeFormatOnWordOrSelection(fmt);
    }
}

void World::filePrint()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog* dlg = new QPrintDialog(&printer, this);

    if (activeChild()->textCursor().hasSelection()) {
       dlg->addEnabledOption(QAbstractPrintDialog::PrintSelection);
    }

    dlg->setWindowTitle(tr("打印文档"));
    if (dlg->exec() == QDialog::Accepted) {
        activeChild()->print(&printer);
    }

    delete dlg;
}

void World::filePrintPreview()
{
    QPrinter printer(QPrinter::HighResolution);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(printPreview(QPrinter*)));
    preview.exec();
}

void World::printPreview(QPrinter *printer)
{
    activeChild()->print(printer);
}


void World::prettyJson()
{
    if (activeChild() && activeChild()->formatJsonStr()) {
         statusBar()->showMessage(tr("格式化成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("格式化失败"), 0);
    }
}

void World::compressJson()
{
    if (activeChild() && activeChild()->compressJsonStr()) {
       statusBar()->showMessage(tr("压缩成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("压缩失败"), 0);
    }
}

void World::modifyJson()
{
    if (activeChild() && activeChild()->modifyJsonStr()) {
        statusBar()->showMessage(tr("修改成功"), 0);
    }
    else {
          statusBar()->showMessage(tr("修改失败"), 0);
    }
}

void World::deleteJson()
{
    if (activeChild() && activeChild()->deletJsonStr()) {
        statusBar()->showMessage(tr("删除成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("删除失败"), 0);
    }
}

void World::queryJson()
{
    if (activeChild()) {
        activeChild()->showFindDlg();
    }
}

void World::checkJson()
{
    if (activeChild() && activeChild()->checkJsonStr()) {
        statusBar()->showMessage(tr("数据格式正确"), 0);
    }
    else {
        statusBar()->showMessage(tr("数据格式错误"), 0);
    }
}

void World::goToLineJson()
{
    if (activeChild() && activeChild()->goToLine()) {
         statusBar()->showMessage(tr("跳转成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("跳转失败"), 0);
    }
}

void World::jsonTree()
{
    if(activeChild() && activeChild()->jsonTree()) {
        statusBar()->showMessage(tr("Json树成功"), 0);
    }
    else {
        statusBar()->showMessage(tr("Json树失败"), 0);
    }
}

/* EOF */
