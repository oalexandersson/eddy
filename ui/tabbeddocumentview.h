#ifndef TABBEDDOCUMENTVIEW_H
#define TABBEDDOCUMENTVIEW_H

#include <QWidget>

class QTabBar;
class QMenu;

namespace Ui {
    class CodeEditor;

    class TabbedDocumentView : public QWidget
    {
        Q_OBJECT
    public:
        class Document {
        public:
            QString filename;
            QString text;
        };

        TabbedDocumentView(QWidget *parent = 0);
        void addDocument(Document *document);

    private:
        QTabBar *tabBar;
        QMenu *contextMenu;
        CodeEditor *editor;

        QList<Document *> documents;

        int contextIndex;

        void setupUi();
        void setupTabBar();
        void setupEditor();
        void addDummyData();

    private Q_SLOTS:
        void onTabBarCurrentChanged(int index);
        void onTabBarTabMoved(int fromIndex, int toIndex);
        void onTabBarTabCloseRequested(int index);
        void onTabBarContextMenuRequested(QPoint point);

        void onCloseFromContextMenu();

        void onCloseCurrent();
        void onFocusNext();
        void onFocusPrevious();

        void onTextChanged();
    };
}

#endif // TABBEDDOCUMENTVIEW_H
