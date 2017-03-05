#ifndef _TREE_VIEW_H_
#define _TREE_VIEW_H_

#include <qtreeview.h>

class TreeView: public QTreeView
{
    Q_OBJECT

public:
    TreeView( QWidget *parent = NULL );

Q_SIGNALS:
    void selected( const QString& );

public Q_SLOTS:
    void setRootPath( const QString& );

protected:
    virtual void currentChanged( const QModelIndex &, const QModelIndex & );
};

#endif
