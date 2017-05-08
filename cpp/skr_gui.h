/************************
 * SKRlib GUI functions *
 *                      *
 * Begun 2017/04/17     *
 ************************/
#ifndef SKR_GUI
#define SKR_GUI 2 // I'm a GUI, level 2, for SKR namespace.
#ifndef SKR_DISP
#define SKR_DISP // I'm a valid skr namespace display library

#include <iostream> // cout, etc.
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

void diefrom(const string lastwords,int glevel = 0) {
	//if (glevel > 1) { // We are GUI level 2
	//	<show message dialog>
	//} else {
		std::cout << lastwords; // In a GUI, we might put this in an error box. Here, we'll just say it to stdout.
	//}
	//endwin();
	exit(-1); // and exit abnormally.
}




#endif // skr_disp in use
#endif // skr_gui established
