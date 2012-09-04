#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class TabbedDocumentView;

    class MainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        MainWindow();

    private:
        TabbedDocumentView *documentView;

        void setupUi();

    private Q_SLOTS:
        void onFileOpenAction();

    };
}

#endif // MAINWINDOW_H
