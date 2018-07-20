#ifndef QUOTE_FACTORY_H
#define QUOTE_FACTORY_H

class QwtOHLCSample;

class QString;
template <typename T> class QVector;

namespace QuoteFactory
{
    enum Stock
    {
        BMW,
        Daimler,
        Porsche,

        NumStocks
    };

    QVector<QwtOHLCSample> samples2010( Stock );
    QString title( Stock );
};

#endif
