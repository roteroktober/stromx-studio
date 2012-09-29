#include "ParameterServer.h"

#include "DataConverter.h"
#include "SetParameterCmd.h"
#include <stromx/core/Operator.h>
#include <stromx/core/Trigger.h>
#include <stromx/core/OperatorException.h>

const unsigned int ParameterServer::TIMEOUT = 100;

ParameterServer::ParameterServer(stromx::core::Operator* op, QUndoStack* undoStack, QObject* parent)
  : QObject(parent),
    m_op(op),
    m_undoStack(undoStack)
{

}

const QVariant ParameterServer::getParameter(unsigned int id, int role)
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(parameterIsReadAccessible(param))
    {
        try
        {
            return DataConverter::toQVariant(m_op->getParameter(id, TIMEOUT), param, role);
        }
        catch(stromx::core::Timeout&)
        {
            emit parameterAccessTimedOut();
            return QVariant();
        }
        catch(stromx::core::Exception&)
        {
            return QVariant();
        }
    }
    else
    {
        return QVariant();
    }
}

bool ParameterServer::setParameter(unsigned int id, const QVariant& value)
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(parameterIsWriteAccessible(param))
    {
        try
        {
            std::auto_ptr<stromx::core::Data> stromxData;
            stromxData = DataConverter::toStromxData(value, param);
            
            if(stromxData.get() == 0)
                return false;
            
            // test if this data is trigger data
            stromx::core::Trigger* trigger =
                stromx::core::data_cast<stromx::core::Trigger*>(stromxData.get());
                
            if(trigger)
            {
                // triggers are set without informing the undo stack (they can not 
                // be undone)
                doSetParameter(id, *stromxData);
            }
            else // any other parameters are set via an undo stack command
            {
                // obtain the current parameter value
                const stromx::core::Data & currentValue = m_op->getParameter(id);
                
                // if the new value is different from the old one
                // construct a set parameter command 
                if(! DataConverter::stromxDataEqualsTarget(*stromxData, currentValue))
                {
                    SetParameterCmd* cmd = new SetParameterCmd(this, id, *stromxData);
                    m_undoStack->push(cmd);
                }
            }
            return true;
        }
        catch(stromx::core::Exception&)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

void ParameterServer::refresh()
{

}

void ParameterServer::doSetParameter(unsigned int paramId, const stromx::core::Data& newValue)
{
    try
    {
        m_op->setParameter(paramId, newValue, TIMEOUT);   
    } 
    catch(stromx::core::Timeout&)
    {
        emit parameterAccessTimedOut();
    }
    catch(stromx::core::ParameterError& e)
    {
        emit parameterErrorOccurred(e);
    }
    catch(stromx::core::Exception&)
    {
    }
    
    emit parameterChanged(paramId);
}

Qt::ItemFlags ParameterServer::parameterFlags(unsigned int id) const
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    if(parameterIsWriteAccessible(param))
        return Qt::ItemIsEditable;
    else
        return Qt::ItemFlags(0);
}

bool ParameterServer::parameterIsReadAccessible(const stromx::core::Parameter& par) const
{
    if(m_op->status() != stromx::core::Operator::NONE)
    {
        if (par.accessMode() == stromx::core::Parameter::NO_ACCESS)
            return false;
        else
            return true;
    }
    else
    {
        if(par.accessMode() == stromx::core::Parameter::NONE_READ || par.accessMode() == stromx::core::Parameter::NONE_WRITE)
            return true;
        else
            return false;
    }
}

bool ParameterServer::parameterIsDisplayed(unsigned int id) const
{
    const stromx::core::Parameter & param = m_op->info().parameter(id);
    
    return parameterIsReadAccessible(param) || param.members().size();
}

bool ParameterServer::parameterIsWriteAccessible(const stromx::core::Parameter& par) const
{
    switch(m_op->status())
    {
        case stromx::core::Operator::NONE:
            if(par.accessMode() == stromx::core::Parameter::NONE_WRITE)
                return true;
            else
                return false;
            
        case stromx::core::Operator::INITIALIZED:
            if(par.accessMode() == stromx::core::Parameter::INITIALIZED_WRITE || par.accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
            
        default:
            if(par.accessMode() == stromx::core::Parameter::ACTIVATED_WRITE)
                return true;
            else
                return false;
    }
}
