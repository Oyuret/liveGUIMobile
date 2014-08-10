#include "../include/streamswidget.h"
#include "ui_streamswidget.h"

StreamsWidget::StreamsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StreamsWidget)
{
    ui->setupUi(this);
    setupKineticScroller(ui->streamListWidget);
}

StreamsWidget::~StreamsWidget()
{
    delete ui;
}

void StreamsWidget::addStream(const Stream &stream)
{
    QListWidgetItem* streamItem = new QListWidgetItem();
    StreamItemWidget* streamItemWidget = new StreamItemWidget(stream);

    connectStreamItemWidget(streamItemWidget);
    streamItem->setSizeHint(streamItemWidget->sizeHint());

    ui->streamListWidget->addItem(streamItem);
    ui->streamListWidget->setItemWidget(streamItem,streamItemWidget);
}

void StreamsWidget::connectStreamItemWidget(StreamItemWidget* streamItemWidget)
{
    QObject::connect(streamItemWidget, SIGNAL(fetchStreamPreview(const Stream&)),
                     this,SIGNAL(fetchStreamPreview(const Stream&)));

    QObject::connect(streamItemWidget, SIGNAL(goToPreview()),this,SIGNAL(goToPreview()));

    QObject::connect(streamItemWidget, SIGNAL(play(QString)),this,SIGNAL(play(QString)));

    QObject::connect(streamItemWidget,SIGNAL(addFavorite(const Stream&)),
                     this,SIGNAL(addFavorite(const Stream&)));
}

void StreamsWidget::clear_streams()
{
    ui->streamListWidget->clear();
}

void StreamsWidget::setupKineticScroller(QObject *target)
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

void StreamsWidget::on_backToGamesButton_clicked()
{
    clear_streams();
    emit goToGamesWidget();
}
