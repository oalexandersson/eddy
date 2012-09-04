#include "mainwindow.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QTreeView>
#include <QDockWidget>
#include <QPlastiqueStyle>
#include <QMotifStyle>
#include <QToolBar>
#include <QFileDialog>
#include <QAction>
#include <QShortcut>
#include <QKeySequence>
#include <QMenuBar>

#include "tabbeddocumentview.h"

namespace Ui {
    MainWindow::MainWindow()
    {
        setupUi();
    }

    void MainWindow::setupUi()
    {
        setStyle(new QMotifStyle());

        QList<QAction*> toolBarActions;
        QAction *openAction = new QAction(QIcon(":/icons/open"), "Open", this);
        openAction->setShortcut(QKeySequence::Open);
        connect(openAction, SIGNAL(triggered()), SLOT(onFileOpenAction()));
        toolBarActions.append(openAction);

        QMenuBar *menuBar = new QMenuBar(this);
        menuBar->setGeometry(0, 0, 600, 23);
        QMenu *menuFile = new QMenu("File", menuBar);
        menuBar->addAction(menuFile->menuAction());
        menuFile->addActions(toolBarActions);
        setMenuBar(menuBar);

        QToolBar *toolBar = new QToolBar(this);
        toolBar->addActions(toolBarActions);
        addToolBar(toolBar);

        documentView = new TabbedDocumentView(this);
        setCentralWidget(documentView);

        QDockWidget *dockWidget = new QDockWidget("File explorer", this);
        //dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        addDockWidget(Qt::LeftDockWidgetArea, dockWidget, Qt::Vertical);

        dockWidget = new QDockWidget("Console output", this);
        //dockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
        addDockWidget(Qt::BottomDockWidgetArea, dockWidget, Qt::Horizontal);
    }

    void MainWindow::onFileOpenAction()
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Open File", "./");
        TabbedDocumentView::Document *document = new TabbedDocumentView::Document();
        document->fileName = filePath.right(filePath.length()-filePath.lastIndexOf("/")-1);

        QFile file(filePath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QString content = file.readAll();
        file.close();
        document->filePath = filePath;
        document->text = content;
        documentView->addDocument(document);
        documentView->setCurrentDocument(document);
    }
}
