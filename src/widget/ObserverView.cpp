#include "widget/ObserverView.h"

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QMenu>
#include "delegate/InputDelegate.h"
#include "model/ObserverModel.h"
#include "model/ObserverTreeModel.h"

ObserverView::ObserverView(ObserverModel* observer, QWidget* parent)
  : QListView(parent),
    m_observer(observer),
    m_removeInputAct(0)
{
    Q_ASSERT(m_observer);
    
    setModel(observer);
    setDropIndicatorShown(true);
    setDragDropMode(QAbstractItemView::DragDrop);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    setItemDelegate(new InputDelegate(this));
    
    m_removeInputAct = createRemoveInputAction(this);
    
    for (int row = 0; row < observer->rowCount(); ++row)
        openPersistentEditor(model()->index(row, 0, QModelIndex()));
        
}

void ObserverView::contextMenuEvent(QContextMenuEvent* event)
{
    QModelIndex index = indexAt(event->pos());
    
    if(index.isValid())
    {
        QMenu menu(this);
        menu.addAction(m_removeInputAct);
        menu.exec(event->globalPos());
    }
}

QAction* ObserverView::createRemoveInputAction(QObject* parent)
{
    QAction* action = new QAction(tr("Remove input"), parent);
    action->setStatusTip(tr("Remove the selected input"));
    connect(action, SIGNAL(triggered()), this, SLOT(removeInput()));
    
    return action;
}

void ObserverView::removeInput()
{
    if(selectionModel())
    {
        QModelIndex index = selectionModel()->currentIndex();
        if(index.isValid())
            model()->parentModel()->removeInput(model(), index.row());
    }
}

void ObserverView::rowsInserted(const QModelIndex& parent, int start, int end)
{
    QListView::rowsInserted(parent, start, end);
    
    for (int row = start; row <= end; ++row)
        openPersistentEditor(model()->index(row, 0, parent));
}


