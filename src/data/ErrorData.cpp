#include "data/ErrorData.h"

#include <QApplication>
#include <stromx/runtime/OperatorException.h>
#include <stromx/runtime/OperatorInfo.h>

ErrorData::ErrorData()
  : m_time(QDateTime::currentDateTime()),
    m_type(UNDEFINED)
{

}

ErrorData::ErrorData(const stromx::runtime::OperatorError& exception, Type type)
  : m_time(QDateTime::currentDateTime()),
    m_type(type)
{
    QString opType = QString("%1::%2").arg(QString::fromStdString(exception.package()))
                                      .arg(QString::fromStdString(exception.type()));
                                     
    QString errorType;
    switch(type)
    {
    case INITIALIZATION:
        errorType = QApplication::tr("Initialization error");
        break;
    case ACTIVATION:
        errorType = QApplication::tr("Activation error");
        break;
    case EXECUTION:
        errorType = QApplication::tr("Execution error");
        break;
    case DEACTIVATION:
        errorType = QApplication::tr("Deactivation error");
        break;
    case DEINITIALIZATION:
        errorType = QApplication::tr("Deinitialization error");
        break;
    case PARAMETER_ACCESS:
        errorType = QApplication::tr("Parameter access error");
        break;
    default:
        errorType = QApplication::tr("Unknown error");
        break;
    }
    
    QString opName = exception.name().empty() ? QString("") :
                     QApplication::tr("\"%1\" ").arg(QString::fromStdString(exception.name()));
    
    m_title = QApplication::tr("%1 in operator %2 of type %3").arg(errorType).arg(opName).arg(opType);
    
    m_description = QString::fromStdString(exception.message());
}
