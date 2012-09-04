#include "ui/tabbeddocumentview.h"

#include <QTabBar>
#include <QVBoxLayout>
#include <QDebug>
#include <QMenu>
#include <QKeySequence>
#include <QShortcut>
#include <QTextCharFormat>

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
        tabBar->addTab(document->filename);
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
        contextMenu->addAction("Properties", this, SLOT(onPropertiesFromContextMenu()));
        contextMenu->addAction("Close", this, SLOT(onCloseFromContextMenu()));

        QShortcut *shortcut = new QShortcut(QKeySequence::Close, this, SLOT(onCloseCurrent()), 0, Qt::WindowShortcut);
        shortcut = new QShortcut(QKeySequence::NextChild, this, SLOT(onFocusNext()), 0, Qt::WindowShortcut);
        shortcut = new QShortcut(QKeySequence::PreviousChild, this, SLOT(onFocusPrevious()), 0, Qt::WindowShortcut);
    }

    void TabbedDocumentView::setupTabBar()
    {
        tabBar = new QTabBar();
        tabBar->setMovable(true);
        tabBar->setExpanding(false);
        tabBar->setContextMenuPolicy(Qt::CustomContextMenu);

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
        document->filename = "mainview.h";
        document->text = "class MainView";
        addDocument(document);

        document = new Document();
        document->filename = "mainview.cpp";
        document->text = "#include \"mainview.h\"";
        addDocument(document);

        document = new Document();
        document->filename = "tabbeddocumentview.h";
        document->text = "class TabbedDocumentView";
        addDocument(document);

        document = new Document();
        document->filename = "tabbeddocumentview.cpp";
        document->text = "#include \"tabbeddocumentview.h\"";
        addDocument(document);
    }

    void TabbedDocumentView::onTabBarCurrentChanged(int index)
    {
        editor->setPlainText(documents.at(index)->text);
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
        qDebug() << "Context menu at: " << point;
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
