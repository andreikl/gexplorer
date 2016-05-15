#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include <QtGui/QDragEnterEvent>

#include <QMessageBox>
#include <QTreeView>
#include <QToolBar>
#include <QMenu>

#include "data/keydata.h"

#include "handlers/keyhandler.h"

#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/editnamedialog.h"

#include "ui/windows/keygallerieswindow.h"
#include "ui_keygallerieswindow.h"

class KeysTree: public QTreeView
{

public:
    KeysTree(QWidget* parent): QTreeView(parent)
    {
        setHeaderHidden(true);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QTreeView::DropOnly);

        model = new GalleryTreeModel(Config::ETypeKey, this);
        setModel(model);
    }

    virtual ~KeysTree()
    {
        if(!model) {
            delete model;
        }
    }

protected:
    void dragEnterEvent(QDragEnterEvent* value)
    {
        if(value->mimeData()->hasFormat(Config::getInstance()->getApplicationSignature())) {
            QByteArray data = value->mimeData()->data(Config::getInstance()->getApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeCustomGallery) {

                dataStream >> galleryId;
                return value->accept();
            }
        }
        value->ignore();
    }

    void dragMoveEvent(QDragMoveEvent* value)
    {
        bool res = false;
        const QModelIndex& index = indexAt(value->pos());
        if(index.isValid()) {
            GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(index.data(Qt::UserRole));
            if(item->getKey()) {
                if(!item->getKey()->isContainGallery(galleryId)) {
                    res = true;
                }
            }
        }
        return (res)? value->accept(): value->ignore();
    }

    void dragLeaveEvent(QDragLeaveEvent* value)
    {
        value->accept();
    }

protected:
    void dropEvent(QDropEvent* value)
    {
        const QModelIndex& index = indexAt(value->pos());
        if(index.isValid()) {
            GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(index.data(Qt::UserRole));
            if(item->getKey() && !item->getKey()->isContainGallery(galleryId)) {
                KeyData* key = item->getKey();
                CustomGalleryData* gallery = KeyHandler::getInstance()->addToKey(*key, galleryId);
                if(gallery) {
                    const QModelIndex& i = model->getIndex(key, gallery);
                    selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }
                return value->accept();
            }
        }
        value->ignore();
    }

public:
    GalleryTreeModel* model;

private:
    int galleryId;

};

KeyGalleriesWindow::KeyGalleriesWindow(QWidget* parent): QDockWidget(parent), ui(new Ui::KeyGalleriesWindow)
{
    ui->setupUi(this);

    TitleControl* title = new TitleControl(this);
    setTitleBarWidget(title);

    twKeys = new KeysTree(this);
    ui->lContent->addWidget(twKeys);

    connect(twKeys->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
    connect(twKeys, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(activatedEvent()));

    createMenuAndActions(title);
    updateButtons();
}

KeyGalleriesWindow::~KeyGalleriesWindow()
{
    delete ui;
}

void KeyGalleriesWindow::selectKey(KeyData* key, CustomGalleryData* gallery)
{
    const QModelIndex& i = twKeys->model->getIndex(key, gallery);
    twKeys->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void KeyGalleriesWindow::contextMenuEvent(QContextMenuEvent* event)
{
    mMenu->exec(event->globalPos());
}

void KeyGalleriesWindow::activatedEvent()
{
    const QModelIndex& index = twKeys->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* d = qvariant_cast<GalleryTreeItem*>(data);
    if(d->getCustomGallery()) {
        emit onGallery(d->getCustomGallery());
    }
}

void KeyGalleriesWindow::selectedEvent()
{
    updateButtons();
}

void KeyGalleriesWindow::gotoEvent()
{
    const QModelIndex& index = twKeys->selectionModel()->currentIndex();
    const QVariant& i = index.data(Qt::UserRole);
    GalleryTreeItem* item = qvariant_cast<GalleryTreeItem*>(i);
    if(item->getCustomGallery()) {
        emit onGotoGallery(item->getCustomGallery());
    }
}

void KeyGalleriesWindow::editEvent()
{
    const QModelIndexList& rows = twKeys->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* d = qvariant_cast<GalleryTreeItem*>(data);
        if(d->getKey()) {
            KeyData* ck = d->getKey()->clone();
            EditNameDialog* dialog = new EditNameDialog(this, *ck);
            dialog->show();
            dialog->exec();

            if(dialog->result() == QDialog::Accepted) {
                KeyData* pk = KeyHandler::getInstance()->updKey(*ck);
                if(pk) {
                    const QModelIndex& i = twKeys->model->getIndex(pk);
                    twKeys->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }
            }
            delete dialog;
            delete ck;
            break;
        }
    }
}

void KeyGalleriesWindow::exitEvent()
{
    Config::getInstance()->setIsKeyGalleriesWindow(false);
    close();
}

void KeyGalleriesWindow::addEvent()
{
    KeyData* key = new KeyData();
    EditNameDialog* dialog = new EditNameDialog(this, *key);
    dialog->show();
    dialog->exec();

    if(dialog->result() == QDialog::Accepted) {
        KeyData* k = KeyHandler::getInstance()->addKey(*key);
        if(k) {
            const QModelIndex& i = twKeys->model->getIndex(k);
            twKeys->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }
    delete dialog;
    delete key;
}

void KeyGalleriesWindow::delEvent()
{
    QList<KeyData*> keys;
    QList<KeyData*> galleryKeys;
    QList<CustomGalleryData*> galleries;
    const QModelIndexList& rows = twKeys->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* d = qvariant_cast<GalleryTreeItem*>(data);
        if(d->getCustomGallery()) {
            GalleryTreeItem* td = d->getParent();
            while(td != NULL && td->getKey() == NULL) {
                td = td->getParent();
            }
            galleryKeys.append(td->getKey());
            galleries.append(d->getCustomGallery());
        }
        if(d->getKey()) {
            keys.append(d->getKey());
        }
    }

    if(keys.count() || galleries.count()) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            for(int i = 0; i < galleries.count(); i++) {
                KeyData* key = galleryKeys.at(i);
                CustomGalleryData* gallery = galleries.at(i);
                KeyHandler::getInstance()->delFromKey(*key, *gallery);
            }
            foreach(KeyData* key, keys) {
                KeyHandler::getInstance()->delKey(*key);
            }
        }
    }
}

void KeyGalleriesWindow::createMenuAndActions(TitleControl* title)
{

    title->getToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    aAddKey = new QAction("&Add", this);
    aAddKey->setStatusTip("Add new key");
    aAddKey->setIcon(QIcon(":/res/resources/add.png"));
    title->getToolBar()->addAction(aAddKey);
    mMenu->addAction(aAddKey);
    connect(aAddKey, SIGNAL(triggered()), this, SLOT(addEvent()));

    aEditKey = new QAction("&Edit", this);
    aEditKey->setStatusTip("Edit actve key");
    aEditKey->setIcon(QIcon(":/res/resources/edit.png"));
    title->getToolBar()->addAction(aEditKey);
    mMenu->addAction(aEditKey);
    connect(aEditKey, SIGNAL(triggered()), this, SLOT(editEvent()));

    aDelKey = new QAction("&Delete", this);
    aDelKey->setStatusTip("Delete selected keys");
    aDelKey->setIcon(QIcon(":/res/resources/del.png"));
    title->getToolBar()->addAction(aDelKey);
    mMenu->addAction(aDelKey);
    connect(aDelKey, SIGNAL(triggered()), this, SLOT(delEvent()));

    title->getToolBar()->addSeparator();
    mMenu->addSeparator();

    aShowGallery = new QAction("&Show", this);
    aShowGallery->setStatusTip("Show active gallery");
    aShowGallery->setIcon(QIcon(":/res/resources/gallery.png"));
    title->getToolBar()->addAction(aShowGallery);
    mMenu->addAction(aShowGallery);
    connect(aShowGallery, SIGNAL(triggered()), this, SLOT(activatedEvent()));

    aGotoGallery = new QAction("&Goto", this);
    aGotoGallery->setStatusTip("Go to active gallery");
    aGotoGallery->setIcon(QIcon(":/res/resources/right.png"));
    title->getToolBar()->addAction(aGotoGallery);
    mMenu->addAction(aGotoGallery);
    connect(aGotoGallery, SIGNAL(triggered()), this, SLOT(gotoEvent()));

    aDelGallery = new QAction("&Delete", this);
    aDelGallery->setStatusTip("Delete selected galleries");
    aDelGallery->setIcon(QIcon(":/res/resources/del.png"));
    title->getToolBar()->addAction(aDelGallery);
    mMenu->addAction(aDelGallery);
    connect(aDelGallery, SIGNAL(triggered()), this, SLOT(delEvent()));

    connect(title->getCloseAction(), SIGNAL(triggered()), SLOT(exitEvent()));
}

void KeyGalleriesWindow::updateButtons()
{
    bool isKey = false;
    bool isGallery = false;
    const QModelIndexList& rows = twKeys->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* d = qvariant_cast<GalleryTreeItem*>(data);
        if(d->getCustomGallery()) {
            isGallery = true;
        }
        if(d->getKey()) {
            isKey = true;
        }
        if(isGallery && isKey) {
            break;
        }
    }

    if(isKey) {
        aEditKey->setEnabled(true);
        aDelKey->setEnabled(true);
    } else {
        aEditKey->setEnabled(false);
        aDelKey->setEnabled(false);
    }

    if(isGallery) {
        aShowGallery->setEnabled(true);
        aGotoGallery->setEnabled(true);
        aDelGallery->setEnabled(true);
    } else {
        aShowGallery->setEnabled(false);
        aGotoGallery->setEnabled(false);
        aDelGallery->setEnabled(false);
    }
}
