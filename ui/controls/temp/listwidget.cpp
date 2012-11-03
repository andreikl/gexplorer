#include <math.h>

#include <QResizeEvent>
#include <QPainter>

#include "listwidget.h"
#include "ui_listwidget.h"

ListDelegate::ListDelegate(ListWidget* pList): QStyledItemDelegate(pList) {
    this->pList = pList;
}

QSize ListDelegate::sizeHint(const QStyleOptionViewItem&, const QModelIndex&) const {
    return QSize(pList->gridSize());
}

void ListDelegate::paint(QPainter* pPainter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    // Default drawing
    if( qVariantCanConvert<ListItem*>(index.data(Qt::UserRole))) {
        ListItem* pItem = qVariantValue<ListItem*>(index.data(Qt::UserRole));

        pPainter->save();

        QRect rect = option.rect;

        if(pItem->Image() == NULL || pItem->Image()->size() != pList->ItemSize()) {
            QImage* pImage = NULL;
            QImage image = QImage(pItem->Path());
            if(!image.isNull()) {
                pImage = new QImage(image.scaled(pList->ItemSize()));
            } else {
                pImage = new QImage(image);
            }
            pItem->Image(pImage);
        }
        pPainter->drawImage(rect, *pItem->Image());
        /*pPainter->setPen( Qt::black );

        // Highlight default store
        if ( pStore == Catalog::Self()->DefaultStore() ) {
            pPainter->fillRect( option.rect, Qt::lightGray );
        }

        QStyledItemDelegate::paint( pPainter, option, index );

        // Get text rect
        QRect textRect = option.rect;
        textRect.setLeft(option.decorationSize.width());

        //textRect.adjust( 5, 5, -5, -5 );
        QStyleOptionViewItemV4 opt = option;
        initStyleOption( &opt, index );
        int textMargin = QApplication::style()->pixelMetric(QStyle::PM_FocusFrameHMargin,
                &opt,
                reinterpret_cast<QWidget*>(parent())) + 1;
        textRect.adjust( textMargin, 0, -textMargin, 0 );
        textRect.setLeft( textRect.left() + 3 );

        QFont defaultFont = QApplication::font();
        QTextOption textOption( Qt::AlignTop | Qt::AlignLeft );

        // Draw distance
        QRect nameRect( textRect );
        if( isDistance ) {
            QString textDistance = QString( "%1 km" ).arg( pStore->Distance(), 0, 'f', 2 );

            QRect itemRect( option.rect );
            itemRect.setRight( itemRect.right() - 3 );

            QTextOption options( Qt::AlignRight | Qt::AlignTop );

            pPainter->setFont(defaultFont);

            pPainter->drawText( itemRect, textDistance, options );

            QFontMetrics metrics( pPainter->font() );
            QRect boundRect = metrics.boundingRect( nameRect, Qt::AlignRight | Qt::AlignTop, textDistance );
            nameRect.setRight( boundRect.left() );
        }

        // Draw Store Name
        QFont font = defaultFont;
        font.setBold( true );
        pPainter->setFont( font );

        QFontMetrics metrics( font );
        QString elidedName = metrics.elidedText( pStore->Name(), Qt::ElideRight, nameRect.width() );

        pPainter->drawText( nameRect, elidedName, textOption );

        // Resize text rect for next drawing
        textRect.setY( textRect.y() + metrics.ascent() + metrics.descent() + 1 );

        // Draw Address
        pPainter->setFont( defaultFont );
        textOption.setWrapMode( QTextOption::WordWrap );
        pPainter->drawText( textRect, pStore->Address(), textOption );

        // Draw feeds badges
        int n = 0;
        foreach( RssItem* pItem, Rss::Self()->Stores() ) {
            if( pItem->Store() == pStore->Code() ) {
                foreach( OfferItem* pOffer, pItem->Offers() ) {
                    if( !pOffer->IsRead() ) {
                        n++;
                    }
                }
                foreach( NewsItem* pNews, pItem->News() ) {
                    if( !pNews->IsRead() ) {
                        n++;
                    }
                }
                break;
            }
        }
        if( n ) {
            pPainter->setBrush( QBrush( Qt::red, Qt::SolidPattern ) );
            QPen pen( Qt::white, Qt::SolidPattern );
            pen.setWidth( 2 );
            pPainter->setPen( pen );

            QFont font = pPainter->font();
#ifdef Q_WS_WINCE
            font.setPointSize( 8 );
#elif defined( Q_OS_SYMBIAN )
            font.setPointSize( 5 );
#endif
            font.setBold( true );
            pPainter->setFont( font );

            QString str = QString( "%1" ).arg( n );
            QFontMetrics metrics(font);

            int h = metrics.height() + 4;
            int w = metrics.width( str ) + h / 2; // +h/2 is radius
            if ( h > w ) w = h;
            int x = option.rect.x() + ( option.decorationSize.width() - w );
            int y = option.rect.y();
            QRect rect( x, y, w, h );
            QTextOption options( Qt::AlignCenter );
            pPainter->drawRoundedRect( rect, h / 2, h / 2);
            pPainter->drawText( rect, str, options );
        }*/

        pPainter->restore();
    } else {
        // Default drawing
        QStyledItemDelegate::paint(pPainter, option, index);
    }
}

ListWidget::ListWidget(QWidget* pParent): QListWidget(pParent), ui(new Ui::ListWidget) {
    itemSize = QSize(200, 200);
    ui->setupUi(this);

    setItemDelegate(new ListDelegate(this));
}

ListWidget::~ListWidget() {

    delete ui;
}

void ListWidget::SetItems(const QList<ListItem*>& items) {

    clear();
    foreach(ListItem* pItem, items) {
        new ListWidgetItem(this, pItem);
    }
}

void ListWidget::resizeEvent(QResizeEvent* pEvent) {

    int count = 1;
    QSize size = itemSize;
    if(pEvent->size().width() > size.width())
    {
        double countF = pEvent->size().width() / (double)size.width();
        count = countF + 0.5;
    }
    size.setWidth(pEvent->size().width() / count - 1);
    setGridSize(size);
    setIconSize(QSize(size.width() - 10, size.height() - 30));

    QListWidget::resizeEvent(pEvent);
}
