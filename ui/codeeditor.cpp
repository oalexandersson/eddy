#include "codeeditor.h"

#include <QTextBlock>
#include <QPainter>

#include "linenumberarea.h"

namespace Ui {
    CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent), countCache(-1, -1)
    {
        lineNumberArea = new LineNumberArea(this);

        connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
        connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
        connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

        updateLineNumberAreaWidth(0);
        highlightCurrentLine();
    }

    int CodeEditor::lineNumberAreaWidth()
    {
        int digits = 1;
        int max = qMax(1, blockCount());
        while (max >= 10) {
            max /= 10;
            ++digits;
        }

        int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

        return space;
    }

    void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
    {
        setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
    }

    void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
    {
        if (dy) {
            lineNumberArea->scroll(0, dy);
        } else if (countCache.first != blockCount()
            || countCache.second != textCursor().block().lineCount()) {
            lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
            countCache.first = blockCount();
            countCache.second = textCursor().block().lineCount();
        }

        if (rect.contains(viewport()->rect()))
            updateLineNumberAreaWidth(0);
    }

    void CodeEditor::resizeEvent(QResizeEvent *e)
    {
        QPlainTextEdit::resizeEvent(e);

        QRect cr = contentsRect();
        lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
    }

    void CodeEditor::highlightCurrentLine()
    {
        QList<QTextEdit::ExtraSelection> extraSelections;

        if (!isReadOnly()) {
            QTextEdit::ExtraSelection selection;

            QColor lineColor = QColor(Qt::yellow).lighter(180);

            selection.format.setBackground(lineColor);
            selection.format.setProperty(QTextFormat::FullWidthSelection, true);
            selection.cursor = textCursor();
            selection.cursor.clearSelection();
            extraSelections.append(selection);
        }

        setExtraSelections(extraSelections);
    }

    void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
    {
        QPainter painter(lineNumberArea);
        painter.fillRect(event->rect(), Qt::lightGray);

        QTextBlock block = firstVisibleBlock();
        int blockNumber = block.blockNumber();
        int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
        int bottom = top + (int) blockBoundingRect(block).height();

        while (block.isValid() && top <= event->rect().bottom()) {
            if (block.isVisible() && bottom >= event->rect().top()) {
                QString number = QString::number(blockNumber + 1);
                painter.setPen(Qt::black);
                painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                 Qt::AlignRight, number);
            }

            block = block.next();
            top = bottom;
            bottom = top + (int) blockBoundingRect(block).height();
            ++blockNumber;
        }
    }

    void CodeEditor::keyPressEvent(QKeyEvent *e)
    {
        if (e->key() == Qt::Key_Tab) {
            QTextCursor theCursor = textCursor();
            do {
                theCursor.insertText(" ");
            } while (theCursor.columnNumber() % 4 != 0);
            e->accept();
        } else {
            QPlainTextEdit::keyPressEvent(e);
        }
    }
}
