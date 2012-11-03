#include <QtCore/QDebug>

#include <QtGui/QMessageBox>
#include <QtGui/QTreeView>
#include <QtGui/QMenu>

#include <QtGui/QDragEnterEvent>

#include "data/keydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/keyhandler.h"

#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/editkeydialog.h"

#include "ui/windows/keygallerieswindow.h"

#include "ui_keygallerieswindow.h"
#include "ui_titlecontrol.h"

class KeysTree: public QTreeView {

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
        if(value->mimeData()->hasFormat(Config::ApplicationSignature())) {
            QByteArray data = value->mimeData()->data(Config::ApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeCustomGallery) {
                //int id = 0;
                //dataStream >> id;
                //gallery = CustomGalleryHandler::getInstance()->getCustomGalleryById(id);

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
            GalleryTreeItem* item = qVariantValue<GalleryTreeItem*>(index.data(Qt::UserRole));
            if(item->Key()) {
                //if(!item->Key()->getGalleries().contains(gallery)) {
                if(!item->Key()->isContainGallery(galleryId)) {
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
            GalleryTreeItem* item = qVariantValue<GalleryTreeItem*>(index.data(Qt::UserRole));
            //if(item->Key() && !item->Key()->getGalleries().contains(gallery)) {
            if(item->Key() && !item->Key()->isContainGallery(galleryId)) {
                KeyData* key = item->Key();
                //if(KeyHandler::getInstance()->addToKey(*key, *gallery)) {
                CustomGalleryData* gallery = KeyHandler::getInstance()->addToKey(*key, galleryId);
                if(gallery) {
                    const QModelIndex& i = model->GetIndex(key, gallery);
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
    //CustomGalleryData* gallery;
    int galleryId;
};

KeyGalleriesWindow::KeyGalleriesWindow(QWidget* parent): QDockWidget(parent), title(NULL), ui(new Ui::KeyGalleriesWindow)
{
    ui->setupUi(this);

    title =  new TitleControl(this);
    setTitleBarWidget(title);

    twKeys = new KeysTree(this);
    ui->plContent->addWidget(twKeys);

    connect(twKeys->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
    connect(twKeys, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(activatedEvent()));

    createMenuAndActions();
    updateButtons();
}

KeyGalleriesWindow::~KeyGalleriesWindow()
{
    delete ui;
}

void KeyGalleriesWindow::contextMenuEvent(QContextMenuEvent* event)
{
    mMenu->exec(event->globalPos());
}

void KeyGalleriesWindow::activatedEvent()
{
    const QModelIndex& index = twKeys->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* d = qVariantValue<GalleryTreeItem*>(data);
    if(d->CustomGallery()) {
        emit onGallery(d->CustomGallery());
    }
}

void KeyGalleriesWindow::selectedEvent()
{
    updateButtons();
}

void KeyGalleriesWindow::gotoEvent()
{
    const QModelIndex& index = twKeys->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* d = qVariantValue<GalleryTreeItem*>(data);
    if(d->CustomGallery()) {
        emit onGotoGallery(d->CustomGallery());
    }
}

void KeyGalleriesWindow::editEvent()
{
    const QModelIndexList& rows = twKeys->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* d = qVariantValue<GalleryTreeItem*>(data);
        if(d->Key()) {
            KeyData* ck = d->Key()->clone();
            EditKeyDialog* dialog = new EditKeyDialog(this, *ck);
            dialog->show();
            dialog->exec();

            if(dialog->result() == QDialog::Accepted) {
                KeyData* pk = KeyHandler::getInstance()->updKey(*ck);
                if(pk) {
                    const QModelIndex& i = twKeys->model->GetIndex(pk);
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
    Config::Self()->IsKeyGalleriesWindow(false);
    close();
}

void KeyGalleriesWindow::addEvent()
{
    KeyData* key = new KeyData();
    EditKeyDialog* dialog = new EditKeyDialog(this, *key);
    dialog->show();
    dialog->exec();

    if(dialog->result() == QDialog::Accepted) {
        KeyData* k = KeyHandler::getInstance()->addKey(*key);
        if(k) {
            const QModelIndex& i = twKeys->model->GetIndex(k);
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
        GalleryTreeItem* d = qVariantValue<GalleryTreeItem*>(data);
        if(d->CustomGallery()) {
            GalleryTreeItem* td = d->Parent();
            while(td != NULL && td->Key() == NULL) {
                td = td->Parent();
            }
            galleryKeys.append(td->Key());
            galleries.append(d->CustomGallery());
        }
        if(d->Key()) {
            keys.append(d->Key());
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

void KeyGalleriesWindow::createMenuAndActions()
{

    title->ToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    aAddKey = new QAction("&Add", this);
    aAddKey->setStatusTip("Add new key");
    aAddKey->setIcon(QIcon(":/res/resources/add.png"));
    title->ToolBar()->addAction(aAddKey);
    mMenu->addAction(aAddKey);
    connect(aAddKey, SIGNAL(triggered()), this, SLOT(addEvent()));

    aEditKey = new QAction("&Edit", this);
    aEditKey->setStatusTip("Edit actve key");
    aEditKey->setIcon(QIcon(":/res/resources/edit.png"));
    title->ToolBar()->addAction(aEditKey);
    mMenu->addAction(aEditKey);
    connect(aEditKey, SIGNAL(triggered()), this, SLOT(editEvent()));

    aDelKey = new QAction("&Delete", this);
    aDelKey->setStatusTip("Delete selected keys");
    aDelKey->setIcon(QIcon(":/res/resources/del.png"));
    title->ToolBar()->addAction(aDelKey);
    mMenu->addAction(aDelKey);
    connect(aDelKey, SIGNAL(triggered()), this, SLOT(delEvent()));

    title->ToolBar()->addSeparator();
    mMenu->addSeparator();

    aShowGallery = new QAction("&Show", this);
    aShowGallery->setStatusTip("Show active gallery");
    aShowGallery->setIcon(QIcon(":/res/resources/view.png"));
    title->ToolBar()->addAction(aShowGallery);
    mMenu->addAction(aShowGallery);
    connect(aShowGallery, SIGNAL(triggered()), this, SLOT(activatedEvent()));

    aGotoGallery = new QAction("&Goto", this);
    aGotoGallery->setStatusTip("Go to active gallery");
    aGotoGallery->setIcon(QIcon(":/res/resources/right.png"));
    title->ToolBar()->addAction(aGotoGallery);
    mMenu->addAction(aGotoGallery);
    connect(aGotoGallery, SIGNAL(triggered()), this, SLOT(gotoEvent()));

    aDelGallery = new QAction("&Delete", this);
    aDelGallery->setStatusTip("Delete selected galleries");
    aDelGallery->setIcon(QIcon(":/res/resources/del.png"));
    title->ToolBar()->addAction(aDelGallery);
    mMenu->addAction(aDelGallery);
    connect(aDelGallery, SIGNAL(triggered()), this, SLOT(delEvent()));

    mMenu->addAction(title->CloseAction());
    connect(title->CloseAction(), SIGNAL(triggered()), SLOT(exitEvent()));
}

void KeyGalleriesWindow::updateButtons()
{
    bool isKey = false;
    bool isGallery = false;
    const QModelIndexList& rows = twKeys->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* d = qVariantValue<GalleryTreeItem*>(data);
        if(d->CustomGallery()) {
            isGallery = true;
        }
        if(d->Key()) {
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
