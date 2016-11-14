#include "widget.h"
#include "QVector.h"


Widget::Widget(QWidget *parent): QWidget(parent)
{

    lay = new QVBoxLayout;
    txtArea = new QPlainTextEdit;

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


    lay->addWidget(txtArea);
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
            txtStr=stream_out.readAll();
            txtArea->setPlainText(txtStr);
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
            txtStr=txtArea->toPlainText();
            QByteArray arr = QByteArray::fromStdString(txtStr.toStdString());
            file.write(arr);
            file.close();
        }

}

void Widget::ClearWorkspace()
{
    txtArea->clear();
    txtStr.clear();
}

void Widget::DelComments()
{
    txtStr = txtArea->toPlainText();
    QVector<QChar>::iterator iter = txtStr.begin();

    ///---Find comments "//..." and "/*....*/"

    while(iter != txtStr.end())
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
                    DelOneLineComment(iter,txtStr);
                }
                else if(*iter == '/' && *(iter+1) == '*')   /// Find /*....*/ commment
                {
                    DelStarLineComment(iter,txtStr);
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
    txtArea->setPlainText(txtStr);

}

void Widget::DelOneLineComment(QVector<QChar>::iterator &delIter, QString &textStr)
{
    while(*delIter != '\n')
    {
       *delIter = ' ';
        delIter++;
        if(*delIter == '\n')
        {
            break;
        }

        if(delIter == textStr.end())
        {
            bool stopChecking = true;
            throw stopChecking;     //exeption to stop checking the  c++ program listing
        }
    }
}

void Widget::DelStarLineComment(QVector<QChar>::iterator &del_iter, QString &textStr)
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

        if(del_iter == textStr.end())
        {
            bool stopChecking = true;
            throw stopChecking;     //exeption to stop checking the  c++ program listing
        }
    }
}



