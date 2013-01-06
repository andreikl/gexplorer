#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDrag>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenu>

#include "data/customgallerydata.h"
#include "data/keydata.h"

#include "handlers/customgalleryhandler.h"

#include "ui/common/galleryitemaction.h"
#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/editnamedialog.h"

#include "ui/windows/customgallerieswindow.h"
#include "ui_customgallerieswindow.h"

class CustomGalleriesTree: public QTreeView
{

public:
    CustomGalleriesTree(QWidget* parent): QTreeView(parent), model(NULL)
    {
        setHeaderHidden(true);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QTreeView::DragDrop);

        reloadModel();
    }

    virtual ~CustomGalleriesTree()
    {
        if(!model) {
            delete model;
        }
    }

    void reloadModel()
    {
        if(model) {
            delete model;
        }
        model = new GalleryTreeModel(Config::ETypeCustomGallery, this);
        setModel(model);
    }

protected:
    void dragEnterEvent(QDragEnterEvent* value)
    {
        if (value->mimeData()->hasFormat(Config::getInstance()->getApplicationSignature())) {
            QByteArray data = value->mimeData()->data(Config::getInstance()->getApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeGallery) {
                return value->accept();
            }
        }
        value->ignore();
    }

    void dragMoveEvent(QDragMoveEvent* value)
    {
        value->accept();
    }

    void dragLeaveEvent(QDragLeaveEvent* value)
    {
        value->accept();
    }

protected:
    void dropEvent(QDropEvent* value)
    {
        if(value->mimeData()->hasFormat(Config::getInstance()->getApplicationSignature())) {
            QByteArray data = value->mimeData()->data(Config::getInstance()->getApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeGallery) {
                int id = 0;
                dataStream >> id;

                CustomGalleryData* customGallery = CustomGalleryHandler::getInstance()->addCustomGallery(id);
                if(customGallery) {
                    const QModelIndex& i = model->getIndex(customGallery);
                    selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }

                return value->accept();
            }
        }
        value->ignore();
    }

    void startDrag(Qt::DropActions /*actions*/)
    {
        const QModelIndexList& rows = selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
        if(item->getCustomGallery()) {
            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << Config::ETypeCustomGallery << item->getCustomGallery()->getId();

            QDrag* drag = new QDrag(this);
            drag->setMimeData(new QMimeData());
            drag->mimeData()->setData(Config::getInstance()->getApplicationSignature(), data);
            drag->setPixmap(QPixmap(":/res/resources/gallery.png"));
            drag->exec(Qt::CopyAction);
        }
    }

public:
    GalleryTreeModel* model;
};

CustomGalleriesWindow::CustomGalleriesWindow(QWidget* parent): QDockWidget(parent), ui(new Ui::CustomGalleriesWindow)
{
    ui->setupUi(this);

    TitleControl* title =  new TitleControl(this);
    setTitleBarWidget(title);

    twGalleries = new CustomGalleriesTree(this);
    ui->lContent->addWidget(twGalleries);

    connect(twGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));

    connect(twGalleries, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(activatedEvent()));

    createMenuAndActions(title);
    updateButtons();
}

CustomGalleriesWindow::~CustomGalleriesWindow()
{
    delete ui;
}

void CustomGalleriesWindow::selectGallery(CustomGalleryData* value)
{
    const QModelIndex& i = twGalleries->model->getIndex(value);
    twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void CustomGalleriesWindow::contextMenuEvent(QContextMenuEvent* pEvent)
{
    mMenu->exec(pEvent->globalPos());
}

void CustomGalleriesWindow::filterChangedEvent(int value)
{
    Config::getInstance()->setCustomGalleryFilterType((Config::FilterTypeEnum)value);

    twGalleries->reloadModel();
    connect(twGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));

    updateButtons();
}

void CustomGalleriesWindow::activatedEvent()
{
    const QModelIndex& index = twGalleries->selectionModel()->currentIndex();
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getCustomGallery()) {
        emit onGallery(item->getCustomGallery());
    }
}

void CustomGalleriesWindow::selectedEvent()
{
    updateButtons();
}

void CustomGalleriesWindow::gotoKeyEvent(KeyData* key, CustomGalleryData* gallery)
{
    emit onGotoKey(key, gallery);
}

void CustomGalleriesWindow::gotoEvent()
{
    CustomGalleryData* gallery = NULL;
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
        if(item->getCustomGallery()) {
            gallery = item->getCustomGallery();
            break;
        }
    }

    emit onGotoKey(gallery->getKeys().at(0), gallery);
}

void CustomGalleriesWindow::editEvent()
{
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    const QModelIndex& index = rows.at(0);
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getCustomGallery()) {
        CustomGalleryData* cloneGallery = item->getCustomGallery()->clone();
        EditNameDialog* dialog = new EditNameDialog(this, *cloneGallery);
        dialog->show();
        dialog->exec();

        if(dialog->result() == QDialog::Accepted) {
            CustomGalleryData* cg = CustomGalleryHandler::getInstance()->updCustomGallery(*cloneGallery);
            if(cg) {
                const QModelIndex& i = twGalleries->model->getIndex(cg);
                twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            }
        }
        delete dialog;
        delete cloneGallery;
    }
}

void CustomGalleriesWindow::exitEvent()
{
    Config::getInstance()->setIsCustomGalleriesWindow(false);
    close();
}

void CustomGalleriesWindow::addEvent()
{
    CustomGalleryData* gallery = new CustomGalleryData();
    EditNameDialog* dialog = new EditNameDialog(this, *gallery);
    dialog->show();
    dialog->exec();

    if(dialog->result() == QDialog::Accepted) {
        CustomGalleryData* cg = CustomGalleryHandler::getInstance()->addCustomGallery(*gallery);
        if(cg) {
            const QModelIndex& i = twGalleries->model->getIndex(cg);
            twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }
    delete dialog;
    delete gallery;
}

void CustomGalleriesWindow::delEvent()
{
    QList<CustomGalleryData*> galleries;
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
        if(item->getCustomGallery()) {
            galleries.append(item->getCustomGallery());
        }
    }

    if(galleries.count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            foreach(CustomGalleryData* pGallery, galleries) {
                CustomGalleryHandler::getInstance()->delCustomGallery(*pGallery);
            }
        }
    }
}

void CustomGalleriesWindow::createMenuAndActions(TitleControl* title)
{

    title->getToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    aAdd = new QAction("&Add", this);
    aAdd->setStatusTip("Add new custom gallery");
    aAdd->setIcon(QIcon(":/res/resources/add.png"));
    title->getToolBar()->addAction(aAdd);
    mMenu->addAction(aAdd);
    connect(aAdd, SIGNAL(triggered()), this, SLOT(addEvent()));

    aShow = new QAction("&Show", this);
    aShow->setStatusTip("Show active custom gallery");
    aShow->setIcon(QIcon(":/res/resources/gallery.png"));
    title->getToolBar()->addAction(aShow);
    mMenu->addAction(aShow);
    connect(aShow, SIGNAL(triggered()), this, SLOT(activatedEvent()));

    mGotoMenu = new QMenu(this);
    mGotoMenu->menuAction()->setText("&Goto ...");
    mGotoMenu->menuAction()->setStatusTip("Selects key Items");
    mGotoMenu->menuAction()->setIcon(QIcon(":/res/resources/right.png"));
    title->getToolBar()->addAction(mGotoMenu->menuAction());
    mMenu->addAction(mGotoMenu->menuAction());
    connect(mGotoMenu->menuAction(), SIGNAL(triggered()), this, SLOT(gotoEvent()));

    aEdit = new QAction("&Edit", this);
    aEdit->setStatusTip("Edit actve custom gallery");
    aEdit->setIcon(QIcon(":/res/resources/edit.png"));
    title->getToolBar()->addAction(aEdit);
    mMenu->addAction(aEdit);
    connect(aEdit, SIGNAL(triggered()), this, SLOT(editEvent()));

    cbFilter = new QComboBox(this);
    cbFilter->setMinimumSize(60, 30);
    cbFilter->addItem("All");
    cbFilter->addItem("Sorted");
    cbFilter->addItem("Unsorted");
    cbFilter->setCurrentIndex(Config::getInstance()->getCustomGalleryFilterType());
    title->getToolBar()->addWidget(cbFilter);
    connect(cbFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(filterChangedEvent(int)));

    aDel = new QAction("&Delete", this);
    aDel->setStatusTip("Delete selected custom galleries");
    aDel->setIcon(QIcon(":/res/resources/del.png"));
    title->getToolBar()->addAction(aDel);
    mMenu->addAction(aDel);
    connect(aDel, SIGNAL(triggered()), this, SLOT(delEvent()));

    connect(title->getCloseAction(), SIGNAL(triggered()), SLOT(exitEvent()));
}

void CustomGalleriesWindow::updateButtons()
{
    CustomGalleryData* gallery = NULL;
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
        if(item->getCustomGallery()) {
            gallery = item->getCustomGallery();
            break;
        }
    }

    if(gallery) {
        if(gallery->getKeys().count() > 0) {
            mGotoMenu->clear();
            mGotoMenu->setEnabled(true);
            foreach(KeyData* i, gallery->getKeys()) {
                GalleryItemAction* action = new GalleryItemAction(this, i, gallery);
                action->setText(i->getName());
                connect(action, SIGNAL(onActivateKey(KeyData*, CustomGalleryData*)), this, SLOT(gotoKeyEvent(KeyData*, CustomGalleryData*)));
                mGotoMenu->addAction(action);
            }
        } else {
            mGotoMenu->setEnabled(false);
        }

        aShow->setEnabled(true);
        aEdit->setEnabled(true);
        aDel->setEnabled(true);
    } else {
        aShow->setEnabled(false);
        mGotoMenu->setEnabled(false);
        aEdit->setEnabled(false);
        aDel->setEnabled(false);
    }
}
