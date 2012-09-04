#include "codedocument.h"

#include <QTextStream>
#include <QDebug>
#include <QPlainTextDocumentLayout>

CodeDocument::CodeDocument(QObject *parent) :
        QObject(parent),
        m_dirty(false)
{
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(&m_textDocument);
    m_textDocument.setDocumentLayout(layout);
}

CodeDocument::CodeDocument(QString absoluteFilePath, QObject *parent) :
        QObject(parent),
        m_fileInfo(absoluteFilePath),
        m_dirty(false)
{
    QPlainTextDocumentLayout *layout = new QPlainTextDocumentLayout(&m_textDocument);
    m_textDocument.setDocumentLayout(layout);
    m_textDocument.setDefaultFont(QFont("monospace", 8));
}

QFileInfo const &CodeDocument::fileInfo() const
{
    return m_fileInfo;
}

void CodeDocument::load()
{
    QFile file(m_fileInfo.absoluteFilePath());
    bool result = file.open(QIODevice::ReadOnly|QIODevice::Text);
    Q_ASSERT(result);

    if(!result) {
        return;
    }

    QTextStream in(&file);
    m_textDocument.setPlainText(in.readAll());
    file.close();
    m_fileInfo.refresh();
    m_lastRead = m_fileInfo.lastRead();
    m_dirty = false;
    connect(&m_textDocument, SIGNAL(contentsChanged()), SLOT(onDocumentContentsChanged()));
}

void CodeDocument::save()
{
    QFile file(m_fileInfo.absoluteFilePath());
    bool result = file.open(QIODevice::WriteOnly|QIODevice::Truncate);
    Q_ASSERT(result);

    if(!result) {
        return;
    }

    QTextStream out(&file);
    out << m_textDocument.toPlainText();
    file.close();
    m_fileInfo.refresh();
    m_lastRead = m_fileInfo.lastModified();
    m_dirty = false;
    emit contentClean();
}

bool CodeDocument::isDirty() const
{
    return m_dirty;
}

QTextDocument &CodeDocument::textDocument()
{
    return m_textDocument;
}

void CodeDocument::onDocumentContentsChanged()
{
    if (!m_dirty) {
        m_dirty = true;
        emit contentDirty();
    }
    qDebug() << m_textDocument.toPlainText();
}
