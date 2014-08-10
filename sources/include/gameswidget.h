#ifndef GAMESWIDGET_H
#define GAMESWIDGET_H

#include <QWidget>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QScroller>
#include "gamesitemdelegate.h"
#include "game.h"

namespace Ui {
class GamesWidget;
}

class GamesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GamesWidget(QWidget *parent = 0);
    void addGame(const Game& game);
    ~GamesWidget();

private:
    void clearGames();
    void setupKineticScroller(QObject *target);

    Ui::GamesWidget *ui;

    // model and proxy for the Games tab
    QStandardItemModel gamesModel;
    QSortFilterProxyModel gamesSortProxy;
    GamesItemDelegate gamesDelegate;

signals:
    void fetchGamesByService(const Service& service);
    void fetchStreamsByGame(const Game& game);
    void goToStreamsWidget();

public slots:

private slots:
    void searchInGameList(QString game);
    void on_gamesItemActivated(const QModelIndex& index);
    void on_twitchButton_clicked();
    void on_azubuButton_clicked();
};

#endif // GAMESWIDGET_H
