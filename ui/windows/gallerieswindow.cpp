#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QDrag>

#include <QMessageBox>
#include <QComboBox>
#include <QTreeView>
#include <QToolBar>
#include <QMenu>

#include "data/gallerydata.h"

#include "handlers/galleryhandler.h"

#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/addgallerydialog.h"
#include "ui/dialogs/editnamedialog.h"

#include "ui/windows/gallerieswindow.h"
#include "ui_gallerieswindow.h"

class GalleriesTree: public QTreeView
{

public:
    GalleriesTree(QWidget* parent): QTreeView(parent), model(NULL)
    {
        setHeaderHidden(true);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QTreeView::DragOnly);

        reloadModel();
    }

    virtual ~GalleriesTree()
    {
        if(model) {
            delete model;
        }
    }

    void reloadModel()
    {
        if(model) {
            delete model;
        }
        model = new GalleryTreeModel(Config::ETypeGallery, this);
        setModel(model);
    }

protected:
    void startDrag(Qt::DropActions)
    {
        const QModelIndexList& rows = selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        const QVariant& id = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(id);
        if(item->getGallery() && item->getGallery()->getType() != GalleryData::DeletedGallery) {
            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << Config::ETypeGallery << item->getGallery()->getId();

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

GalleriesWindow::GalleriesWindow(QWidget* parent): QDockWidget(parent), ui(new Ui::GalleriesWindow)
{
    ui->setupUi(this);

    TitleControl* title =  new TitleControl(this);
    setTitleBarWidget(title);

    twGalleries = new GalleriesTree(this);
    ui->lContent->addWidget(twGalleries);

    connect(twGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
    connect(twGalleries->model, SIGNAL(onAddGallery(GalleryData*)), this, SLOT(addGalleryEvent(GalleryData*)));

    connect(twGalleries, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(activatedEvent()));

    createMenuAndActions(title);
    updateButtons();
}

GalleriesWindow::~GalleriesWindow()
{
    delete ui;
}

void GalleriesWindow::selectGallery(GalleryData* value)
{
    const QModelIndex& i = twGalleries->model->getIndex(value);
    twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void GalleriesWindow::contextMenuEvent(QContextMenuEvent* event)
{
    mMenu->exec(event->globalPos());
}

void GalleriesWindow::addGalleryEvent(GalleryData* gallery)
{
    const QModelIndex& i = twGalleries->model->getIndex(gallery);
    twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void GalleriesWindow::filterChangedEvent(int value)
{
    Config::getInstance()->setGalleryFilterType((Config::FilterTypeEnum)value);

    twGalleries->reloadModel();
    connect(twGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
    connect(twGalleries->model, SIGNAL(onAddGallery(GalleryData*)), this, SLOT(addGalleryEvent(GalleryData*)));

    updateButtons();
}

void GalleriesWindow::activatedEvent()
{
    const QModelIndex& index = twGalleries->selectionModel()->currentIndex();
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getGallery()  && item->getGallery()->getType() != GalleryData::DeletedGallery) {
        emit onGallery(item->getGallery());
    }
}

void GalleriesWindow::selectedEvent()
{
    updateButtons();
}

void GalleriesWindow::browseEvent()
{
    const QModelIndex& index = twGalleries->selectionModel()->currentIndex();
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getGallery()) {
        emit onBrowse(item->getGallery());
    }
}

void GalleriesWindow::editEvent()
{
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    const QModelIndex& index = rows.at(0);
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getGallery()) {
        GalleryData* cloneGallery = item->getGallery()->clone();
        EditNameDialog* dialog = new EditNameDialog(this, *cloneGallery);
        dialog->show();
        dialog->exec();

        if(dialog->result() == QDialog::Accepted) {
            GalleryData* g = GalleryHandler::getInstance()->updGallerySource(*cloneGallery);
            if(g) {
                const QModelIndex& i = twGalleries->model->getIndex(g);
                twGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            }
        }

        delete dialog;
        delete cloneGallery;
    }
}

void GalleriesWindow::exitEvent()
{
    Config::getInstance()->setIsGalleriesWindow(false);
    close();
}

void GalleriesWindow::addEvent()
{
    AddGalleryDialog* gallery = new AddGalleryDialog(this);
    gallery->show();
    gallery->exec();

    if(gallery->result() == QDialog::Accepted) {
        QString source = gallery->getSource();
        emit onAdd(source);
    }
    delete gallery;
}

void GalleriesWindow::delEvent()
{
    QList<GalleryData*> galleries;
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
        if(item->getGallery()) {
            galleries.append(item->getGallery());
        }
    }

    if(galleries.count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            foreach(GalleryData* gallery, galleries) {
                GalleryHandler::getInstance()->delGallery(*gallery);
            }
        }
    }
}

void GalleriesWindow::createMenuAndActions(TitleControl* title)
{

    title->getToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    aAdd = new QAction("&Add", this);
    aAdd->setStatusTip("Add new gallery");
    aAdd->setIcon(QIcon(":/res/resources/add.png"));
    title->getToolBar()->addAction(aAdd);
    mMenu->addAction(aAdd);
    connect(aAdd, SIGNAL(triggered()), this, SLOT(addEvent()));

    aShow = new QAction("&Show", this);
    aShow->setStatusTip("Show active gallery");
    aShow->setIcon(QIcon(":/res/resources/gallery.png"));
    title->getToolBar()->addAction(aShow);
    mMenu->addAction(aShow);
    connect(aShow, SIGNAL(triggered()), this, SLOT(activatedEvent()));

    aBrowse = new QAction("&Browse", this);
    aBrowse->setStatusTip("Browse active gallery");
    aBrowse->setIcon(QIcon(":/res/resources/view.png"));
    title->getToolBar()->addAction(aBrowse);
    mMenu->addAction(aBrowse);
    connect(aBrowse, SIGNAL(triggered()), this, SLOT(browseEvent()));

    aEdit = new QAction("&Edit", this);
    aEdit->setStatusTip("Edit actve gallery");
    aEdit->setIcon(QIcon(":/res/resources/edit.png"));
    title->getToolBar()->addAction(aEdit);
    mMenu->addAction(aEdit);
    connect(aEdit, SIGNAL(triggered()), this, SLOT(editEvent()));

    cbFilter = new QComboBox(this);
    cbFilter->setMinimumSize(60, 30);
    cbFilter->addItem("All");
    cbFilter->addItem("Sorted");
    cbFilter->addItem("Unsorted");
    cbFilter->setCurrentIndex(Config::getInstance()->getGalleryFilterType());
    title->getToolBar()->addWidget(cbFilter);
    connect(cbFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(filterChangedEvent(int)));

    aDel = new QAction("&Delete", this);
    aDel->setStatusTip("Delete selected galleries");
    aDel->setIcon(QIcon(":/res/resources/del.png"));
    title->getToolBar()->addAction(aDel);
    mMenu->addAction(aDel);
    connect(aDel, SIGNAL(triggered()), this, SLOT(delEvent()));

    connect(title->getCloseAction(), SIGNAL(triggered()), SLOT(exitEvent()));
}

void GalleriesWindow::updateButtons()
{
    GalleryData* gallery = NULL;
    const QModelIndexList& rows = twGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& i = index.data(Qt::UserRole);
        GalleryTreeItem* data = qvariant_cast<GalleryTreeItem*>(i);
        if(data->getGallery()) {
            gallery = data->getGallery();
            break;
        }
    }
    if(gallery) {
        if(gallery->getType() != GalleryData::DeletedGallery) {
            aShow->setEnabled(true);
        } else {
            aShow->setEnabled(false);
        }
        if(gallery->getType() == GalleryData::WebGallery || gallery->getType() == GalleryData::DeletedGallery) {
            aBrowse->setEnabled(true);
        } else {
            aBrowse->setEnabled(false);
        }
        aEdit->setEnabled(true);
        aDel->setEnabled(true);
    } else {
        aShow->setEnabled(false);
        aBrowse->setEnabled(false);
        aEdit->setEnabled(false);
        aDel->setEnabled(false);
    }
}
