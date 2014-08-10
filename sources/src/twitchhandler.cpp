#include "../include/twitchhandler.h"

TwitchHandler::TwitchHandler()
{
}

QUrl TwitchHandler::getGamesUrl() const
{
    return urls.at(GAMES);
}

QUrl TwitchHandler::getStreamsUrl(QString game) const
{
    QUrlQuery query;

    query.addQueryItem("game",game);
    query.addQueryItem("limit","100");


    QUrl url = urls.at(STREAMS);
    url.setQuery(query);

    return url;
}

QUrl TwitchHandler::getPreviewUrl(QString channelName) const
{
    QString urlString = urls.at(PREVIEW).toString();
    urlString.append(channelName);

    QUrl url(urlString);

    return url;
}

QUrl TwitchHandler::getStatusUrl(QString channelName) const
{
    QString urlString = urls.at(STATUS).toString();
    urlString.append(channelName);

    QUrl url(urlString);
    return url;

}

void TwitchHandler::handleGames()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonArray topArray = responseObject["top"].toArray();

    for(int i=0; i<topArray.size(); ++i) {

        QJsonObject top = topArray[i].toObject();
        TwitchGame game;

        // populate the game
        game.read(top);

        // emit the signal
        emit addGame(game);
    }

    // See if there were more games to fetch
    QJsonValue moreGamesAvailable = responseObject["_links"].toObject()["next"];
    int totalGames = responseObject["_total"].toInt();
    if(!moreGamesAvailable.isNull() && !moreGamesAvailable.isUndefined()) {
        QUrlQuery nextQuery(moreGamesAvailable.toString());
        int offset = nextQuery.queryItemValue("offset").toInt();
        if(totalGames > offset) emit fetch_next_games(moreGamesAvailable.toString(), Service("twitch"));
    }

}

void TwitchHandler::handleStreams() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonArray streamsArray = responseObject["streams"].toArray();

    for(int i=0; i<streamsArray.size(); ++i) {

        QJsonObject streamJson = streamsArray[i].toObject();
        TwitchStream stream;

        // populate the stream
        stream.read(streamJson);

        // emit signal
        emit addStream(stream);
    }

}

void TwitchHandler::handlePreview() {

    QNetworkReply *reply = qobject_cast<QNetworkReply*>(QObject::sender());
    QByteArray data = reply->readAll();

    // delete the reply
    reply->deleteLater();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();


    QJsonObject streamJson = responseObject["stream"].toObject();
    QJsonValue online = responseObject["stream"];

    if(!online.isNull() && !online.isUndefined()) {
        TwitchStream stream;
        stream.read(streamJson);
        emit setPreview(stream);
    } else {
        emit resetPreview();
    }

}

void TwitchHandler::handleStatus(const Stream &stream, QNetworkReply* reply)
{

    if(reply->error() != QNetworkReply::NoError) {
        emit streamUncertain(stream);
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();

    QJsonDocument response(QJsonDocument::fromJson(data));
    QJsonObject responseObject = response.object();

    QJsonValue online = responseObject["stream"];

    if(online.isNull()) {
        emit streamOffline(stream);
    } else if(online.isUndefined()) {
        emit streamUncertain(stream);
    } else {
        emit streamOnline(stream);
    }

    reply->deleteLater();
}


void TwitchHandler::TwitchGame::read(QJsonObject gameJson)
{
    QString viewers = QString::number(qRound(gameJson["viewers"].toDouble()));
    QString channels = QString::number(qRound(gameJson["channels"].toDouble()));

    QJsonObject gameInner = gameJson["game"].toObject();
    QString name = gameInner["name"].toString();

    QString serviceName = TwitchHandler::getServiceName();
    QString serviceLogoResource = ":/icons/icons/twitchicon.png";

    this->name = name;
    this->viewers = viewers;
    this->channels = channels;

    this->serviceName = serviceName;
    this->serviceLogoResource = serviceLogoResource;
}


void TwitchHandler::TwitchStream::read(QJsonObject streamJson)
{
    QString game = streamJson["game"].toString();
    QString viewers = QString::number(qRound(streamJson["viewers"].toDouble()));

    QJsonObject channel = streamJson["channel"].toObject();

    QString status = channel["status"].toString().replace(QString("\n"),QString(""));
    QString url = channel["url"].toString();
    QString displayName = channel["display_name"].toString();
    QString channelName = channel["name"].toString();

    QString delay = QString::number(qRound(channel["delay"].toDouble()));
    QString logoUrl = channel["logo"].toString();

    QJsonObject preview = streamJson["preview"].toObject();
    QString previewUrl = preview["large"].toString();

    QString serviceName = TwitchHandler::getServiceName();
    QString serviceLogoResource = ":/icons/icons/twitchicon.png";

    this->displayName = displayName;
    this->channelName = channelName;
    this->status = status;
    this->url = url;
    this->game = game;
    this->viewers = viewers;
    this->delay = delay;
    this->logoUrl = logoUrl;
    this->previewUrl = previewUrl;

    this->serviceName = serviceName;
    this->serviceLogoResource = serviceLogoResource;

}
