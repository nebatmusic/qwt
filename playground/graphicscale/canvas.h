#ifndef CANVAS_H
#define CANVAS_H

#include <qwt_global.h>
#include <qwidget.h>

class QSvgRenderer;
class QwtGraphic;

class Canvas: public QWidget
{
public:
    enum Mode
    {
        Svg,
        VectorGraphic
    };

    Canvas( Mode, QWidget *parent = NULL );
    virtual ~Canvas();

    void setSvg( const QByteArray & );

protected:
    virtual void paintEvent( QPaintEvent * ) QWT_OVERRIDE;

private:
    void render( QPainter *, const QRect & ) const;

    const Mode d_mode;
    union
    {
        QSvgRenderer *d_renderer;
        QwtGraphic *d_graphic;
    };
};

#endif
