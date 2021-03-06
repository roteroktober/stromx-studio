#include "item/ConnectorItem.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QToolTip>
#include <stromx/runtime/Operator.h>
#include "Common.h"
#include "StreamEditorScene.h"
#include "item/ArrowItem.h"
#include "item/ConnectionItem.h"
#include "model/OperatorModel.h"
#include "model/StreamModel.h"

const qreal ConnectorItem::SIZE = 8;
const qreal ConnectorItem::FLOAT_Z_VALUE = 1;

ConnectorItem::ConnectorItem(OperatorModel* op, unsigned int id, ConnectorType type, QGraphicsItem* parent)
  : QGraphicsRectItem(parent),
    m_op(op),
    m_id(id),
    m_connectorType(type),
    m_currentArrow(0)
{
    setRect(-SIZE/2, -SIZE/2, SIZE, SIZE);
    setAcceptHoverEvents(true);
    setBrush(Qt::black);
    setPen(Qt::NoPen);
}

void ConnectorItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if(! canConnect())
        return;
    
    m_currentArrow = new ArrowItem(this);
    if(m_connectorType == INPUT)
    {
        m_currentArrow->setStart(mapFromScene(event->scenePos()));
        m_currentArrow->setEnd(QPointF(0, 0));
    }
    else
    {
        m_currentArrow->setStart(QPointF(0, 0));
        m_currentArrow->setEnd(mapFromScene(event->scenePos()));
    }
}

void ConnectorItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentArrow)
    {
        if(m_connectorType == INPUT)
        {
            QPointF start(mapFromScene(event->scenePos()));
            m_currentArrow->setStart(start);
        }
        else
        {
            QPointF end(mapFromScene(event->scenePos()));
            m_currentArrow->setEnd(end);
        }
        
        ConnectorItem* connectorItem = connectorItemAt(event->scenePos());
            
        if(connectorItem)
        {
            m_currentArrow->setActive(true);
            QToolTip::showText(event->screenPos(), connectorToolTip(), event->widget());
        }
        else
        {
            m_currentArrow->setActive(false);
            QToolTip::hideText();
        }
    }
    
    QGraphicsRectItem::mouseMoveEvent(event);
}

ConnectorItem* ConnectorItem::connectorItemAt(const QPointF& pos) const
{
    QList<QGraphicsItem*> items = scene()->items(pos);
    ConnectorItem* connectorItem = 0;
    
    foreach(QGraphicsItem* item, items)
    {
        if((connectorItem = qgraphicsitem_cast<ConnectorItem*>(item)))
        {
            if(connectorType() != connectorItem->connectorType())
            {
                if(! connectorItem->canConnect())
                    return 0;
                
                return connectorItem;
            }
        }
    }
    
    return 0;
}

int ConnectorItem::numConnections() const
{
    return m_connections.count();
}

bool ConnectorItem::canConnect() const
{
    // can not draw any connections when the stream is active
    if(m_op->isActive())
        return false;
    
    // inputs can connect to at most one output
    if(connectorType() == INPUT && ! m_connections.empty())
        return false;
    
    return true;
}

void ConnectorItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if(m_currentArrow)
    {
        delete m_currentArrow;
        m_currentArrow = 0;
    }
    
    ConnectorItem* connectorItem = connectorItemAt(event->scenePos());
        
    if(connectorItem)
    {
        StreamEditorScene* streamEditorScene = qobject_cast<StreamEditorScene*>(scene());
        
        if(streamEditorScene)
        {
            StreamModel* model = streamEditorScene->model();
            if(connectorType() == INPUT)
                model->addConnection(connectorItem->op(), connectorItem->id(), op(), id());
            else
                model->addConnection(op(), id(), connectorItem->op(), connectorItem->id());
        }
    }
}

void ConnectorItem::addConnection(ConnectionItem* connection)
{
    m_connections.insert(connection);
    updateConnectionPosition(connection);
}

void ConnectorItem::removeConnection(ConnectionItem* connection)
{
    m_connections.remove(connection);
}

void ConnectorItem::updateConnectionPositions() const
{
    foreach(ConnectionItem* connection, m_connections)
        updateConnectionPosition(connection);
}

void ConnectorItem::updateConnectionPosition(ConnectionItem* connection) const
{
    if(connectorType() == INPUT)
        connection->setEnd(mapToScene(QPointF(0, 0)));
    else
        connection->setStart(mapToScene(QPointF(0, 0)));
}

QString ConnectorItem::connectorToolTip() const
{
    const stromx::runtime::OperatorInfo & info = m_op->op()->info();
    
    QString connectorTitle;
    QString variantTitle;
    if(connectorType() == INPUT)
    {
        connectorTitle = fromStromxTitle(info.input(m_id).title());
        variantTitle = fromStromxTitle(info.input(m_id).variant().title());
    }
    else
    {
        connectorTitle = fromStromxTitle(info.output(m_id).title());
        variantTitle = fromStromxTitle(info.output(m_id).variant().title());
    }
    
    return QString("%1 (%2)").arg(connectorTitle).arg(variantTitle);
}

void ConnectorItem::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    QToolTip::showText(event->screenPos(), connectorToolTip(), event->widget());
    
    QGraphicsItem::hoverEnterEvent(event);
}

void ConnectorItem::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    QToolTip::hideText();
    
    QGraphicsItem::hoverLeaveEvent(event);
}

void ConnectorItem::setOccupied(bool occupied)
{
    if(m_connectorType == INPUT)
    {
        foreach(ConnectionItem* connection, m_connections)
            connection->setInputOccupied(occupied);
    }
    
    if(m_connectorType == OUTPUT)
    {
        foreach(ConnectionItem* connection, m_connections)
            connection->setOutputOccupied(occupied);
    }
}



