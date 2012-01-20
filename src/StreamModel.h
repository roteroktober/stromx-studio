/* 
*  Copyright 2012 Matthias Fuchs
*
*  This file is part of stromx-studio.
*
*  Stromx-studio is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  Stromx-studio is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with stromx-studio.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef STREAMMODEL_H
#define STREAMMODEL_H

#include <QObject>
#include <QPointF>
#include <QSet>
#include "ThreadListModel.h"

namespace stromx
{
    namespace core
    { 
        class Operator;
        class Stream;
        class Thread;
    }
}

class QAbstractTableModel;
class QUndoStack;
class ConnectionModel;
class OperatorModel;
class ThreadListModel;
class ThreadModel;

class StreamModel : public QObject
{
    Q_OBJECT
    
    friend class AddOperatorCmd;
    friend class RemoveOperatorCmd;
    friend class InitializeOperatorCmd;
    friend class DeinitializeOperatorCmd;
    friend class AddConnectionCmd;
    friend class RemoveConnectionCmd;
    friend class AddThreadCmd;
    friend class RemoveThreadCmd;
    
public:
    explicit StreamModel(QUndoStack* undoStack, QObject *parent = 0);
    virtual ~StreamModel();
    
    const QList<OperatorModel*> operators() const { return m_operators; }
    void addOperator(stromx::core::Operator* const op, const QPointF & pos);
    void removeOperator(OperatorModel* op);
    
    const QList<ConnectionModel*> connections() const { return m_connections; }
    void addConnection(OperatorModel* sourceOp, unsigned int outputId,
                       OperatorModel* targetOp, unsigned int inputId);
    void removeConnection(ConnectionModel* connection);
    
    const QList<ThreadModel*> threads() const { return m_threadListModel->threads(); }
    void addThread();
    void removeThread(ThreadModel* thread);
    
    void initializeOperator(OperatorModel* op);
    void deinitializeOperator(OperatorModel* op);
    
    ThreadListModel* threadListModel() const { return m_threadListModel; }
    
    QUndoStack* undoStack() const { return m_undoStack; }
    
signals:
    void operatorAdded(OperatorModel* op);
    void operatorRemoved(OperatorModel* op);
    void connectionAdded(ConnectionModel* connection);
    void connectionRemoved(ConnectionModel* connection);
    void threadAdded(ThreadModel* thread);
    void threadRemoved(ThreadModel* thread);
    
private:
    void doAddOperator(OperatorModel* op);
    void doRemoveOperator(OperatorModel* op);
    void doInitializeOperator(OperatorModel* op);
    void doDeinitializeOperator(OperatorModel* op);
    void doAddConnection(ConnectionModel* connection);
    void doRemoveConnection(ConnectionModel* connection);
    void doAddThread(ThreadModel* threadModel);
    void doRemoveThread(ThreadModel* threadModel);
    
    stromx::core::Stream* m_stream;
    ThreadListModel* m_threadListModel;
    QUndoStack* m_undoStack;
    QSet<stromx::core::Operator*> m_offlineOperators;
    QSet<stromx::core::Thread*> m_offlineThreads;
    QList<ConnectionModel*> m_connections;
    QList<OperatorModel*> m_operators;
};

#endif // STREAMMODEL_H
