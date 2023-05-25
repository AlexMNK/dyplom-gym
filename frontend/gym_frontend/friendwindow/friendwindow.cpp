#include "friendwindow.h"
#include "ui_friendwindow.h"

#include <QDebug>
#include <QMessageBox>

FriendWindow::FriendWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FriendWindow)
{
    ui->setupUi(this);

    ui->backButton->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");
    ui->delete_2->setStyleSheet("QPushButton {border: 1px solid black; } QPushButton:hover { border: 1px solid darkred;}");

    this->setStyleSheet("background-color: white;");
}

FriendWindow::~FriendWindow()
{
    delete ui;
}

QString FriendWindow::GetProperRankName(int points)
{
    if (points < 2000) return "Silver";
    if (points < 5000) return "Gold";
    if (points < 10000) return "Platinum";
    if (points < 20000) return "Diamond";
    // else it is immortal
    return "Imoortal";
}

QPixmap FriendWindow::GetProperRankIcon(int points)
{
    if (points < 2000) return QPixmap(":/ranks/ranks/silver.png");
    if (points < 5000) return QPixmap(":/ranks/ranks/gold.png");
    if (points < 10000) return QPixmap(":/ranks/ranks/platinum.png");
    if (points < 20000) return QPixmap(":/ranks/ranks/diamond.png");
    // else it is immortal
    return QPixmap(":/ranks/ranks/immortal.png");
}

void FriendWindow::FriendWindowOpened(Client* clientInstance, MainUser* mainUser, FriendUser* friendUser)
{
    this->mClientInstance = clientInstance;
    this->mCurrentUser = mainUser;
    this->mFriendInstance = friendUser;
    mClientInstance->SetCurrentWindow(this);

    ui->name->setText("Name: " + mFriendInstance->GetUserName());
    ui->hashtag->setText("Hashtag: " + mFriendInstance->GetUserHashtag());
    ui->email->setText("Email: " + mFriendInstance->GetUserEmail());
    ui->points->setText("Points: " + QString::number(mFriendInstance->GetUserPoints()));
    ui->rank->setText("Rank: " + GetProperRankName(mFriendInstance->GetUserPoints()));
    ui->rankicon->setPixmap(GetProperRankIcon(mFriendInstance->GetUserPoints()));

    QString age = mFriendInstance->GetUserAge() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserAge());
    QString height = mFriendInstance->GetUserHeight() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserHeight()) + " sm";
    QString weight = mFriendInstance->GetUserWeight() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserWeight()) + " kg";
    QString bench = mFriendInstance->GetUserMaxBench() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserMaxBench()) + " kg";
    QString squat = mFriendInstance->GetUserMaxSquat() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserMaxSquat()) + " kg";
    QString deadlift = mFriendInstance->GetUserMaxDeadlift() == 0 ? "Not stated" : QString::number(mFriendInstance->GetUserMaxDeadlift()) + " kg";

    ui->age->setText("Age: " + age);
    ui->height->setText("Height: " + height);
    ui->weight->setText("Weight: " + weight);
    ui->bench->setText("Max Bench: " + bench);
    ui->squat->setText("Max Squat: " + squat);
    ui->deadlift->setText("Max Deadlift: " + deadlift);

    QPixmap pixmap;

    if (mFriendInstance->GetUserPicture() == nullptr)
    {
        pixmap = QPixmap(":/img/noimage.jpg");
    }
    else
    {
        bool loadImage = pixmap.loadFromData(QByteArray::fromHex(*mFriendInstance->GetUserPicture()));

        if (!loadImage)
        {
            QMessageBox::warning(this, "Warning", "Failed to load user image");
        }
    }

    ui->image->setPixmap(pixmap);
}

void FriendWindow::on_backButton_clicked()
{
    emit BackToMainWindow();
}

