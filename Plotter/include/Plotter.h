#ifndef PLOTTER_H
#define PLOTTER_H

#include <QMap>
#include <QPixmap>
#include <QVector>
#include <QWidget>

class QToolButton;
class PlotSettings;

/*-------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------- Plotter class --------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------*/
class Plotter : public QWidget
{
    Q_OBJECT

/***********************************************  public functions **************************************/
public:
	//constructor
    Plotter(QWidget *parent = 0);	

	//functions for setting up the plot, the curve data is stored as a QVector of QPointF
    void setPlotSettings(const PlotSettings &settings);			
    void setCurveData(int id, const QVector<QPointF> &data);	//QPointF is floating version of QPoint
    void clearCurve(int id);

	//implementations of functions inherited from Qwidget
    QSize minimumSizeHint() const;	 //can't modify the class members
    QSize sizeHint() const;

/********************************************  public slots *********************************************/
public slots:
    void zoomIn();
    void zoomOut();


/******************************  QWidget event handlers to reimplement **********************************/
protected:
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);


/******************************************  private members *********************************************/
private:
    //painting functions
	void updateRubberBandRegion();
    void refreshPixmap();
    void drawGrid(QPainter *painter);
    void drawCurves(QPainter *painter);
   
	//the Margin constant is used to provide some spacing around the graph
	enum {Margin = 50};  

	// buttons
    QToolButton *zoomInButton;
    QToolButton *zoomOutButton;


    QMap<int, QVector<QPointF> > curveMap;
    QVector<PlotSettings> zoomStack;			//holds the different zoom settings as a vector of PlotSettings
    int curZoom;								//holds the current PlotSettings index
    bool rubberBandIsShown;
    QRect rubberBandRect;
    
	//pixmap holds a copy of the whole widget's rendering, identical to what is shown on the screen.
	//the plot is always drawn onto this off-screen pixmap first; then the pixmap is copied onto the widget
	QPixmap pixmap; 

};


/*--------------------------------------------------------------------------------------------------------*/
/*----------------------------------------- PlotSettings class--------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------*/
//specifies the range of x and y axes and the number of ticks for these axes
class PlotSettings
{
public:
    PlotSettings();
	PlotSettings(int minX, int maxX,int minY,int maxY);
    void scroll(int dx, int dy);
    void adjust();
    double spanX() const{return maxX - minX;}
    double spanY() const{return maxY - minY;}
    double minX;
    double maxX;
    int numXTicks;
    double minY;
    double maxY;
    int numYTicks;
private:
    static void adjustAxis(double&min, double&max, int&numTicks);
};


#endif
