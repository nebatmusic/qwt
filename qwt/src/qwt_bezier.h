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
    QwtBezier( double tolerance = 0.5 );
    ~QwtBezier();

    void setTolerance( double tolerance );
    double tolerance() const;
        
    QPolygonF toPolygon( const QPointF &p1, const QPointF &cp1,
        const QPointF &cp2, const QPointF &p2 ) const;

    void appendToPolygon( const QPointF &p1, const QPointF &cp1,
        const QPointF &cp2, const QPointF &p2, QPolygonF &polygon ) const;

    static QPointF pointAt( const QPointF &p1, const QPointF &cp1,
        const QPointF &cp2, const QPointF &p2, double t );

private:
    double m_tolerance;
    double m_flatness;
};

inline double QwtBezier::tolerance() const
{
    return m_tolerance;
}

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
