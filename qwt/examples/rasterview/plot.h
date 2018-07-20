#ifndef PLOT_H
#define PLOT_H

#include <qwt_plot.h>

class QwtPlotSpectrogram;

class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot( QWidget * = NULL );

public Q_SLOTS:
    void exportPlot();
    void setResampleMode( int );

private:
    QwtPlotSpectrogram *d_spectrogram;
};

#endif
