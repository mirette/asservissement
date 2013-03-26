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

int main(int argc, char *argv[])
{
    //create an application
	QApplication app(argc, argv);	
    
    MainFram *fram = new MainFram;
    fram->show();
    return app.exec();
    
    
    /************************************************* example as a standalone window ************************************************************/
    /*	
     //create an application
     QApplication app(argc, argv);					
     
     // create and fill the data vector
     QVector<QPointF> data1;
     for (float i=0;i<20;i+=0.1)
     data1.append(QPointF(i,sin(i)));
     
     // create and fill another data vector
     QVector<QPointF> data2;
     for (float i=0;i<20;i+=0.1)
     data2.append(QPointF(i,sin(i/2)));
     
     //creat the plotter and set its data
     Plotter *plotter = new Plotter;							// create a plotter
     plotter->setPlotSettings(PlotSettings(0,20,-2,2));		//initialise the axis
     plotter->setCurveData(0,data1);							//set the first plotter data set
     plotter->setCurveData(1,data2);							//set the second plotter data set
     
     //show the plotter widget
     plotter->show();										
     
     //execute the application
     return app.exec();						
     */
    /*********************************************************************************************************************************************/
    
    
    /******************************************** example of use as a widget among others ****************************************************** /
     
     //create an application
     QApplication app(argc, argv);				
     
     // create a window
     QWidget *window = new QWidget;
     window->setWindowTitle("Plotter widget application");
     
     // create a label
     QLabel *label = new QLabel("<h2><i><font color=blue> Plot example </font> </i></h2>");
     
     //create the plotter and its data
     Plotter *plotter = new Plotter;	
     plotter->setPlotSettings(PlotSettings(0,20,-2,2));		
     
     QVector<QPointF> data1;
     for (float i=0;i<20;i+=0.1)
     data1.append(QPointF(i,sin(i)));
     QVector<QPointF> data2;
     for (float i=0;i<20;i+=0.1)
     data2.append(QPointF(i,sin(i/2)));
     
     plotter->setCurveData(0,data1);							
     plotter->setCurveData(1,data2);							
     
     
     //layout the widgets
     QVBoxLayout *layout = new QVBoxLayout;
     layout -> addWidget (label);
     layout -> addWidget (plotter);
     window->setLayout(layout);
     
     //show the window
     window->show();
     
     //execute the application
     return app.exec();	
     
     / ***********************************************************************************************************/
}

