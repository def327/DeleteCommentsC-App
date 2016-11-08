#ifndef WIDGET_H
#define WIDGET_H

#include "QtWidgets"

class Widget : public QWidget
{
    Q_OBJECT
private:
    QString str;
    QVBoxLayout *lay;
    QTextEdit *txt;

    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    QPushButton *btn4;

    void EditTextArea(QTextEdit *textEdit);


public:
    Widget(QWidget *parent = 0);

public slots:
    void DelComments();
    void SetText();
    void SaveFile();
    void ClearWorkspace();
};

#endif // WIDGET_H
