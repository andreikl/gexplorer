#ifndef DOWNLOADS_WINDOW_H
#define DOWNLOADS_WINDOW_H

#include <QDockWidget>

class DownloadHandler;

namespace Ui
{
    class DownloadsWindow;
}

class DownloadsWindow: public QDockWidget
{
    Q_OBJECT

public:
    explicit DownloadsWindow(QWidget* parent);
    virtual ~DownloadsWindow();

private slots:
    void dequeueEvent(DownloadHandler* downloader);
    void enqueueEvent(DownloadHandler* downloader);
    void dataEvent(uint percent);
    void exitEvent();

private:
    Ui::DownloadsWindow* ui;

};

#endif // DOWNLOADS_WINDOW_H
