#include <QtCore/QDebug>

#include <QtGui/QMessageBox>
#include <QtGui/QTreeView>
#include <QtGui/QMenu>

#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QDrag>

#include "data/customgalleryitemdata.h"
#include "data/customgallerydata.h"
#include "data/gallerydata.h"

#include "handlers/customgalleryhandler.h"
#include "handlers/galleryhandler.h"

#include "ui/common/gallerytreemodel.h"
#include "ui/common/gallerytreeitem.h"

#include "ui/controls/titlecontrol.h"

#include "ui/dialogs/editcustomgallerydialog.h"

#include "ui/windows/customgallerieswindow.h"

#include "ui_customgallerieswindow.h"
#include "ui_titlecontrol.h"

class CustomGalleriesTree: public QTreeView {

public:
    CustomGalleriesTree(QWidget* pParent = NULL): QTreeView(pParent) {
        setHeaderHidden(true);

        setSelectionMode(QAbstractItemView::ExtendedSelection);

        setDragEnabled(true);
        setDragDropMode(QTreeView::DragDrop);

        pModel = new GalleryTreeModel(Config::ETypeCustomGallery, this);
        setModel(pModel);
    }

    virtual ~CustomGalleriesTree() {
        if(!pModel) {
            delete pModel;
        }
    }

protected:
    void dragEnterEvent(QDragEnterEvent* pValue) {
        if (pValue->mimeData()->hasFormat(Config::ApplicationSignature())) {
            QByteArray data = pValue->mimeData()->data(Config::ApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeGallery) {
                return pValue->accept();
            }
        }
        pValue->ignore();
    }

    void dragMoveEvent(QDragMoveEvent* pValue) {
        pValue->accept();
    }

    void dragLeaveEvent(QDragLeaveEvent* pValue) {
        pValue->accept();
    }

protected:
    void dropEvent(QDropEvent* pValue) {
        if (pValue->mimeData()->hasFormat(Config::ApplicationSignature())) {
            QByteArray data = pValue->mimeData()->data(Config::ApplicationSignature());
            QDataStream dataStream(&data, QIODevice::ReadOnly);

            int type = 0;
            dataStream >> type;
            if(type == Config::ETypeGallery) {
                int id = 0;
                dataStream >> id;

                /*GalleryData* pGallery = GalleryHandler::getInstance()->getGalleryById(id);
                CustomGalleryData* pCustomGallery = new CustomGalleryData();
                pCustomGallery->setName(pGallery->getSource());

                int n = 1;
                foreach(GalleryItemData* pItem, pGallery->getItems()) {
                    CustomGalleryItemData* pCustomItem = new CustomGalleryItemData(*pCustomGallery, *pItem);
                    QString name = QString("%1").arg(n++, 3, 10, QChar('0'));
                    pCustomItem->setName(name);
                }

                CustomGalleryData* pcg = CustomGalleryHandler::getInstance()->addCustomGallery(*pCustomGallery);
                if(pcg) {
                    const QModelIndex& i = pModel->GetIndex(pcg);
                    selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }

                delete pCustomGallery;*/

                CustomGalleryData* customGallery = CustomGalleryHandler::getInstance()->addCustomGallery(id);
                if(customGallery) {
                    const QModelIndex& i = pModel->GetIndex(customGallery);
                    selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
                }

                return pValue->accept();
            }
        }
        pValue->ignore();
    }

    void startDrag(Qt::DropActions /*actions*/) {
        const QModelIndexList& rows = selectionModel()->selectedRows();
        const QModelIndex& index = rows.at(0);
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->CustomGallery()) {
            QByteArray data;
            QDataStream dataStream(&data, QIODevice::WriteOnly);
            dataStream << Config::ETypeCustomGallery << pData->CustomGallery()->getId();

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

CustomGalleriesWindow::CustomGalleriesWindow(QWidget* pParent): QDockWidget(pParent), pTitle(NULL), pUi(new Ui::CustomGalleriesWindow) {
    pUi->setupUi(this);

    pTitle =  new TitleControl(this);
    setTitleBarWidget(pTitle);

    ptwGalleries = new CustomGalleriesTree(this);
    pUi->plContent->addWidget(ptwGalleries);

    connect(ptwGalleries->selectionModel(), SIGNAL(selectionChanged(QItemSelection, QItemSelection)), SLOT(SelectedEvent()));
    connect(ptwGalleries, SIGNAL(doubleClicked(const QModelIndex&)), SLOT(ActivatedEvent()));

    CreateMenuAndActions();
    UpdateButtons();
}

CustomGalleriesWindow::~CustomGalleriesWindow() {
    delete pUi;
}

void CustomGalleriesWindow::selectGallery(CustomGalleryData* value) {
    const QModelIndex& i = ptwGalleries->pModel->GetIndex(value);
    ptwGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
}

void CustomGalleriesWindow::contextMenuEvent(QContextMenuEvent* pEvent) {
    pmMenu->exec(pEvent->globalPos());
}

void CustomGalleriesWindow::AddEvent() {
    CustomGalleryData* pGallery = new CustomGalleryData();
    EditCustomGalleryDialog* pDialog = new EditCustomGalleryDialog(this, *pGallery);
    pDialog->show();
    pDialog->exec();

    if(pDialog->result() == QDialog::Accepted) {
        CustomGalleryData* pcg = CustomGalleryHandler::getInstance()->addCustomGallery(*pGallery);
        if(pcg) {
            const QModelIndex& i = ptwGalleries->pModel->GetIndex(pcg);
            ptwGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
        }
    }
    delete pDialog;
    delete pGallery;
}

void CustomGalleriesWindow::DelEvent() {
    QList<CustomGalleryData*> galleries;
    const QModelIndexList& rows = ptwGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->CustomGallery()) {
            galleries.append(pData->CustomGallery());
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

void CustomGalleriesWindow::EditEvent() {
    const QModelIndexList& rows = ptwGalleries->selectionModel()->selectedRows();
    const QModelIndex& index = rows.at(0);
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
    if(pData->CustomGallery()) {
        CustomGalleryData* pCloneGallery = pData->CustomGallery()->clone();
        EditCustomGalleryDialog* pDialog = new EditCustomGalleryDialog(this, *pCloneGallery);
        pDialog->show();
        pDialog->exec();

        if(pDialog->result() == QDialog::Accepted) {
            CustomGalleryData* pcg = CustomGalleryHandler::getInstance()->updCustomGallery(*pCloneGallery);
            if(pcg) {
                const QModelIndex& i = ptwGalleries->pModel->GetIndex(pcg);
                ptwGalleries->selectionModel()->setCurrentIndex(i, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows);
            }
        }
        delete pDialog;
        delete pCloneGallery;
    }
}

void CustomGalleriesWindow::ExitEvent() {
    Config::Self()->IsCustomGalleriesWindow(false);
    close();
}

void CustomGalleriesWindow::ActivatedEvent() {
    const QModelIndex& index = ptwGalleries->selectionModel()->currentIndex();
    const QVariant& data = index.data(Qt::UserRole);
    GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
    if(pData->CustomGallery()) {
        emit OnGallery(pData->CustomGallery());
    }
}

void CustomGalleriesWindow::SelectedEvent() {
    UpdateButtons();
}

void CustomGalleriesWindow::CreateMenuAndActions() {

    pTitle->ToolBar()->setVisible(true);
    pmMenu = new QMenu(this);

    paAdd = new QAction("&Add", this);
    paAdd->setStatusTip("Add new custom gallery");
    paAdd->setIcon(QIcon(":/res/resources/add.png"));
    pTitle->ToolBar()->addAction(paAdd);
    pmMenu->addAction(paAdd);
    connect(paAdd, SIGNAL(triggered()), this, SLOT(AddEvent()));

    paShow = new QAction("&Show", this);
    paShow->setStatusTip("Show active custom gallery");
    paShow->setIcon(QIcon(":/res/resources/view.png"));
    pTitle->ToolBar()->addAction(paShow);
    pmMenu->addAction(paShow);
    connect(paShow, SIGNAL(triggered()), this, SLOT(ActivatedEvent()));

    paEdit = new QAction("&Edit", this);
    paEdit->setStatusTip("Edit actve custom gallery");
    paEdit->setIcon(QIcon(":/res/resources/edit.png"));
    pTitle->ToolBar()->addAction(paEdit);
    pmMenu->addAction(paEdit);
    connect(paEdit, SIGNAL(triggered()), this, SLOT(EditEvent()));

    paDel = new QAction("&Delete", this);
    paDel->setStatusTip("Delete selected custom galleries");
    paDel->setIcon(QIcon(":/res/resources/del.png"));
    pTitle->ToolBar()->addAction(paDel);
    pmMenu->addAction(paDel);
    connect(paDel, SIGNAL(triggered()), this, SLOT(DelEvent()));

    pmMenu->addAction(pTitle->CloseAction());
    connect(pTitle->CloseAction(), SIGNAL(triggered()), SLOT(ExitEvent()));
}

void CustomGalleriesWindow::UpdateButtons() {
    bool isFind = false;
    const QModelIndexList& rows = ptwGalleries->selectionModel()->selectedRows();
    foreach(const QModelIndex& index, rows) {
        const QVariant& data = index.data(Qt::UserRole);
        GalleryTreeItem* pData = qVariantValue<GalleryTreeItem*>(data);
        if(pData->CustomGallery()) {
            isFind = true;
            break;
        }
    }

    if(isFind) {
        paShow->setEnabled(true);
        paEdit->setEnabled(true);
        paDel->setEnabled(true);
    } else {
        paShow->setEnabled(false);
        paEdit->setEnabled(false);
        paDel->setEnabled(false);
    }
}
