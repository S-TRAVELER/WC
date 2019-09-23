#ifndef WINGUI_H
#define WINGUI_H

#include <QMainWindow>

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
};

#endif // WINGUI_H
