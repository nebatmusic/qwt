/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_spline_approximation.h"
#include "qwt_spline_parametrization.h"

class QwtSpline::PrivateData
{
public:
    PrivateData():
        boundaryType( QwtSpline::ConditionalBoundaries )
    {
        parametrization = new QwtSplineParametrization( 
            QwtSplineParametrization::ParameterChordal );

        // parabolic runout at both ends
        
        boundaryConditions[0].type = QwtSpline::Clamped3;
        boundaryConditions[0].value = 0.0;
        
        boundaryConditions[1].type = QwtSpline::Clamped3;
        boundaryConditions[1].value = 0.0;
    }

    ~PrivateData()
    {
        delete parametrization;
    }

    QwtSplineParametrization *parametrization;
    QwtSpline::BoundaryType boundaryType;

    struct
    {   
        int type;
        double value;
    
    } boundaryConditions[2];
};

/*!
  \fn QPainterPath QwtSpline::painterPath( const QPolygonF &points ) const

  Approximates a polygon piecewise with cubic Bezier curves
  and returns them as QPainterPath.

  \param points Control points
  \return Painter path, that can be rendered by QPainter
 */

/*!
  \brief Constructor

  The default setting is a non closing spline with chordal parametrization

  \sa setParametrization(), setBoundaryType()
 */
QwtSpline::QwtSpline()
{
    d_data = new PrivateData;
}

//! Destructor
QwtSpline::~QwtSpline()
{
    delete d_data;
}

/*!
  The locality of an spline interpolation identifies how many adjacent
  polynoms are affected, when changing the position of one point.

  A locality of 'n' means, that changing the coordinates of a point
  has an effect on 'n' leading and 'n' following polynoms.
  Those polynoms can be calculated from a local subpolygon.

  A value of 0 means, that the interpolation is not local and any modification
  of the polygon requires to recalculate all polynoms ( f.e cubic splines ). 

  \return Order of locality
 */
uint QwtSpline::locality() const
{
    return 0;
}

/*!
  Define the parametrization for a parametric spline approximation
  The default setting is a chordal parametrization.

  \param type Type of parametrization, ususally one of QwtSplineParametrization::Type
  \sa parametrization()
 */
void QwtSpline::setParametrization( int type )
{
    if ( d_data->parametrization->type() != type )
    {
        delete d_data->parametrization;
        d_data->parametrization = new QwtSplineParametrization( type );
    }
}

/*!
  Define the parametrization for a parametric spline approximation
  The default setting is a chordal parametrization.

  \param parametrization Parametrization
  \sa parametrization()
 */
void QwtSpline::setParametrization( QwtSplineParametrization *parametrization )
{
    if ( ( parametrization != NULL ) && ( d_data->parametrization != parametrization ) )
    {
        delete d_data->parametrization;
        d_data->parametrization = parametrization;
    }
}   

/*!
  \return parametrization
  \sa setParametrization()
 */
const QwtSplineParametrization *QwtSpline::parametrization() const
{
    return d_data->parametrization;
}

/*!
  Define the boundary type for the endpoints of the approximating
  spline.

  \param boundaryType Boundary type
  \sa boundaryType()
 */
void QwtSpline::setBoundaryType( BoundaryType boundaryType )
{
    d_data->boundaryType = boundaryType;
}

/*!
  \return Boundary type
  \sa setBoundaryType()
 */
QwtSpline::BoundaryType QwtSpline::boundaryType() const
{
    return d_data->boundaryType;
}

/*!
  \brief Define the condition for an endpoint of the spline

  \param position At the beginning or the end of the spline
  \param condition Condition
    
  \sa BoundaryCondition, QwtSplineC2::BoundaryCondition, boundaryCondition()
 */
void QwtSpline::setBoundaryCondition( BoundaryPosition position, int condition )
{
    if ( ( position == QwtSpline::AtBeginning ) || ( position == QwtSpline::AtEnd ) )
        d_data->boundaryConditions[position].type = condition;
}

/*!
  \return Condition for an endpoint of the spline
  \param position At the beginning or the end of the spline

  \sa setBoundaryCondition(), boundaryValue(), setBoundaryConditions()
 */
int QwtSpline::boundaryCondition( BoundaryPosition position ) const
{
    if ( ( position == QwtSpline::AtBeginning ) || ( position == QwtSpline::AtEnd ) )
        return d_data->boundaryConditions[position].type;
        
    return d_data->boundaryConditions[0].type; // should never happen
}   

/*!
  \brief Define the boundary value

  The boundary value is an parameter used in combination with
  the boundary condition. Its meaning depends on the condition.

  \param position At the beginning or the end of the spline
  \param value Value used for the condition at the end point

  \sa boundaryValue(), setBoundaryCondition()
 */
void QwtSpline::setBoundaryValue( BoundaryPosition position, double value )
{
    if ( ( position == QwtSpline::AtBeginning ) || ( position == QwtSpline::AtEnd ) )
        d_data->boundaryConditions[position].value = value;
}       

/*!
  \return Boundary value
  \param position At the beginning or the end of the spline

  \sa setBoundaryValue(), boundaryCondition()
 */
double QwtSpline::boundaryValue( BoundaryPosition position ) const
{
    if ( ( position == QwtSpline::AtBeginning ) || ( position == QwtSpline::AtEnd ) )
        return d_data->boundaryConditions[position].value;
        
    return d_data->boundaryConditions[0].value; // should never happen
}   

/*!
  \brief Define the condition at the endpoints of a spline

  \param condition Condition
  \param valueBegin Used for the condition at the beginning of te spline
  \param valueEnd Used for the condition at the end of te spline

  \sa BoundaryCondition, QwtSplineC2::BoundaryCondition, 
      tsetBoundaryCondition(), setBoundaryValue()
 */
void QwtSpline::setBoundaryConditions(
    int condition, double valueBegin, double valueEnd )
{   
    setBoundaryCondition( QwtSpline::AtBeginning, condition );
    setBoundaryValue( QwtSpline::AtBeginning, valueBegin ); 
    
    setBoundaryCondition( QwtSpline::AtEnd, condition );
    setBoundaryValue( QwtSpline::AtEnd, valueEnd );
}   

