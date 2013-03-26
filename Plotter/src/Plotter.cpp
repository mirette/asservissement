#include <QtGui>
#include <cmath>

#include"Plotter.h"
// dd

/******************************************************* plotter constructor *****************************************************************/
Plotter::Plotter(QWidget *parent)
    :QWidget(parent)
{
	setBackgroundRole(QPalette::Light);	//tells the widget to use the light component of the palette  as the color for erasing the widget
	setAutoFillBackground(true);		//enable the filling of the background

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding); //this tells the layout manager that handles this widgets that the widget is especially willing to grow but can also shrink (in both directions) 
	setFocusPolicy(Qt::StrongFocus);  //makes the widget accept focus by clicking or by pressing Tab
	
	rubberBandIsShown = false;		// zoom rubber band not shown initially

	//zoom in button
	zoomInButton = new QToolButton(this);								//specify the parent since no layout is used
	zoomInButton-> setText("In");
	//zoomInButton->setIcon(QIcon(":/images/zoomin.png"));				//set the icon
	zoomInButton->adjustSize();											//set the size to that of the size hint
	connect(zoomInButton, SIGNAL(clicked()), this, SLOT(zoomIn()));		//connect the clicked signal to the zoomin slot

	//zoom out button
	zoomOutButton = new QToolButton(this);								//specify the parent since no layout is used
	zoomOutButton-> setText("Out");
	//zoomOutButton->setIcon(QIcon(":/images/zoomout.png"));			//set the icon
	zoomOutButton->adjustSize();										//set the size to that of the size hint
	connect(zoomOutButton, SIGNAL(clicked()), this, SLOT(zoomOut()));   //connect the clicked signal to the zoomout slot

	//plot settings
	setPlotSettings(PlotSettings());
}



/************************************************* plotter setPlotSettings function **********************************************************/
void Plotter::setPlotSettings(const PlotSettings &settings)
{
	zoomStack.clear();
    zoomStack.append(settings);
    curZoom = 0;
    zoomInButton->hide();
    zoomOutButton->hide();
    refreshPixmap();
}



/************************************************* plotter zoomOut function **********************************************************/

void Plotter::zoomOut()
{
    if (curZoom > 0){
        --curZoom;
        zoomOutButton->setEnabled(curZoom > 0);
        zoomInButton->setEnabled(true);
        zoomInButton->show();
        refreshPixmap();
    }
}


/************************************************* plotter zoomIut function **********************************************************/
void Plotter::zoomIn()
{
    if (curZoom < zoomStack.count() - 1){
        ++curZoom;
        zoomInButton->setEnabled(curZoom < zoomStack.count() - 1);
        zoomOutButton->setEnabled(true);
        zoomOutButton->show();
        refreshPixmap();
    }
}

/************************************************* plotter setCurveData function **********************************************************/
void Plotter::setCurveData(int id, const QVector<QPointF>&data)
{
	//sets the curve data for a given ID
    curveMap[id] = data;
    refreshPixmap();
}

/************************************************* plotter clearCurve function **********************************************************/
void Plotter::clearCurve(int id)
{
	//removes the specified curve from the curve map
    curveMap.remove(id);
	refreshPixmap();
}

/************************************************* plotter minimumSizeHint function **********************************************************/
QSize Plotter::minimumSizeHint() const
{
	//specifies a widget's ideal minimum size, a layout never resizes a widget below it's minimum size
    return QSize(6 * Margin, 4 * Margin);
}

/************************************************* plotter sizeHint function **********************************************************/
QSize Plotter::sizeHint() const
{
	// specifies the widget's ideal size
    return QSize(12 * Margin, 8 * Margin);
}

/***************************************************** plotter Event handlers ****************************************************************/
//paintEvent simply copies the pixmap onto the widget at position (0,0). the drawing is taken care of in refreshPixmap()
void Plotter::paintEvent(QPaintEvent */* event */)
{
    // create a painter
	QStylePainter painter(this);
	//draw the pixmap 
    painter.drawPixmap(0, 0, pixmap);

	//if the rubber band is shown draw it on top of the plot. Draw directly on the widget (leave the pixmap untouched)
    if (rubberBandIsShown){
        painter.setPen(palette().dark().color());	//to ensure good contrast with the light background
        painter.drawRect(rubberBandRect.normalized().adjusted(0, 0, -1, -1));
		//normalized ensures that the rectangle has positif width and height (swapping coordiantes of necessary
		//adjusted reduces the size of the rectangle by one pixel to allow for the outline
    }

	//if the plotter has focus draw a focus rectangle
    if (hasFocus()){
        QStyleOptionFocusRect option;
        option.initFrom(this);								//inherit the draw options from the widget
        option.backgroundColor = palette().dark().color();	//must specify the color
        painter.drawPrimitive(QStyle::PE_FrameFocusRect, option);	//draw the rectangle according to the windows style
    }
}

/*********************************************************************************************************************************************/
//generated when the Plotter is resized. Qt always generates a resize event before a widget is shown for the first time
void Plotter::resizeEvent(QResizeEvent */* event */)
{
    //reposition the buttons. because no layout is used we have to do this manually
	int x = width() - (zoomInButton->width()
                       + zoomOutButton->width() + 10);
    zoomInButton->move(x, 5);
    zoomOutButton->move(x + zoomInButton->width() + 5, 5);
    refreshPixmap();  //redraw the pixmap at the new size
}

/*********************************************************************************************************************************************/
void Plotter::mousePressEvent(QMouseEvent *event)
{
	//start displaying a rubber band if the left mouse button is clicked inside the Plotter
    QRect rect(Margin, Margin,
               width() - 2 * Margin, height() - 2 * Margin);
    if (event->button() == Qt::LeftButton){
        if (rect.contains(event->pos())){
            rubberBandIsShown = true;
            rubberBandRect.setTopLeft(event->pos());			
            rubberBandRect.setBottomRight(event->pos());
            updateRubberBandRegion();			//schedule a paint event to paint the rubber band
            setCursor(Qt::CrossCursor);			//change the mouse to have a crosshair shape 
        }
    }
}


/*********************************************************************************************************************************************/
// when the mouse moves, if the rubber band is shown erase it and redraw it at the new position
void Plotter::mouseMoveEvent(QMouseEvent *event)
{
	if (rubberBandIsShown){
        updateRubberBandRegion();						//schedule a paint event to repaint hte area where the rubber band was
        rubberBandRect.setBottomRight(event->pos());	//recompute the rubber band rectangle
        updateRubberBandRegion();						//repaint the area where the rubber band has moved to
    }
}

/*********************************************************************************************************************************************/
//when the mouse is released zoom while defining a rubber band
void Plotter::mouseReleaseEvent(QMouseEvent *event)
{
    if ((event->button() == Qt::LeftButton)&&rubberBandIsShown){
        // erase the rubber band
		rubberBandIsShown = false;					
        updateRubberBandRegion();
		//restore the standrad cursor
        unsetCursor();
		//make sure the rectangle has positif dimensions
        QRect rect = rubberBandRect.normalized();
        if (rect.width() < 4||rect.height() < 4)
            return;
		//if the rectangle is at least 4x4, zoom in
		rect.translate(-Margin, -Margin);  //convert rubberbandrect from widget coordinates to plot coordiantes
        //compute new plotsettings and push t on top of the zoom stack
		PlotSettings prevSettings = zoomStack[curZoom];   
        PlotSettings settings;
        double dx = prevSettings.spanX()/(width() - 2 * Margin);
        double dy = prevSettings.spanY()/(height() - 2 * Margin);
		        settings.minX = prevSettings.minX + dx * rect.left();
        settings.maxX = prevSettings.minX + dx * rect.right();
        settings.minY = prevSettings.maxY - dy * rect.bottom();
        settings.maxY = prevSettings.maxY - dy * rect.top();
        settings.adjust();
        zoomStack.resize(curZoom + 1);
        zoomStack.append(settings);
		//zoom in
        zoomIn();
    }
}


/*********************************************************************************************************************************************/
//when the user presses a key and the plotter has focused the keyPressEvent is called
//implement zoom in and zoom out and scrolling
void Plotter::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()){
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    case Qt::Key_Left:
        zoomStack[curZoom].scroll(-1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Right:
        zoomStack[curZoom].scroll(+1, 0);
        refreshPixmap();
        break;
    case Qt::Key_Down:
        zoomStack[curZoom].scroll(0, -1);
        refreshPixmap();
        break;
    case Qt::Key_Up:
        zoomStack[curZoom].scroll(0, +1);
        refreshPixmap();
        break;
    default:
        QWidget::keyPressEvent(event);
	}
}


/*********************************************************************************************************************************************/
//respond to mouse wheel to scroll the plot
void Plotter::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->delta()/8;	//delta returns the distance the wheel was rotated in eighths of a degree
    int numTicks = numDegrees/15;
    if (event->orientation() == Qt::Horizontal){		//horizontal wheel on some mice
        zoomStack[curZoom].scroll(numTicks, 0);
    }else{
        zoomStack[curZoom].scroll(0, numTicks);			//vertical wheel
    }
    refreshPixmap();
}


/*********************************************************************************************************************************************/
//called from mousePressEvent(), mouseMoveEvent(), and mouseREleaseEvent() to erase or redraw the rubber band
void Plotter::updateRubberBandRegion()
{
	//four calls to update that schedule a paint event for the rectangle border lines
    QRect rect = rubberBandRect.normalized();
    update(rect.left(), rect.top(), rect.width(), 1);
    update(rect.left(), rect.top(), 1, rect.height());
    update(rect.left(), rect.bottom(), rect.width(), 1);
    update(rect.right(), rect.top(), 1, rect.height());
}

/************************************************** refreshPixmap Plotter function************************************************************/
// redraws the plot onto the off-screen pixmap and updates the display 
void Plotter::refreshPixmap()
{
    pixmap = QPixmap(size());		//rezise the pixmap to have the same size of the widget
    pixmap.fill(this, 0, 0);		//fill it with the widget's erase color
    QPainter painter(&pixmap);		//create a painter to draw on the pixmap
    painter.initFrom(this);			//set the painter style to that of the plotter (pen, background, and font)
    //perform the drawing
	drawGrid(&painter);				
    drawCurves(&painter);
	//schedule a paint event for the whole widget. The pixmap is copied to widget in the paintEvent function
    update();
}


/************************************************** drawGrid Plotter function************************************************************/
//draw the grid behind the curves and the axes
void Plotter::drawGrid(QPainter *painter)
{
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;
    //plot settings
	PlotSettings settings = zoomStack[curZoom];
    QPen quiteLight = palette().light().color().dark();
    QPen dark = palette().dark().color();

	//draw the grid's vertical lines
    for (int i = 0; i <= settings.numXTicks; ++i){
        int x = rect.left() + (i * (rect.width() - 1)/settings.numXTicks);
        double label = settings.minX + (i * settings.spanX()/settings.numXTicks);
        painter->setPen(quiteLight);
        painter->drawLine(x, rect.top(), x, rect.bottom());
        painter->setPen(dark);
        painter->drawLine(x, rect.bottom(), x, rect.bottom() + 5);
        painter->drawText(x - 50, rect.bottom() + 5, 100, 15,
                          Qt::AlignHCenter|Qt::AlignTop,
                          QString::number(label));			//draw the tick marks
    }

	//draw the grid's horizontal lines
    for (int j = 0; j <= settings.numYTicks; ++j){
        int y = rect.bottom() - (j * (rect.height() - 1) /settings.numYTicks);
        double label = settings.minY + (j * settings.spanY() /settings.numYTicks);
        painter->setPen(quiteLight);
        painter->drawLine(rect.left(), y, rect.right(), y);
        painter->setPen(dark);
        painter->drawLine(rect.left() - 5, y, rect.left(), y);
        painter->drawText(rect.left() - Margin, y - 10, Margin - 5, 20,
                          Qt::AlignRight|Qt::AlignVCenter,
                          QString::number(label));			//draw the tick marks
    }

	//draw a rectangle along the margins
    painter->drawRect(rect.adjusted(0, 0, -1, -1));
}




/************************************************** drawCurves Plotter function************************************************************/
// draw the curves on top of the grid
void Plotter::drawCurves(QPainter *painter)
{
    static const QColor colorForIds[6] ={ Qt::blue, Qt::red , Qt::green, Qt::cyan, Qt::magenta, Qt::yellow };
    PlotSettings settings = zoomStack[curZoom];
    QRect rect(Margin, Margin, width() - 2 * Margin, height() - 2 * Margin);
    if (!rect.isValid())
        return;
    painter->setClipRect(rect.adjusted(+1, +1, -1, -1));    //define the retangle that will contain the plot
    QMapIterator<int, QVector<QPointF> > i(curveMap);		//iterate for each curve and over its constituent points QPointF
    while (i.hasNext()){
        i.next();
        int id = i.key();  //the curve id
        const QVector<QPointF>&data = i.value();	//the curve data as a veector of QPointF
        QPolygonF polyline(data.count());			//create a polyline variable
		//convert the QPointF coordiantes from plotter coordiantes to widget coordinates and store them in the polyline varaible
        for (int j = 0; j < data.count(); ++j){
            double dx = data[j].x() - settings.minX;
            double dy = data[j].y() - settings.minY;
            double x = rect.left() + (dx * (rect.width() - 1)
                                         /settings.spanX());
            double y = rect.bottom() - (dy * (rect.height() - 1)
                                           /settings.spanY());
            polyline[j] = QPointF(x, y);
        }
		//set the pen color for the curve       
		painter->setPen(colorForIds[uint(id) % 6]);
		//draw a line that goes through all the curve's points
        painter->drawPolyline(polyline);
    }
}




/************************************************** PlotSettings class function ************************************************************/
//constructor
PlotSettings::PlotSettings()
{
    minX = 0.0;
    maxX = 10.0;
    numXTicks = 5;
    minY = 0.0;
    maxY = 10.0;
    numYTicks = 5;
}

//constructor
PlotSettings::PlotSettings(int minX, int maxX,int minY, int maxY)
{
    this->minX = minX;
    this->maxX = maxX;
    numXTicks = 5;
    this->minY = minY;
    this->maxY = maxY;
    numYTicks = 5;
}



//scroll function used in Plotter::keyPressedEvent()
void PlotSettings::scroll(int dx, int dy)
{
    double stepX = spanX()/numXTicks;
    minX += dx * stepX;
    maxX += dx * stepX;
    double stepY = spanY()/numYTicks;
    minY += dy * stepY;
    maxY += dy * stepY;
}

//called from mouseReleaseEvent() round the min and max to 'nice' values and determine the number of ticks appropriate for each axis
void PlotSettings::adjust()
{
    adjustAxis(minX, maxX, numXTicks);
    adjustAxis(minY, maxY, numYTicks);
}

//adjut the axis 
void PlotSettings::adjustAxis(double&min, double&max,
                              int&numTicks)
{
    const int MinTicks = 4;
    double grossStep = (max - min)/MinTicks;
    double step = pow(10.0, floor(log10(grossStep)));
    if (5 * step < grossStep){
        step *= 5;
    }else if (2 * step < grossStep){
        step *= 2;
    }
    numTicks = int(ceil(max/step) - floor(min/step));
    if (numTicks < MinTicks)
        numTicks = MinTicks;
    min = floor(min/step) * step;
    max = ceil(max/step) * step;
}