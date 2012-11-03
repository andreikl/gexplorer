#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QStyledItemDelegate>
#include <QListWidget>

namespace Ui {
    class ListWidget;
}

class ListItem {

public:
    ListItem(const QString& text, const QString& path) {
        this->text = text;
        this->path = path;

        pImage = NULL;
    }

    virtual ~ListItem() {
        Image(NULL);
    }

public:
    const QString& Text() const {
        return text;
    }

    const QString& Path() const {
        return path;
    }

public:
    QImage* Image() {
        return pImage;
    }

    void Image(QImage* pImage) {
        if(this->pImage != NULL) {
            delete this->pImage;
        }
        this->pImage = pImage;
    }

private:
    QString text;
    QString path;

private:
    QImage* pImage;
};

// forvard declaration;
class ListWidget;

class ListDelegate: public QStyledItemDelegate {
    Q_OBJECT

public:
    ListDelegate(ListWidget* pList);

// QStyledItemDelegate
public:
    QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const;
    void paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const;

private:
    ListWidget* pList;

};

class ListWidgetItem: public QListWidgetItem {

public:
    ListWidgetItem(QListWidget* pList, ListItem* pItem) : QListWidgetItem(pList) {
        setData(Qt::UserRole, qVariantFromValue(pItem));
    }
};

class ListWidget : public QListWidget {
    Q_OBJECT

public:
    explicit ListWidget(QWidget* pParent = NULL);
    ~ListWidget();

public:
    void SetItems(const QList<ListItem*>& items);

public:
    const QSize& ItemSize() const {
        return itemSize;
    }

    void ItemSize(QSize size) {
        itemSize = size;
    }

protected:
    void resizeEvent(QResizeEvent* pEvent);

private:
    QSize itemSize;

private:
    Ui::ListWidget *ui;
};

Q_DECLARE_METATYPE(ListItem*)

#endif // LISTWIDGET_H
