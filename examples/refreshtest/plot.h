#ifndef PLOT_H
#define PLOT_H

#include "settings.h"

#include <qwt_plot.h>
#include <qwt_system_clock.h>

class QwtPlotGrid;
class QwtPlotCurve;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget* = NULL );

public Q_SLOTS:
    void setSettings( const Settings & );

protected:
    virtual void timerEvent( QTimerEvent * ) QWT_OVERRIDE;

private:
    void alignScales();

    QwtPlotGrid *d_grid;
    QwtPlotCurve *d_curve;

    QwtSystemClock d_clock;
    double d_interval;

    int d_timerId;

    Settings d_settings;
};

#endif
