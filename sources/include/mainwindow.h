#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "networkmanager.h"
#include "remoteplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum TABS {
        BROWSE_TAB,
        FAVORITE_TAB,
        PREVIEW_TAB
    };

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    // The network manager in charge of making requests
    NetworkManager network;

    // The remote player
    RemotePlayer remote;


    // setup connections and other stuff
    void setupPreview();
    void setupFavorites();
    void setupBrowsingwidget();

    // settings
    void loadSettings();
    void saveSettings();

    // closing
    void closeEvent(QCloseEvent *event);

signals:
    void load_favs();
    void save_favs();


public slots:

    // Livestream stuff
    void play(QString url);

    // preview stuff
    void on_goToPreview();
};

#endif // MAINWINDOW_H
