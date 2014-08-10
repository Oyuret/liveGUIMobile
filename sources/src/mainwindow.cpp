#include "../include/mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    setupPreview();
    setupFavorites();
    setupBrowsingwidget();
    //loadSettings();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::play(QString url) {
    remote.play(url,"http://192.168.1.108:1337");
}

void MainWindow::setupPreview()
{
    // connect signal to show preview
    QObject::connect(&network, SIGNAL(setPreview(const Stream&)),
                     ui->previewStreamWidget, SLOT(on_setPreview(const Stream&)));

    // connect signal to reset preview
    QObject::connect(&network, SIGNAL(resetPreview()),
                     ui->previewStreamWidget, SLOT(on_resetPreview()));

    // connect play to this
    QObject::connect(ui->previewStreamWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

    // connect add favorite to this
    QObject::connect(ui->previewStreamWidget,SIGNAL(addFavorite(const Stream&)),
                     ui->favoriteWidget,SLOT(on_addFavorite(const Stream&)));
}

void MainWindow::setupFavorites()
{
    // connect go to preview
    QObject::connect(ui->favoriteWidget, SIGNAL(goToPreview()), this, SLOT(on_goToPreview()));

    // connect add favorite refresh to network
    QObject::connect(ui->favoriteWidget,SIGNAL(fetchStreamStatus(const Stream&)),
                     &network,SLOT(on_fetchStreamStatus(const Stream&)));

    // connect play
    QObject::connect(ui->favoriteWidget,SIGNAL(play(QString)),
                     this,SLOT(play(QString)));

    // connect preview
    QObject::connect(ui->favoriteWidget,SIGNAL(fetchStreamPreview(const Stream&)),
                     &network,SLOT(on_fetchStreamPreview(const Stream&)));

    // loading and closing
    QObject::connect(this,SIGNAL(load_favs()),
                     ui->favoriteWidget,SLOT(on_loadFavorites()));

    QObject::connect(this,SIGNAL(save_favs()),
                     ui->favoriteWidget,SLOT(on_saveFavorites()));

    // stream status
    QObject::connect(&network,SIGNAL(streamOnline(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamOnline(const Stream&)));
    QObject::connect(&network,SIGNAL(streamOffline(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamOffline(const Stream&)));
    QObject::connect(&network,SIGNAL(streamUncertain(const Stream&)),
                     ui->favoriteWidget,SLOT(on_streamUncertain(const Stream&)));

}

void MainWindow::setupBrowsingwidget()
{
    QObject::connect(ui->browsingWidget,SIGNAL(fetchGamesByService(const Service&)),
                     &network, SLOT(on_fetchGamesByService(const Service&)));
    QObject::connect(&network, SIGNAL(addGame(const Game&)),
                     ui->browsingWidget, SLOT(on_addGame(const Game&)));


    QObject::connect(ui->browsingWidget, SIGNAL(fetchStreamsByGame(const Game&)),
                     &network, SLOT(on_fetchStreamsByGame(const Game&)));
    QObject::connect(&network, SIGNAL(addStream(const Stream&)),
                     ui->browsingWidget, SLOT(on_addStream(const Stream&)));

    QObject::connect(ui->browsingWidget,SIGNAL(fetchStreamPreview(const Stream&)),
                     &network,SLOT(on_fetchStreamPreview(const Stream&)));
    QObject::connect(ui->browsingWidget,SIGNAL(goToPreview()),
                     this, SLOT(on_goToPreview()));

    QObject::connect(ui->browsingWidget,SIGNAL(addFavorite(const Stream&)),
                     ui->favoriteWidget,SLOT(on_addFavorite(const Stream&)));

    QObject::connect(ui->browsingWidget, SIGNAL(play(QString)),
                     this, SLOT(play(QString)));

}

void MainWindow::loadSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);
    emit load_favs();
}

void MainWindow::saveSettings()
{
    QSettings settings(QApplication::applicationDirPath() + "/settings.ini", QSettings::IniFormat);

    emit save_favs();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    //saveSettings();
}

void MainWindow::on_goToPreview()
{
    ui->tabWidget->setCurrentIndex(PREVIEW_TAB);
}
