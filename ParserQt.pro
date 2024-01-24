QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Parser/Exceptions.cpp \
    Parser/FileReader.cpp \
    Parser/ParserBase.cpp \
    Parser/ParserSsd.cpp \
    Parser/Preview.cpp \
    Parser/ReaderBase.cpp \
    main.cpp \
    parserqt.cpp

HEADERS += \
    Parser/Exceptions.h \
    Parser/FileReader.h \
    Parser/ParserBase.h \
    Parser/ParserSsd.h \
    Parser/ParserTemplate.h \
    Parser/Preview.h \
    Parser/ReaderBase.h \
    parserqt.h

FORMS += \
    parserqt.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    Parser/SampleFiles/Medium.ssd \
    Parser/SampleFiles/MillionSamples WithTail.ssd \
    Parser/SampleFiles/TeskTaskDescription.doc \
    Parser/SampleFiles/big.ssd \
    Parser/SampleFiles/blocks.ssd \
    Parser/SampleFiles/blocks_0.ssd \
    Parser/SampleFiles/just.rsd \
    Parser/SampleFiles/sinusoids.ssd
