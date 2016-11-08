#include "widget.h"
#include "QVector.h"

Widget::Widget(QWidget *parent): QWidget(parent)
{
    lay = new QVBoxLayout;
    txt = new QTextEdit;

    btn1 = new QPushButton;
    btn1->setText("Set Text");

    btn2 = new QPushButton;
    btn2->setText("Delete Comments");

    btn3 = new QPushButton;
    btn3->setText("Save txt file");

    btn4 = new QPushButton;
    btn4->setText("Clear workspace");

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
}

Widget::~Widget()
{

}

void Widget::SetText()
{
    QString file_name_str = QFileDialog::getOpenFileName(0,
                                                 "Open file",
                                                 "",
                                                 "*.txt ;; *.cpp ;; *.h ;; All files (*.*)"
                                                 );
    if(file_name_str =="")
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
}

void Widget::DelComments()
{
    str = txt->toPlainText();
    QVector<QChar>::iterator iter = str.begin();

    ///---Find comments "//___//"


    while(iter != str.end())
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


        if(*iter == '/' && *(iter+1) == '/')
        {
            while(*iter != '\n')
            {
               *iter = ' ';
                iter++;
                if(*iter == '\n')
                {
                    break;
                }
            }
        }
        iter++;
    }


    ///---Find comments "/*___*/"

    iter = str.begin();

    while(iter != str.end())
    {
        if(*iter == '"')
        {
            do
            {
                iter++;
                if(*iter == '\\' && *(iter+1) == '\"')
                {
                    iter = iter+2;
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

            if(*iter == '/' && *(iter+1) == '*')
            {
                *iter = ' ';
                iter++;
                *iter = ' ';
                iter++;

                ///--- New Fix bug fix

                    /*
                    *   find this comment
                    *
                    */

                while((*iter != '*' && *(iter+1) != '/') | (*iter == '*' && *(iter+1) != '/'))
                {
                    *iter = ' ';
                    iter++;
                    if(*iter == '*' && *(iter+1) == '/')
                    {
                        *iter = ' ';
                        *(iter+1) = ' ';
                        break;
                    }
                }
            }

         iter++;
    }

    txt->setPlainText(str);
}
