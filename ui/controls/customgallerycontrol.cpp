#include <QtCore/QSortFilterProxyModel>
#include <QtCore/QMimeData>
#include <QtCore/QDebug>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDrag>

#include <QtWidgets/QMessageBox>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QListView>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QMenu>

#include "config.h"

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "helpers/commonhelper.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"
#include "handlers/dbhandler.h"

#include "ui/common/gallerylistdelegate.h"
#include "ui/common/galleryitemaction.h"
#include "ui/common/gallerylistmodel.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"
#include "ui/dialogs/unitedialog.h"

#include "ui/controls/customgallerycontrol.h"
#include "ui_customgallerycontrol.h"

bool customGalleryItemDataComparer(const CustomGalleryItemData* item1, const CustomGalleryItemData* item2)
{
    return item1->getName() < item2->getName();
}

class CustomGalleryList: public QListView
{
public:
    CustomGalleryList(CustomGalleryControl* control, QWidget* parent): QListView(parent)
    {
        this->parent = control;

        delegate = new GalleryListDelegate(this);
        setItemDelegate(delegate);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        //setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        setContextMenuPolicy(Qt::CustomContextMenu);

        setViewMode(QListView::IconMode);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragDrop);
    }

    virtual ~CustomGalleryList()
    {
        if(delegate) {
            delete delegate;
        }
    }

protected:
    void dragEnterEvent(QDragEnterEvent* value)
    {
        if(value->mimeData()->hasFormat(QString(Config::getInstance()->getApplicationSignature()) + "Item")) {
            return value->accept();
        }
        value->ignore();
    }

    void dragMoveEvent(QDragMoveEvent* value)
    {
        if(model()->rowCount() > 0) {
            const QModelIndex& index = indexAt(value->pos());
            if(index.isValid()) {
                const QRect& rect = rectForIndex(index);
                bool isBefore = (value->pos().x() < rect.x() + rect.width() / 2)? true: false;
                if(delegate->getDragIndex() != index || delegate->getIsDragBefore() != isBefore) {
                    delegate->setDragIndex(index, isBefore);
                }
                return value->accept();
            } else if(delegate->getDragIndex() != index) {
                delegate->setDragIndex(index, false);
            }
        } else {
            return value->accept();
        }
        value->ignore();
    }

    void dragLeaveEvent(QDragLeaveEvent* value)
    {
        delegate->setDragIndex(QModelIndex(), false);
        value->accept();
    }

protected:
    void dropEvent(QDropEvent* value)
    {
        CustomGalleryData* gallery = parent->getGallery();
        GalleryListModel* model = parent->getModel();

        QByteArray data = value->mimeData()->data(QString(Config::getInstance()->getApplicationSignature()) + "Item");
        QDataStream stream(&data, QIODevice::ReadOnly);
        int type;
        stream >> type;

        QList<CustomGalleryItemData*> items;

        if(type == Config::ETypeCustomGalleryItem) {
            while(!stream.atEnd()) {
                int gid;
                int id;
                stream >> gid >> id;
                CustomGalleryItemData* item = CustomGalleryHandler::getInstance()->getCustomGalleryItemById(gid, id);
                items.append(item);

            }
            /*const QModelIndexList& indexes = selectedIndexes();
            foreach(const QModelIndex& index, indexes) {
                CustomGalleryItemData* item = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                items.append(item);
            }*/
            //qSort(items.begin(), items.end(), customGalleryItemDataComparer);
        } else  if(type == Config::ETypeGalleryItem) {
            QList<CustomGalleryItemId> ids;
            while(!stream.atEnd()) {
                int gid;
                int id;
                stream >> gid >> id;

                CustomGalleryItemId ii;
                ii.gid = gid;
                ii.id = id;
                ids.append(ii);

                /*CustomGalleryItemData* customItem = CustomGalleryHandler::getInstance()->addToCustomGallery(*gallery, gid, id);
                if(customItem != NULL) {
                    items.append(customItem);
                }*/
            }
            items = CustomGalleryHandler::getInstance()->addToCustomGallery(*gallery, ids);
        }

        if(items.count() > 0) {
            int row = 0;
            if(delegate->getDragIndex().isValid()) {
                row = delegate->getDragIndex().row();
                if(!delegate->getIsDragBefore()) {
                    row++;
                }
            }
            QList<CustomGalleryItemName> ins;
            int n = 1;
            for(int i = 0; i < row; i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                const QModelIndex& index = model->getProxy()->index(i, 0);
                CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                if(item->getName() != name) {
                    CustomGalleryItemName in;
                    in.item = item;
                    in.name = name;
                    ins.append(in);
                }
                //CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*item, name);
            }
            n = row + items.count() + 1;
            for(int i = row; i < model->rowCount(); i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                const QModelIndex& index = model->getProxy()->index(i, 0);
                CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                if(item->getName() != name) {
                    CustomGalleryItemName in;
                    in.item = item;
                    in.name = name;
                    ins.append(in);
                }
                //CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*item, name);
            }
            n = row + 1;
            for(int i = 0; i < items.count(); i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                if(items[i]->getName() != name) {
                    CustomGalleryItemName in;
                    in.item = items[i];
                    in.name = name;
                    ins.append(in);
                }
                //CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*items[i], name);
            }
            CustomGalleryHandler::getInstance()->updCustomGalleryItemNames(ins);
            model->getProxy()->sort(0);
        }

        delegate->setDragIndex(QModelIndex(), false);
        value->accept();
    }

    void startDrag(Qt::DropActions)
    {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        stream << Config::ETypeCustomGalleryItem;

        QList<CustomGalleryItemData*> items;
        const QModelIndexList& indexes = selectedIndexes();
        foreach(const QModelIndex& index, indexes) {
            CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
            items.append(item);
        }
        qSort(items.begin(), items.end(), customGalleryItemDataComparer);
        foreach(CustomGalleryItemData* item, items) {
            stream << item->getCustomGallery()->getId() << item->getId();
        }

        QDrag* drag = new QDrag(this);
        drag->setMimeData(new QMimeData());
        drag->mimeData()->setData(QString(Config::getInstance()->getApplicationSignature()) + "Item", data);
        drag->setPixmap(QPixmap(":/res/resources/gallery.png"));
        drag->exec(Qt::MoveAction);

        //delete pDrag;
    }

private:
    CustomGalleryControl* parent;
    GalleryListDelegate* delegate;
};

CustomGalleryControl::CustomGalleryControl(QWidget* parent): QWidget(parent), data(NULL), model(NULL), ui(new Ui::CustomGalleryControl)
{
    ui->setupUi(this);

    splitter = new QSplitter(Qt::Vertical, this);

    wSplit1 = new QWidget(this);
    title1 = new TitleControl(this);
    list1 = new CustomGalleryList(this, this);

    wSplit2 = NULL;
    title2 = NULL;
    list2 = NULL;

    QLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(title1);
    layout->addWidget(list1);
    wSplit1->setLayout(layout);

    splitter->addWidget(wSplit1);

    ui->lLayout->addWidget(splitter);

    connect(list1, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenu1Event(QPoint)));

    create1MenuAndActions();
    updateButtons();
}

CustomGalleryControl::~CustomGalleryControl()
{
    if(wSplit2) {
        delete wSplit2;
        wSplit2 = NULL;
    }

    if(model) {
        delete model;
    }

    delete ui;
}

CustomGalleryData* CustomGalleryControl::getGallery()
{
    return data;
}

GalleryListModel* CustomGalleryControl::getModel()
{
    return model;
}

TitleControl* CustomGalleryControl::getTitle()
{
    return title1;
}

void CustomGalleryControl::selectItem(CustomGalleryItemData* value)
{
    const QModelIndex& i = model->getIndexByItem(value);
    list1->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void CustomGalleryControl::setGallery(CustomGalleryData* value)
{
    data = value;

    if(model) {
        delete model;
        model = NULL;
    }
    if(value) {
        model = new GalleryListModel(*value, (int)Config::getInstance()->getCustomGalleryIconSize(), this);

        list1->setModel(model->getProxy());
        connect(list1->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));

        if(list2) {
            list2->setModel(model->getProxy());
            connect(list2->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
        }
    }
}

void CustomGalleryControl::contextMenu1Event(const QPoint& pos)
{
    mMenu1->exec(list1->mapToGlobal(pos));
}

void CustomGalleryControl::contextMenu2Event(const QPoint& pos)
{
    mMenu2->exec(list2->mapToGlobal(pos));
}

void CustomGalleryControl::iconSizeChangedEvent(int value)
{
    switch(value) {
    case 0:
        Config::getInstance()->setCustomGalleryIconSize(Config::ELargeSize);
        break;

    case 1:
        Config::getInstance()->setCustomGalleryIconSize(Config::EMediumSize);
        break;

    case 2:
        Config::getInstance()->setCustomGalleryIconSize(Config::ESmallSize);
        break;

    default:
        break;
    }

    if(model) {
        model->changeItemsSize((int)Config::getInstance()->getCustomGalleryIconSize());
    }
    updateButtons();
}

void CustomGalleryControl::selectedEvent()
{
    updateButtons();
}

void CustomGalleryControl::uniteEvent()
{
    QList<CustomGalleryItemData*> items1;
    const QModelIndexList& sr1 = list1->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, sr1) {
        items1.append(qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1)));
    }
    qSort(items1.begin(), items1.end(), customGalleryItemDataComparer);

    QList<CustomGalleryItemData*> items2;
    const QModelIndexList& sr2 = list2->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, sr2) {
        items2.append(qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1)));
    }
    qSort(items2.begin(), items2.end(), customGalleryItemDataComparer);

    int i = 0;
    while(i < items1.count()) {
        UniteDialog* dialog = new UniteDialog(this, *items1.at(i), *items2.at(i));
        dialog->show();
        dialog->exec();

        if(dialog->result() == QDialog::Accepted) {
            if(dialog->getIsFirst()) {
                CustomGalleryHandler::getInstance()->updCustomGalleryItemUnite(*items2.at(i), items1.at(i)->getId());
                GalleryHandler::getInstance()->updGalleryItemReference(*items2.at(i), *items1.at(i));
            } else {
                CustomGalleryHandler::getInstance()->updCustomGalleryItemUnite(*items1.at(i), items2.at(i)->getId());
                GalleryHandler::getInstance()->updGalleryItemReference(*items1.at(i), *items2.at(i));
            }
        } else {
            break;
        }
        delete dialog;
        i++;
    }
}

void CustomGalleryControl::splitEvent()
{
    if(list1->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = list1->selectionModel()->selectedRows();
        foreach(const QModelIndex& index, sr) {
            CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
            CustomGalleryHandler::getInstance()->updCustomGalleryItemSplit(*item);
        }
        model->getProxy()->sort(0);
    }
}

void CustomGalleryControl::show1Event()
{
    if(list1->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = list1->selectionModel()->selectedRows();
        ViewerDialog* dialog = new ViewerDialog(NULL, reinterpret_cast<GalleryListModel&>(*model), sr[0].row());
        dialog->showMaximized();
        dialog->exec();

        //delete pDialog;
    }
}

void CustomGalleryControl::show2Event()
{
    if(list2->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = list2->selectionModel()->selectedRows();
        ViewerDialog* dialog = new ViewerDialog(NULL, reinterpret_cast<GalleryListModel&>(*model), sr[0].row());
        dialog->showMaximized();
        dialog->exec();

        //delete pDialog;
    }
}

void CustomGalleryControl::left1Event()
{
    if(list1->selectionModel()->selectedRows().count() == 1) {
        const QModelIndexList& rows = list1->selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*item, item->getAngle() - 90);
    }
}

void CustomGalleryControl::left2Event()
{
    if(list2->selectionModel()->selectedRows().count() == 1) {
        const QModelIndexList& rows = list2->selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*item, item->getAngle() - 90);
    }
}

void CustomGalleryControl::right1Event()
{
    if(list1->selectionModel()->selectedRows().count() == 1) {
        const QModelIndexList& rows = list1->selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*item, item->getAngle() + 90);
    }
}

void CustomGalleryControl::right2Event()
{
    if(list2->selectionModel()->selectedRows().count() == 1) {
        const QModelIndexList& rows = list2->selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*item, item->getAngle() + 90);
    }
}

void CustomGalleryControl::gotoChildrenEvent(GalleryItemData* gallery)
{
    emit onGotoItem(gallery);
}

void CustomGalleryControl::gotoEvent()
{
    const QModelIndexList& sr = list1->selectionModel()->selectedRows();
    const QModelIndex& index = sr[0];
    CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
    emit onGotoItem(&item->getItem());
}

void CustomGalleryControl::delEvent()
{
    if(list1->selectionModel()->selectedRows().count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            while(list1->selectionModel()->selectedRows().count() > 0) {
                const QModelIndexList& rows = list1->selectionModel()->selectedRows();
                const QModelIndex& index = rows.at(0);
                CustomGalleryItemData* item = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                CustomGalleryHandler::getInstance()->delFromCustomGallery(*item);
            }
        }
    }
}

void CustomGalleryControl::exitCustomGallery2Event()
{
    aEdit->setChecked(false);
    editEvent();
}

void CustomGalleryControl::editEvent()
{
    if(aEdit->isChecked()) {
        wSplit2 = new QWidget(this);

        title2 = new TitleControl(this);

        list2 = new CustomGalleryList(this, this);

        create2MenuAndActions();

        QLayout* layout = new QVBoxLayout();
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
        layout->addWidget(title2);
        layout->addWidget(list2);
        wSplit2->setLayout(layout);

        splitter->addWidget(wSplit2);

        list2->setModel(model->getProxy());
        connect(list2->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(selectedEvent()));
        connect(list2, SIGNAL(customContextMenuRequested(QPoint)), SLOT(contextMenu2Event(QPoint)));
        connect(title2->getCloseAction(), SIGNAL(triggered()), SLOT(exitCustomGallery2Event()));
    } else {
        wSplit2->setParent(NULL);
    }
}

void CustomGalleryControl::create1MenuAndActions()
{
    title1->getToolBar()->setVisible(true);
    mMenu1 = new QMenu(this);

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
    title1->getToolBar()->addWidget(cbSize);
    connect(cbSize, SIGNAL(currentIndexChanged(int)), this, SLOT(iconSizeChangedEvent(int)));

    aShow1 = new QAction("&Show", this);
    aShow1->setStatusTip("Show active item");
    aShow1->setIcon(QIcon(":/res/resources/gallery.png"));
    title1->getToolBar()->addAction(aShow1);
    mMenu1->addAction(aShow1);
    connect(aShow1, SIGNAL(triggered()), this, SLOT(show1Event()));

    aLeft1 = new QAction("&Rotate Left", this);
    aLeft1->setStatusTip("Rotate Left selected items");
    aLeft1->setIcon(QIcon(":/res/resources/rotate_left.png"));
    title1->getToolBar()->addAction(aLeft1);
    mMenu1->addAction(aLeft1);
    connect(aLeft1, SIGNAL(triggered()), this, SLOT(left1Event()));

    aRight1 = new QAction("&Rotate Right", this);
    aRight1->setStatusTip("Rotate Right selected items");
    aRight1->setIcon(QIcon(":/res/resources/rotate_right.png"));
    title1->getToolBar()->addAction(aRight1);
    mMenu1->addAction(aRight1);
    connect(aRight1, SIGNAL(triggered()), this, SLOT(right1Event()));

    mGotoMenu = new QMenu(this);
    mGotoMenu->menuAction()->setText("&Goto ...");
    mGotoMenu->menuAction()->setStatusTip("Selects gallery Items");
    mGotoMenu->menuAction()->setIcon(QIcon(":/res/resources/right.png"));
    title1->getToolBar()->addAction(mGotoMenu->menuAction());
    mMenu1->addAction(mGotoMenu->menuAction());
    connect(mGotoMenu->menuAction(), SIGNAL(triggered()), this, SLOT(gotoEvent()));

    aEdit = new QAction("&Edit", this);
    aEdit->setStatusTip("Edit mode");
    aEdit->setIcon(QIcon(":/res/resources/edit.png"));
    aEdit->setCheckable(true);
    title1->getToolBar()->addAction(aEdit);
    mMenu1->addAction(aEdit);
    connect(aEdit, SIGNAL(triggered()), this, SLOT(editEvent()));

    aUnite = new QAction("&Unite", this);
    aUnite->setStatusTip("Unite two selected items");
    aUnite->setIcon(QIcon(":/res/resources/unite.png"));
    title1->getToolBar()->addAction(aUnite);
    mMenu1->addAction(aUnite);
    connect(aUnite, SIGNAL(triggered()), this, SLOT(uniteEvent()));

    aSplit = new QAction("&Split", this);
    aSplit->setStatusTip("Split item");
    aSplit->setIcon(QIcon(":/res/resources/split.png"));
    title1->getToolBar()->addAction(aSplit);
    mMenu1->addAction(aSplit);
    connect(aSplit, SIGNAL(triggered()), this, SLOT(splitEvent()));

    aDel = new QAction("&Delete", this);
    aDel->setStatusTip("Delete selected items");
    aDel->setIcon(QIcon(":/res/resources/del.png"));
    title1->getToolBar()->addAction(aDel);
    mMenu1->addAction(aDel);
    connect(aDel, SIGNAL(triggered()), this, SLOT(delEvent()));
}

void CustomGalleryControl::create2MenuAndActions()
{
    title2->getToolBar()->setVisible(true);
    mMenu2 = new QMenu(this);

    aShow2 = new QAction("&Show", this);
    aShow2->setStatusTip("Show active item");
    aShow2->setIcon(QIcon(":/res/resources/gallery.png"));
    title2->getToolBar()->addAction(aShow2);
    mMenu2->addAction(aShow2);
    connect(aShow2, SIGNAL(triggered()), this, SLOT(show2Event()));

    aLeft2 = new QAction("&Rotate Left", this);
    aLeft2->setStatusTip("Rotate Left selected items");
    aLeft2->setIcon(QIcon(":/res/resources/rotate_left.png"));
    title2->getToolBar()->addAction(aLeft2);
    mMenu2->addAction(aLeft2);
    connect(aLeft2, SIGNAL(triggered()), this, SLOT(left2Event()));

    aRight2 = new QAction("&Rotate Right", this);
    aRight2->setStatusTip("Rotate Right selected items");
    aRight2->setIcon(QIcon(":/res/resources/rotate_right.png"));
    title2->getToolBar()->addAction(aRight2);
    mMenu2->addAction(aRight2);
    connect(aRight2, SIGNAL(triggered()), this, SLOT(right2Event()));

    title2->getToolBar()->addAction(aUnite);
    mMenu2->addAction(aUnite);
}

void CustomGalleryControl::updateButtons()
{
    QList<CustomGalleryItemData*> items1;
    if(list1->selectionModel() && list1->selectionModel()->selectedRows().count() == 1) {
        aShow1->setEnabled(true);
        aLeft1->setEnabled(true);
        aRight1->setEnabled(true);
    } else {
        aShow1->setEnabled(false);
        aLeft1->setEnabled(false);
        aRight1->setEnabled(false);
    }

    if(list1->selectionModel() && list1->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = list1->selectionModel()->selectedRows();

        foreach(const QModelIndex& index, sr) {
            items1.append(qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1)));
        }

        aDel->setEnabled(true);

        if(sr.count() == 1) {
            mGotoMenu->clear();
            mGotoMenu->setEnabled(true);
            QAction* action = mGotoMenu->addAction(items1.at(0)->getItem().getGallery()->getSource() + "/" + items1.at(0)->getItem().getFileName());
            connect(action, SIGNAL(triggered()), this, SLOT(gotoEvent()));

            QList<CustomGalleryItemData*> children;
            CommonHelper::getChildren(*items1.at(0), children);

            foreach(CustomGalleryItemData* i, children) {
                GalleryItemAction* action = new GalleryItemAction(this, &i->getItem());
                action->setText(i->getItem().getGallery()->getSource() + "/" + i->getItem().getFileName());
                connect(action, SIGNAL(onActivateGallery(GalleryItemData*)), this, SLOT(gotoChildrenEvent(GalleryItemData*)));
                mGotoMenu->addAction(action);
            }
        } else {
            mGotoMenu->setEnabled(false);
        }
    } else {
        aDel->setEnabled(false);
        mGotoMenu->setEnabled(false);
    }

    bool isSplit = false;
    foreach(CustomGalleryItemData* i, items1) {
        if(i->getChildren().count() > 0) {
            isSplit = true;
        } else {
            isSplit = false;
            break;
        }
    }

    if(isSplit) {
        aSplit->setEnabled(true);
    } else {
        aSplit->setEnabled(false);
    }

    bool isCanUnite = false;
    if(list2 && list2->selectionModel()) {
        if(list2->selectionModel()->selectedRows().count() == 1) {
            aShow2->setEnabled(true);
            aLeft2->setEnabled(true);
            aRight2->setEnabled(true);
        } else {
            aShow2->setEnabled(false);
            aLeft2->setEnabled(false);
            aRight2->setEnabled(false);
        }

        QList<CustomGalleryItemData*> items2;
        if(list2->selectionModel()->selectedRows().count() > 0) {
            isCanUnite = true;
            const QModelIndexList& sr = list2->selectionModel()->selectedRows();

            foreach(const QModelIndex& index, sr) {
                CustomGalleryItemData* i = qvariant_cast<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                if(items1.contains(i)) {
                    isCanUnite = false;
                    break;
                }
                items2.append(i);
            }
        }

        if(isCanUnite) {
            if(items1.count() != items2.count()) {
                isCanUnite = false;
            }
        }
    }

    if(isCanUnite) {
        aUnite->setEnabled(true);
    } else {
        aUnite->setEnabled(false);
    }
}
