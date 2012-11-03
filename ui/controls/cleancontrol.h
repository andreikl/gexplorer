#ifndef CLEANCONTROL_H
#define CLEANCONTROL_H

#include <QtGui/QWidget>

#include <QtDeclarative/QtDeclarative>

#include "data/gallerydata.h"

namespace Ui {
    class CleanControl;
}

class TitleControl;

class CleanControl: public QWidget {
    Q_OBJECT

public:
    explicit CleanControl(QWidget* pParent = NULL);
    virtual ~CleanControl();

private:
    TitleControl* pTitle;
    QDeclarativeView* pView;

private:
    Ui::CleanControl* pUi;
};

#endif // CLEANCONTROL_H
