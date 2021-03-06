#include "test/StreamModelTest.h"

#include <QtTest/QtTest>
#include <QUndoStack>
#include <stromx/cvsupport/Cvsupport.h>
#include <stromx/runtime/DirectoryFileInput.h>
#include <stromx/runtime/Factory.h>
#include <stromx/runtime/ZipFileInput.h>

#include "Exception.h"
#include "model/OperatorLibraryModel.h"
#include "model/StreamModel.h"

StreamModelTest::StreamModelTest()
  : m_undoStack(new QUndoStack(this)),
    m_operatorLibraryModel(new OperatorLibraryModel(this))
{
    try
    {
#ifdef UNIX
    m_operatorLibraryModel->loadPackage("libstromx_test");
#endif // UNIX
    
#ifdef WIN32
    m_operatorLibraryModel->loadPackage("stromx_test");
#endif // WIN32
    }
    catch(LoadPackageFailed&)
    {
    }
    
    stromxRegisterCvsupport(m_operatorLibraryModel->factory());
}

void StreamModelTest::testDefaultConstructor()
{
    new StreamModel(m_undoStack, m_operatorLibraryModel, this);
}

void StreamModelTest::testFileConstructorCamera()
{
    stromx::runtime::ZipFileInput input("camera.stromx");
    StreamModel* model = new StreamModel(input, "stream", m_undoStack, m_operatorLibraryModel, this);
    model->readObserverData(input);
}

void StreamModelTest::testFileConstructorConnector()
{
    stromx::runtime::DirectoryFileInput input(".");
    new StreamModel(input, "connector", m_undoStack, m_operatorLibraryModel, this);
}

void StreamModelTest::testFileConstructorExtraParameter()
{
    stromx::runtime::ZipFileInput input("extra_parameter.stromx");
    try
    {
        new StreamModel(input, "stream", m_undoStack, m_operatorLibraryModel, this);
        QFAIL("ReadStreamFailed not thrown");
    }
    catch(ReadStreamFailed &)
    {
    }
}




