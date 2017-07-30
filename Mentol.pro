TEMPLATE = app

QT += core qml quick widgets testlib

SRC_DIR = $$TOP_PWD

# Enable C++11 support (but on MSVC not needed - it generates warnings)
QMAKE_CXXFLAGS += -std=c++0x

# Code Coverage
QMAKE_CXXFLAGS += --coverage
QMAKE_LFLAGS += --coverage

SOURCES +=  src/MentolApp/main.cpp \
            src/MentolLib/MainViewModel.cpp \
            src/MentolLib/ComplexityToPrecision_Model.cpp \
            src/MentolLib/ComplexityToPrecisionProjection.cpp

SOURCES +=  src/MentolLib/calcoperation.cpp \
            src/MentolLib/KnownOperations.cpp \
            src/MentolLib/CalcResolver.cpp \
            src/MentolLib/CalcOperationsVisualizer.cpp \
            src/MentolLib/CalcOperationsToGeometryConverter.cpp \
            src/MentolLib/KeypadProcessor.cpp

SOURCES  += UnitTests/CalcOperation_Tests.cpp \
            UnitTests/KnownOperations_Tests.cpp \
            UnitTests/CalcResolver_Tests.cpp \
            UnitTests/CalcOperationsVisualizer_Tests.cpp \
            UnitTests/CalcOperationsSvgVisualizer_Tests.cpp \
            UnitTests/CalcOperationsToGeometryConverter_Tests.cpp \
            UnitTests/KeypadProcessor_Tests.cpp \
            UnitTests/ComplexityToPrecisionProjection_Tests.cpp \
            UnitTests/MainViewModel_Tests.cpp


HEADERS  += src/MentolLib/MainViewModel.h \
            src/MentolLib/ComplexityToPrecision_Model.h \
            src/MentolLib/ComplexityToPrecisionProjection.h

HEADERS +=  src/MentolLib/ICalcOperation.h \
            src/MentolLib/calcoperation.h \
            src/MentolLib/IKnownOperations.h \
            src/MentolLib/KnownOperations.h \
            src/MentolLib/CalcResolver.h \
            src/MentolLib/ICalcResolver.h \
            src/MentolLib/CalcOperationsVisualizer.h \
            src/MentolLib/ICalcOperationsVisualizer.h \
            src/MentolLib/ICalcOperationsToGeometryConverter.h \
            src/MentolLib/KeypadProcessor.h

HEADERS +=  UnitTests/AutoTest.h

RESOURCES += src/MentolApp/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

DISTFILES += \
    android/AndroidManifest.xml \
    src/MentolApp/Keypad.qml \
    src/MentolApp/KeypadButton.qml

OTHER_FILES += \
    UnitTests/scripts/runCoverage.sh

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
