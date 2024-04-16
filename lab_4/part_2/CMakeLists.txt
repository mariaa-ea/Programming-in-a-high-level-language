cmake_minimum_required (VERSION 3.13) 

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
set (PROJECT lab_4_part_2)
project (${PROJECT})

# установка путей
set(FFTW_PATH "C:/Program Files")

#-------------------------------------------------------------------------------
# исходники 
file (GLOB SOURCES  main.cpp
                                  qcustomplot.cpp 
                                  calc_psf.cpp
                                  sample_complex.cpp
                                  dqt_psf.cpp)
								
file (GLOB HEADERS  qcustomplot.h 
                                 sample.h 
                                 calc_psf.h
                                 sample_complex.h
 								 dqt_psf.h)												

set (RESOURCES       dqt_psf.ui)

#-------------------------------------------------------------------------------
# стандарт C++ для Qt6
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt
find_package(Qt6Widgets REQUIRED)
find_package(Qt6PrintSupport REQUIRED)
include_directories(${Qt6Widgets_INCLUDES})
include_directories(${Qt6PrintSupport_INCLUDES})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# подключение библиотеки fftw
include_directories(${FFTW_PATH})
set(FFTW_LIB "${FFTW_PATH}/libfftw3-3.lib")
#-------------------------------------------------------------------------------
# добавить FFTW_LIB в проект
# устанавливаем все библиотеки
add_executable (${PROJECT} WIN32 ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT} Qt6::Widgets Qt6::PrintSupport ${QT_QTMAIN_LIBRARY} ${FFTW_LIB})	
