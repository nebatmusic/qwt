/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_BEZIER_H
#define QWT_BEZIER_H 1

#include "qwt_global.h"
#include <qpolygon.h>

class QWT_EXPORT QwtBezier
{
public:
    static double minFlatness( double tolerance );

    static void toPolygon( double minFlatness,
        const QPointF &p1, const QPointF &cp1,
        const QPointF &cp2, const QPointF &p2,
        QPolygonF &polygon );

    static QPointF pointAt( const QPointF &p1,
        const QPointF &cp1, const QPointF &cp2, const QPointF &p2, double t );
};

inline QPointF QwtBezier::pointAt( const QPointF &p1,
    const QPointF &cp1, const QPointF &cp2, const QPointF &p2, double t )
{
    const double d1 = 3.0 * t;
    const double d2 = 3.0 * t * t;
    const double d3 = t * t * t;
    const double s  = 1.0 - t;

    const double x = (( s * p1.x() + d1 * cp1.x() ) * s + d2 * cp2.x() ) * s + d3 * p2.x();
    const double y = (( s * p1.y() + d1 * cp1.y() ) * s + d2 * cp2.y() ) * s + d3 * p2.y();

    return QPointF( x, y );
}


#endif
