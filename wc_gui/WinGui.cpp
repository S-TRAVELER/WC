#include <iostream>
#include <QDateTime>
#include <string>
#include <regex>
#include <sstream>
#include "WinGui.h"
#include "ui_WinGui.h"
#include <QFileDialog>
#include "counter.h"


WinGui::WinGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinGui)
{
    ui->setupUi(this);
    QIcon icon("wc_icon.png");
    setWindowIcon(icon);

    RulesParser::Instance().Foreach([&](const map<string, RulesParser::RuleList_ptr>::const_iterator& it){
          ui->comboBox->addItem(QString::fromStdString(it->first));
    });
    _poller.reset(new Poller([this](const std::shared_ptr<ostream> &stream,const string &arg){
        long lcount,wcount,bcount,ncount,ecount;
         Counter::Instance().count(arg,lcount,wcount,bcount,ncount,ecount);
         if(bcount==-1){
             (*stream)<<"无法打开文件："<<arg<<endl;
             return;
         }
         (*stream)<<"File: "<<arg<<endl;
        if(ui->checkBox->isChecked()){
            (*stream)<<"行数: "<<lcount<<endl;
        }
        if(ui->checkBox_2->isChecked()){

            (*stream)<<"词数: "<<wcount<<endl;
        }
        if(ui->checkBox_3->isChecked()){

            (*stream)<<"Bytes: "<<bcount<<endl;
        }
        if(ui->checkBox_4->isChecked()){
            (*stream)<<"空行: "<<ecount<<endl;
            (*stream)<<"注释行: "<<ncount<<endl;
            (*stream)<<"代码行: "<<lcount-ecount-ncount<<endl;
        }

    }));
}

WinGui::~WinGui()
{
    delete ui;
}

void WinGui::on_pushButton_2_clicked()
{

    string filePath=ui->lineEdit->text().toStdString();

    QDateTime current_date_time =QDateTime::currentDateTime();
    QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
    ui->textBrowser->append(QString("Time: ")+current_date);

    shared_ptr<stringstream> ss(new stringstream);
    Counter::Instance().setRule(RulesParser::Instance().getRule(ui->comboBox->currentText().toStdString()));
    _poller->travel(filePath,ss,ui->checkBox_5->isChecked(),true);

    ui->textBrowser->append(QString::fromStdString(ss->str()));

}

void WinGui::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("文件对话框！"),"." );
    ui->lineEdit->setText(fileName);
}
