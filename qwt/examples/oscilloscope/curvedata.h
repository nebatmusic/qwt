#ifndef CURVE_DATA
#define CURVE_DATA

#include <qwt_series_data.h>

class SignalData;

class CurveData: public QwtSeriesData<QPointF>
{
public:
    const SignalData &values() const;
    SignalData &values();

    virtual QPointF sample( size_t index ) const QWT_OVERRIDE;
    virtual size_t size() const QWT_OVERRIDE;

    virtual QRectF boundingRect() const QWT_OVERRIDE;
};

#endif
