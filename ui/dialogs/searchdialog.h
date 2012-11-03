#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class SearchDialog;
}

class SearchDialog: public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget *pParent, const QString& url, const QString& path, const QString& extension);
    virtual ~SearchDialog();

public:
    QString Path();
    QString Extension();

private:
    Ui::SearchDialog *pUi;

};

#endif // SEARCHDIALOG_H
