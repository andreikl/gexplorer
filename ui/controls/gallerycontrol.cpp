#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QDrag>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QListView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenu>

#include "config.h"

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "handlers/galleryhandler.h"

#include "ui/common/gallerylistdelegate.h"
#include "ui/common/galleryitemaction.h"
#include "ui/common/gallerylistmodel.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"

#include "ui/controls/gallerycontrol.h"
#include "ui_gallerycontrol.h"

bool galleryItemDataComparer(const GalleryItemData* item1, const GalleryItemData* item2)
{
    return item1->getFileName() < item2->getFileName();
}

class GalleryList: public QListView
{
public:
    GalleryList(GalleryControl* parent): QListView(parent)
    {
        setItemDelegate(new GalleryListDelegate(this));

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        setViewMode(QListView::IconMode);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
    }

    virtual ~GalleryList()
    {
    }

protected:
    void startDrag(Qt::DropActions)
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        stream << Config::ETypeGalleryItem;

        QList<GalleryItemData*> items;
        const QModelIndexList& indexes = selectedIndexes();
        foreach(const QModelIndex& index, indexes) {
            GalleryItemData* item = qvariant_cast<GalleryItemData*>(index.data(Qt::UserRole + 1));
            items.append(item);
        }
        qSort(items.begin(), items.end(), galleryItemDataComparer);
        foreach(GalleryItemData* item, items) {
            stream << item->getGallery()->getId() << item->getId();
        }

        QDrag* drag = new QDrag(this);
        drag->setMimeData(new QMimeData());
        drag->mimeData()->setData(QString(Config::getInstance()->getApplicationSignature()) + "Item", data);
        drag->setPixmap(QPixmap(":/res/resources/gallery.png"));
        //pDrag->setPixmap(pItem->Pixmap());
        drag->exec(Qt::CopyAction);

        //delete pDrag;
    }
};

GalleryControl::GalleryControl(QWidget* parent): QWidget(parent), data(NULL), model(NULL), ui(new Ui::GalleryControl)
{
    ui->setupUi(this);

    title = new TitleControl(this);
    ui->lLayout->addWidget(title);

    list = new GalleryList(this);
    ui->lLayout->addWidget(list);

    createMenuAndActions();
    updateButtons();
}

GalleryControl::~GalleryControl()
{
    if(model) {
        delete model;
    }

    delete ui;
}

const GalleryData* GalleryControl::getGallery() const
{
    return data;
}

GalleryListModel* GalleryControl::getModel()
{
    return model;
}

TitleControl* GalleryControl::getTitle()
{
    return title;
}

void GalleryControl::selectItem(GalleryItemData* value)
{
    const QModelIndex& i = model->getIndexByItem(value);
    list->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void GalleryControl::setGallery(GalleryData* value)
{
    data = value;

    if(model) {
        delete model;
        model = NULL;
    }

    if(value) {
        model = new GalleryListModel(*value, (int)Config::getInstance()->getGalleryIconSize(), this);

        list->setModel(model->getProxy());
        connect(list->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
    }
}

void GalleryControl::contextMenuEvent(QContextMenuEvent* event)
{
    mMenu->exec(event->globalPos());
}

void GalleryControl::iconSizeChangedEvent(int value)
{
    switch(value) {
    case 0:
        Config::getInstance()->setGalleryIconSize(Config::ELargeSize);
        break;

    case 1:
        Config::getInstance()->setGalleryIconSize(Config::EMediumSize);
        break;

    case 2:
        Config::getInstance()->setGalleryIconSize(Config::ESmallSize);
        break;

    default:
        break;
    }

    if(model) {
        model->changeItemsSize((int)Config::getInstance()->getGalleryIconSize());
    }
    updateButtons();
}

void GalleryControl::selectedEvent()
{
    updateButtons();
}

void GalleryControl::showEvent()
{
    if(list->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = list->selectionModel()->selectedRows();
        ViewerDialog* dialog = new ViewerDialog(NULL, reinterpret_cast<GalleryListModel&>(*model), sr[0].row());
        dialog->showMaximized();
        dialog->exec();

        //dialog->deleteLater();
        //delete dialog;
    }
}

void GalleryControl::gotoEvent(CustomGalleryItemData* value)
{
    emit onGotoItem(value);
}

void GalleryControl::gotoEvent()
{
    const QModelIndexList& sr = list->selectionModel()->selectedRows();
    const QModelIndex& index = sr[0];
    GalleryItemData* item = qvariant_cast<GalleryItemData*>(index.data(Qt::UserRole + 1));
    emit onGotoItem(item->getCustomItems().at(0));
}

void GalleryControl::delEvent()
{
    if(list->selectionModel()->selectedRows().count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            while(list->selectionModel()->selectedRows().count() > 0) {
                const QModelIndexList& rows = list->selectionModel()->selectedRows();
                const QModelIndex& index = rows.at(0);
                GalleryItemData* item = qvariant_cast<GalleryItemData*>(index.data(Qt::UserRole + 1));
                if(!GalleryHandler::getInstance()->delFromGallery(*item)) {
                    break;
                }
            }
        }
    }
}

void GalleryControl::createMenuAndActions()
{
    title->getToolBar()->setVisible(true);
    mMenu = new QMenu(this);

    cbSize = new QComboBox(this);
    cbSize->setMinimumSize(100, 30);
    cbSize->addItem("Large");
    cbSize->addItem("Medium");
    cbSize->addItem("Small");
    switch(Config::getInstance()->getGalleryIconSize()) {
    case Config::ELargeSize:
        cbSize->setCurrentIndex(0);
        break;

    case Config::EMediumSize:
        cbSize->setCurrentIndex(1);
        break;

    case Config::ESmallSize:
        cbSize->setCurrentIndex(2);
        break;

    default:
        break;
    }
    title->getToolBar()->addWidget(cbSize);
    connect(cbSize, SIGNAL(currentIndexChanged(int)), this, SLOT(iconSizeChangedEvent(int)));

    aShow = new QAction("&Show", this);
    aShow->setStatusTip("Show active item");
    aShow->setIcon(QIcon(":/res/resources/gallery.png"));
    title->getToolBar()->addAction(aShow);
    mMenu->addAction(aShow);
    connect(aShow, SIGNAL(triggered()), this, SLOT(showEvent()));

    mGotoMenu = new QMenu(this);
    mGotoMenu->menuAction()->setText("&Goto ...");
    mGotoMenu->menuAction()->setStatusTip("Selects gallery Items");
    mGotoMenu->menuAction()->setIcon(QIcon(":/res/resources/right.png"));
    title->getToolBar()->addAction(mGotoMenu->menuAction());
    mMenu->addAction(mGotoMenu->menuAction());
    connect(mGotoMenu->menuAction(), SIGNAL(triggered()), this, SLOT(gotoEvent()));

    aDel = new QAction("&Delete", this);
    aDel->setStatusTip("Delete selected items");
    aDel->setIcon(QIcon(":/res/resources/del.png"));
    title->getToolBar()->addAction(aDel);
    mMenu->addAction(aDel);
    connect(aDel, SIGNAL(triggered()), this, SLOT(delEvent()));
}

void GalleryControl::updateButtons()
{
    if(list->selectionModel() && list->selectionModel()->selectedRows().count() > 0) {
        aDel->setEnabled(true);

        const QModelIndexList& sr = list->selectionModel()->selectedRows();

        if(sr.count() == 1) {
            aShow->setEnabled(true);

            const QModelIndex& index = sr[0];
            GalleryItemData* item = qvariant_cast<GalleryItemData*>(index.data(Qt::UserRole + 1));
            if(item->getCustomItems().count()) {
                mGotoMenu->clear();
                mGotoMenu->setEnabled(true);
                foreach(CustomGalleryItemData* i, item->getCustomItems()) {
                    GalleryItemAction* action = new GalleryItemAction(this, i);
                    action->setText(i->getCustomGallery()->getName() + "/" + i->getName());
                    connect(action, SIGNAL(onActivateCustomGallery(CustomGalleryItemData*)), this, SLOT(gotoEvent(CustomGalleryItemData*)));
                    mGotoMenu->addAction(action);
                }
            } else {
                mGotoMenu->setEnabled(false);
            }
        } else {
            aShow->setEnabled(false);

            mGotoMenu->setEnabled(false);
        }
    } else {
        aShow->setEnabled(false);
        mGotoMenu->setEnabled(false);
        aDel->setEnabled(false);
    }
}
