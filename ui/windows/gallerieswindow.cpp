#include <QtCore/QMimeData>
#include <QtCore/QDebug>
#include <QtGui/QMenu>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QMessageBox>
#include <QtGui/QTreeView>
#include <QtGui/QDrag>

#include "data/gallerydata.h"

#include "handlers/galleryhandler.h"

#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/addgallerydialog.h"

#include "ui/windows/gallerieswindow.h"

#include "ui_titlecontrol.h"
#include "ui_gallerieswindow.h"

class GalleriesTree: public QTreeView {

public:
    GalleriesTree(QWidget* pParent = NULL): QTreeView(pParent) {
        setHeaderHidden(true);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QTreeView::DragOnly);

        pModel = new GalleryTreeModel(Config::ETypeGallery, this);
        setModel(pModel);
    }

    virtual ~GalleriesTree() {
        if(!pModel) {
            delete pModel;
        }
    }

private slots:
    void AddGalleryEvent(GalleryTreeModel*, GalleryData* pGallery) {
        const QModelIndex& i = pModel->GetIndex(pGallery);
        selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
    }

protected:
    void startDrag(Qt::DropActions) {
        const QModelIndexList& rows = selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->Gallery()) {
            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << Config::ETypeGallery << pData->Gallery()->getId();

            QDrag* pDrag = new QDrag(this);
            pDrag->setMimeData(new QMimeData());
            pDrag->mimeData()->setData(Config::ApplicationSignature(), data);
            pDrag->setPixmap(QPixmap(":/res/resources/gallery.png"));
            pDrag->exec(Qt::CopyAction);
        }
    }
public:
    GalleryTreeModel* pModel;
};

GalleriesWindow::GalleriesWindow(QWidget* pParent): QDockWidget(pParent), pTitle(NULL), pUi(new Ui::GalleriesWindow) {
    pUi->setupUi(this);

    pTitle =  new TitleControl(this);
    setTitleBarWidget(pTitle);

    ptwGalleries = new GalleriesTree(this);
    pUi->plContent->addWidget(ptwGalleries);

    connect(ptwGalleries->pModel, SIGNAL(OnAddGallery(GalleryTreeModel*, GalleryData*)), this, SLOT(AddGalleryEvent(GalleryTreeModel*, GalleryData*)));

    connect(ptwGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(SelectedEvent()));
    connect(ptwGalleries, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(ActivatedEvent()));

    CreateMenuAndActions();
    UpdateButtons();
}

GalleriesWindow::~GalleriesWindow() {
    delete pUi;
}

void GalleriesWindow::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void GalleriesWindow::AddEvent() {
    AddGalleryDialog* pGallery = new AddGalleryDialog(this);
    pGallery->show();
    pGallery->exec();

    if(pGallery->result() == QDialog::Accepted) {
        QString source = pGallery->Source();
        emit OnAdd(source);
    }
    delete pGallery;
}

void GalleriesWindow::DelEvent() {
    QList<GalleryData*> galleries;
    const QModelIndexList& rows = ptwGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->Gallery()) {
            galleries.append(pData->Gallery());
        }
    }

    if(galleries.count() > 0) {
        if(QMessageBox::Yes == QMessageBox::question(this, windowTitle(), "Are you sure?", QMessageBox::Yes | QMessageBox::No, QMessageBox::No)) {
            foreach(GalleryData* pGallery, galleries) {
                GalleryHandler::getInstance()->delGallery(*pGallery);
            }
        }
    }
}

void GalleriesWindow::BrowseEvent() {
    const QModelIndex& index = ptwGalleries->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
    if(pData->Gallery()) {
        emit OnBrowse(pData->Gallery());
    }
}

void GalleriesWindow::ExitEvent() {
    Config::Self()->IsGalleriesWindow(false);
    close();
}

void GalleriesWindow::AddGalleryEvent(GalleryTreeModel*, GalleryData* pGallery) {
    const QModelIndex& i = ptwGalleries->pModel->GetIndex(pGallery);
    ptwGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void GalleriesWindow::ActivatedEvent() {
    const QModelIndex& index = ptwGalleries->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
    if(pData->Gallery()) {
        emit OnGallery(pData->Gallery());
    }
}

void GalleriesWindow::SelectedEvent() {
    UpdateButtons();
}

void GalleriesWindow::CreateMenuAndActions() {

    pTitle->ToolBar()->setVisible(true);
    pmMenu = new QMenu(this);

    paAdd = new QAction("&Add", this);
    paAdd->setStatusTip("Add new gallery");
    paAdd->setIcon(QIcon(":/res/resources/add.png"));
    pTitle->ToolBar()->addAction(paAdd);
    pmMenu->addAction(paAdd);
    connect(paAdd, SIGNAL(triggered()), this, SLOT(AddEvent()));

    paShow = new QAction("&Show", this);
    paShow->setStatusTip("Show active gallery");
    paShow->setIcon(QIcon(":/res/resources/view.png"));
    pTitle->ToolBar()->addAction(paShow);
    pmMenu->addAction(paShow);
    connect(paShow, SIGNAL(triggered()), this, SLOT(ActivatedEvent()));

    paBrowse = new QAction("&Browse", this);
    paBrowse->setStatusTip("Browse active gallery");
    paBrowse->setIcon(QIcon(":/res/resources/view.png"));
    pTitle->ToolBar()->addAction(paBrowse);
    pmMenu->addAction(paBrowse);
    connect(paBrowse, SIGNAL(triggered()), this, SLOT(BrowseEvent()));

    paDel = new QAction("&Delete", this);
    paDel->setStatusTip("Delete selected galleries");
    paDel->setIcon(QIcon(":/res/resources/del.png"));
    pTitle->ToolBar()->addAction(paDel);
    pmMenu->addAction(paDel);
    connect(paDel, SIGNAL(triggered()), this, SLOT(DelEvent()));

    pmMenu->addAction(pTitle->CloseAction());
    connect(pTitle->CloseAction(), SIGNAL(triggered()), SLOT(ExitEvent()));
}

void GalleriesWindow::UpdateButtons() {
    GalleryData* pGallery = NULL;
    const QModelIndexList& rows = ptwGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->Gallery()) {
            pGallery = pData->Gallery();
            break;
        }
    }

    if(pGallery) {
        paShow->setEnabled(true);
        if(pGallery->getType() == GalleryData::WebGallery) {
            paBrowse->setEnabled(true);
        } else {
            paBrowse->setEnabled(false);
        }
        paDel->setEnabled(true);
    } else {
        paShow->setEnabled(false);
        paBrowse->setEnabled(false);
        paDel->setEnabled(false);
    }
}
