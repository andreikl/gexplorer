#include <QtCore/QDebug>

#include <QtNetwork/QNetworkReply>

#include <QtGui/QMouseEvent>

#include <QtWidgets/QStatusBar>
#include <QtWidgets/QLineEdit>

#include <QtWebKit/QWebElement>

#include <QtWebKitWidgets/QWebFrame>

#include "ui/controls/browsercontrol.h"

#include "ui/controls/browserviewcontrol.h"

extern QStatusBar* bar;

BrowserViewControl::BrowserViewControl(BrowserControl* parent): QWebView(parent)
{
    page()->setNetworkAccessManager(new BrowserNetworkAccessManager(this));

    connect(this, SIGNAL(loadStarted()), this, SLOT(loadStartedEvent()));
    connect(this, SIGNAL(loadFinished(bool)), this, SLOT(loadFinishedEvent()));
    connect(this, SIGNAL(loadProgress(int)), this, SLOT(loadProgressEvent(int)));

    connect(page(), SIGNAL(linkHovered(QString, QString, QString)), this, SLOT(linkHoveredEvent(QString, QString, QString)));
    connect(page()->mainFrame(), SIGNAL(urlChanged(QUrl)), this, SLOT(urlChangedEvent(QUrl)));
}

BrowserViewControl::~BrowserViewControl()
{
}

/*QStack<QString>& BrowserViewControl::getHistory()
{
    return history;
}*/

QString BrowserViewControl::getTopHref(int x, int y)
{
    QPoint point = page()->mainFrame()->scrollPosition();
    x += point.x();
    y += point.y();

    const QWebElement& element = page()->mainFrame()->documentElement();
    QString topHref;

    foreach(const QWebElement& e, element.findAll("[href]")) {
        const QWebElement& lc = e.lastChild();
        QRect r = e.geometry();

        const QRect& lcr = lc.geometry();
        r.setRight((lcr.right() > r.right()) ? lcr.right(): r.right());
        r.setBottom((lcr.bottom() > r.bottom()) ? lcr.bottom(): r.bottom());

        if(r.left() <= x && r.right() >= x && r.top() <= y && r.bottom() >= y) {
            topHref = e.attribute("href", "");
            break;
        }
    }
    return topHref;
}

const QString& BrowserViewControl::getTopHref()
{
    return topHref;
}

const QList<BrowserNetworkAccessManager::StreamData>& BrowserViewControl::getStreamData()
{
    return reinterpret_cast<BrowserNetworkAccessManager*>(page()->networkAccessManager())->getStreams();
}

void BrowserViewControl::linkHoveredEvent(const QString& link, const QString& title, const QString& textContent) {
    (void) title;
    (void) textContent;

    topHref = link;
    bar->showMessage(link);
    emit onTopHref();
}

void BrowserViewControl::urlChangedEvent(const QUrl& url) {
    relatedUrl = url.toString();
    reinterpret_cast<BrowserControl*>(parent())->lineEdit->setText(relatedUrl);
}

void BrowserViewControl::loadProgressEvent(int progress) {
    //reinterpret_cast<BrowserControl*>(parent())->lineEdit->setText(QString("%1 (%2)").arg(relatedUrl).arg(progress));
}

void BrowserViewControl::loadFinishedEvent() {
    //qDebug() << "stop";
    //const QString sUrl = url().toString();

    /*if(!history.isEmpty()) {
        const QString& sTopUrl = history.top();
        if(sUrl.compare(sTopUrl, Qt::CaseInsensitive)) {
           history.push(sUrl);
        }
    } else {
        history.push(sUrl);
    }*/

    //reinterpret_cast<BrowserControl*>(parent())->lineEdit->setText(sUrl);
}

void BrowserViewControl::loadStartedEvent() {
    reinterpret_cast<BrowserControl*>(parent())->lineEdit->setText("");
    reinterpret_cast<BrowserNetworkAccessManager*>(page()->networkAccessManager())->clear();

    //qDebug() << "start";
}
