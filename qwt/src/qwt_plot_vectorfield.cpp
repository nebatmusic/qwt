/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_vectorfield.h"
#include "qwt_scale_map.h"
#include "qwt_painter.h"
#include "qwt_math.h"
#include "qwt_graphic.h"
#include <qpainter.h>
#include <qpainterpath.h>

#define DEBUG_RENDER 0

#if DEBUG_RENDER
#include <qelapsedtimer.h>
#include <qdebug.h>
#endif

static inline double qwtVector2Radians( double u, double v )
{
    if ( u == 0.0 )
        return 0.0;

    const double radians = ::atan( v / u );
    return ( v >= 0.0 ) ? radians : ( 2 * M_PI - radians ); 
}

static inline double qwtVector2Magnitude( double u, double v )
{
    return sqrt( u * u + v * v );
}

namespace
{
    class Arrow : public QPainterPath
    {
    public:
        Arrow( double headWidth = 6.0, double tailWidth = 1.0 ):
            m_headWidth( headWidth ),
            m_tailWidth( tailWidth )
        {
            lineTo( -m_headWidth, m_headWidth );
            lineTo( -m_headWidth, m_tailWidth );
            lineTo( -m_headWidth - 1.0, m_tailWidth );
            lineTo( -m_headWidth - 1.0, -m_tailWidth );
            lineTo( -m_headWidth, -m_tailWidth );
            lineTo( -m_headWidth, -m_headWidth );

            closeSubpath();
        }

        inline double headWidth() const
        {
            return m_headWidth;
        }

        inline double tailWidth() const
        {
            return m_tailWidth;
        }

        void setTailLength( qreal length )
        {
            const qreal x = -( m_headWidth + length );

            setElementPositionAt( 3, x, m_tailWidth );
            setElementPositionAt( 4, x, -m_tailWidth );
        }

    private:
        const double m_headWidth;
        const double m_tailWidth;
    };

    class FilterMatrix
    {
    public:
        class Entry
        {
        public:
            Entry():
                count( 0 ),
                x( 0 ),
                y( 0 ),
                u( 0 ),
                v( 0 )
            {
            }

            inline void addSample( double sx, double sy,
                double su, double sv )
            {
                x += sx;
                y += sy;

                u += su;
                v += sv;

                count++;
            }

            int count;

            double x;
            double y;
            double u;
            double v;
        };

        FilterMatrix( const QRectF& dataRect,
            const QRectF& canvasRect, const QSizeF& cellSize )
        {
            d_dx = cellSize.width();
            d_dy = cellSize.height();

            d_x0 = dataRect.x();
            if ( d_x0 < canvasRect.x() )
                d_x0 += int ( ( canvasRect.x() - d_x0 ) / d_dx ) * d_dx;

            d_y0 = dataRect.y();
            if ( d_y0 < canvasRect.y() )
                d_y0 += int ( ( canvasRect.y() - d_y0 ) / d_dy ) * d_dy;

            d_numColumns = canvasRect.width() / d_dx + 1;
            d_numRows = canvasRect.height() / d_dy + 1;

            d_x1 = d_x0 + d_numColumns * d_dx;
            d_y1 = d_y0 + d_numRows * d_dy;

            d_entries.resize( d_numRows * d_numColumns );
        }

        inline int numColumns() const
        {
            return d_numColumns;
        }

        inline int numRows() const
        {
            return d_numRows;
        }

        inline void addSample( double x, double y,
            double u, double v )
        {
            if ( x >= d_x0 && x < d_x1
                && y >= d_y0 && y < d_y1 )
            {
                Entry &entry = d_entries[ indexOf( x, y ) ];
                entry.addSample( x, y, u, v );
            }
        }

        const FilterMatrix::Entry* entries() const
        {
            return d_entries.constData();
        }

    private:
        inline int indexOf( qreal x, qreal y ) const
        {
            const int col = ( x - d_x0 ) / d_dx;
            const int row = ( y - d_y0 ) / d_dy;

            return row * d_numColumns + col;
        }

        qreal d_x0, d_x1, d_y0, d_y1, d_dx, d_dy;
        int d_numColumns;
        int d_numRows;

        QVector< Entry > d_entries;
    };
}

class QwtPlotVectorField::PrivateData
{
public:
    PrivateData():
        pen( Qt::NoPen ),
        brush( Qt::black ),
        indicatorOrigin( QwtPlotVectorField::OriginHead ),
        magnitudeScaleFactor( 1.0 )
    {
    }

    QPen pen;
    QBrush brush;

    IndicatorOrigin indicatorOrigin;
    Arrow arrow;

    qreal magnitudeScaleFactor;
    QSizeF rasterSize;
};

/*!
  Constructor
  \param title Title of the curve
*/
QwtPlotVectorField::QwtPlotVectorField( const QwtText &title ):
    QwtPlotSeriesItem( title )
{
    init();
}

/*!
  Constructor
  \param title Title of the curve
*/
QwtPlotVectorField::QwtPlotVectorField( const QString &title ):
    QwtPlotSeriesItem( QwtText( title ) )
{
    init();
}

//! Destructor
QwtPlotVectorField::~QwtPlotVectorField()
{
    delete d_data;
}

/*!
  \brief Initialize data members
*/
void QwtPlotVectorField::init()
{
    setItemAttribute( QwtPlotItem::Legend );
    setItemAttribute( QwtPlotItem::AutoScale );

    d_data = new PrivateData;
    setData( new QwtVectorFieldData() );

    setZ( 20.0 );
}

void QwtPlotVectorField::setPen( const QPen &pen )
{
    if ( d_data->pen != pen )
    {
        d_data->pen = pen;

        itemChanged();
        legendChanged();
    }
}

QPen QwtPlotVectorField::pen() const
{
    return d_data->pen;
}

void QwtPlotVectorField::setBrush( const QBrush &brush )
{
    if ( d_data->brush != brush )
    {
        d_data->brush = brush;

        itemChanged();
        legendChanged();
    }
}

QBrush QwtPlotVectorField::brush() const
{
    return d_data->brush;
}

void QwtPlotVectorField::setIndicatorOrigin( IndicatorOrigin origin )
{
    d_data->indicatorOrigin = origin;
    if ( d_data->indicatorOrigin != origin )
    {
        d_data->indicatorOrigin = origin;
        itemChanged();
    }
}

QwtPlotVectorField::IndicatorOrigin QwtPlotVectorField::indicatorOrigin() const
{
    return d_data->indicatorOrigin;
}

void QwtPlotVectorField::setMagnitudeScaleFactor( qreal factor )
{
    if ( factor != d_data->magnitudeScaleFactor )
    {
        d_data->magnitudeScaleFactor = factor;
        itemChanged();
    }
}

qreal QwtPlotVectorField::magnitudeScaleFactor() const
{
    return d_data->magnitudeScaleFactor;
}

void QwtPlotVectorField::setRasterSize( const QSizeF& size )
{
    if ( size != d_data->rasterSize )
    {
        d_data->rasterSize = size;
        itemChanged();
    }
}

QSizeF QwtPlotVectorField::rasterSize() const
{
    return d_data->rasterSize;
}

//! \return QwtPlotItem::Rtti_PlotField
int QwtPlotVectorField::rtti() const
{
    return QwtPlotItem::Rtti_PlotVectorField;
}

/*!
  Initialize data with an array of samples.
  \param samples Vector of points
*/
void QwtPlotVectorField::setSamples( const QVector<QwtVectorFieldSample> &samples )
{
    setData( new QwtVectorFieldData( samples ) );
}

/*!
  Assign a series of samples
    
  setSamples() is just a wrapper for setData() without any additional
  value - beside that it is easier to find for the developer.
    
  \param data Data
  \warning The item takes ownership of the data object, deleting
           it when its not used anymore. 
*/
void QwtPlotVectorField::setSamples( QwtVectorFieldData *data )
{
    setData( data );
}  

QwtGraphic QwtPlotVectorField::legendIcon(
    int index, const QSizeF &size ) const
{
    Q_UNUSED( index );

    QwtGraphic icon;
    icon.setDefaultSize( size );

    if ( size.isEmpty() )
        return icon;

    QPainter painter( &icon );
    painter.setRenderHint( QPainter::Antialiasing,
        testRenderHint( QwtPlotItem::RenderAntialiased ) );

    painter.translate( -size.width(), -0.5 * size.height() );

    painter.setPen( d_data->pen );
    painter.setBrush( d_data->brush );

    d_data->arrow.setTailLength( 2 * d_data->arrow.headWidth() );
    painter.drawPath( d_data->arrow );

    return icon;
}

/*!
  Draw a subset of the points

  \param painter Painter
  \param xMap Maps x-values into pixel coordinates.
  \param yMap Maps y-values into pixel coordinates.
  \param canvasRect Contents rectangle of the canvas
  \param from Index of the first sample to be painted
  \param to Index of the last sample to be painted. If to < 0 the
         series will be painted to its last sample.

  \sa drawDots()
*/
void QwtPlotVectorField::drawSeries( QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRectF &canvasRect, int from, int to ) const
{
    if ( !painter || dataSize() <= 0 )
        return;

    if ( to < 0 )
        to = dataSize() - 1;

    if ( from < 0 )
        from = 0;

    if ( from > to )
        return;

#if DEBUG_RENDER
    QElapsedTimer timer;
    timer.start();
#endif
    drawArrows( painter, xMap, yMap, canvasRect, from, to );
#if DEBUG_RENDER
    qDebug() << timer.elapsed();
#endif
}

void QwtPlotVectorField::drawArrows( QPainter *painter,
    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
    const QRectF &canvasRect, int from, int to ) const
{
    const bool doAlign = QwtPainter::roundingAlignment( painter );
    const bool doClip = false;

    const bool isInvertingX = xMap.isInverting();
    const bool isInvertingY = yMap.isInverting();

    const QwtSeriesData<QwtVectorFieldSample> *series = data();

    painter->setPen( d_data->pen );
    painter->setBrush( d_data->brush );

    if ( !d_data->rasterSize.isEmpty() )
    {
        const QRectF dataRect = QwtScaleMap::transform(
            xMap, yMap, boundingRect() );

        FilterMatrix matrix( dataRect, canvasRect, d_data->rasterSize );

        for ( int i = from; i <= to; i++ )
        {
            const QwtVectorFieldSample sample = series->sample( i );

            matrix.addSample( xMap.transform( sample.x ),
                yMap.transform( sample.y ), sample.u, sample.v );
        }

        const int numEntries = matrix.numRows() * matrix.numColumns();
        const FilterMatrix::Entry* entries = matrix.entries();

        for ( int i = 0; i < numEntries; i++ )
        {
            const FilterMatrix::Entry &entry = entries[i];

            if ( entry.count == 0 )
                continue;

            double xi = entry.x / entry.count;
            double yi = entry.y / entry.count;

            if ( doAlign )
            {
                xi = qRound( xi );
                yi = qRound( yi );
            }

            const double u = entry.u / entry.count;
            const double v = entry.v / entry.count;

            const double magnitude = qwtVector2Magnitude( u, v );

            double radians = qwtVector2Radians( u, v );

            if ( isInvertingY )
                radians = -radians;

            if ( isInvertingX )
                radians = M_PI - radians;

            drawArrow( painter, xi, yi, radians, magnitude );
        }
    }
    else
    {
        for ( int i = from; i <= to; i++ )
        {
            const QwtVectorFieldSample sample = series->sample( i );

            double xi = xMap.transform( sample.x );
            double yi = yMap.transform( sample.y );

            if ( doAlign )
            {
                xi = qRound( xi );
                yi = qRound( yi );
            }

            if ( doClip )
            {
                if ( !canvasRect.contains( xi, yi ) )
                    continue;
            }

            const double magnitude = qwtVector2Magnitude( sample.u, sample.v );

            double radians = qwtVector2Radians( sample.u, sample.v );

            if ( isInvertingY )
                radians = -radians;

            if ( isInvertingX )
                radians = M_PI - radians;

            drawArrow( painter, xi, yi, radians, magnitude );
        }
    }
}

void QwtPlotVectorField::drawArrow( QPainter *painter,
    double x, double y, double direction, double magnitude ) const
{
    auto &arrow = d_data->arrow;

    double tailLength = magnitude * d_data->magnitudeScaleFactor * arrow.tailWidth();
#if 0
    if ( tailLength > 40 )
        tailLength = 40;
#endif

    arrow.setTailLength( tailLength );

    const QTransform oldTransform = painter->transform();

    QTransform transform = oldTransform;
    transform.translate( x, y );
    transform.rotateRadians( direction );

    if( d_data->indicatorOrigin == OriginTail )
    {
        const qreal dx = arrow.headWidth() + tailLength;
        transform.translate( dx, 0.0 );
    }
    else if ( d_data->indicatorOrigin == OriginCenter )
    {
        const qreal dx = arrow.headWidth() + tailLength;
        transform.translate( 0.5 * dx, 0.0 );
    }

    painter->setWorldTransform( transform, false );
    painter->drawPath( arrow );

    // restore previous matrix
    painter->setWorldTransform( oldTransform, false );
}