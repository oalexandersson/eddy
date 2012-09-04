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
#include "codedocument.h"

namespace Ui {
    MainWindow::MainWindow()
    {
        setupUi();
    }

    void MainWindow::setupUi()
    {
        setStyle(new QMotifStyle());

        QList<QAction*> toolBarActions;
        QAction *action = new QAction(QIcon(":/icons/open"), "Open", this);
        action->setShortcut(QKeySequence::Open);
        connect(action, SIGNAL(triggered()), SLOT(onFileOpenAction()));
        toolBarActions.append(action);

        action = new QAction(QIcon(":/icons/save"), "Save", this);
        action->setShortcut(QKeySequence::Save);
        connect(action, SIGNAL(triggered()), SLOT(onFileSaveAction()));
        toolBarActions.append(action);

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

        resize(800, 600);
    }

    void MainWindow::onFileOpenAction()
    {
        QString filePath = QFileDialog::getOpenFileName(this, "Open File", "./");
        CodeDocument *document = new CodeDocument(filePath);
        document->load();

        documentView->addDocument(document);
        documentView->setCurrentDocument(document);
    }

    void MainWindow::onFileSaveAction()
    {
        CodeDocument *document = documentView->currentDocument();
        document->save();
    }
}
