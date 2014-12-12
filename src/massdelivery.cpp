//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

#include "massdelivery.h"
#include <localization.hpp>
#include <querypool.hpp>
#include <ui_mainwindow.h>
#include <syslog.hpp>
#include <qdeclarative.h>

MassDelivery::MassDelivery(QDeclarativeItem *parent): QDeclarativeItem(parent)
{
    // By default, QDeclarativeItem does not draw anything. If you subclass
    // QDeclarativeItem to create a visual item, you will need to uncomment the
    // following line:

    // setFlag(ItemHasNoContents, false);
    this->menu = NULL;
    this->form = NULL;
}

MassDelivery::~MassDelivery()
{
    delete this->form;
    delete this->menu;
}

bool MassDelivery::Register()
{
    Huggle::Core::HuggleCore = (Huggle::Core*) this->HuggleCore;
    Huggle::QueryPool::HugglePool = Huggle::Core::HuggleCore->HGQP;
    Huggle::Localizations::HuggleLocalizations = (Huggle::Localizations*) this->Localization;
    Huggle::Syslog::HuggleLogs = Huggle::Core::HuggleCore->HuggleSyslog;
    Huggle::GC::gc = Huggle::Core::HuggleCore->gc;
    Huggle::Query::NetworkManager = this->Networking;
    Huggle::Configuration::HuggleConfiguration = (Huggle::Configuration*) this->Configuration;
    return true;
}

bool MassDelivery::IsWorking()
{
    return true;
}

void MassDelivery::Hook_MainWindowOnLoad(void *window)
{
    // here we need to make a menu item
    Huggle::MainWindow *w = (Huggle::MainWindow*)window;
    this->menu = new QAction("Send mass message", w->ui->menuFile);
    w->ui->menuFile->insertAction(w->ui->actionExit, this->menu);
    connect(this->menu, SIGNAL(triggered()), this, SLOT(OnClick()));
}

void MassDelivery::OnClick()
{
    if (this->form != NULL)
    {
        delete this->form;
    }
    form = new DeliveryForm();
    form->show();
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2("org.huggle.extension.qt", enwiki)
#endif