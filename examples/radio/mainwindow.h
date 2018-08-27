#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <qwt_global.h>
#include <qwidget.h>

class MainWindow : public QWidget
{
public:
    MainWindow();

protected:
    virtual void resizeEvent( QResizeEvent * ) QWT_OVERRIDE;

private:
    void updateGradient();
};

#endif
