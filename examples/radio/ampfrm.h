#ifndef AMP_FRAME_H
#define AMP_FRAME_H

#include <qwt_global.h>
#include <qframe.h>

class Knob;
class Thermo;

class AmpFrame : public QFrame
{
    Q_OBJECT
public:
    AmpFrame( QWidget * );

public Q_SLOTS:
    void setMaster( double v );

protected:
    virtual void timerEvent( QTimerEvent * ) QWT_OVERRIDE;

private:
    Knob *d_knbVolume;
    Knob *d_knbBalance;
    Knob *d_knbTreble;
    Knob *d_knbBass;
    Thermo *d_thmLeft;
    Thermo *d_thmRight;
    double d_master;
};

#endif
