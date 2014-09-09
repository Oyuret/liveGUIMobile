#include "../include/gameswidget.h"
#include "ui_gameswidget.h"

GamesWidget::GamesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GamesWidget)
{
    ui->setupUi(this);

    gamesSortProxy.setSourceModel(&gamesModel);
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    ui->gamesListView->setModel(&gamesSortProxy);
    ui->gamesListView->setItemDelegate(&gamesDelegate);

    // connect an activated event to browsing streams
    QObject::connect(ui->gamesListView, SIGNAL(clicked(QModelIndex)),
                     this, SLOT(on_gamesItemActivated(QModelIndex)));

    setupKineticScroller(ui->gamesListView);

    // searching
    gamesSortProxy.setFilterCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(ui->searchLineEdit,SIGNAL(textChanged(QString)),this,SLOT(searchInGameList(QString)));

}

void GamesWidget::setupKineticScroller(QObject* target)
{
    QScrollerProperties sp;

    sp.setScrollMetric(QScrollerProperties::DragVelocitySmoothingFactor, 0.6);
    sp.setScrollMetric(QScrollerProperties::MinimumVelocity, 0.0);
    sp.setScrollMetric(QScrollerProperties::MaximumVelocity, 0.5);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickMaximumTime, 0.4);
    sp.setScrollMetric(QScrollerProperties::AcceleratingFlickSpeedupFactor, 1.2);
    sp.setScrollMetric(QScrollerProperties::SnapPositionRatio, 0.2);
    sp.setScrollMetric(QScrollerProperties::MaximumClickThroughVelocity, 0);
    sp.setScrollMetric(QScrollerProperties::DragStartDistance, 0.001);
    sp.setScrollMetric(QScrollerProperties::MousePressEventDelay, 0.5);
    QScroller* scroller = QScroller::scroller(target);

    scroller->grabGesture(target, QScroller::LeftMouseButtonGesture);

    scroller->setScrollerProperties(sp);
}

GamesWidget::~GamesWidget()
{
    delete ui;
}

void GamesWidget::addGame(const Game &game)
{
    QStandardItem* item = new QStandardItem(game.getName());
    item->setEditable(false);
    item->setData(game.getName(), ROLE_NAME);
    item->setData(game.getViewers(), ROLE_VIEWERS);
    item->setData(game.getChannels(), ROLE_CHANNEL_NR);
    item->setData(game.getServiceName(),ROLE_SERVICE_NAME);
    item->setData(game.getServiceLogoResource(), ROLE_SERVICE_LOGO);
    gamesModel.appendRow(item);

}

void GamesWidget::searchInGameList(QString game)
{
    gamesSortProxy.setFilterWildcard(game);
}

void GamesWidget::on_gamesItemActivated(const QModelIndex& index)
{
    emit goToStreamsWidget();

    QString name = index.data(ROLE_NAME).toString();
    QString serviceName = index.data(ROLE_SERVICE_NAME).toString();

    Game game(name,serviceName);

    emit fetchStreamsByGame(game);
}

void GamesWidget::on_twitchButton_clicked()
{
    clearGames();
    Service twitch("twitch");
    emit fetchGamesByService(twitch);
}

void GamesWidget::on_azubuButton_clicked()
{
    clearGames();
    Service azubu("azubu");
    emit fetchGamesByService(azubu);
}

void GamesWidget::clearGames()
{
    gamesModel.clear();
}
