/*#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtCore/QTimer>

#include <QtGui/QMouseEvent>

#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>

#include "controls/searchdialog.h"

#include "controls/browser.h"

Browser::Browser(QWidget* pParent): QWebView(pParent) {
    installEventFilter(this);
}

Browser::~Browser() {
}

bool Browser::eventFilter(QObject* pObject, QEvent* pEvent) {
    bool isHandled = false;

    if(pEvent->type() == QEvent::MouseButtonPress) {
        qDebug() << "QEvent::MouseButtonPress";

        QMouseEvent* pMouseEvent = dynamic_cast<QMouseEvent*>(pEvent);
        int x = pMouseEvent->x();
        int y = pMouseEvent->y();

        int width = page()->viewportSize().width() - page()->mainFrame()->scrollBarGeometry(Qt::Vertical).width();
        int height = page()->viewportSize().height() - page()->mainFrame()->scrollBarGeometry(Qt::Horizontal).height();
        if(x < width && y < height) {
            QPoint point = this->page()->mainFrame()->scrollPosition();
            x += point.x();
            y += point.y();

            urlExtension = "";
            urlPath = "";
            topUrl = "";

            QWebElement element = page()->mainFrame()->documentElement();
            SearchTopElement(element, x, y);

            ParseUrl(topUrl);
            qDebug() << "topUrl: " << topUrl;
            qDebug() << "urlPath: " << urlPath;
            qDebug() << "urlExt: " << urlExtension;

            QTimer::singleShot(1, this, SLOT(OnSearch()));

            isHandled = true;
        }
    }

    if(!isHandled) {
        return QWebView::eventFilter(pObject, pEvent);
    } else {
        return true;
    }
}

void Browser::OnSearch() {
    SearchDialog* pSearch = new SearchDialog(this, topUrl, urlPath, urlExtension);
    pSearch->show();

    pSearch->exec();

    if(pSearch->result() == 0) {
        qDebug() << "sameUrls: ";

        QWebElement element = page()->mainFrame()->documentElement();
        SearchSameUrls(element, urlPath, urlExtension);
    }
}

void Browser::SearchSameUrls(QWebElement element, const QString& path, const QString& extension) {
    if(path.isEmpty()) {
        return;
    }

    QString ext = "." + extension;
    foreach(QWebElement e, element.findAll("[href]")) {
        QString url = e.attribute("href");
        if(url.startsWith(path, Qt::CaseInsensitive) && url.endsWith(ext, Qt::CaseInsensitive)) {
            qDebug() << url;
        }
    }
}

void Browser::SearchTopElement(QWebElement element, int x, int y) {
    foreach(QWebElement e, element.findAll("[href]")) {
        QRect r = e.geometry();
        QWebElement lc = e.lastChild();
        int right = (lc.geometry().right() > r.right()) ? lc.geometry().right(): r.right();
        int bottom = (lc.geometry().bottom() > r.bottom()) ? lc.geometry().bottom(): r.bottom();

        if(r.left() <= x && right >= x && r.top() <= y && bottom >= y) {
            topUrl = e.attribute("href", "");
            qDebug() << e.toOuterXml();
            qDebug() << "geometry: " << r;
            qDebug() << "geometry: " << r.left() << "x" << r.top() << " " << right << "x" << bottom;
            qDebug() << "x: " << x << ", y: " << y;
        }
    }*/
    /*bool isFind = false;
    QWebElement child = element.firstChild();
    while(!child.isNull()) {
        QRect r = child.geometry();
        if((r.right() == -1 && r.bottom() == -1) || (r.left() <= x && r.right() >= x && r.top() <= y && r.bottom() >= y)) {
            isFind = true;

            SearchTopElement(child, x, y);
            if(r.right() != -1 || r.bottom() != -1) {
                //break;
            }
        }
        child = child.nextSibling();
    }
    if(!isFind) {
        if(element.geometry().right() != -1 || element.geometry().bottom() != -1) {
            topUrl = element.attribute("href", "");
            qDebug() << element.toOuterXml();
        }
    }*/
/*}

void Browser::ParseUrl(const QString& url) {
    if(!url.isEmpty()) {
        QStringList list = url.split("/");
        if(list.count() > 0) {
            QStringList listExt = list[list.count() - 1].split(".");
            if(listExt.count() > 1) {
                urlExtension = listExt[listExt.count() - 1];
            }
            urlPath = url.left(url.length() - list[list.count() - 1].length());
        }
    }
}*/
