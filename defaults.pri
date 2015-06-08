# Directories
# INCLUDEPATH += $$TOP_PWD/src/libs
SRC_DIR = $$TOP_PWD

# Enable C++11 support (but on MSVC not needed - it generates warnings)
unix:QMAKE_CXXFLAGS += -std=c++0x

HEADERS += \
    $$PWD/src/MentolLib/ComplexityToPrecisionProjection.h

SOURCES += \
    $$PWD/src/MentolLib/ComplexityToPrecisionProjection.cpp
