#include "../include/favoritewidget.h"
#include "ui_favoritewidget.h"

FavoriteWidget::FavoriteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FavoriteWidget)
{
    ui->setupUi(this);
    setupKineticScroller(ui->favListWidget);
}

FavoriteWidget::~FavoriteWidget()
{
    delete ui;
}

void FavoriteWidget::on_addFavorite(const Stream &stream)
{
    if(alreadyInFavorites(stream))
        return;

    QListWidgetItem* favoriteItem = new QListWidgetItem();
    FavoriteItemWidget* favoriteItemWidget = new FavoriteItemWidget(stream, favoriteItem);

    favoriteItem->setSizeHint(favoriteItemWidget->sizeHint());
    connectFavoriteItemWidget(favoriteItemWidget);

    ui->favListWidget->addItem(favoriteItem);
    ui->favListWidget->setItemWidget(favoriteItem,favoriteItemWidget);
}

bool FavoriteWidget::alreadyInFavorites(const Stream &stream) const
{
    return !findFavorite(stream).isEmpty();
}

void FavoriteWidget::setupKineticScroller(QObject *target)
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

QList<FavoriteItemWidget *> FavoriteWidget::findFavorite(const Stream &stream) const
{
    QList<FavoriteItemWidget*> foundItems;

    for(int row=0; row < ui->favListWidget->count(); ++row) {
        FavoriteItemWidget* favoriteItemWidget = getFavoriteItemWidget(row);
        Stream favoriteItemStream = favoriteItemWidget->getStream();

        if(favoriteItemStream.equals(stream)) {
            foundItems.append(favoriteItemWidget);
        }
    }

    return foundItems;
}

void FavoriteWidget::connectFavoriteItemWidget(FavoriteItemWidget* favoriteItemWidget)
{
    QObject::connect(favoriteItemWidget, SIGNAL(goToPreview()),
                     this,SIGNAL(goToPreview()));
    QObject::connect(favoriteItemWidget, SIGNAL(fetchStreamPreview(const Stream&)),
                     this,SIGNAL(fetchStreamPreview(const Stream&)));
    QObject::connect(favoriteItemWidget, SIGNAL(play(QString)),
                     this,SIGNAL(play(QString)));
    QObject::connect(favoriteItemWidget, SIGNAL(removeFavorite(QListWidgetItem*)),
                     this,SLOT(removeFavorite(QListWidgetItem*)));
}

void FavoriteWidget::on_loadFavorites()
{
    QString path ;
    QString filename;

    path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) ;
    filename = "liveGUIconfig.ini" ;
    QSettings settings(path + "/" + filename,QSettings::IniFormat);

    int numberOfFavorites = settings.beginReadArray("favorites");

    for(int i=0; i < numberOfFavorites; ++i) {
        settings.setArrayIndex(i);

        QString displayName = settings.value("displayName").toString();
        QString channelName = settings.value("channelName").toString();
        QString url = settings.value("url").toString();
        QString serviceName = settings.value("serviceName").toString();
        QString serviceLogoResource = settings.value("serviceLogoResource").toString();

        FavoriteStream favorite(displayName,channelName,url,serviceName,serviceLogoResource);
        on_addFavorite(favorite);
    }

    settings.endArray();
}

FavoriteItemWidget * FavoriteWidget::getFavoriteItemWidget(int row) const
{
    QListWidgetItem *item = ui->favListWidget->item(row);
    FavoriteItemWidget* widget = qobject_cast<FavoriteItemWidget*>(ui->favListWidget->itemWidget(item));

    return widget;
}

void FavoriteWidget::on_saveFavorites()
{
    QString path ;
    QString filename;

    path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) ;
    filename = "liveGUIconfig.ini" ;
    QSettings settings(path + "/" + filename,QSettings::IniFormat);

    settings.beginWriteArray("favorites");

    for(int row=0; row < ui->favListWidget->count(); ++row) {
        settings.setArrayIndex(row);

        FavoriteItemWidget* widget = getFavoriteItemWidget(row);
        Stream stream = widget->getStream();

        settings.setValue("displayName", stream.getDisplayName());
        settings.setValue("channelName", stream.getChannelName());
        settings.setValue("url", stream.getUrl());
        settings.setValue("serviceName", stream.getServiceName());
        settings.setValue("serviceLogoResource", stream.getServiceLogoResource());
    }

    settings.endArray();
}

void FavoriteWidget::on_streamOnline(const Stream &stream)
{
    QList<FavoriteItemWidget*> favorite = findFavorite(stream);

    for(FavoriteItemWidget* item : favorite) {
        item->setOnline();
    }
}

void FavoriteWidget::on_streamOffline(const Stream &stream)
{
    QList<FavoriteItemWidget*> favorite = findFavorite(stream);

    for(FavoriteItemWidget* item : favorite) {
        item->setOffline();
    }
}

void FavoriteWidget::on_streamUncertain(const Stream &stream)
{
    QList<FavoriteItemWidget*> favorite = findFavorite(stream);

    for(FavoriteItemWidget* item : favorite) {
        item->setChecking();
    }
}

void FavoriteWidget::removeFavorite(QListWidgetItem *item)
{
    int row = ui->favListWidget->row(item);
    QListWidgetItem* removed = ui->favListWidget->takeItem(row);
    delete removed;
}

void FavoriteWidget::on_refreshStatusButton_clicked()
{
    for(int row = 0; row < ui->favListWidget->count(); row++)
    {
        FavoriteItemWidget* favoriteItemWidget = getFavoriteItemWidget(row);
        Stream stream = favoriteItemWidget->getStream();

        favoriteItemWidget->setChecking();
        emit fetchStreamStatus(stream);
    }
}


FavoriteWidget::FavoriteStream::FavoriteStream(QString displayName, QString channelName, QString url, QString serviceName, QString logo)
{
    this->displayName = displayName;
    this->channelName = channelName;
    this->url = url;
    this->serviceName = serviceName;
    this->serviceLogoResource = logo;
}
