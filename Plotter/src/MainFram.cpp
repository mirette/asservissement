//
//  MainFram.cpp
//  Ma_Solution
//
//  Created by Mirette BEKHIT on 21/03/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include <iostream>
#include <QtGui>
#include "MainFram.hpp"

MainFram::MainFram(): QWidget()
{
    // create a window
	setWindowTitle("Asservissement");
    setFixedSize(500, 400);
    
    // create a label
	enterSpeedLabel = new QLabel("<h2><i><font color=red>Entrer la vitesse</i></font></h2>");
    
    // create a button
    button = new QPushButton("Quit");
    button->setFixedSize(100, 20);
    //QObject::connect(button, SIGNAL(clicked()), &app, SLOT(quit()));
    QObject::connect(button, SIGNAL(clicked()), this, SLOT(showDialog()));
    
    // create a spinBox et spin
    spinBox = new QSpinBox;
    slider = new QSlider(Qt::Horizontal);
    spinBox->setRange(0, 500);
    slider->setRange(0, 500);
    
    spinBox->setFixedSize(50,20);
    slider->setFixedSize(200,20);
    
    QObject::connect(spinBox, SIGNAL(valueChanged(int)), slider, SLOT(setValue(int)));
    QObject::connect(slider, SIGNAL(valueChanged(int)), spinBox, SLOT(setValue(int)));
    spinBox->setValue(200);
    
    //create the plotter and its data
	plotter = new Plotter;	
	plotter->setPlotSettings(PlotSettings(0,20,-2,2));		
	
	QVector<QPointF> data1;
	for (float i=0;i<20;i+=0.1)
		data1.append(QPointF(i,sin(i)));
	QVector<QPointF> data2;
	for (float i=0;i<20;i+=0.1)
		data2.append(QPointF(i,sin(i/2)));
	
	plotter->setCurveData(0,data1);							
	plotter->setCurveData(1,data2);	
    
    //layout the SpinLayout
    QHBoxLayout *spinLayout = new QHBoxLayout;
    spinLayout->addWidget(spinBox);
    spinLayout->addWidget(slider);
    
    //Vertical Left Layout
	QVBoxLayout *leftLayout = new QVBoxLayout;
	leftLayout -> addWidget (enterSpeedLabel);
    leftLayout -> addLayout(spinLayout);
	leftLayout -> addWidget (plotter);
	leftLayout -> addWidget (button);
    
    //label de la vitesse renvoyé
    returnSpeedLabel = new QLabel("<i><font color=black>Vitesse de le moteur</font></i>");
    
    //textBox
    lineEdit = new QLineEdit; 
    lineEdit->setFixedSize(100,20);
    
    //Machin de la position QDial

    //Vertical Right Layout//
    QVBoxLayout *rightLayout = new QVBoxLayout;
	rightLayout -> QLayout::addWidget(returnSpeedLabel);
	rightLayout -> QLayout::addWidget(lineEdit);
    
    //layout générale
    QHBoxLayout *layout = new QHBoxLayout;
    layout -> addLayout(leftLayout);
    layout -> addLayout(rightLayout);

    setLayout(layout);
	
	//show the window
	//window->show();
}

void MainFram::showDialog()
{
    FindDialog *dialog = new FindDialog;
    dialog->show();
}