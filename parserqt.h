#ifndef PARSERQT_H
#define PARSERQT_H

#include <QMainWindow>
#include "Parser/ParserBase.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class ParserQt;
}
QT_END_NAMESPACE

class ParserQt : public QMainWindow
{
    Q_OBJECT

public:
    ParserQt(QWidget *parent = nullptr);
    ~ParserQt();
    void paint(ParserBase* provider);
private:
    Ui::ParserQt *ui;
private slots:
    void on_actionOpen_clicked();
};
#endif // PARSERQT_H
