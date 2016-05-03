#ifndef BROWSERVIEWCONTROL_H
#define BROWSERVIEWCONTROL_H

#include <QtCore/QStack>

#include <QtWebKitWidgets/QWebView>

#include "ui/common/browsernetworkaccessmanager.h"

class BrowserControl;

class BrowserViewControl: public QWebView
{
    Q_OBJECT

public:
    BrowserViewControl(BrowserControl* parent);
    virtual ~BrowserViewControl();

public:
    //QStack<QString>& getHistory();

    const QList<BrowserNetworkAccessManager::StreamData>& getStreamData();
    QString getTopHref(int x, int y);
    const QString& getTopHref();

signals:
    void onTopHref();

private slots:
    void linkHoveredEvent(const QString & link, const QString & title, const QString & textContent);
    void urlChangedEvent(const QUrl& url);
    void loadProgressEvent(int progress);
    void loadFinishedEvent();
    void loadStartedEvent();

    //void requestFinishedEvent(QNetworkReply* reply);

private:
    QString topHref;
    QString relatedUrl;
    //QStack<QString> history;
};

#endif // BROWSERVIEWCONTROL_H
