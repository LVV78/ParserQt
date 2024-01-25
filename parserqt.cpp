#include "parserqt.h"
#include "ui_parserqt.h"
#include <QMessageBox>
#include <QPainter>
#include <QFileDialog>
#include "Parser/Exceptions.h"
#include "Parser/FileReader.h"
#include "Parser/ParserSsd.h"
#include "Parser/StringReader.h"
#include "Parser/ParserStringTest.h"

Ui::ParserQt* ParserQtClass;
ParserQt::ParserQt(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ParserQt)
{
    ui->setupUi(this);
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(on_actionOpen_clicked()));
    connect(ui->actionNameTest, SIGNAL(triggered()), this, SLOT(on_actionNameTest_clicked()));
    connect(ui->actionNameError, SIGNAL(triggered()), this, SLOT(on_actionNameError_clicked()));
    connect(ui->actionValueError, SIGNAL(triggered()), this, SLOT(on_actionValueError_clicked()));
    ParserQtClass = ui;
}

ParserQt::~ParserQt()
{
    delete ui;
}

void previewBlock(Preview* previrew, std::string& text)
{
    auto e = ParserQtClass->textEdit;
    QTextCursor c = e->textCursor();
    e->moveCursor(QTextCursor::End);
    e->insertPlainText(QString::fromStdString(text));
    for (size_t i = 0; i < previrew->items_->size(); i++)
    {
        PreviewItem item = previrew->items_->at(i);
        c.movePosition(QTextCursor::StartOfLine);
        c.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor,(int) item.start);
        c.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor,(int) item.length);
        e->setTextCursor(c);
        switch (item.phraseType)
        {
        case ptNone:
            e->setTextColor(QColor("Black"));
            break;
        case ptName:
            e->setTextColor(QColor("blue"));
            break;
        case ptValue:
            e->setTextColor(QColor("DarkMagenta"));
            break;
        case ptArray:
            e->setTextColor(QColor("Magenta"));
            break;
        case ptError:
            e->setTextColor(QColor("red"));
            break;
        }


    }
    e->moveCursor(QTextCursor::EndOfLine);
    e->setTextColor(QColor("Black"));
    e->insertPlainText(QString::fromStdString("\r\n"));
}
void ParserQt::paint(ParserBase* provider)
{
    auto minMax = provider->minMax();
    if (minMax.width() == 0 || minMax.height() == 0)
    {
        return;
    }
    QPicture pi;
    QPainter painter(&pi);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine));
    painter.fillRect(0, 0, ui->paintLabel->width(), ui->paintLabel->height(), QColor("white"));
    double xf = ui->paintLabel->width() / minMax.width();
    double yf = ui->paintLabel->height() / minMax.height();
    Point p1, p;
    p.x = 0;
    int x, x1, y =0 , y1;
    int xPos = 0, cnt =0;
    PointMinMax mm;
    for (size_t i = 0; i < provider->count(); i++)
    {
        if (i == 0)
        {
            p = provider->points(i);
            continue;
        }
        p1 = provider->points(i);

        x1 = (int)((p1.x - minMax.minX) * xf);
        y1 = (int)std::round((p1.y - minMax.minY) * yf);
        mm.setY(p1.y);
        cnt++;
        if (x1 > xPos)
        {
            x = (int)((p.x - minMax.minX) * xf);
            y = (int)std::round((p.y - minMax.minY) * yf);
            painter.drawLine(x, y, x1, y1);
            if (cnt>1)
            {
                y = (int)std::round((mm.minY - minMax.minY) * yf);
                y1 = (int)std::round((mm.maxY - minMax.minY) * yf);
                painter.drawLine(x, y, x, y1);
            }
            mm.reset();
            cnt = 0;
            p = p1;
            xPos = x1;
        }
    }
    painter.end();
    ui->paintLabel->setPicture(pi);
}
void ParserQt::on_actionNameTest_clicked()
{
    ui->textEdit->clear();
    auto pb = new StringReader("Name= Value\rname= Value1", "\r", 12);
    auto pssd = new ParserStringTest(pb);
    pssd->previewCallback = previewBlock;
    try
    {
        pssd->parse();
    }
    catch (const ProviderException& ex)
    {
        QMessageBox messageBox;
        messageBox.critical(0, QCoreApplication::applicationName(), QString::fromStdString(ex.getMessage()));
    }
}
void ParserQt::on_actionNameError_clicked()
{
    ui->textEdit->clear();
    auto pb = new StringReader("Name= Value\rname1= Value1", "\r", 20);
    auto pssd = new ParserStringTest(pb);
    pssd->previewCallback = previewBlock;
    try
    {
        pssd->parse();
    }
    catch (const ProviderException& ex)
    {
        QMessageBox messageBox;
        messageBox.critical(0, QCoreApplication::applicationName(), QString::fromStdString(ex.getMessage()));
    }
}
void ParserQt::on_actionValueError_clicked()
{
    ui->textEdit->clear();
    auto pb = new StringReader("Name= Value\rName= ;Name2=value2", "\r", 100);
    auto pssd = new ParserStringTest(pb);
    pssd->previewCallback = previewBlock;
    try
    {
        pssd->parse();
    }
    catch (const ProviderException& ex)
    {
        QMessageBox messageBox;
        messageBox.critical(0, QCoreApplication::applicationName(), QString::fromStdString(ex.getMessage()));
    }
}
void ParserQt::on_actionOpen_clicked()
{
    // std::string fileName = "c:/0/big.ssd";
    auto fileName = QFileDialog::getOpenFileName(this,tr("Open sample file"), "", tr("Sample Files (*.ssd; *.rsd)")).toStdString();
    if (fileName == "")
    {
        return;
    }
    clock_t start, end;
    start = clock();
    ui->textEdit->clear();

    auto pb = new FileReader(fileName.c_str(),"\r\n", 1000);
    auto pssd = new ParserSsd(pb);
    pssd->previewCallback = previewBlock;
    try
    {
        pssd->parse();

    }
    catch (const ProviderException& ex)
    {
        QMessageBox messageBox;
        messageBox.critical(0, QCoreApplication::applicationName(), QString::fromStdString(ex.getMessage()));
    }
    end = clock();
    int dif = end - start;
    QString caption = "Parse time [ms]: "+QString::number(dif)+", Block count: "+QString::number(pb->blockCount());
    QWidget::setWindowTitle(caption);
    paint(pssd);
    delete pssd;

}
