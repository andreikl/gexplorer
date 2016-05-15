#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>

namespace Ui
{
    class SearchDialog;
}

class SearchDialog: public QDialog
{
    Q_OBJECT

public:
    explicit SearchDialog(QWidget* parent, const QString& url, const QString& path, const QString& extension);
    virtual ~SearchDialog();

public:
    QString getExtension();
    QString getPath();

private:
    Ui::SearchDialog* ui;
};

#endif // SEARCHDIALOG_H
