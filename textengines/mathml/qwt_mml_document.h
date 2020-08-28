#ifndef QWT_MML_DOCUMENT_H
#define QWT_MML_DOCUMENT_H

#include <qwt_global.h>

class QString;
class QSize;
class QPainter;
class QPoint;
class QColor;

class QwtMmlDocument;

class QWT_EXPORT QwtMathMLDocument
{
public:
    enum MmlFont
    {
        NormalFont,
        FrakturFont,
        SansSerifFont,
        ScriptFont,
        MonospaceFont,
        DoublestruckFont
    };

    QwtMathMLDocument();
    ~QwtMathMLDocument();

    void clear();

    bool setContent( const QString& text, QString *errorMsg = 0,
        int *errorLine = 0, int *errorColumn = 0 );

    void paint( QPainter *, const QPoint &pos ) const;
    QSize size() const;

    QString fontName( MmlFont type ) const;
    void setFontName( MmlFont type, const QString &name );

    int baseFontPointSize() const;
    void setBaseFontPointSize( int size );

    QColor foregroundColor() const;
    void setForegroundColor( const QColor &color );

    QColor backgroundColor() const;
    void setBackgroundColor( const QColor &color );

private:
    QwtMmlDocument *m_doc;
};

#endif
