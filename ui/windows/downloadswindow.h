#ifndef DOWNLOADS_WINDOW_H
#define DOWNLOADS_WINDOW_H

#include <QtGui/QStyledItemDelegate>
#include <QtGui/QDockWidget>

#include "application.h"

namespace Ui {
    class DownloadsWindow;
}

class DownloadsWindowDelegate: public QStyledItemDelegate {
    Q_OBJECT

public:
    DownloadsWindowDelegate(QWidget* pParent);
    virtual ~DownloadsWindowDelegate();

public:
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

};

class DownloadsWindow: public QDockWidget {
    Q_OBJECT

public:
    explicit DownloadsWindow(QWidget* pParent = NULL);
    virtual ~DownloadsWindow();

private slots:
    void dequeueEvent(DownloadHandler* pDownloader);
    void enqueueEvent(DownloadHandler* pDownloader);
    void dataEvent(uint percent);
    void ExitEvent();

private:
    Ui::DownloadsWindow* pUi;

};

#endif // DOWNLOADS_WINDOW_H
