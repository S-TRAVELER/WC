#ifndef WINGUI_H
#define WINGUI_H

#include <QMainWindow>
#include "poller.h"

namespace Ui {
class WinGui;
}

class WinGui : public QMainWindow
{
    Q_OBJECT

public:
    explicit WinGui(QWidget *parent = nullptr);
    ~WinGui();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


private:
    Ui::WinGui *ui;
    shared_ptr<Poller> _poller;
};

#endif // WINGUI_H
