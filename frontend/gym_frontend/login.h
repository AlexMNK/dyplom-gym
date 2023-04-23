#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

#include <QTcpSocket>
#include <QDebug>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class Login; }
QT_END_NAMESPACE

class Login : public QMainWindow
{
    Q_OBJECT

public:
    Login(QWidget *parent = nullptr);
    ~Login();

private:
    Ui::Login *ui;
};
#endif // LOGIN_H
