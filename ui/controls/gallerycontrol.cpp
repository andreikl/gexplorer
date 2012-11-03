#include <QtGui/QSortFilterProxyModel>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QListView>
#include <QtGui/QComboBox>
#include <QtGui/QDrag>
#include <QtGui/QMenu>

#include "config.h"

#include "data/galleryitemdata.h"
#include "data/gallerydata.h"

#include "ui/common/gallerylistdelegate.h"
#include "ui/common/gallerylistmodel.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/viewerdialog.h"

#include "ui_gallerycontrol.h"
#include "ui_titlecontrol.h"

#include "ui/controls/gallerycontrol.h"

bool GalleryItemDataComparer(const GalleryItemData* pItem1, const GalleryItemData* pItem2) {
    return pItem1->getFileName() < pItem2->getFileName();
}

class GalleryList: public QListView {
public:
    GalleryList(GalleryControl* pParent): QListView(pParent) {
        setItemDelegate(new GalleryListDelegate(this));

        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

        setViewMode(QListView::IconMode);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QAbstractItemView::DragOnly);
    }

    virtual ~GalleryList() {
    }

protected:
    void startDrag(Qt::DropActions) {
        QByteArray data;
        QDataStream stream(&data, QIODevice::WriteOnly);

        stream << Config::ETypeGalleryItem;

        QList<GalleryItemData*> items;
        const QModelIndexList& indexes = selectedIndexes();
        foreach(const QModelIndex& index, indexes) {
            GalleryItemData* pItem = qVariantValue<GalleryItemData*>(index.data(Qt::UserRole + 1));
            items.append(pItem);
        }
        qSort(items.begin(), items.end(), GalleryItemDataComparer);
        foreach(GalleryItemData* pItem, items) {
            stream << pItem->getGallery()->getId() << pItem->getId();
        }

        QDrag* pDrag = new QDrag(this);
        pDrag->setMimeData(new QMimeData());
        pDrag->mimeData()->setData(QString(Config::ApplicationSignature()) + "Item", data);
        pDrag->setPixmap(QPixmap(":/res/resources/gallery.png"));
        //pDrag->setPixmap(pItem->Pixmap());
        pDrag->exec(Qt::CopyAction);

        //delete pDrag;
    }
};

GalleryControl::GalleryControl(QWidget* pParent): QWidget(pParent), pData(NULL), pModel(NULL), pUi(new Ui::GalleryControl) {
    pUi->setupUi(this);

    pTitle = new TitleControl(this);
    pUi->plLayout->addWidget(pTitle);

    pList = new GalleryList(this);
    pUi->plLayout->addWidget(pList);

    CreateMenuAndActions();
    UpdateButtons();
}

GalleryControl::~GalleryControl() {
    delete pUi;

    if(pModel) {
        delete pModel;
    }
}

GalleryListModel* GalleryControl::Model() {
    return pModel;
}

TitleControl* GalleryControl::Title() {
    return pTitle;
}

void GalleryControl::Gallery(GalleryData* pValue) {
    pData = pValue;

    if(pModel) {
        delete pModel;
        pModel = NULL;
    }

    if(pValue) {
        pModel = new GalleryListModel(*pValue, (int)Config::Self()->GalleryIconSize(), this);

        pList->setModel(pModel->Proxy());
        connect(pList->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(SelectedEvent()));
    }
}

const GalleryData* GalleryControl::Gallery() const {
    return pData;
}

void GalleryControl::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void GalleryControl::IconSizeChangedEvent(int value) {
    switch(value) {
    case 0:
        Config::Self()->GalleryIconSize(Config::ELargeSize);
        break;

    case 1:
        Config::Self()->GalleryIconSize(Config::EMediumSize);
        break;

    case 2:
        Config::Self()->GalleryIconSize(Config::ESmallSize);
        break;

    default:
        break;
    }

    if(pModel) {
        pModel->ChangeItemsSize((int)Config::Self()->GalleryIconSize());
    }
    UpdateButtons();
}

void GalleryControl::ShowEvent() {
    if(pList->selectionModel()->selectedRows().count() > 0) {
        const QModelIndexList& sr = pList->selectionModel()->selectedRows();
        ViewerDialog* pDialog = new ViewerDialog(NULL, reinterpret_cast<GalleryListModel&>(*pModel), sr[0].row());
        pDialog->showFullScreen();
        pDialog->exec();

        //delete pDialog;
    }
}

void GalleryControl::SelectedEvent() {
    UpdateButtons();
}

void GalleryControl::CreateMenuAndActions() {
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
}

void GalleryControl::UpdateButtons() {
    QItemSelectionModel* pSelection = pList->selectionModel();
    if(pSelection && pSelection->selectedRows().count() > 0) {
        paShow->setEnabled(true);
    } else {
        paShow->setEnabled(false);
    }
}
