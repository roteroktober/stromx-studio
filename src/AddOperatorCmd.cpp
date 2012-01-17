#include "AddOperatorCmd.h"

#include "StreamModel.h"

AddOperatorCmd::AddOperatorCmd(StreamModel* stream, OperatorModel* model, QUndoCommand* parent)
  : QUndoCommand(parent),
    m_stream(stream),
    m_model(model)
{
}

void AddOperatorCmd::undo()
{
    m_stream->doRemoveOperator(m_model);
}

void AddOperatorCmd::redo()
{
    m_stream->doAddOperator(m_model);
}