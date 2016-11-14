#ifndef WIDGET_H
#define WIDGET_H

#include "QtWidgets"

class Widget : public QWidget
{
    Q_OBJECT
private:
    QString txtStr;
    QVBoxLayout *lay;
    QPlainTextEdit *txtArea;

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;

    void DelOneLineComment(QVector<QChar>::iterator &delIter, QString &textStr);
    void DelStarLineComment(QVector<QChar>::iterator &delIter, QString &textStr);

public:
    Widget(QWidget *parent = 0);

public slots:
    void DelComments();
    void SetText();
    void SaveFile();
    void ClearWorkspace();
};

#endif // WIDGET_H
