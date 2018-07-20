#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>
#include <qwt_scale_div.h>

class QwtPlotCurve;
class QwtPlotIntervalCurve;
class QwtIntervalSample;

template <typename T> class QVector;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    enum Mode
    {
        Bars,
        Tube
    };

    Plot( QWidget * = NULL );

public Q_SLOTS:
    void setMode( int );
    void exportPlot();

private:
    void insertCurve( const QString &title,
        const QVector<QPointF> &, const QColor & );

    void insertErrorBars( const QString &title,
        const QVector<QwtIntervalSample> &,
        const QColor &color );


    QwtScaleDiv yearScaleDiv() const;

    QwtPlotIntervalCurve *d_intervalCurve;
    QwtPlotCurve *d_curve;
};

#endif
