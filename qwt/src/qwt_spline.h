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

  Splines can be classified according to conditions of the polynomials that
  are met at the start/endpoints of the pieces: 

  - Geometric Continuity

    - G0: polynamials are joined
    - G1: first derivatives are proportional at the join point
          The curve tangents thus have the same direction, but not necessarily the 
          same magnitude. i.e., C1'(1) = (a,b,c) and C2'(0) = (k*a, k*b, k*c).
    - G2: first and second derivatives are proportional at join point 

  - Parametric Continuity

    - C0: curves are joined
    - C1: first derivatives equal
    - C2: first and second derivatives are equal

  Geometric continuity requires the geometry to be continuous, while parametric 
  continuity requires that the underlying parameterization be continuous as well.
  Parametric continuity of order n implies geometric continuity of order n,
  but not vice-versa. 

  QwtSpline is a base class for spline interpolations of any continuity.
*/
class QWT_EXPORT QwtSpline: public QwtSplineApproximation
{
public:
    /*!
      position of a boundary condition
      \sa boundaryCondition(), boundaryValue()
     */
    enum BoundaryPosition
    {
        //! the condiation is at the beginning of the polynomial
        AtBeginning,

        //! the condiation is at the end of the polynomial
        AtEnd
    };

    /*!
      boundary condition type

      \sa boundaryCondition()
      \sa QwtSplineC2::BoundaryConditionC2
     */
    enum BoundaryCondition
    {
        /*!
          The first derivative is given
          \sa boundaryValue()
         */
        Clamped1,

        /*!
          The second derivative is given

          \sa boundaryValue()
          \note a condition having a second derivative of 0 
                is also called "natural".
         */
        Clamped2,

        /*!
          The third derivative is given

          \sa boundaryValue()
          \note a condition having a third derivative of 0 
                is also called "parabolic runout".
         */
        Clamped3,

        /*!
          The first derivate at the endpoint is related to the first derivative
          at its neighbour by the boundyry value. F,e when the boundary
          value at the end is 1.0 then slope at the last 2 points is
          the same.

          \sa boundaryValue().
         */
        LinearRunout 
    };

    QwtSpline();
    virtual ~QwtSpline();

    void setBoundaryCondition( BoundaryPosition, int condition );
    int boundaryCondition( BoundaryPosition ) const;

    void setBoundaryValue( BoundaryPosition, double value );
    double boundaryValue( BoundaryPosition ) const;

    void setBoundaryConditions( int condition,
        double valueBegin = 0.0, double valueEnd = 0.0 );

    virtual QPolygonF equidistantPolygon( const QPolygonF &, 
        double distance, bool withNodes ) const;

    virtual QPolygonF polygon( const QPolygonF &, double tolerance );

    virtual QPainterPath painterPath( const QPolygonF & ) const;
    virtual QVector<QLineF> bezierControlLines( const QPolygonF &points ) const = 0;

private:
    Q_DISABLE_COPY(QwtSpline)

    class PrivateData;
    PrivateData *d_data;
};

/*!
  \brief Base class for spline interpolations providing a 
         first order geometric continuity ( G1 ) between adjoing curves
 */
class QWT_EXPORT QwtSplineG1: public QwtSpline
{           
public:     
    QwtSplineG1();
    virtual ~QwtSplineG1();
};

/*!
  \brief Base class for spline interpolations providing a 
         first order parametric continuity ( C1 ) between adjoing curves
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

    // calculating the parametric equations
    virtual QVector<QwtSplinePolynomial> polynomials( const QPolygonF & ) const;
    virtual QVector<double> slopes( const QPolygonF & ) const = 0;

    // resolving the boundary conditions
    virtual double slopeAtBeginning( const QPolygonF &, double slopeNext ) const;
    virtual double slopeAtEnd( const QPolygonF &, double slopeBefore ) const;
};

/*!
  \brief Base class for spline interpolations providing a 
         second order parametric continuity ( C2 ) between adjoing curves
 */
class QWT_EXPORT QwtSplineC2: public QwtSplineC1
{
public:
    /*!
      boundary condition that requires C2 continuity
      \sa QwtSpline::setBoundaryCondition, QwtSpline::BoundaryCondition
     */
    enum BoundaryConditionC2
    {
        /*!
          The second derivate at the endpoint is related to the second derivatives
          at the 2 neighbours: cv[0] := 2 * cv[1] - cv[2].

          \sa boundaryValue().
         */
        CubicRunout = LinearRunout + 1, 

        /*!
          The 3rd derivate at the endpoint matches the 3rd derivate at its neighbours.
          Or in other words: the first/last curve segment extents the polynomial of its
          neighboured polynomial
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
