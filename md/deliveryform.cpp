//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "deliveryform.h"
#include "ui_deliveryform.h"

DeliveryForm::DeliveryForm(QWidget *parent) : QDialog(parent), ui(new Ui::DeliveryForm)
{
    CurrentUser = 0;
    this->t = new QTimer();
    ui->setupUi(this);
    ui->lineEdit_2->setText(Huggle::Configuration::HuggleConfiguration->UserName + " is delivering a mass message "
                            + Huggle::Configuration::HuggleConfiguration->EditSuffixOfHuggle);
}

DeliveryForm::~DeliveryForm()
{
    delete this->t;
    delete ui;
}

void DeliveryForm::OnTime()
{
    if (Users.count() > 0)
    {
        QString text = ui->textEdit_2->toPlainText();
        Huggle::WikiUser *wu = this->Users.at(0);
        this->Users.removeAt(0);
        text = text.replace("$target_user", wu->Username);
        Huggle::Core::HuggleCore->MessageUser(wu, text, ui->lineEdit->text(), ui->lineEdit_2->text(), true, NULL, true);
        CurrentUser++;
        Refresh();
        delete wu;
    } else
    {
        t->stop();
    }
}

void DeliveryForm::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    this->Users.clear();
    QString text = ui->textEdit->toPlainText();
    text = text.replace("\n", "");
    text = text.replace(" ", "");
    while (text.contains(","))
    {
        if (text.startsWith(","))
        {
            text = text.mid(1);
            continue;
        }
        QString user = text.mid(0, text.indexOf(","));
        text = text.mid(text.indexOf(",") + 1);
        Users.append(new Huggle::WikiUser(user));
    }
    if (text != "")
    {
        Users.append(new Huggle::WikiUser(text));
    }

    connect(this->t, SIGNAL(timeout()), this, SLOT(OnTime()));
    this->t->start(6000);
    this->CurrentUser = 0;
}

void DeliveryForm::Refresh()
{
    ui->pushButton->setText("Sending (" + QString::number(this->CurrentUser) + "/" + QString::number(this->Users.count()) + ")");
}
