//
//  FindDialog.hpp
//  Ma_Solution
//
//  Created by Mirette BEKHIT on 21/03/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Ma_Solution_FindDialog_hpp
#define Ma_Solution_FindDialog_hpp
#include <QDialog>
class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class FindDialog : public QDialog {
    Q_OBJECT
public:
    FindDialog(QWidget *parent = 0);
    
signals:
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);
    
    private slots:
    void findClicked();
    void enableFindButton(const QString &text);
    
private:
    QLabel *label;
    QLineEdit *lineEdit;
    QCheckBox *caseCheckBox;
    QCheckBox *backwardCheckBox;
    QPushButton *findButton;
    QPushButton *closeButton;
};

#endif
