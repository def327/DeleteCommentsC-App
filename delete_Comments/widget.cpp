#include "widget.h"
#include "QVector.h"


Widget::Widget(QWidget *parent): QWidget(parent)
{

    lay = new QVBoxLayout;
    txt = new QTextEdit;

    btn1 = new QPushButton;
    btn1->setText("Set Text");
    btn1->setStyleSheet("text-transform: uppercase;"
                        "background-color: #726EF7;");


    btn2 = new QPushButton;
    btn2->setText("Delete Comments");
    btn2->setStyleSheet("text-transform: uppercase;"
                        "background-color: #726EF7;");

    btn3 = new QPushButton;
    btn3->setText("Save txt file");
    btn3->setStyleSheet("text-transform: uppercase;"
                        "background-color: #726EF7;");

    btn4 = new QPushButton;
    btn4->setText("Clear workspace");
    btn4->setStyleSheet("text-transform: uppercase;"
                        "background-color: #726EF7;");

    QFont fontForButtons("Arial",14,QFont::Bold);
    btn1->setFont(fontForButtons);
    btn2->setFont(fontForButtons);
    btn3->setFont(fontForButtons);
    btn4->setFont(fontForButtons);


    lay->addWidget(txt);
    lay->addWidget(btn1);
    lay->addWidget(btn2);
    lay->addWidget(btn3);
    lay->addWidget(btn4);

    this->setLayout(lay);
    this->setFixedSize(800,600);

    connect(btn1,SIGNAL(clicked(bool)),this,SLOT(SetText()));
    connect(btn2,SIGNAL(clicked(bool)),this,SLOT(DelComments()));
    connect(btn3,SIGNAL(clicked(bool)),this,SLOT(SaveFile()));
    connect(btn4,SIGNAL(clicked(bool)),this,SLOT(ClearWorkspace()));

    EditTextArea(txt);
}

void Widget::SetText()
{
    QString file_name_str = QFileDialog::getOpenFileName(0,
                                                 "Open file",
                                                 "",
                                                 "*.txt ;; *.cpp ;; *.h ;; All files (*.*)"
                                                 );
    if(file_name_str == "")
     {
        return;
     }
        else
        {
            QFile file(file_name_str);
            file.open(QIODevice::ReadOnly);
            QTextStream stream_out(&file);
            str=stream_out.readAll();            
            txt->setPlainText(str);
            file.close();
            EditTextArea(txt);
        }
}

void Widget::SaveFile()
{
    QString file_name_str=QFileDialog::getSaveFileName(0,
                                               tr("Save File"),
                                               "New_file.txt",
                                               "*.txt"
                                               );
    if(file_name_str =="")
     {
        return;
     }
        else
        {
            QFile file(file_name_str);
            file.open(QIODevice::WriteOnly | QIODevice::Text);
            str=txt->toPlainText();
            QByteArray arr = QByteArray::fromStdString(str.toStdString());
            file.write(arr);
            file.close();
        }

}

void Widget::ClearWorkspace()
{
    txt->clear();
    str.clear();
    EditTextArea(txt);
}

void Widget::DelComments()
{
    str = txt->toPlainText();
    QVector<QChar>::iterator iter = str.begin();

    ///---Find comments "//..." and "/*....*/"

    while(iter != str.end())
    {
        try
        {
            if(*iter == '"')
            {
                do
                {
                    iter++;
                    if(*iter == '\\' && *(iter+1) == '\"')
                    {
                        iter+=2;
                        continue;
                    }
                }
                while(*iter != '"');
                iter++;
                continue;
            }
                else if( *iter == '\'')
                {
                    do
                    {
                        iter++;
                        if(*iter == '\\' && *(iter+1) == '\'')
                        {
                            iter+=2;
                            continue;
                        }
                            else if(*iter == '\\' && *(iter+1) == '\\')
                            {
                                iter++;
                            }
                    }
                    while(*iter != '\'');
                    iter++;
                    continue;
                }
                else if(*iter == '/' && *(iter+1) == '/')   /// Find //..... commment
                {
                    DelOneLineComment(iter,str);
                }
                else if(*iter == '/' && *(iter+1) == '*')   /// Find /*....*/ commment
                {
                    DelStarLineComment(iter,str);
                }
        }catch(bool stopChecking)
        {
            if(stopChecking)
            {
                break;
            }
                else
                {

                }
        }
        iter++;
    }
    txt->setPlainText(str);
    EditTextArea(txt);
}

void Widget::EditTextArea(QTextEdit * textEdit)
{
    QFont myClassicFont("Arial", 12, QFont::Bold);
    textEdit->setFont(myClassicFont);
    textEdit->setTextColor(QColor(Qt::black));
    textEdit->setTextBackgroundColor(QColor(Qt::white));
}

void Widget::DelOneLineComment(QVector<QChar>::iterator &del_iter, QString &text_str)
{
    while(*del_iter != '\n')
    {
       *del_iter = ' ';
        del_iter++;
        if(*del_iter == '\n')
        {
            break;
        }

        if(del_iter == text_str.end())
        {
            bool stopChecking = true;
            throw stopChecking;     //exeption to stop checking the  c++ program listing
        }
    }
}

void Widget::DelStarLineComment(QVector<QChar>::iterator &del_iter, QString &text_str)
{
    *del_iter = ' ';
    del_iter++;
    *del_iter = ' ';
    del_iter++;
    while(true) // unlimited cycle to find both symbols '*' and '/'
    {                          //together at one place at the end of comment
        *del_iter = ' ';
        del_iter++;
        if((*del_iter == '*') && (*(del_iter+1) == '/'))
        {
            *del_iter = ' ';
            *(del_iter+1) = ' ';
            break;
        }

        if(del_iter == text_str.end())
        {
            bool stopChecking = true;
            throw stopChecking;     //exeption to stop checking the  c++ program listing
        }
    }
}
