//
//  MainFram.hpp
//  Ma_Solution
//
//  Created by Mirette BEKHIT on 21/03/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef Ma_Solution_MainFram_hpp
#define Ma_Solution_MainFram_hpp

#include <QWidget>
#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QSpinBox>

#include "Plotter.h"
#include "FindDialog.hpp"
#include "MainFram.hpp"
#include "math.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class MainFram : public QWidget {
    Q_OBJECT
public:
    MainFram();
    
signals:
    void clicked(); // l'evénement qui lance lance l'action
    
    private slots:
    void showDialog(); //l'action lancé par l'événement
    
private:
    QLabel *enterSpeedLabel;
    QLabel *returnSpeedLabel;
    QPushButton *button;
    QSpinBox *spinBox;
    QSlider *slider;
    QLineEdit *lineEdit;
    Plotter *plotter;
};

#endif
