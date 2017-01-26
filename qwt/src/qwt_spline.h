/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_SPLINE_H
#define QWT_SPLINE_H 1

#include "qwt_global.h"
#include "qwt_spline_approximation.h"
#include "qwt_spline_polynomial.h"
#include <qpolygon.h>
#include <qpainterpath.h>
#include <qmath.h>

class QwtSplineParametrization;

/*!
  \brief Base class for a spline interpolation

  Spline interpolation is the process of interpolating a set of points
  piecewise with polynomials. The initial set of points is preserved.
*/
class QWT_EXPORT QwtSplineInterpolating: public QwtSpline
{
public:
    QwtSplineInterpolating();
    virtual ~QwtSplineInterpolating();

    virtual QPolygonF equidistantPolygon( const QPolygonF &, 
        double distance, bool withNodes ) const;

    virtual QPolygonF polygon( const QPolygonF &, double tolerance );

    virtual QPainterPath painterPath( const QPolygonF & ) const;
    virtual QVector<QLineF> bezierControlLines( const QPolygonF &points ) const = 0;

private:
    Q_DISABLE_COPY(QwtSplineInterpolating)
};

/*!
  \brief Base class for spline interpolations providing a 
         first order geometric continuity ( G1 ) between adjoing curves
 */
class QWT_EXPORT QwtSplineG1: public QwtSplineInterpolating
{           
public:     
    QwtSplineG1();
    virtual ~QwtSplineG1();
};

/*!
  \brief Base class for spline interpolations providing a 
         first order parametric continuity ( C1 ) between adjoing curves

  All interpolations with C1 continuity are based on rules for finding
  the 1. derivate at some control points.

  In case of non parametric splines those points are the curve points, while
  for parametric splines the calculation is done twice using a parameter value t.

  \sa QwtSplineParametrization
 */
class QWT_EXPORT QwtSplineC1: public QwtSplineG1
{
public:
    QwtSplineC1();
    virtual ~QwtSplineC1();

    virtual QPainterPath painterPath( const QPolygonF & ) const;
    virtual QVector<QLineF> bezierControlLines( const QPolygonF & ) const;

    virtual QPolygonF equidistantPolygon( const QPolygonF &,
        double distance, bool withNodes ) const;

    // these methods are the non parametric part
    virtual QVector<QwtSplinePolynomial> polynomials( const QPolygonF & ) const;
    virtual QVector<double> slopes( const QPolygonF & ) const = 0;

    virtual double slopeAtBeginning( const QPolygonF &, double slopeNext ) const;
    virtual double slopeAtEnd( const QPolygonF &, double slopeBefore ) const;
};

/*!
  \brief Base class for spline interpolations providing a 
         second order parametric continuity ( C2 ) between adjoing curves

  All interpolations with C2 continuity are based on rules for finding
  the 2. derivate at some control points.

  In case of non parametric splines those points are the curve points, while
  for parametric splines the calculation is done twice using a parameter value t.

  \sa QwtSplineParametrization
 */
class QWT_EXPORT QwtSplineC2: public QwtSplineC1
{
public:
    /*!
      Boundary condition that requires C2 continuity
      
      \sa QwtSpline::boundaryCondition, QwtSpline::BoundaryCondition
     */
    enum BoundaryConditionC2
    {
        /*!
          The second derivate at the endpoint is related to the second derivatives
          at the 2 neighbours: cv[0] := 2.0 * cv[1] - cv[2].

          \note boundaryValue() is ignored
         */
        CubicRunout = LinearRunout + 1, 

        /*!
          The 3rd derivate at the endpoint matches the 3rd derivate at its neighbours.
          Or in other words: the first/last curve segment extents the polynomial of its
          neighboured polynomial

          \note boundaryValue() is ignored
         */
        NotAKnot
    };

    QwtSplineC2();
    virtual ~QwtSplineC2();

    virtual QPainterPath painterPath( const QPolygonF & ) const;
    virtual QVector<QLineF> bezierControlLines( const QPolygonF & ) const;

    virtual QPolygonF equidistantPolygon( const QPolygonF &,
        double distance, bool withNodes ) const;

    // calculating the parametric equations
    virtual QVector<QwtSplinePolynomial> polynomials( const QPolygonF & ) const;
    virtual QVector<double> slopes( const QPolygonF & ) const;
    virtual QVector<double> curvatures( const QPolygonF & ) const = 0;
};

#endif
