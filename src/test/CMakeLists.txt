add_test(NAME stromx_studio_test COMMAND stromx_studio_test)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/camera.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/extra_parameter.stromx DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/connector.xml DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/lenna_bw.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

set(stromxstudiotest_HEADERS
    DataConverterTest.h
    ImageTest.h
    ObserverSchedulerTest.h
    OperatorLibraryModelTest.h
    ParameterServerTest.h
    StreamModelTest.h
    ../ParameterServer.h
    ../data/InputData.h
    ../data/OperatorData.h
    ../model/ConnectionModel.h
    ../model/InputModel.h
    ../model/ObserverModel.h
    ../model/ObserverTreeModel.h
    ../model/OperatorLibraryModel.h
    ../model/OperatorModel.h
    ../model/PropertyModel.h
    ../model/ThreadListModel.h
    ../model/ThreadModel.h
    ../model/StreamModel.h
    ../task/GetParameterTask.h
    ../task/SetParameterTask.h
    ../task/Task.h
)

set(stromxstudiotest_SOURCES
    main.cpp
    DataConverterTest.cpp
    ImageTest.cpp
    ObserverSchedulerTest.cpp
    OperatorLibraryModelTest.cpp
    ParameterServerTest.cpp
    StreamModelTest.cpp
    ../cmd/AddConnectionCmd.cpp
    ../cmd/AddOperatorCmd.cpp
    ../cmd/AddThreadCmd.cpp
    ../cmd/InitializeOperatorCmd.cpp
    ../cmd/InsertInputCmd.cpp
    ../cmd/InsertObserverCmd.cpp
    ../cmd/MoveInputCmd.cpp
    ../cmd/MoveOperatorCmd.cpp
    ../cmd/DeinitializeOperatorCmd.cpp
    ../cmd/RemoveInputCmd.cpp
    ../cmd/RemoveConnectionCmd.cpp
    ../cmd/RemoveOperatorCmd.cpp
    ../cmd/RemoveObserverCmd.cpp
    ../cmd/RemoveThreadCmd.cpp
    ../cmd/RenameObserverCmd.cpp
    ../cmd/RenameOperatorCmd.cpp
    ../cmd/RenameThreadCmd.cpp
    ../cmd/SetParameterCmd.cpp
    ../cmd/SetStreamSettings.cpp
    ../cmd/SetThreadCmd.cpp
    ../cmd/SetThreadColorCmd.cpp
    ../cmd/SetVisualizationStateCmd.cpp
    ../data/ErrorData.cpp
    ../data/InputData.cpp
    ../data/OperatorData.cpp
    ../event/ConnectorDataEvent.cpp
    ../event/ConnectorOccupyEvent.cpp
    ../model/ConnectionModel.cpp
    ../model/InputModel.cpp
    ../model/OperatorLibraryModel.cpp
    ../model/ObserverModel.cpp
    ../model/ObserverTreeModel.cpp
    ../model/OperatorModel.cpp
    ../model/PropertyModel.cpp
    ../model/StreamModel.cpp
    ../model/ThreadListModel.cpp
    ../model/ThreadModel.cpp
    ../task/GetParameterTask.cpp
    ../task/SetParameterTask.cpp
    ../task/Task.cpp
    ../visualization/VisualizationState.cpp
    ../Common.cpp
    ../ConnectorObserver.cpp
    ../DataConverter.cpp
    ../ExceptionObserver.cpp
    ../Image.cpp
    ../Matrix.cpp
    ../ObserverScheduler.cpp
    ../ParameterServer.cpp
)

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_BINARY_DIR}/src
)

if(USE_QT5)
    add_executable(stromx_studio_test
        ${stromxstudiotest_SOURCES} 
    )

    target_link_libraries(stromx_studio_test
        ${stromx_RUNTIME_LIB}
        ${stromx_CVCORE_LIB}
        ${stromx_CVHIGHGUI_LIB}
        ${stromx_CVIMGPROC_LIB}
        ${stromx_CVSUPPORT_LIB}
        ${stromx_TEST_LIB}
    )

    qt5_use_modules(stromx_studio_test Widgets Concurrent Test)
else()
    set(QT_USE_QTTEST TRUE) 
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})
    qt4_wrap_cpp(stromxstudiotest_HEADERS_MOC ${stromxstudiotest_HEADERS})

    add_executable(stromx_studio_test
        ${stromxstudiotest_SOURCES} 
        ${stromxstudiotest_HEADERS_MOC} 
    )

    target_link_libraries(stromx_studio_test
        ${QT_LIBRARIES}
        ${stromx_RUNTIME_LIB}
        ${stromx_CVCORE_LIB}
        ${stromx_CVHIGHGUI_LIB}
        ${stromx_CVIMGPROC_LIB}
        ${stromx_CVSUPPORT_LIB}
        ${stromx_TEST_LIB}
    )
endif()

