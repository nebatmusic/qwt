######################################################################
# Qwt Examples - Copyright (C) 2002 Uwe Rathmann
# This file may be used under the terms of the 3-clause BSD License
######################################################################

include( $${PWD}/../playground.pri )

!contains(QWT_CONFIG, QwtSvg) {

    message("Are you trying to build Qwt with the Qt Creator as Shadow Build ?")
    error("Qwt is configured without SVG support !")
}

TARGET   = svgmap
QT      += svg

RESOURCES += \
    svgmap.qrc

HEADERS = \
    plot.h

SOURCES = \
    plot.cpp \
    main.cpp
