#include "VersionChecker.hpp"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

namespace D2ModGen {

namespace {
const QString authorName      = "mapron";
const QString projectName     = "d2modgen";
const QString apiTemplate     = "https://api.github.com/repos/%1/%2/tags?per_page=1";
const QString releaseTemplate = "https://github.com/%1/%2/releases/tag/%3";
}

VersionChecker::VersionChecker(QObject* parent)
    : QObject(parent)
{
    m_currentVersion = APP_VERSION;
    updateTxt();
}

void VersionChecker::checkForUpdates()
{
    if (m_state != State::AppStart && m_state != State::Error)
        return;

    m_state = State::StartedCheck;
    updateTxt();

    // GitHub API requires a User-Agent header, otherwise it returns a 403 Forbidden
    QNetworkRequest request(QUrl(apiTemplate.arg(authorName).arg(projectName)));

    QString userAgent = QString("%1/%2").arg(projectName).arg(m_currentVersion);
    request.setHeader(QNetworkRequest::UserAgentHeader, userAgent);
    request.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply* reply = m_networkManager.get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onNetworkReply(reply, reply->error());
    });
    connect(reply, &QNetworkReply::errorOccurred, this, [this, reply](QNetworkReply::NetworkError error) {
        onNetworkReply(reply, error);
    });
}

void VersionChecker::onNetworkReply(QNetworkReply* reply, QNetworkReply::NetworkError error)
{
    reply->deleteLater();

    if (error != QNetworkReply::NetworkError::NoError) {
        qWarning() << "Reply error: " << reply->errorString();
        onError("HTTP request fail");
        return;
    }

    QByteArray responseData = reply->readAll();
    qDebug() << "Reply json: " << responseData;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

    if (!jsonDoc.isArray()) {
        onError("bad API response");
        return;
    }

    QJsonArray jsonArray = jsonDoc.array();
    if (jsonArray.isEmpty()) {
        onError("bad API response");
        return;
    }

    // Grab the [0]["name"] element exactly as planned
    QJsonObject firstTag = jsonArray.at(0).toObject();
    if (firstTag.contains("name")) {
        m_latestVersion = firstTag.value("name").toString();
        m_state         = State::VersionDetected;
        updateTxt();
    } else {
        onError("bad API response");
    }
}

void VersionChecker::updateTxt()
{
    m_versionTxt = "Current: " + m_currentVersion + ", latest: ";
    if (m_state == State::AppStart) {
        m_versionTxt += "?";
    } else if (m_state == State::StartedCheck) {
        m_versionTxt += tr("checking...");
    } else if (m_state == State::Error) {
        m_versionTxt += tr("error=") + m_error;
    } else if (m_state == State::VersionDetected) {
        if (m_currentVersion == m_latestVersion) {
            m_versionTxt += m_latestVersion;
        } else {
            auto releasePageUrl = releaseTemplate.arg(authorName).arg(projectName).arg(m_latestVersion);
            m_versionTxt += QString("<a href=\"%1\">%2</a>").arg(releasePageUrl).arg(m_latestVersion);
        }
    }
    emit versionTxtChanged();
}

void VersionChecker::onError(QString err)
{
    m_state = State::Error;
    m_error = err;
    updateTxt();
}

}
