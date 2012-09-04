#include <QApplication>
#include <QTextDocument>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QBoxLayout>

#include "ui/tabbeddocumentview.h"
#include "ui/mainwindow.h"

int main(int argc, char **args)
{
	QApplication app(argc, args);
	
        //Ui::TabbedDocumentView docView;
        //docView.show();

        Ui::MainWindow mainWindow;
        mainWindow.show();

	return app.exec();
}
