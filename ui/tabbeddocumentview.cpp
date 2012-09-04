#include "ui/tabbeddocumentview.h"

#include <QTabBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QKeySequence>
#include <QShortcut>
#include <QTextCharFormat>
#include <QList>

#include "ui/codeeditor.h"

namespace Ui {
    TabbedDocumentView::TabbedDocumentView(QWidget *parent) : QWidget(parent)
    {
        setupUi();
        addDummyData();
    }

    void TabbedDocumentView::addDocument(Document *document)
    {
        if (documents.isEmpty()) {
            editor->show();
        }
        documents.append(document);
        int index = tabBar->count();
        tabBar->insertTab(index, document->fileName);
        tabBar->setTabToolTip(index, document->filePath);
    }

    void TabbedDocumentView::setCurrentDocument(Document *document)
    {
        int foundIndex = -1;
        int index = 0;

        for( index = 0; index < documents.size() && foundIndex == -1; index++) {
            if (documents.at(index) == document) {
                foundIndex = index;
            }
        }

        if (foundIndex != -1) {
            tabBar->setCurrentIndex(foundIndex);
        }
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

        connect(editor, SIGNAL(textChanged()), SLOT(onTextChanged()));
    }

    void TabbedDocumentView::addDummyData()
    {
        Document *document = new Document();
        document->fileName = "mainview.h";
        document->text = "class MainView";
        document->filePath = "src/mainview.h";
        addDocument(document);

        document = new Document();
        document->fileName = "mainview.cpp";
        document->text = "#include \"mainview.h\"";
        document->filePath = "src/mainview.cpp";
        addDocument(document);

        document = new Document();
        document->fileName = "tabbeddocumentview.h";
        document->text = "class TabbedDocumentView";
        document->filePath = "src/tabbeddocumentview.h";
        addDocument(document);

        document = new Document();
        document->fileName = "tabbeddocumentview.cpp";
        document->text = "#include \"tabbeddocumentview.h\"";
        document->filePath = "src/tabbeddocumentview.cpp";
        addDocument(document);
    }

    void TabbedDocumentView::onTabBarCurrentChanged(int index)
    {
        if (index >= 0) {
            editor->setPlainText(documents.at(index)->text);
        }
    }

    void TabbedDocumentView::onTabBarTabMoved(int newIndex, int oldIndex)
    {
        Document *movedDocument = documents.at(oldIndex);
        documents.removeAt(oldIndex);
        documents.insert(newIndex, movedDocument);
    }

    void TabbedDocumentView::onTabBarTabCloseRequested(int index)
    {
        Document *closedDocument = documents.at(index);
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

    void TabbedDocumentView::onTextChanged()
    {
        int currentIndex = tabBar->currentIndex();
        Document *document = documents.at(currentIndex);
        document->text = editor->toPlainText();
    }
}
