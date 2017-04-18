/**********************************************
 * Command translation for non-curses modules *
 **********************************************/
#ifndef NOCURSE_H
#define NOCURSE_H // prevent duplicate load
#ifndef SKR_DISP
#define SKR_DISP // I'm a valid skr namespace display library
#include <cstdlib> // I'll be using this
using std::exit;
#include <string> // I'll be using strings
using std::string;
#include <sstream> // I'll be using string streams
#ifdef REQ_TIMESTAMP // If the main needs a timestamper...
namespace skr {
	string timestamp(void);
}
string skr::timestamp();
#endif

void dNl(void) { std::cout << "\n"; } // display a newline

int getNum() { // If we had curses, we could take input. We can't.
	// But the includer may mistakenly ask for it, and this whole header is about graceful failure of that sort.
	std::cout << "Function getNum() unavailable without curses.";
	return 9; // I hope you like 9.
}

float getFloat(float a) { // Same thing, different type.
	std::cout << "Function getFloat() unavailable without curses.";
	return a; // This one takes a default
}

#ifdef CANUSE_DOLLAR // Did we load a dollar library?
USDollar getUSD(USDollar a) { // Too bad we still can't get user input without curses.
	getFloat(0.00);
	return a; // This one has a default option
}
#endif

// Our standard colorSayCharacter
void cSayC(int color, const string say) {
	std::cout << say; // ignoring the color
}

// Our standard colorSayMenuitem
void cSayM(int colork, int color, const string say, bool shft) {
	char key = say[0]; // ignoring the color, just say it
	std::cout << "[" << ( shft ? "Shift-" : "" ) << key << "]" << say.substr(1);
}

// Our standard colorSayNumeric+String
void cSay2(int c1, int c2,int c3, const string c4) {
	std::cout << c2 << c4; // ignoring the color
}

// Our standard colorSay3Strings
void cSay3(int c1, const string c2,int c3, const string c4,int c5, const string c6) {
	std::cout << c2 << c4 << c6; // Sensing a pattern?
}

// Our standard colorSayInt
void cSayI(int color, int say) {
	std::cout << say; // Lather, rinse, repeat
}

// Our standard colorSayFloat
void cSayF(int color, float say) {
	std::cout << say; // Does the same with a float
}

#ifdef CANUSE_DOLLAR // I can has dollars?
// Our standard colorSayDollar
void cSayD(int color, USDollar say) {
	if (say.c() < 10) {
		std::cout << "$" << say.d() << ".0" << say.c();
//	} else if ((say.c() % 10) == 0) { // probably don't need this
//		std::cout << "$" << say.d() << "." << say.c() << "0";
	} else {
		std::cout << "$" << say.d() << "." << say.c();
	}
} // cSayD
#endif

void initCurses() { return; } // Nothing to initialize, but we'll pretend to.
void move(int,int) { return; } // Can't move the cursor, but OK.
void echo(void) {return; } // Can't turn echo on...
void noecho(void) { return; } // ...or off.
void doupdate(void) { return; } // cout updates fine without this.
int stdscr = 0; // This is needed, though I can't recall why.
void getyx(int,int,int) { return; } // Can't store the cursor position.
void endwin() { return; } // Didn't init the window, so can't end it.
char getch(void) { return 'q'; } // Can't get a character.
void clrtoeol(void) { return; } // Can't clear to end of the line.
void clrtobot(void) { return; } // Can't clear to bottom of screen.
void getnstr(char*,int) { return; } // Can't do whatever this does.
void reChar(char k) { std::cout << k; return; } // This is important, but I forget why.

void diefrom(const string lastwords) { // When a program can't continue, it likes to let the user know why.
			endwin();
			std::cout << lastwords; // In a GUI, we might put this in an error box. Here, we'll just say it to stdout.
			exit(-1); // and exit abnormally.
}

#ifdef REQ_TIMESTAMP // Do we have to have a timestamp?
void goodExit(int verbosity) { // Exit properly
#ifdef DBOUT
	if(debug > 7) { dbfile << skr::timestamp() << "<=\n"; dbfile.flush(); } // log the exit
#endif // no DBOUT
	if(verbosity > 0) cSayC(7,skr::timestamp()); // Say the timestamp (in curses, this would go on the right of the screen)
}
#endif // no timestamping

string my_dtoa(double d) { // Make a string from a double
	std::ostringstream out; // TODO fix this.
	out << d;
	return out.str();
}

#endif // skr_disp in use
#endif // End of non-curses functions.
