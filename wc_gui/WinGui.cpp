#include "WinGui.h"
#include "ui_WinGui.h"
#include <QFileDialog>
#include "counter.h"
#include <iostream>
#include <QDateTime>
#include <string>
#include <regex>

WinGui::WinGui(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WinGui)
{
    ui->setupUi(this);
}

WinGui::~WinGui()
{
    delete ui;
}

void WinGui::on_pushButton_2_clicked()
{

    string filePath=ui->lineEdit->text().toStdString();
    if(filePath.size()==0){
        return;
    }
    size_t pos=filePath.rfind('/');

    string path;
    vector<QString> fileNames;
    string file;
    if(pos!=filePath.npos){
        path=filePath.substr(0,pos);
        file=filePath.substr(pos+1);
    }else{
        path=".";
        file=filePath;
    }
    try {
        QRegExp rx(file.c_str());
        rx.setPatternSyntax(QRegExp::Wildcard);

        QDir d(path.c_str());
        const QFileInfoList list = d.entryInfoList();

        for(auto &it:list){
            QString &&tmp=it.fileName();

            if (rx.exactMatch(tmp)){
                fileNames.push_back(tmp);
            }
        }

        QDateTime current_date_time =QDateTime::currentDateTime();
        QString current_date =current_date_time.toString("yyyy.MM.dd hh:mm:ss.zzz ddd");
        ui->textBrowser->append(QString("Time: ")+current_date);

        for(auto &it:fileNames){
            long lcount,wcount,bcount,ncount,ecount;
            string tmpFile=path+"/"+it.toStdString();

            Counter::Instance().count(tmpFile,lcount,wcount,bcount,ncount,ecount);
            if(bcount==-1){
                ui->textBrowser->append(QString("打开文件失败: ")+QString::fromStdString(tmpFile)+"\n");
            }
            ui->textBrowser->append(QString("File: ")+QString::fromStdString(tmpFile));
            if(ui->checkBox->isChecked()){
                ui->textBrowser->append(QString("行数: ")+QString::number(lcount,10));
            }
            if(ui->checkBox_2->isChecked()){
                ui->textBrowser->append(QString("词数: ")+QString::number(wcount,10));
            }
            if(ui->checkBox_3->isChecked()){
                ui->textBrowser->append(QString("Bytes: ")+QString::number(bcount,10));
            }
            if(ui->checkBox_4->isChecked()){
                ui->textBrowser->append(QString("空行: ")+QString::number(ecount,10));
                ui->textBrowser->append(QString("注释行: ")+QString::number(ncount,10));
                ui->textBrowser->append(QString("代码行: ")+QString::number(lcount-ncount-ecount,10));
            }
        }
    } catch (exception &e) {
        return;
    }

}

void WinGui::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("文件对话框！"),"." );
    ui->lineEdit->setText(fileName);
}
