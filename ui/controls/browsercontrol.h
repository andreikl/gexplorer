#ifndef BROWSERCONTROL_H
#define BROWSERCONTROL_H

#include <QtWebKit/QWebView>

class TitleControl;
class GalleryData;

namespace Ui {
    class BrowserControl;
}

class BrowserHrefItem {
public:
    BrowserHrefItem(const QRect& rect, const QString& url) {
        this->rect = rect;
        this->url = url;
    }

public:
    QRect rect;
    QString url;
};

class BrowserView: public QWebView {
    Q_OBJECT

public:
    BrowserView(QWidget* pWidget);
    virtual ~BrowserView();

signals:
    void OnTopHref();

public:
    const QString& TopHref();

protected:
    void mouseMoveEvent(QMouseEvent* pEvent);

private slots:
    void LoadStartedEvent();
    void LoadFinishedEvent();

private:
    QString topHref;
    QList<BrowserHrefItem> hrefs;
};

class BrowserControl: public QWidget {
    Q_OBJECT

public:
    explicit BrowserControl(QWidget* pParent = NULL);
    virtual ~BrowserControl();

public:
    TitleControl* Title();

public:
    void GoTo(GalleryData* pGallery);
    void GoTo(const QString& url);

protected:
    bool eventFilter(QObject* pObject, QEvent* pEvent);
    void contextMenuEvent(QContextMenuEvent* pEvent);

private slots:
    void AddGalleryEvent();
    void AddItemsEvent();

private slots:
    void TopHrefEvent();

private:
    void SearchSameUrls(QWebElement& element, const QString& path, const QString& ext, GalleryData* pData);
    void ParseUrl(const QString& url, QString& path, QString& ext);
    bool IsContainsFile(GalleryData* pData, const QString& file);

private:
    void CreateMenuAndActions();
    void UpdateButtons();

private:
    GalleryData* pGallery;

private:
    QMenu* pmMenu;
    QAction* paAdd;
    QAction* paAddItem;
    QAction* paReloadItem;

private:
    TitleControl* pTitle;
    BrowserView* pWeb;

private:
    Ui::BrowserControl* pUi;
};

#endif // BROWSERCONTROL_H
