#ifndef __qpwgraph_appearance_h
#define __qpwgraph_appearance_h

#include "ui_qpwgraph_appearance.h"
#include <QFont>
#include <QList>
class qpwgraph_main;

class qpwgraph_appearance : public QDialog
{
    Q_OBJECT

public:
    
    qpwgraph_appearance(qpwgraph_main *parent);

    ~qpwgraph_appearance();

protected slots:
    
    void changed();

    void accept();
    void reject();
    void updateSelectedFont();
    void searchFont();
protected:
    void stabilize();

private:

    Ui::qpwgraph_appearance m_ui;

    int m_dirty;
    QFont m_selected_font;
};


#endif