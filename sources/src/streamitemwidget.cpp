#include "../include/streamitemwidget.h"
#include "ui_streamitemwidget.h"

StreamItemWidget::StreamItemWidget(const Stream &stream) :
    QWidget(),
    ui(new Ui::StreamItemWidget),
    stream(stream)
{
    ui->setupUi(this);
    ui->streamerName->setText(stream.getDisplayName());
    ui->gameText->setText(stream.getGame());


    QPixmap icon(stream.getServiceLogoResource());
    ui->serviceIcon->setPixmap(icon);
}

StreamItemWidget::~StreamItemWidget()
{
    delete ui;
}

void StreamItemWidget::on_playStreamButton_clicked()
{
    emit play(stream.getUrl());
}

void StreamItemWidget::on_previewStreamButton_clicked()
{
    emit goToPreview();
    emit fetchStreamPreview(stream);
}

void StreamItemWidget::on_favoriteButton_clicked()
{
    emit addFavorite(stream);
}
