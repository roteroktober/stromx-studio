#include "SetInputVisualizationCmd.h"

SetInputVisualizationCmd::SetInputVisualizationCmd(InputModel* model, const InputModel::Visualization & newVisualization, QUndoCommand* parent)
  : QUndoCommand(QObject::tr("set input visualization"), parent),
    m_model(model),
    m_oldVisualization(model->visualization()),
    m_newVisualization(newVisualization)
{
}

void SetInputVisualizationCmd::redo()
{
    m_model->doSetVisualization(m_newVisualization);
}

void SetInputVisualizationCmd::undo()
{
    m_model->doSetVisualization(m_oldVisualization);
}
