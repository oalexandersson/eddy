#ifndef TABBEDDOCUMENTVIEW_H
#define TABBEDDOCUMENTVIEW_H

#include <QWidget>

class QTabBar;
class QMenu;
class CodeDocument;

namespace Ui {
    class CodeEditor;

    class TabbedDocumentView : public QWidget
    {
        Q_OBJECT
    public:
        class Document {
        public:
            QString fileName;
            QString filePath;
            QString text;
        };

        TabbedDocumentView(QWidget *parent = 0);
        void addDocument(CodeDocument *document);
        void setCurrentDocument(CodeDocument *document);
        CodeDocument *currentDocument() const;

    private:
        QTabBar *tabBar;
        QMenu *contextMenu;
        CodeEditor *editor;

        QList<CodeDocument *> documents;

        int contextIndex;

        void setupUi();
        void setupTabBar();
        void setupEditor();
        void addDummyData();
        int getDocumentIndex(CodeDocument *document) const;
        void updateTab(int index);

    private Q_SLOTS:
        void onTabBarCurrentChanged(int index);
        void onTabBarTabMoved(int fromIndex, int toIndex);
        void onTabBarTabCloseRequested(int index);
        void onTabBarContextMenuRequested(QPoint point);

        void onCloseFromContextMenu();

        void onCloseCurrent();
        void onFocusNext();
        void onFocusPrevious();

        void onDocumentContentDirtyChanged();
    };
}

#endif // TABBEDDOCUMENTVIEW_H
