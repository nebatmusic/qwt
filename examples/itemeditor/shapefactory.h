#ifndef SHAPE_FACTORY_H
#define SHAPE_FACTORY_H

class QPainterPath;
class QPointF;
class QSizeF;

namespace ShapeFactory
{
    enum Shape
    {
        Rect,
        Triangle,
        Ellipse,
        Ring,
        Star,
        Hexagon
    };

    QPainterPath path( Shape, const QPointF &, const QSizeF & );
}

#endif
