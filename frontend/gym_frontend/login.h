#ifndef LOGIN_H
#define LOGIN_H

#include <nlohmann/json.hpp>

#include "transport/socketconnection.h"
#include "utils/QByteArrayConverter.hpp"

#include <QMainWindow>

#include <QTcpSocket>
#include <QDebug>
#include <QFileDialog>
#include <QImageReader>
#include <QImage>
#include <QBuffer>

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

private slots:
    void on_pushButton_clicked();

private:
    Ui::Login *ui;
    QTcpSocket * socket;
};
#endif // LOGIN_H
