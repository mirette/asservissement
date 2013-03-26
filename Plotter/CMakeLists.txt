PROJECT(PlotterProject)
#===================================================================================================
# For Qt4 usage 
FIND_PACKAGE(Qt4 REQUIRED)
INCLUDE(${QT_USE_FILE})
INCLUDE_DIRECTORIES(${QT_INCLUDE_DIR})
LINK_DIRECTORIES (${QT_LIBRARIES})
ADD_DEFINITIONS(${QT_DEFINITIONS})
#C++ Project Include
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/Plotter/include) 
#=================================================================================================== on peut enlevé, c'est pour win32
# console suppression for windows based applications
IF(WIN32)
LINK_LIBRARIES(${QT_QTMAIN_LIBRARY})
#ADD_DEFINITIONS (-DQT_DLL)
ENDIF (WIN32)
#===================================================================================================
#Project libraries
LINK_DIRECTORIES (${CMAKE_SOURCE_DIR}/Plotter/lib)
#===================================================================================================
SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/Plotter/bin)
#===================================================================================================
SET(PROJECT_SOURCES 
#project source
${CMAKE_SOURCE_DIR}/Plotter/src/main.cpp
${CMAKE_SOURCE_DIR}/Plotter/src/Plotter.cpp
${CMAKE_SOURCE_DIR}/Plotter/src/FindDialog.cpp
${CMAKE_SOURCE_DIR}/Plotter/src/MainFram.cpp

#project headers
${CMAKE_SOURCE_DIR}/Plotter/include/Plotter.h
${CMAKE_SOURCE_DIR}/Plotter/include/FindDialog.hpp
${CMAKE_SOURCE_DIR}/Plotter/include/MainFram.hpp
) 
# MOCing process mettre tous les class QT
SET(PROJECT_MOC_CLASSES 
${CMAKE_SOURCE_DIR}/Plotter/include/Plotter.h
${CMAKE_SOURCE_DIR}/Plotter/include/FindDialog.hpp
${CMAKE_SOURCE_DIR}/Plotter/include/MainFram.hpp
)
QT4_WRAP_CPP(PROJECT_MOC_SOURCES  ${PROJECT_MOC_CLASSES})
SOURCE_GROUP(moc FILES ${PROJECT_MOC_SOURCES}) 
#===================================================================================================
IF(WIN32)
# Compilation and installation
ADD_EXECUTABLE(PlotterProject WIN32  ${PROJECT_SOURCES} ${PROJECT_MOC_SOURCES})
TARGET_LINK_LIBRARIES(PlotterProject 
${QT_LIBRARIES} odbc32 odbccp32
)
ENDIF(WIN32)

IF(UNIX)
ADD_EXECUTABLE(PlotterProject  ${PROJECT_SOURCES} ${PROJECT_MOC_SOURCES})
TARGET_LINK_LIBRARIES(PlotterProject 
${QT_LIBRARIES}
)
ENDIF(UNIX)
#===================================================================================================
SET_TARGET_PROPERTIES(PlotterProject PROPERTIES DEBUG_POSTFIX "_d" )
INSTALL(TARGETS PlotterProject RUNTIME DESTINATION ${CMAKE_SOURCE_DIR}/Plotter/bin )