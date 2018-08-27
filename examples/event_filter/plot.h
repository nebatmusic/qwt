#ifndef COLOR_BAR_H
#define COLOR_BAR_H

#include <qwt_plot.h>

class ColorBar;
class QwtWheel;

class Plot: public QwtPlot
{
    Q_OBJECT
public:
    Plot( QWidget *parent = NULL );
    virtual bool eventFilter( QObject *, QEvent * ) QWT_OVERRIDE;

public Q_SLOTS:
    void setCanvasColor( const QColor & );
    void insertCurve( int axis, double base );

private Q_SLOTS:
    void scrollLeftAxis( double );

private:
    void insertCurve( Qt::Orientation, const QColor &, double base );

    ColorBar *d_colorBar;
    QwtWheel *d_wheel;
};

#endif
