#### Laser Scanner Program #####
# Copyright Paul Tsouchlos
# Author: Paul Tsouchlos
# Last Update: November 2, 2015

cmake_minimum_required(VERSION 3.3)

# Set the version number
set (LASER_SCANNER_VERSION_MAJOR "0")
set (LASER_SCANNER_VERSION_MINOR "1")
set (LASER_SCANNER_VERSION_PATCH "0")
set (ENG_UTILS_VERSION "${LASER_SCANNER_VERSION_MAJOR}.${LASER_SCANNER_VERSION_MINOR}.${LASER_SCANNER_VERSION_PATCH}")

#set project name. 
project(LaserScanner)

set(CMAKE_PREFIX_PATH "C:/Qt/5.5/msvc2013_64/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)

#set all the project source files. 
set(Project_Sources 
	main.cpp 
	mainwindow.cpp
	ImageUtils.cpp)

#set the project headers. 
set(Project_Headers 
	mainwindow.h
	ImageUtils.h)

#set project forms
set(Project_Forms mainwindow.ui)

find_package(OpenCV REQUIRED )

#find needed project dependencies. 
find_package(Qt5Core)
find_package(Qt5Gui)
find_package(Qt5Widgets)
find_package(Qt5SerialPort)

#wrap the sources and the ui. 
QT5_WRAP_CPP(Project_HEADERS_MOC ${Project_Headers})
QT5_WRAP_UI(Project_FORMS_HEADERS ${Project_Forms})

#add second project to wrap the gui. Updates to the ui files will show up 
#only after you rebuild. 
add_library(GUI ${Project_FORMS_HEADERS} ${Project_FORMS_HEADERS})
target_link_libraries(GUI Qt5::Widgets Qt5::Core Qt5::Gui)

include_directories(${OpenCV_INCLUDE_DIRS})
#add other sources, libs and headers as executables. 
add_executable(LaserScanner WIN32 ${Project_Sources} ${GUI} ${Project_Headers})
target_link_libraries(LaserScanner GUI ${OpenCV_LIBS} Qt5::SerialPort)

#definitions for debug/release builds. 
option(ENG_DEBUG "Define if the mirror assembly is motorized." OFF)
if(ENG_DEBUG)
	add_definitions(-DENG_DEBUG)
else() 
	add_definitions(-DENG_RELEASE)
endif(ENG_DEBUG)

set(QT_VERSION_MAJOR} "5")
include(CPACK)

#set properties. 
if(WIN32)
	set(QT_USE_QTMAIN true)
	
	
	if(MSVC)
		set_target_properties(LaserScanner PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS")
		
		#installer rules. 
		install(TARGETS LaserScanner
			DESTINATION bin COMPONENT binary_laser_scanner)
			
		 install(FILES
        ${Qt5Core_DIR}/../../../bin/libEGL.dll
        ${Qt5Core_DIR}/../../../bin/libGLESv2.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Core.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Declarative.dll
        ${Qt5Core_DIR}/../../../bin/Qt5Network.dll
        ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll
		${Qt5Core_DIR}/../../../bin/Qt5Script.dll
		${Qt5Core_DIR}/../../../bin/Qt5Sql.dll
		${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll
		${Qt5Core_DIR}/../../../bin/Qt5Xml.dll
		${Qt5Core_DIR}/../../../bin/Qt5XmlPatterns.dll
		${Qt5Core_DIR}/../../../bin/Qt5SerialPort.dll
		${OpenCV_DIR}/bin/Release/opencv_calib3d300.dll
		${OpenCV_DIR}/bin/Release/opencv_core300.dll
		${OpenCV_DIR}/bin/Release/opencv_features2d300.dll
		${OpenCV_DIR}/bin/Release/opencv_ffmpeg300_64.dll
		${OpenCV_DIR}/bin/Release/opencv_flann300.dll
		${OpenCV_DIR}/bin/Release/opencv_highgui300.dll
		${OpenCV_DIR}/bin/Release/opencv_imgcodecs300.dll
		${OpenCV_DIR}/bin/Release/opencv_imgproc300.dll
		${OpenCV_DIR}/bin/Release/opencv_ml300.dll
		${OpenCV_DIR}/bin/Release/opencv_objdetect300.dll
		${OpenCV_DIR}/bin/Release/opencv_photo300.dll
		${OpenCV_DIR}/bin/Release/opencv_shape300.dll
		${OpenCV_DIR}/bin/Release/opencv_stitching300.dll
		${OpenCV_DIR}/bin/Release/opencv_superres300.dll
		${OpenCV_DIR}/bin/Release/opencv_video300.dll
		${OpenCV_DIR}/bin/Release/opencv_videoio300.dll
		${OpenCV_DIR}/bin/Release/opencv_videostab300.dll
        DESTINATION bin COMPONENT binary_laser_scanner
    )
	else()
		message(SEND_ERROR "Unsupported Compiler")
	endif()	

	#installer stuff
	SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "LaserScanner")
	SET(CPACK_PACKAGE_VENDOR "Paul Tsouchlos")
	SET(CPACK_PACKAGE_VERSION_MAJOR "1")
	SET(CPACK_PACKAGE_VERSION_MINOR "3")
	SET(CPACK_PACKAGE_VERSION_PATCH "2")
	SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${LASER_SCANNER_VERSION_MAJOR}.${LASER_SCANNER_VERSION_MINOR}")
	SET(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\LaserScanner.exe")
	SET(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} LaserScanner")
	SET(CPACK_NSIS_MODIFY_PATH ON)
	SET(CPACK_COMPONENTS_ALL binary_laser_scanner)
	if (CMAKE_CL_64)
		set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES64")
	else (CMAKE_CL_64)
		set(CPACK_NSIS_INSTALL_ROOT "$PROGRAMFILES")
	endif (CMAKE_CL_64)
elseif(UNIX)
	SET(CPACK_STRIP_FILES "bin/LaserScanner")
	SET(CPACK_SOURCE_STRIP_FILES "")
else()
	message(SEND_ERROR "Unsupported platform")
endif()

#custom commands for copying dll to build directory so .exe works. 
add_custom_command(TARGET LaserScanner POST_BUILD 
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGLd.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2d.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Cored.dll         ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Guid.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declaratived.dll  ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Networkd.dll      ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGLd.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Scriptd.dll       ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sqld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgetsd.dll      ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xmld.dll          ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatternsd.dll  ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPortd.dll	${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_calib3d300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_core300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_features2d300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_ffmpeg300_64.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_flann300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_highgui300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_imgcodecs300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_imgproc300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_ml300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_objdetect300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_photo300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_shape300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_stitching300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_superres300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_video300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_videoio300d.dll   ${CMAKE_BINARY_DIR}/Debug
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Debug/opencv_videostab300d.dll   ${CMAKE_BINARY_DIR}/Debug
  
  # release version
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libEGL.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/libGLESv2.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Core.dll          ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Gui.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Declarative.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Network.dll       ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5OpenGL.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Script.dll        ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Sql.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Widgets.dll       ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5Xml.dll           ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5XmlPatterns.dll   ${CMAKE_BINARY_DIR}/Release
   COMMAND ${CMAKE_COMMAND} -E copy ${Qt5Core_DIR}/../../../bin/Qt5SerialPort.dll	${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_calib3d300.dll    ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_core300.dll   	 ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_features2d300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_ffmpeg300_64.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_flann300.dll    ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_highgui300.dll  ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_imgcodecs300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_imgproc300.dll  ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_ml300.dll   	 ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_objdetect300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_photo300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_shape300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_stitching300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_superres300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_video300.dll   ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_videoio300.dll ${CMAKE_BINARY_DIR}/Release
  COMMAND ${CMAKE_COMMAND} -E copy ${OpenCV_DIR}/bin/Release/opencv_videostab300.dll ${CMAKE_BINARY_DIR}/Release
  
  # Output Message
  COMMENT "Copying Qt binaries from '${Qt5Core_DIR}/../../bin/' to '${CMAKE_BINARY_DIR}'" VERBATIM
)

#Add target to build documents from visual studio.
set(DOXYGEN_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
set(DOXYGEN_OUTPUT_DIR CACHE PATH "Path for document generation output")
find_package(Doxygen)

if(DOXYGEN_FOUND)

ADD_CUSTOM_TARGET(DOCUMENTATION
  COMMAND ${CMAKE_COMMAND} -E echo_append "Building API Documentation..."
  COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_INPUT}
  COMMAND ${CMAKE_COMMAND} -E echo "Done."
  WORKING_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

endif(DOXYGEN_FOUND)
