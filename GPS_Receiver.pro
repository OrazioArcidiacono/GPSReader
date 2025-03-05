QT       += core gui serialport sql widgets concurrent network

CONFIG   += c++20

TARGET = GPSReceiver
TEMPLATE = app

SOURCES += main.cpp \
           GPSLogTableWidget.cpp \
           GPSReceiverConfig.cpp \
           GPSReader.cpp \
           FIFODataSource.cpp \
           FIFOWriter.cpp \
           RawDataDialog.cpp \
           StandaloneGPSHandler.cpp \
           PluginGPSHandler.cpp \
           GPSHandlerFactory.cpp \
           SocketChannel.cpp

HEADERS += GPSReceiverConfig.h \
           GPSLogTableWidget.h \
           GPSReader.h \
           FIFODataSource.h \
           FIFOWriter.h \
           RawDataDialog.h \
           IGPSHandler.h \
           StandaloneGPSHandler.h \
           PluginGPSHandler.h \
           GPSHandlerFactory.h \
           SocketChannel.h \
           CommunicationChannel.h
