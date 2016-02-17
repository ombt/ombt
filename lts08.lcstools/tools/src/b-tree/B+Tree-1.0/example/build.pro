win32:TEMPLATE	= vcapp
unix:TEMPLATE = app
macx:TEMPLATE = app
LANGUAGE	= C++

CONFIG	+= warn_on 

win32:CONFIG += console

ROOT_DIR = $$PWD
CONFIG += debug

QT -= gui
QT += xml

win32{
DEFINES += WIN32
}

include(../jstreams/__build__.pri)
include(../btree/__build__.pri)

TARGET = TestProject

INCLUDEPATH     += $$ROOT_DIR/Common

DESTDIR = $$ROOT_DIR/dist_release/
OBJECTS_DIR = $$ROOT_DIR/obj/release/$$TARGET

CONFIG(debug, debug|release) {
 DESTDIR = $$ROOT_DIR/dist_debug/
 OBJECTS_DIR = $$ROOT_DIR/obj/debug/$$TARGET

}
CONFIG(release) {
 DEFINES += QT_NO_DEBUG_OUTPUT
}

SOURCES += $$PWD/main.cpp


