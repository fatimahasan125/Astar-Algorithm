/*
* gp142display.cpp - C++ wrapper around gp142 graphics package
*
* Shuichi Koga and a cast of thousands.
*
* CSE 143 Spring 2000, HW #4
*/

#include "gp142.h" 
#include "gp142display.h"
#ifdef X11R6
#include <unistd.h> /* for usleep */
#endif
#include <iostream>
using namespace std;

bool GP142Display::initialized = false;

GP142Display::GP142Display()
	: width(GP142_XMAX * 2), height(GP142_YMAX * 2)
{
	if (!initialized)
	{
		GP142_open();
		GP142_logging(LOG_OFF);
		initialized = true; //MD 07/21/2000
	}
	else
	{
		errorMsg("Error: Attempted to instantiate multiple GP142 displays");
		exit(-1);
	}
}

GP142Display::~GP142Display()
{
	GP142_close();
}

void GP142Display::errorMsg(const char* msg) {
	// Not sure why Win is different; old bug in cout, since fixed?
	// I'll leave it, in case not.  WLR 4/00
#ifdef WINDOWS
	//char result[80];
	//GP142_gets(msg, result);
	printf(msg); //be bold; hope it's fixed.  WLR 5/4/00
#else
	cout << msg << endl;
#endif
}

// Returns the display width
int GP142Display::getWidth()
{
	return width;
}

// Returns the display height
int GP142Display::getHeight()
{
	return height;
}

// Returns the maximum possible width (maximum possible x coordinate)
int GP142Display::getMaxWidth()
{
	return width / 2;
}

// Returns the maximum possible height (maximum possible y coordinate)
int GP142Display::getMaxHeight()
{
	return height / 2;
}



// Clears the screen to the specified color
void GP142Display::clear(GP142Color color)
{
	GP142_flush();
	GP142_clear();
	drawRectangle(-width / 2, -height / 2, width / 2, height / 2, color, 0);
}

// Pauses for a number of milliseconds
// (Does NOTHING on Mac.)
void GP142Display::pause(int time)
{
#ifdef WINDOWS
	Sleep(time);
#elif defined(X11R6)
	usleep((unsigned long)time * 1000);
#elif defined(MACINTOSH)
	errorMsg("Sorry, pause is not implemented on Mac.\n");
	time = time;    // avoid warning about unreferenced var.
#endif
}

// Waits until the user clicks on the specified button
// requires gp142button
void GP142Display::wait(GP142Button *button)
{
	/* Variables for event processing */
	int mouseX, mouseY;   // The x and y coordinates that the user clicked on
	char keyPress;        // Holds the character that the user pressed

						  /* Keep looping until the user clicks on the specified button */
	while (true) {
		GP142Event event = getNextEvent(mouseX, mouseY, keyPress);
		if (event == Quit) {
			exit(0);
		}
	}
}

// Event handling

// handles arbitrary events
// Get the next event
GP142Event GP142Display::getNextEvent(int& mouseX, int& mouseY, char& keyPress)
{
	int result = GP142_await_event(&mouseX, &mouseY, &keyPress);
	GP142Event event;

	switch (result)
	{
	case GP142_MOUSE:   event = Mouse;  break;
	case GP142_KBD:     event = Key;    break;
	case GP142_PERIODIC:event = Periodic; break;
	case GP142_QUIT:    event = Quit;   break;
	default:
		errorMsg("Error: unknown GP142 Event Code");
		event = Quit;
	}

	return event;
}

// Sets whether or not to animate
void GP142Display::setAnimation(GP142Periodic newPeriod)
{
	int set;
	switch (newPeriod)
	{
	case Halt:
		set = ANI_HALT;
		break;
	case SingleStep:
		set = ANI_SINGLE_STEP;
		break;
	case Run:
		set = ANI_RUN;
		break;
	case SendQuit:
		set = ANI_QUIT;
		break;
	default:
		break;
	}

	GP142_animate(set);
}

// Flushes the output queue of things to write out.
void GP142Display::flush()
{
	GP142_flush();
}



// The following methods allow for writing characters to the screen.

// Write a character at a given x and y coordinate
void GP142Display::write(int x, int y, char c, GP142Color color, int fontSize)
{
	GP142_printfXY((int)color, x, y, fontSize, "%c", c);
}

// Write a string at a given x and y coordinate
// if the string does not fit on the line, it will wrap around the screen
void GP142Display::write(int x, int y, const char *s, GP142Color color, int fontSize)
{
	GP142_printfXY((int)color, x, y, fontSize, "%s", s);
}

void GP142Display::write(int x, int y, const string& s, GP142Color color, int fontSize)
{
	write(x, y, s.c_str(), color, fontSize);
}

// Write an integer at a given x and y coordinate
void GP142Display::write(int x, int y, int i, GP142Color color, int fontSize)
{
	GP142_printfXY((int)color, x, y, fontSize, "%d", i);
}


// Get the dimensions of a printed string
void GP142Display::getStringDimensions(const char *s, int fontSize, int &height, int &width) {
	int tHeight, tWidth;

	GP142_getDimensions(fontSize, &tHeight, &tWidth, "%s", s);

	height = tHeight;
	width = tWidth;
}

void GP142Display::getStringDimensions(const string& s, int fontSize, int &height, int &width) {
	getStringDimensions(s.c_str(), fontSize, height, width);
}

int GP142Display::getStringHeight(const char *s, int fontSize) {
	int tHeight, tWidth;

	GP142_getDimensions(fontSize, &tHeight, &tWidth, "%s", s);

	return tHeight;
}

int GP142Display::getStringHeight(const string& s, int fontSize) {
	return getStringHeight(s.c_str(), fontSize);
}

int GP142Display::getStringWidth(const char *s, int fontSize) {
	int tHeight, tWidth;

	GP142_getDimensions(fontSize, &tHeight, &tWidth, "%s", s);

	return tWidth;
}

int GP142Display::getStringWidth(const string& s, int fontSize) {
	return getStringWidth(s.c_str(), fontSize);
}

// The following methods allow for pixel manipulation.

// Draws a pixel
void GP142Display::drawPixel(int x, int y, GP142Color color)
{
	GP142_pixelXY((int)color, x, y);
}

// Draws a line
void GP142Display::drawLine(int x1, int y1, int x2, int y2, GP142Color color,
	int thickness)
{
	GP142_lineXY((int)color, x1, y1, x2, y2, thickness);
}

// Draws a rectangle
void GP142Display::drawRectangle(int x1, int y1, int x2, int y2, GP142Color color,
	int thickness)
{
	GP142_rectangleXY((int)color, x1, y1, x2, y2, thickness);
}

// Draws a triangle
void GP142Display::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
	GP142Color color, int thickness)
{
	GP142_triangleXY((int)color, x1, y1, x2, y2, x3, y3, thickness);
}

// Draws an oval
void GP142Display::drawOval(int x1, int y1, int x2, int y2,
	GP142Color color, int thickness)
{
	GP142_ovalXY((int)color, x1, y1, x2, y2, thickness);
}

// Draws a circle
void GP142Display::drawCircle(int x, int y, int radius, GP142Color color)
{
	GP142_circleXY((int)color, x, y, radius);
}