/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SPLINE_APPROXIMATION_H
#define QWT_SPLINE_APPROXIMATION_H 1

#include "qwt_global.h"
#include <qpainterpath.h>

class QwtSplineParametrization;

/*!
  \brief Base class for a spline approximation

  A spline approximation offers an algorithm for calculating a spline
  from a given set of points. The start and endpoints of the spline
  polynomials do not necessarily match the given points.

  For interpolating spline algorithms ( preserving the points )
  see QwtSpline.
*/
class QWT_EXPORT QwtSplineApproximation
{
public:
    /*!
       Boundary type specifying the spline at its endpoints

       \sa setBoundaryType(), boundaryType()
     */
    enum BoundaryType
    {
        /*!
           The polynomials at the start/endpoint depend on specific conditions

           \sa QwtSpline::BoundaryCondition
         */
        ConditionalBoundaries,

        /*!
          The polynomials at the start/endpoint are found by using
          imaginary additional points. Additional points at the end
          are found by translating points from the beginning or v.v.
         */
        PeriodicPolygon,

        /*!
          ClosedPolygon is similar to PeriodicPolygon beside, that
          the interpolation includes the connection between the last 
          and the first control point.

          \note Only works for parametrizations, where the parameter increment 
                for the the final closing line is positive. 
                This excludes QwtSplineParametrization::ParameterX and 
                QwtSplineParametrization::ParameterY
         */

        ClosedPolygon
    };

    QwtSplineApproximation();
    virtual ~QwtSplineApproximation();

    void setParametrization( int type );
    void setParametrization( QwtSplineParametrization * );
    const QwtSplineParametrization *parametrization() const;

    void setBoundaryType( BoundaryType );
    BoundaryType boundaryType() const;

    virtual QPainterPath painterPath( const QPolygonF & ) const = 0;
    virtual uint locality() const;

private:
    Q_DISABLE_COPY(QwtSplineApproximation)

    class PrivateData;
    PrivateData *d_data;
};

#endif
