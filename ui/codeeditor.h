#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

namespace Ui {
    class CodeEditor : public QPlainTextEdit
    {
        Q_OBJECT

    public:
        CodeEditor(QWidget *parent = 0);

        void lineNumberAreaPaintEvent(QPaintEvent *event);
        int lineNumberAreaWidth();
        void setDocument(QTextDocument *document);

    protected:
        virtual void resizeEvent(QResizeEvent *event);
        virtual void keyPressEvent(QKeyEvent *e);

    private slots:
        void updateLineNumberAreaWidth();
        void highlightCurrentLine();
        void updateLineNumberArea(const QRect &, int);

    private:
        QWidget *lineNumberArea;
        QPair<int, int> countCache;
    };
}
#endif // CODEEDITOR_H
