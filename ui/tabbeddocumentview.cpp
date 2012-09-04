#include "ui/tabbeddocumentview.h"

#include <QTabBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QKeySequence>
#include <QShortcut>
#include <QTextCharFormat>
#include <QList>
#include <QFileInfo>
#include <QPushButton>

#include "ui/codeeditor.h"
#include "codedocument.h"

namespace Ui {
    TabbedDocumentView::TabbedDocumentView(QWidget *parent) : QWidget(parent)
    {
        setupUi();
        //addDummyData();
    }

    void TabbedDocumentView::addDocument(CodeDocument *document)
    {
        if (documents.isEmpty()) {
            editor->show();
        }
        documents.append(document);
        int index = tabBar->count();
        tabBar->insertTab(index, QString());
        updateTab(index);
        connect(document, SIGNAL(contentDirty()), SLOT(onDocumentContentDirtyChanged()));
        connect(document, SIGNAL(contentClean()), SLOT(onDocumentContentDirtyChanged()));
    }

    void TabbedDocumentView::setCurrentDocument(CodeDocument *document)
    {
        int index = getDocumentIndex(document);
        if (index >= 0) {
            tabBar->setCurrentIndex(index);
        }
    }

    CodeDocument *TabbedDocumentView::currentDocument() const
    {
        int index = tabBar->currentIndex();
        CodeDocument *document = 0;

        if (index >= 0) {
            document = documents.at(index);
        }

        return document;
    }

    int TabbedDocumentView::getDocumentIndex(CodeDocument *document) const
    {
        int foundIndex = -1;
        int index = 0;

        for( index = 0; index < documents.size() && foundIndex == -1; index++) {
            if (documents.at(index) == document) {
                foundIndex = index;
            }
        }

        return foundIndex;
    }

    void TabbedDocumentView::updateTab(int index)
    {
        CodeDocument *document = documents.at(index);
        if (document->isDirty()) {
            tabBar->setTabText(index, document->fileInfo().fileName().append(" *"));
            tabBar->setTabTextColor(index, Qt::darkGreen);
        }
        else {
            tabBar->setTabText(index, document->fileInfo().fileName());
            tabBar->setTabTextColor(index, Qt::black);
        }
        tabBar->setTabToolTip(index, document->fileInfo().absoluteFilePath());
    }

    void TabbedDocumentView::setupUi()
    {
        resize(600, 400);

        setupTabBar();
        setupEditor();

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->setSpacing(0);

        layout->addWidget(tabBar);
        layout->addWidget(editor);

        contextMenu = new QMenu(this);
        contextMenu->addAction("Properties (NYI)", this, SLOT(onPropertiesFromContextMenu()));
        contextMenu->addAction("Close", this, SLOT(onCloseFromContextMenu()));

        new QShortcut(QKeySequence::Close, this, SLOT(onCloseCurrent()), 0, Qt::WindowShortcut);
        new QShortcut(QKeySequence::NextChild, this, SLOT(onFocusNext()), 0, Qt::WindowShortcut);
        new QShortcut(QKeySequence::PreviousChild, this, SLOT(onFocusPrevious()), 0, Qt::WindowShortcut);
    }

    void TabbedDocumentView::setupTabBar()
    {
        tabBar = new QTabBar();
        tabBar->setMovable(true);
        tabBar->setExpanding(false);
        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);
        tabBar->setTabsClosable(true);
        tabBar->setElideMode(Qt::ElideRight);

        connect(tabBar, SIGNAL(currentChanged(int)), SLOT(onTabBarCurrentChanged(int)));
        connect(tabBar, SIGNAL(tabMoved(int,int)), SLOT(onTabBarTabMoved(int, int)));
        connect(tabBar, SIGNAL(tabCloseRequested(int)), SLOT(onTabBarTabCloseRequested(int)));
        connect(tabBar, SIGNAL(customContextMenuRequested(QPoint)), SLOT(onTabBarContextMenuRequested(QPoint)));
    }

    void TabbedDocumentView::setupEditor()
    {
        editor = new CodeEditor();
        editor->hide();

        QTextCharFormat format;
        format.setFontFamily("monospace");
        editor->setCurrentCharFormat(format);
    }

    void TabbedDocumentView::addDummyData()
    {
//        Document *document = new Document();
//        document->fileName = "mainview.h";
//        document->text = "class MainView";
//        document->filePath = "src/mainview.h";
//        addDocument(document);
//
//        document = new Document();
//        document->fileName = "mainview.cpp";
//        document->text = "#include \"mainview.h\"";
//        document->filePath = "src/mainview.cpp";
//        addDocument(document);
//
//        document = new Document();
//        document->fileName = "tabbeddocumentview.h";
//        document->text = "class TabbedDocumentView";
//        document->filePath = "src/tabbeddocumentview.h";
//        addDocument(document);
//
//        document = new Document();
//        document->fileName = "tabbeddocumentview.cpp";
//        document->text = "#include \"tabbeddocumentview.h\"";
//        document->filePath = "src/tabbeddocumentview.cpp";
//        addDocument(document);
    }

    void TabbedDocumentView::onTabBarCurrentChanged(int index)
    {
        if (index >= 0) {
            editor->setDocument(&documents.at(index)->textDocument());
        }
        else {
            editor->setDocument(0);
        }
    }

    void TabbedDocumentView::onTabBarTabMoved(int newIndex, int oldIndex)
    {
        CodeDocument *movedDocument = documents.at(oldIndex);
        documents.removeAt(oldIndex);
        documents.insert(newIndex, movedDocument);
    }

    void TabbedDocumentView::onTabBarTabCloseRequested(int index)
    {
        CodeDocument *closedDocument = documents.at(index);
        documents.removeAt(index);
        tabBar->removeTab(index);
        delete closedDocument;

        if (documents.isEmpty()) {
            editor->hide();
        }
    }

    void TabbedDocumentView::onTabBarContextMenuRequested(QPoint point)
    {
        contextIndex = tabBar->tabAt(point);
        QPoint globalPos = tabBar->mapToGlobal(point);
        contextMenu->exec(globalPos);
    }

    void TabbedDocumentView::onCloseFromContextMenu()
    {
        onTabBarTabCloseRequested(contextIndex);
        contextIndex = -1;
    }

    void TabbedDocumentView::onCloseCurrent()
    {
        int currentIndex = tabBar->currentIndex();
        onTabBarTabCloseRequested(currentIndex);
    }

    void TabbedDocumentView::onFocusNext()
    {
        int currentIndex = tabBar->currentIndex();
        int nbrTabs = tabBar->count();
        int newIndex = (currentIndex+1) % nbrTabs;
        tabBar->setCurrentIndex(newIndex);
    }

    void TabbedDocumentView::onFocusPrevious()
    {
        int currentIndex = tabBar->currentIndex();
        int nbrTabs = tabBar->count();
        int newIndex = (currentIndex+nbrTabs-1) % nbrTabs;
        tabBar->setCurrentIndex(newIndex);
    }

    void TabbedDocumentView::onDocumentContentDirtyChanged()
    {
        CodeDocument *document = dynamic_cast<CodeDocument*>(sender());
        if (!document) {
            return;
        }

        int index = getDocumentIndex(document);
        updateTab(index);
    }
}
