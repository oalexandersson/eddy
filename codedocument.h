#ifndef CODEDOCUMENT_H
#define CODEDOCUMENT_H

#include <QTextDocument>
#include <QFileInfo>
#include <QDateTime>

class CodeDocument : public QObject
{
    Q_OBJECT

public:
    CodeDocument(QObject *parent = 0);
    CodeDocument(QString absoluteFilePath, QObject *parent = 0);

    QFileInfo const &fileInfo() const;
    void load();
    void save();
    bool isDirty() const;

    QTextDocument &textDocument();

Q_SIGNALS:
    void contentDirty();
    void contentClean();

private:
    QTextDocument m_textDocument;
    QFileInfo m_fileInfo;
    QDateTime m_lastRead;
    bool m_dirty;

private Q_SLOTS:
    void onDocumentContentsChanged();
};

#endif // CODEDOCUMENT_H
