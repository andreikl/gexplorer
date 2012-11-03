#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QListView>
#include <QtGui/QComboBox>
#include <QtGui/QMenu>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDrag>

#include "config.h"

#include "data/customgalleryitemdata.h"
#include "data/galleryitemdata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"

#include "ui/common/gallerylistdelegate.h"
#include "ui/common/gallerylistmodel.h"

#include "ui/controls/customgallerycontrol.h"
#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"
#include "ui/dialogs/unitedialog.h"

#include "ui_customgallerycontrol.h"
#include "ui_titlecontrol.h"

bool CustomGalleryItemDataComparer(const CustomGalleryItemData* pItem1, const CustomGalleryItemData* pItem2) {
    return pItem1->getName() < pItem2->getName();
}

class CustomGalleryList: public QListView {
public:
    CustomGalleryList(CustomGalleryControl* pParent): QListView(pParent) {
        pDelegate = new GalleryListDelegate(this);
        setItemDelegate(pDelegate);

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        setViewMode(QListView::IconMode);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragDrop);
    }

    virtual ~CustomGalleryList() {
        if(pDelegate) {
            delete pDelegate;
        }
    }

protected:
    void dragEnterEvent(QDragEnterEvent* pValue) {
        if(pValue->mimeData()->hasFormat(QString(Config::ApplicationSignature()) + "Item")) {
            return pValue->accept();
        }
        pValue->ignore();
    }

    void dragMoveEvent(QDragMoveEvent* pValue) {
        if(model()->rowCount() > 0) {
            const QModelIndex& index = indexAt(pValue->pos());
            if(index.isValid()) {
                const QRect& rect = rectForIndex(index);
                bool isBefore = (pValue->pos().x() < rect.x() + rect.width() / 2)? true: false;
                if(pDelegate->DragIndex() != index || pDelegate->IsDragBefore() != isBefore) {
                    pDelegate->DragIndex(index, isBefore);
                }
                return pValue->accept();
            } else if(pDelegate->DragIndex() != index) {
                pDelegate->DragIndex(index, false);
            }
        } else {
            return pValue->accept();
        }
        pValue->ignore();
    }

    void dragLeaveEvent(QDragLeaveEvent* pValue) {
        pDelegate->DragIndex(QModelIndex(), false);
        pValue->accept();
    }

protected:
    void dropEvent(QDropEvent* pValue) {
        CustomGalleryData* pGallery = reinterpret_cast<CustomGalleryControl*>(parent())->Gallery();
        GalleryListModel* pModel = reinterpret_cast<CustomGalleryControl*>(parent())->Model();

        QByteArray data = pValue->mimeData()->data(QString(Config::ApplicationSignature()) + "Item");
        QDataStream stream(&data, QIODevice::ReadOnly);
        int type;
        stream >> type;

        QList<CustomGalleryItemData*> items;

        if(type == Config::ETypeCustomGalleryItem) {
            const QModelIndexList& indexes = selectedIndexes();
            foreach(const QModelIndex& index, indexes) {
                CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                items.append(pItem);
            }
            qSort(items.begin(), items.end(), CustomGalleryItemDataComparer);
        } else  if(type == Config::ETypeGalleryItem) {
            while(!stream.atEnd()) {
                int gid;
                int id;
                stream >> gid >> id;

                //GalleryItemData* pItem = GalleryHandler::getInstance()->getGalleryItemById(gid, id);
                //CustomGalleryItemData* pCustomItem = CustomGalleryHandler::getInstance()->addToCustomGallery(*pGallery, *pItem, pItem->getFileName());
                CustomGalleryItemData* pCustomItem = CustomGalleryHandler::getInstance()->addToCustomGallery(*pGallery, gid, id);
                if(pCustomItem != NULL) {
                    items.append(pCustomItem);
                }
            }
        }

        if(items.count() > 0) {
            int row = 0;
            if(pDelegate->DragIndex().isValid()) {
                row = pDelegate->DragIndex().row();
                if(!pDelegate->IsDragBefore()) {
                    row++;
                }
            }
            int n = 1;
            for(int i = 0; i < row; i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                const QModelIndex& index = pModel->Proxy()->index(i, 0);
                CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*pItem, name);
            }
            n = row + items.count() + 1;
            for(int i = row; i < pModel->rowCount(); i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                const QModelIndex& index = pModel->Proxy()->index(i, 0);
                CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*pItem, name);
            }
            n = row + 1;
            for(int i = 0; i < items.count(); i++) {
                QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                CustomGalleryHandler::getInstance()->updCustomGalleryItemName(*items[i], name);
            }
            pModel->Proxy()->sort(0);
        }

        pDelegate->DragIndex(QModelIndex(), false);
        pValue->accept();
    }

    void startDrag(Qt::DropActions) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        stream << Config::ETypeCustomGalleryItem;

        QDrag* pDrag = new QDrag(this);
        pDrag->setMimeData(new QMimeData());
        pDrag->mimeData()->setData(QString(Config::ApplicationSignature()) + "Item", data);
        pDrag->setPixmap(QPixmap(":/res/resources/gallery.png"));
        pDrag->exec(Qt::MoveAction);

        //delete pDrag;
    }

private:
    GalleryListDelegate* pDelegate;
};

CustomGalleryControl::CustomGalleryControl(QWidget* pParent): QWidget(pParent), pData(NULL), pModel(NULL), pUi(new Ui::CustomGalleryControl) {
    pUi->setupUi(this);

    pTitle = new TitleControl(this);
    pUi->plLayout->addWidget(pTitle);

    pList = new CustomGalleryList(this);
    pUi->plLayout->addWidget(pList);

    CreateMenuAndActions();
    UpdateButtons();
}

CustomGalleryControl::~CustomGalleryControl() {
    delete pUi;

    if(pModel) {
        delete pModel;
    }
}

GalleryListModel* CustomGalleryControl::Model() {
    return pModel;
}

TitleControl* CustomGalleryControl::Title() {
    return pTitle;
}

void CustomGalleryControl::Gallery(CustomGalleryData* pValue) {
    pData = pValue;

    if(pModel) {
        delete pModel;
        pModel = NULL;
    }
    if(pValue) {
        pModel = new GalleryListModel(*pValue, (int)Config::Self()->CustomGalleryIconSize(), this);

        pList->setModel(pModel->Proxy());
        connect(pList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(SelectedEvent()));
    }
}

CustomGalleryData* CustomGalleryControl::Gallery() {
    return pData;
}

void CustomGalleryControl::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void CustomGalleryControl::IconSizeChangedEvent(int value) {
    switch(value) {
    case 0:
        Config::Self()->CustomGalleryIconSize(Config::ELargeSize);
        break;

    case 1:
        Config::Self()->CustomGalleryIconSize(Config::EMediumSize);
        break;

    case 2:
        Config::Self()->CustomGalleryIconSize(Config::ESmallSize);
        break;

    default:
        break;
    }

    if(pModel) {
        pModel->ChangeItemsSize((int)Config::Self()->CustomGalleryIconSize());
    }
    UpdateButtons();
}

void CustomGalleryControl::ShowEvent() {
    if(pList->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = pList->selectionModel()->selectedRows();
        ViewerDialog* pDialog = new ViewerDialog(NULL, reinterpret_cast<GalleryListModel&>(*pModel), sr[0].row());
        pDialog->showFullScreen();
        pDialog->exec();

        //delete pDialog;
    }
}

void CustomGalleryControl::LeftEvent() {
    const QModelIndexList& rows = pList->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*pItem, pItem->getAngle() - 90);
    }
}

void CustomGalleryControl::RightEvent() {
    const QModelIndexList& rows = pList->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
        CustomGalleryHandler::getInstance()->updCustomGalleryItemAngle(*pItem, pItem->getAngle() + 90);
    }
}

void CustomGalleryControl::UniteEvent() {
    if(pList->selectionModel()->selectedRows().count() == 2) {
        const QModelIndexList& indexes = pList->selectionModel()->selectedRows();
        QList<CustomGalleryItemData*> items;
        foreach(const QModelIndex& index, indexes) {
            CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
            items.append(pItem);
        }
        qSort(items.begin(), items.end(), CustomGalleryItemDataComparer);

        UniteDialog* pDialog = new UniteDialog(this, *items.at(0), *items.at(1));
        pDialog->show();
        pDialog->exec();

        if(pDialog->result() == QDialog::Accepted) {
            if(pDialog->IsFirst()) {
                CustomGalleryHandler::getInstance()->updCustomGalleryItemCustomId(*items.at(1), items.at(0)->getId());
            } else {
                CustomGalleryHandler::getInstance()->updCustomGalleryItemCustomId(*items.at(0), items.at(1)->getId());
            }
        }
        delete pDialog;
    }
}

void CustomGalleryControl::DelEvent() {
    if(pList->selectionModel()->selectedRows().count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            while(pList->selectionModel()->selectedRows().count() > 0) {
                const QModelIndexList& rows = pList->selectionModel()->selectedRows();
                const QModelIndex& index = rows.at(0);
                CustomGalleryItemData* pItem = qVariantValue<CustomGalleryItemData*>(index.data(Qt::UserRole + 1));
                CustomGalleryHandler::getInstance()->delFromCustomGallery(*pItem);
            }
        }
    }
}

void CustomGalleryControl::SelectedEvent() {
    UpdateButtons();
}

void CustomGalleryControl::CreateMenuAndActions() {
    pTitle->ToolBar()->setVisible(true);
    pmMenu = new QMenu(this);

    pcbSize = new QComboBox(this);
    pcbSize->setMinimumSize(100, 30);
    pcbSize->addItem("Large");
    pcbSize->addItem("Medium");
    pcbSize->addItem("Small");
    switch(Config::Self()->GalleryIconSize()) {
    case Config::ELargeSize:
        pcbSize->setCurrentIndex(0);
        break;

    case Config::EMediumSize:
        pcbSize->setCurrentIndex(1);
        break;

    case Config::ESmallSize:
        pcbSize->setCurrentIndex(2);
        break;

    default:
        break;
    }
    pTitle->ToolBar()->addWidget(pcbSize);
    connect(pcbSize, SIGNAL(currentIndexChanged(int)), this, SLOT(IconSizeChangedEvent(int)));

    paShow = new QAction("&Show", this);
    paShow->setStatusTip("Show active item");
    paShow->setIcon(QIcon(":/res/resources/view.png"));
    pTitle->ToolBar()->addAction(paShow);
    pmMenu->addAction(paShow);
    connect(paShow, SIGNAL(triggered()), this, SLOT(ShowEvent()));

    paLeft = new QAction("&Rotate Left", this);
    paLeft->setStatusTip("Rotate Left selected items");
    paLeft->setIcon(QIcon(":/res/resources/rotate_left.png"));
    pTitle->ToolBar()->addAction(paLeft);
    pmMenu->addAction(paLeft);
    connect(paLeft, SIGNAL(triggered()), this, SLOT(LeftEvent()));

    paRight = new QAction("&Rotate Right", this);
    paRight->setStatusTip("Rotate Right selected items");
    paRight->setIcon(QIcon(":/res/resources/rotate_right.png"));
    pTitle->ToolBar()->addAction(paRight);
    pmMenu->addAction(paRight);
    connect(paRight, SIGNAL(triggered()), this, SLOT(RightEvent()));

    paUnite = new QAction("&Unite", this);
    paUnite->setStatusTip("Unite two selected items");
    paUnite->setIcon(QIcon(":/res/resources/unite.png"));
    pTitle->ToolBar()->addAction(paUnite);
    pmMenu->addAction(paUnite);
    connect(paUnite, SIGNAL(triggered()), this, SLOT(UniteEvent()));

    paDel = new QAction("&Delete", this);
    paDel->setStatusTip("Delete selected items");
    paDel->setIcon(QIcon(":/res/resources/del.png"));
    pTitle->ToolBar()->addAction(paDel);
    pmMenu->addAction(paDel);
    connect(paDel, SIGNAL(triggered()), this, SLOT(DelEvent()));
}

void CustomGalleryControl::UpdateButtons() {
    QItemSelectionModel* pSelection = pList->selectionModel();
    if(pSelection && pSelection->selectedRows().count() > 0) {
        paShow->setEnabled(true);
        paLeft->setEnabled(true);
        paRight->setEnabled(true);
        if(pSelection->selectedRows().count() == 2) {
            paUnite->setEnabled(true);
        } else {
            paUnite->setEnabled(false);
        }
        paDel->setEnabled(true);
    } else {
        paShow->setEnabled(false);
        paLeft->setEnabled(false);
        paRight->setEnabled(false);
        paUnite->setEnabled(false);
        paDel->setEnabled(false);
    }
}
