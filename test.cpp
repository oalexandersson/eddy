#include <QApplication>
#include <QTextDocument>
#include <QTextEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QBoxLayout>

#include "ui/tabbeddocumentview.h"

int main(int argc, char **args)
{
	QApplication app(argc, args);
	
        Ui::TabbedDocumentView docView;
        docView.show();

	return app.exec();
}
