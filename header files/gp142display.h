#pragma once
#ifndef __GP142_DISPLAY_H__
#define __GP142_DISPLAY_H__

#include <cassert>
#include <string>
using namespace std;

// GP142Display: Display that's been retrofitted to work with the
// C-centric GP142 libraries.

// Written by Shuichi Koga
// Sun May 16, 1999

// getDimensions by Richard Dunn
// April, 2000

// GP142Display Model: Under GP142, the center of the screen is considered
// to be position (0,0), so that is preserved under this display (see the
// GP142 docs for details).

// Colors
enum GP142Color
{
	Black, White, Red, Green, Blue, Yellow, Magenta, Cyan,
	Purple, NavyBlue, DustyPlum, IceBlue, Turquoise, Orange,
	Brown, Pink, Chalk, Gold, Peach, ForestGreen, SeaGreen,
	Olive, MedGray, LightGray, MaxColors
};

// Events that this display can understand
enum GP142Event
{
	Mouse, Key, Periodic, Quit
};

// Control over how quickly periodic events occur
enum GP142Periodic
{
	Halt, SingleStep, Run, SendQuit
};

// Forward declaration for the button class.  
// We need the button class in order to 
// implement the "wait" function.  J. Goshi, 5/2000
class GP142Button;

// The actual display abstraction. Note that we do not support all of 
// the methods that are given as part of GP142.

class GP142Display
{
public:
	GP142Display();
	~GP142Display();

	// Returns the display width
	int getWidth();

	// Returns the display height
	int getHeight();

	// Returns the maximum possible width
	// (More precisely, the maximum possible x coordinate)
	int getMaxWidth();

	// Returns the maximum possible height
	// (More precisely, the maximum possible y coordinate)
	int getMaxHeight();

	// Clears the screen to the specified color
	void clear(GP142Color color = Black);

	// Pauses for a number of milliseconds
	void pause(int time);

	// Waits until the user clicks on the specified button
	void wait(GP142Button *button);
	// The following methods allow for control over input that one can get

	// Get the next event
	GP142Event getNextEvent(int& mouseX, int& mouseY, char& keyPress);

	// Sets whether or not to animate
	void setAnimation(GP142Periodic newPeriod);

	// Flushes the output queue of things to write out.
	void flush();


	// The following methods allow for writing characters to the screen.
	// fontSize unit is "points", approx == average pixel width.

	// Write a character at a given x and y coordinate
	void write(int x, int y, char c, GP142Color color = White, int fontSize = 8);

	// Write a string at a given x and y coordinate
	// if the string does not fit on the line, it will wrap around the screen
	void write(int x, int y, const char*   s, GP142Color color = White, int fontSize = 8);
	void write(int x, int y, const string& s, GP142Color color = White, int fontSize = 8);

	// Write an integer at a given x and y coordinate
	void write(int x, int y, int i, GP142Color color = White, int fontSize = 8);

	// Get the height and/or width, in pixels, needed to print string s 
	// at a given font size.  fontSize is roughly the width of an average 
	// character in pixels.  E.g., 10-14 is a typical size for text on screen.
	void getStringDimensions(const char*   s, int fontSize, int &height, int &width);
	void getStringDimensions(const string& s, int fontSize, int &height, int &width);
	int  getStringHeight(const char*   s, int fontSize);
	int  getStringHeight(const string& s, int fontSize);
	int  getStringWidth(const char*   s, int fontSize);
	int  getStringWidth(const string& s, int fontSize);

	// The following methods allow for pixel manipulation.

	// Draws a pixel
	void drawPixel(int x, int y, GP142Color color = White);

	// Draws a line
	void drawLine(int x1, int y1, int x2, int y2,
		GP142Color color = White, int thickness = 1);

	// Draws a rectangle
	void drawRectangle(int x1, int y1, int x2, int y2,
		GP142Color color = White, int thickness = 1);

	// Draws a triangle
	void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3,
		GP142Color color = White, int thickness = 1);

	// Draws an oval
	void drawOval(int x1, int y1, int x2, int y2,
		GP142Color color = White, int thickness = 1);

	// Draws a circle
	void drawCircle(int x, int y, int radius, GP142Color color = White);


private:
	int width, height;              // width and height of the screen
	static bool initialized;        // never make two GP142Display's
	void errorMsg(const char* msg); // for internally generated error msgs.
};


#endif