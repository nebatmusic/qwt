######################################################################
# Qwt Examples - Copyright (C) 2002 Uwe Rathmann
# This file may be used under the terms of the 3-clause BSD License
######################################################################

include( $${PWD}/../examples.pri )

TARGET       = event_filter

HEADERS = \
    colorbar.h \
    scalepicker.h \
    canvaspicker.h \
    plot.h 

SOURCES = \
    colorbar.cpp \
    scalepicker.cpp \
    canvaspicker.cpp \
    plot.cpp \
    event_filter.cpp 
