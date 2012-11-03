#ifndef BROWSER_H
#define BROWSER_H

#include <QWebView>

/*class Browser: public QWebView {
    Q_OBJECT

public:
    Browser(QWidget* pParent);
    virtual ~Browser();

protected:
    virtual bool eventFilter(QObject* pObject, QEvent* pEvent);

protected slots:
    void OnSearch();

private:
    void SearchSameUrls(QWebElement element, const QString& path, const QString& extension);
    void SearchTopElement(QWebElement element, int x, int y);
    void ParseUrl(const QString& url);

private:
    QString topUrl;
    QString urlPath;
    QString urlExtension;
};*/

#endif // BROWSER_H
