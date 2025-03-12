QT       += core gui serialport sql widgets concurrent network

CONFIG   += c++20

TARGET = GPSReceiver
TEMPLATE = app

SOURCES += main.cpp \
           DummyFifoReader.cpp \
           GPGGAParser.cpp \
           GPGLLParser.cpp \
           GPGSAParser.cpp \
           GPGSVParser.cpp \
           GPRMCParser.cpp \
           GPSLogTableWidget.cpp \
           GPSReceiverConfig.cpp \
           GPSReader.cpp \
           FIFODataSource.cpp \
           FIFOWriter.cpp \
           GPTXTParser.cpp \
           GPVTGParser.cpp \
           GPZDAParser.cpp \
           GenericFallbackParser.cpp \
           HDGParser.cpp \
           HDMParser.cpp \
           HDTParser.cpp \
           NMEAParser.cpp \
           ParserWorker.cpp \
           RawDataDialog.cpp \
           StandaloneGPSHandler.cpp \
           PluginGPSHandler.cpp \
           GPSHandlerFactory.cpp \
           SocketChannel.cpp

HEADERS += GPSReceiverConfig.h \
           DummyFifoReader.h \
           DummyFifoReaderThread.h \
           GPGGAParser.h \
           GPGLLParser.h \
           GPGSAParser.h \
           GPGSVParser.h \
           GPRMCParser.h \
           GPSLogTableWidget.h \
           GPSReader.h \
           FIFODataSource.h \
           FIFOWriter.h \
           GPTXTParser.h \
           GPVTGParser.h \
           GPZDAParser.h \
           GenericFallbackParser.h \
           GenericNMEASentenceParser.h \
           HDGParser.h \
           HDMParser.h \
           HDTParser.h \
           INMEASentenceParser.h \
           NMEAParser.h \
           ParserWorker.h \
           RawDataDialog.h \
           IGPSHandler.h \
           StandaloneGPSHandler.h \
           PluginGPSHandler.h \
           GPSHandlerFactory.h \
           SocketChannel.h \
           CommunicationChannel.h \
           ThreadSafeQueue.h

DISTFILES += \
    GPSReader Project Documentation.docx
