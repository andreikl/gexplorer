#ifndef BROWSERCONTROL_H
#define BROWSERCONTROL_H

#include <QtWidgets/QWidget>

class BrowserViewControl;
class TitleControl;
class GalleryData;

QT_BEGIN_NAMESPACE
class QWebElement;
class QLineEdit;
class QMenu;
QT_END_NAMESPACE

namespace Ui
{
    class BrowserControl;
}

class BrowserControl: public QWidget
{
    Q_OBJECT

    friend class BrowserViewControl;

public:
    explicit BrowserControl(QWidget* parent);
    virtual ~BrowserControl();

public:
    TitleControl* getTitle();

public:
    void goTo(GalleryData* gallery);
    void goTo(const QString& url);

protected:
    //bool eventFilter(QObject* object, QEvent* event);
    void contextMenuEvent(QContextMenuEvent* event);

private slots:
    void addDeletedGalleryEvent();

    void addGalleryEvent();
    void addStreamGalleryEvent();
    void addItemsEvent();

    void topHrefEvent();
    void openEvent();
    void backEvent();
    void goEvent();

private:
    void searchSameUrls(QWebElement& element, const QString& path, const QString& ext, GalleryData* data);
    void parseUrl(const QString& url, QString& path, QString& ext);
    bool isContainsFile(GalleryData* data, const QString& file);

private:
    void createMenuAndActions();
    void updateButtons();

private:
    GalleryData* gallery;

private:
    BrowserViewControl* web;
    TitleControl* title;
    QLineEdit* lineEdit;
    QString topHref;
    int menuX;
    int menuY;

private:
    QAction* aAddDeleted;

    QAction* aAdd;
    QAction* aAddStream;
    QAction* aAddItem;

    QAction* aOpen;
    QAction* aBack;
    QAction* aGo;
    QMenu* mMenu;

private:
    Ui::BrowserControl* ui;

};

#endif // BROWSERCONTROL_H
