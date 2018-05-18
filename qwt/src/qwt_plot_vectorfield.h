/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_VECTOR_FIELD_H
#define QWT_PLOT_VECTOR_FIELD_H

#include "qwt_global.h"
#include "qwt_plot_seriesitem.h"
#include "qwt_series_data.h"

class QPen;
class QBrush;

class QWT_EXPORT QwtPlotVectorField: 
    public QwtPlotSeriesItem, QwtSeriesStore<QwtVectorFieldSample>
{
public:
    enum IndicatorOrigin
    {
        OriginHead,
        OriginTail,
        OriginCenter
    };

    explicit QwtPlotVectorField( const QString &title = QString() );
    explicit QwtPlotVectorField( const QwtText &title );

    virtual ~QwtPlotVectorField();

#if 1
    // temporary: there will be a QwtVectorSymbol later TODO ...
    void setPen( const QPen & );
    QPen pen() const;

    void setBrush( const QBrush & );
    QBrush brush() const;
#endif

    void setRasterSize( const QSizeF& );
    QSizeF rasterSize() const;

    void setIndicatorOrigin( IndicatorOrigin );
    IndicatorOrigin indicatorOrigin() const;

    void setSamples( const QVector<QwtVectorFieldSample> & );
    void setSamples( QwtVectorFieldData * );

    virtual void drawSeries( QPainter *,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    virtual int rtti() const;

    virtual QwtGraphic legendIcon( int index, const QSizeF & ) const;

    void setMagnitudeScaleFactor( qreal factor );
    qreal magnitudeScaleFactor() const;

protected:
    virtual void drawArrows( QPainter *,
        const QwtScaleMap &xMap, const QwtScaleMap &yMap,
        const QRectF &canvasRect, int from, int to ) const;

    virtual void drawArrow( QPainter *,
        double x, double y,
        double direction, double magnitude ) const;

private:
    void init();

    class PrivateData;
    PrivateData *d_data;
};

#endif
