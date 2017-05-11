/******************************
  Common Functions by Skreyola
 ******************************/
#ifndef SKRF_H
#define SKRF_H // prevent duplicate load

#include <iostream> // cout, etc.
#ifndef SKR_CURSESOK // Not using curses terminal library
	#include "nocurse.h"
#endif
#include <string> // Use strings
using std::string;
#include <cstring> // and cstrings
#include <algorithm> // and reverse
using std::reverse;
#ifdef INCLUDE3RDPARTY // Are we limited by licenses?
#include "tpf.h" // If not, we can use functions from unkown or restrictive licenses
#else // Not using 3rd party functions
#include "mysadtry.h" // Otherwise, we'll have to use the ones I tried to replicate on my own
#endif // End extra functions (3rd party or my sad attempts)
#include "parrot.h" // File reader

// In the skr namespace...
namespace skr {
	string timestamp(void);
	int fun_err = 0; // Function error type
	int errorlevel = 0; // Error level
	int debug = 0; // Debug level
}
// string skr::timestamp(void);
void diefrom(const string);
unsigned long atoul(const string);
string cleanStr(string);
#include "skr_sql.h" // These prototypes might go to SQL functions. Better ensure they're loaded.
int findC(const char,const string);
string sanitecho(string, const string);
string sanitizeInput(const string, const char);
string trimTrailing(string);

unsigned long atoul(const string a) { // I'm tired of having to remember these extra args that I always use with this.
	return strtoul(a.c_str(),NULL,10);
} // atoul()

unsigned long atoul(const string a, const unsigned int b) {
	return strtoul(a.c_str(),NULL,b); // In case I want to do it differently.
} // atoul()

// Encrypt a character with another character...
char charcrypt(char a,char b) { // Originally for DORA
	char c = '?';
	string vc = "1!2@34$56^789(0)QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm`";
	int l = vc.length();
	int pa = findC(a,vc);
	if(pa == -1) pa = (int)a % l;
	int pb = findC(b,vc);
	if(pb == -1) pb = (int)b % l;
	pb = (pa + pb) % l;
	c = vc[pb];
	return c;
} // Not sure when this will be useful, but I must've used it more than once...

// Does the string contain bad characters (;%'"# and maybe space)?
bool checkText(int b, std::string a, bool allowspace) {
	int l = a.length();
	if(l>b) return false;
	int c = 0;
	for(int i=0;i<l;i++){
		if(a[i] == ' ' && !allowspace) c++;
		if(a[i] == ';') c++;
		if(a[i] == '%') c++;
		if(a[i] == '\'') c++;
		if(a[i] == '#') c++;
		if(a[i] == '\"') c++;
	}
//	c++; // Function not functional.
	if (c == 0) { return true; }
	return false;
}

// Lazy overload. No length required.
bool checkText(std::string a) { return checkText(a.length(),a,true); }
// Same
bool checkText(int b, std::string a) { return checkText(b, a, true); }
// Same function, but with a character array. Can't be sure what a user will send, can we?
bool checkText(int b, char a[1024]) {
	int c = 0;
	for (int i = 0;i < b;i++) {
		if (a[i] < ' ' && a[i] != '\0') {
//			cSayC(1,"^-");
			c++; i = b;
		} else if (a[i] > '~') {
//			cSayC(1,"^+");
			c++; i = b;
		} else {
			if (a[i] == '\0') i = b;
//			cSayC(7,".");
		}
	}
	if (c == 0) {
	  if (a[0] != '\0')	{
			return true;
		}
	}
	return false;
}

// Does username contain only valid characters?
bool checkUname(const string a) {
	if (checkText(21,a)) {
		string b = sanitecho(a,"0123456789-_QWERTYUIOPqwertyuiopASDFGHJKLasdfghjklZXCVBNMzxcvbnm");
		if (a != b) {
			if (skr::debug > 4) std::cout << a << " =/= " << b << "\n";
			return false;
		}
	}
	return true;
}

// URL validator - brute force
int checkURL(unsigned int b, string a) {
	if (a.length() < 8 || a.length() > b) return -1;
	int c = 0; int len = a.length();
	char d[8] = "http://";
	char e[9] = "https://";
	if ((a[0] != d[0]) ||	(a[1] != d[1]) ||	(a[2] != d[2]) ||	(a[3] != d[3])) {
		if (skr::debug > 7) std::cout << "checkURL: http not in string<br />";
		return 1;
	}
	if (a[4] == 's') {
	 	if ((a[5] != e[5]) || (a[6] != e[6]) || (a[7] != e[7])) {
			if (skr::debug > 7) std::cout << "checkURL: https, but no ://<br />";
			return 2;
		}
	} else if (a[4] == ':') {
		if ((a[5] != d[5]) || (a[6] != d[6])) {
//			std::cout << "In: " << a[6] << " " << d[6] << " ";
			if (skr::debug > 7) std::cout << "checkURL: http:, but no //<br />";
			return 3;
		}	
	} else {
		if (skr::debug > 7) std::cout << "checkURL: http, but no ://<br />";
		return 4;
	}
	if ((a[11] == ':') || (a[12] == ':') || (a[13] == ':') || (a[14] == ':')) {
		if (skr::debug > 7) std::cout << "checkURL: possible doubling of http://<br />";
		return 5;
	}
	 // End htt*:// check
	for (int i = 0;i < len;i++) {
//		std::cout << "*";
		if (a[i] <= ' ' && a[i] != '\0') {
			if (skr::debug > 7) std::cout << "checkURL: Space Found!<br />";
			c++; i = b;
		} else if (a[i] > '~') {
			if (skr::debug > 7) std::cout << "checkURL: High ASCII Character Found!<br />";
			c++; i = b;
		} else if (strchr("\"\'<>{}[]`^|\\",a[i]) != NULL) {
			if (skr::debug > 7) std::cout << "checkURL: Forbidden URL Character Found!<br />";
			c++; i = b;
		} else {
			if (a[i] == '\0') {
				if (skr::debug > 7) std::cout << "checkURL: Unexpected NULL Encountered!<br />";
			 len = i;
			 i = b;
			}
		}
	}
	if (c == 0) {
//		std::cout << "Len: " << len << "\n<BR>";
	  if (len > 12)	{
			return 0;
		}
		return 7;
	}
	if(skr::debug > 7) {
#ifdef DBOUT // Using debug file
	dbfile << "checkURL: " << c << " errors!<br />";
#else // Not using debug file
	std::cout << "checkURL: " << c << " errors!<br />";
#endif
	}
	return 6;
}

// Email validator component
int countEaches(const string a) {
	int b = 0; int c = a.length();
	for (int i=0;i<c;i++) {
		if (a[i] == '@') b++;
	}
	return b;
}

// Another encrypter, limited to 16 characters (usernames/passwords?)
string encrypt16(const string b) {
	string a = b;
	while(a.length() < 15) {
		if (a.length() %2 == 1) {
			a += (char) 75 +  a.length();
		} else {
			a += (char) 117 - a.length();
		}
	}
	string c = a;
//	dbfile << c << " " << a << "\n";
	a[0] = charcrypt(b[0],'d');
	for (int i = 1; i < 16; i++) {
		a[i] = charcrypt(a[i-1],c[16-i]);
	}
#ifdef DBOUT // Using debug file
	if(debug > 9) dbfile << c << " " << a << "\n";
#endif
	return a;
}
// */

/*
int findC(const char suspect,const string valid) {
	int l = valid.length();
	for(int i=0;i<l;i++) {
		if(suspect == valid[i]) return i;
	}
	return -1;
}
// */

// Our standard character finder
int findC(const char suspect,const string valid) {
	if(valid.find(suspect) == string::npos) return -1;
	return valid.find(suspect);
}

// Password hasher.
string hashpass(string a) {
	string hash = sanitizeInput(encrypt16(a),'p');
	return hash;
}

// Numeric to hex string
string hexl(unsigned long a) {
	string st;
	char c;
	int i=0;
	if (a == 0) {
		st = "0";
	}
	while (a > 0) {
		switch (a % 16) {
			case 0:
				c = '0'; break;
			case 1:
				c = '1'; break;
			case 2:
				c = '2'; break;
			case 3:
				c = '3'; break;
			case 4:
				c = '4'; break;
			case 5:
				c = '5'; break;
			case 6:
				c = '6'; break;
			case 7:
				c = '7'; break;
			case 8:
				c = '8'; break;
			case 9:
				c = '9'; break;
			case 10:
				c = 'A'; break;
			case 11:
				c = 'B'; break;
			case 12:
				c = 'C'; break;
			case 13:
				c = 'D'; break;
			case 14:
				c = 'E'; break;
			case 15:
				c = 'F'; break;
		}
		a -= (a % 16);
		a /= 16;
		st += c;
		i++;
	}
	return st; // Might be reversed? Check before using this.
}

// Substitute '~6' for a leading '0x' for database sanitization.
// Might be unneeded with parameterizing,
// but I'll leave it here in case it's wanted.
string kill0x(string in) {
	string out = "";
	int inlen = in.length();
	out.append(in,0,1);
	for(int i=1;i<inlen;i++) {
		if(in[i] == 'x' && in[i - 1] == '0') {
			out += "~6";
		} else {
			out.append(in,i,1);
		}
	}
	return out;
}

// ask for a number, then take it in. Lazy function.
unsigned long numIn(string b = "Enter a number:") {
	unsigned long a = 0;
#ifdef SKR_CURSESOK // I can has curses library?
	cSayC(5,b);
	std::cout << " ";
	a = getNum();
#else
	a = -1;
#endif
 	return a;
}

// pad a hex number with 0x and 0's
string padHex(const string a, const unsigned int w) {
	if(a.length() >= w) {
		return a;
	} else {
		string b;
		b.reserve(w);
		b.assign(w - a.length(),'0');
		b += a;
		b.insert(0,"0x");
		return b;
	}
}

// Pad an integer for display.
string padint(const long int a, const unsigned int w) {
	string c = my_itoa(a,10);
	if(c.length() >= w) {
		return c;
	} else {
		string b;
		b.reserve(w);
		b.assign(w - c.length(),'0');
		if(a < 0) {
			b.insert(0,"-");
			c.erase(0,1);
		}
		b += c;
		return b;
	}
}

// automatically determine a number's display width.
unsigned int placeWidth(long int a) {
	unsigned int w = 1;
	if(a < 0) {
		w++; // space for minus sign
		a *= -1;
	}
	while (a > 10) {
		w++;
		a /= 10;
	}
	return w;
}

// Generalized sanitizer. You provide the valid characters.
string sanitecho(string in, const string valid) { // Returns valid characters
	string out = "";
	int inlen = in.length();
	for(int i=0;i<inlen;i++) {
		if(findC(in[i],valid) != -1) {
			out.append(in,i,1);
		} else {
			if(skr::debug > 4) {
#ifdef SKR_CURSESOK // I can has curses library?
				cSayC(1,"!"); // Drop the character
#else
				std::cout << "!";
#endif
			}
		}
	}
	//nothing yet
	return out;
}

// Generalized sanitizer. You provide the forbidden characters.
string saniterase(string in,const string bad) { // Returns not-bad characters
	string out = "";
	int inlen = in.length();
	for(int i=0;i<inlen;i++) {
		if(findC(in[i],bad) == -1) {
			out.append(in,i,1);
		} else {
			if(skr::debug > 4) {
#ifdef SKR_CURSESOK // I can has curses library?
				cSayC(1,"!"); // Drop the character
#else
				std::cout << "!";
#endif
			}
		}
	}
	//nothing yet
	return out;
}

// Generalized sanitization of input. See further comments for options.
string sanitizeInput(const string instr, const char t){
	string os;
	int l = instr.length();
	switch(t) {
		case '_':{ // remove spaces
			for(int i=0;i<l;i++){
				if(instr[i] == ' '){
//					os += "_";
				} else {
					os += instr[i];
				}
			} break;
		case '#':{ // telephone #s
			int digits = 10; // standard US phone #
#ifdef PHONE_LENGTH
			digits = PHONE_LENGTH; // international numbers may use this flag to set a new maximum
#endif
			os.assign(sanitecho(instr,"0123456789")); // strip off ()-., etc.
			int b = os.length();
			if (b > digits && os.substr(0,1) == "1") {
				os.assign(os.substr(1));
				b--;
			}
			if (b != digits) { // too long/short
				return "";
			}
#ifndef PHONE_LENGTH // This is a US number
			os.assign(sanitecho(instr,"0123456789")); // strip off bad characters
			os.assign("(" + os.substr(0,3) + ")" + os.substr(3,3) + "-" + os.substr(6));
#endif
			} break;
		case 'x':{ // xml validity
			for(int i=0;i<l;i++){
				if(false){
				} else if (instr[i] == '<'){
					os += "&lt;";
				} else if (instr[i] == '>'){
					os += "&gt;";
				} else if (instr[i] == '\''){
					os += "&apos;";
				} else if (instr[i] == '&'){
					os += "&amp;";
				} else {
					os += instr[i];
				}
			}
		} break;
		case 's':{ // SQL
			for(int i=0;i<l;i++){
				if(instr[i] == '~'){
					os += "~0";
				} else if(i > 0 && instr[i] == 'x') {
					if(instr[i-1] == '0') {
						os += "~6";
					} else {
						os += "x";
					}
				} else if (instr[i] == '-' && i<l) {
					if (instr[i+1] == '-') {
						os += "~5"; i++;
					} else {
						os += "-";
					}
				} else if (instr[i] == '='){
					os += "~4";
				} else if (instr[i] == ';'){
					os += "~2";
				} else if (instr[i] == '\''){
					os += "~1";
				} else if (instr[i] == '#'){
					os += "~7";
				} else if (instr[i] == '*'){
					os += "~8";
				} else if (instr[i] == '\"'){
					os += "~3";
				} else if (instr[i] == '&'){
					os += "~9";
				} else {
						os += instr[i];
				}
			}
		} break;
		case 'u': // username
		case 'e':{ // e-mail address
			os.assign(sanitecho(instr,"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@.-_+"));
/*			for(int i=0;i<l;i++){
				if(findC(instr[i],"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789@.-_+") != -1) {
					os += instr[i]; // Keep the character
				} else {
					if(debug > 4) cSayC(1,"!"); // Drop the character
				}
			}
*/
		} break;
		case 'p': { // password
			os.assign(sanitecho(instr,"1!2@34$5%6^789(0)QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm`"));
			os.assign(kill0x(os));
/*			for(int i=0;i<l;i++){
				if(findC(instr[i],"1!2@34$5%6^789(0)QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm`") != -1) {
					if(i > 0 && instr[i] == 'x') {
						if(instr[i-1] == '0') {
							os += "~6";
						} else {
							os += instr[i]; // Keep the character
						}
					} else {
						os += instr[i]; // Keep the character
					}
				} else { // How'd you get past encrypt16()?
					if(debug > 4) cSayC(1,"#"); // Drop the character
					os += "?";
				}
			}
*/
		} break;
		case 't':{ // token (URL validity + SQL safe)
			int b = 1;
			string val = "1234567890QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm";
			os.assign(sanitecho(instr,val));
			os.assign(kill0x(os));
			b = os.length();
			while (b < 8) {
				if((8 - b) % 2 == 0) {
					os += val[val.length() - (8 + b)];
				} else {
					os += val[val.length() - b];
				}
				b++;
			}
		} break;
		case 'v':{ // verification code (URL validity)
			int b = 1;
			string val = "1!2@34$5%6^789(0)QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm`";
			os.assign(sanitecho(instr,val));
			os.assign(kill0x(os));
			b = os.length();
			while (b < 8) {
				if((8 - b) % 2 == 0) {
					os += val[val.length() - (8 + b)];
				} else {
					os += val[val.length() - b];
				}
				b++;
			}
/*			for(int i=0;i<l;i++){
				if(findC(instr[i],"\"\'<>{}[]`^|\\") != -1) {
					os += my_itoa(b); b++; // Replace the character
					if(debug > 9) cSayC(1,"!");
				} else {
					os += instr[i]; // Keep the character
				}
			} // */
		} break;
		default:{ // Must be one of preceding types
			std::cerr << "Error: sanitizeText() called with invalid type.";
			} break;
		}
	}
	return os;
}

// Timestamp provider.
string skr::timestamp(void) {
	struct tm *x;
	time_t y;
	time(&y);
	x = localtime(&y);
	string s;
	if( x->tm_year < 10 ) s+= "0"; s += my_itoa(x->tm_year-100,10) + "-";
	if( x->tm_mon < 10 ) s+= "0"; s += my_itoa(x->tm_mon+1,10) + "-";
	if( x->tm_mday < 10 ) s+= "0"; s += my_itoa(x->tm_mday,10) + " ";
	if( x->tm_hour < 10 ) s+= "0"; s += my_itoa(x->tm_hour,10) + ":";
	if( x->tm_min < 10 ) s+= "0"; s += my_itoa(x->tm_min,10) + ":";
	if( x->tm_sec < 10 ) s+= "0"; s += my_itoa(x->tm_sec,10) + " - ";
	return s;
}

// Deletes space, tab, CR, and LF.
string trimLeading(string in) { // Called trimLeading, but appears to delete all spaces.
	string badc = " \t\r\n";
	string out = "";
	int inlen = in.length();
	int pos = -1;
	for (int i=0;i<inlen;i++) {
		while (pos < 0) {
			pos = findC(in[i],badc);
			if(pos == -1) {
				out.append(in,i,1);
			}
		}
		out.append(in,i,1);
	}
	return out;
}

string trimOuter(string in) {
	in.assign(trimLeading(in));
	in.assign(trimTrailing(in));
	return in;
}

string trimTrailing(string in) {
	std::reverse(in.begin(), in.end());
	in.assign(trimLeading(in));
	std::reverse(in.begin(), in.end());
	return in;
}

// Deletes space, tab, CR, and LF.
string trimWhite(string in) {
	string badc = " \t\r\n";
	string out = "";
	int inlen = in.length();
	int pos = 0;
	// TODO: Can we use our generalized sanitizer for this?
	for (int i=0;i<inlen;i++) {
		pos = findC(in[i],badc);
		if(pos == -1) {
			out.append(in,i,1);
		}
	}
	return out;
}

// Reverses hexl. Turns a hex string into a numeric.
unsigned long unhexl(const string a) {
	unsigned long b = 0; int c = 0; unsigned long d = 1;
	int i = 0; int l = a.length();
	if(l == 0 || a[0] == '0') return 0;
	while(i < l) {
		switch (a[i]) {
			case '0':
				c = 0; break;
			case '1':
				c = 1; break;
			case '2':
				c = 2; break;
			case '3':
				c = 3; break;
			case '4':
				c = 4; break;
			case '5':
				c = 5; break;
			case '6':
				c = 6; break;
			case '7':
				c = 7; break;
			case '8':
				c = 8; break;
			case '9':
				c = 9; break;
			case 'a':
			case 'A':
				c = 10; break;
			case 'b':
			case 'B':
				c = 11; break;
			case 'c':
			case 'C':
				c = 12; break;
			case 'd':
			case 'D':
				c = 13; break;
			case 'e':
			case 'E':
				c = 14; break;
			case 'f':
			case 'F':
				c = 15; break;
			default:
				c = 0; break;
		}
		b += (c * d);
		d *= 16;
		i++;
	}
	return b;
}

// shorthand for sanitizeInput(=>SQL)
string mkSQL(const string given) {
	return sanitizeInput(given,'s');
}

// Unsanitize SQL-safed strings, for display.
string unSQL(const string stored){
#ifdef DBOUT // Using debug file
	if(skr::debug > 4) dbfile << "uSQL "; dbfile.flush();
#endif // End using debug file
	string out;
	int l = stored.length();
	for(int i=0;i<l;i++) {
		if(stored[i] == '~' && i<l){
			switch(stored[i+1]){
				case '0': out += "~"; i++; break;
				case '1': out += "\'"; i++; break;
				case '2': out += ";"; i++; break;
				case '3': out += "\""; i++; break;
				case '4': out += "="; i++; break;
				case '5': out += "--"; i++; break;
				case '6': out += "x"; i++; break;
				case '7': out += "#"; i++; break;
				case '8': out += "*"; i++; break;
				case '9': out += "&"; i++; break;
				default: return "Invalid String";
			}
		} else {
			out += stored[i];
		}
	}
	return out;
} // unSQL

#endif // Not skr_f
