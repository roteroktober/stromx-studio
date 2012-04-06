#include "ThreadModel.h"

#include <stromx/core/Thread.h>
#include "RenameThreadCmd.h"
#include "SetThreadColorCmd.h"
#include "StreamModel.h"

ThreadModel::ThreadModel(stromx::core::Thread* thread, StreamModel* stream)
  : QObject(stream),
    m_thread(thread),
    m_stream(stream),
    m_color(Qt::black)
{
    if(m_thread)
        m_name = QString::fromStdString(m_thread->name());
}

void ThreadModel::setThread(stromx::core::Thread* thread)
{
    m_thread = thread;
    if(m_thread)
        m_name = QString::fromStdString(m_thread->name());
    else
        m_name = "";
}

void ThreadModel::setName(const QString& name)
{
    if(name != m_name)
    {
        QUndoCommand* cmd = new RenameThreadCmd(this, name);
        m_stream->undoStack()->push(cmd);
    }
}

void ThreadModel::doSetName(const QString& name)
{
    if(m_thread)
    {
        m_thread->setName(name.toStdString());
        m_name = name;
        emit nameChanged(m_name);
        emit changed(this);
    }
}

void ThreadModel::setColor(const QColor& color)
{
    if(color != m_color)
    {
        QUndoCommand* cmd = new SetThreadColorCmd(this, color);
        m_stream->undoStack()->push(cmd);
    }
}

void ThreadModel::doSetColor(const QColor& color)
{
    m_color = color;
    emit colorChanged(m_color);
    emit changed(this);
}

QDataStream& operator<<(QDataStream& stream, const ThreadModel* thread)
{
    return stream << thread->m_color;
}

QDataStream& operator>>(QDataStream& stream, ThreadModel* thread)
{
    return stream >> thread->m_color;
}
