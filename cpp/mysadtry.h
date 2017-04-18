/* My sad attempts to write replacements
 * for code I couldn't distribute.
 * This is all my own work, such as it is.
 */

// Turns ints into strings in bases from 2 to 16.
std::string my_itoa(long int x, short int base = 10) {
	std::string a; std::string out;
	if (base < 2 || base > 16) return out;

	int r = x;
	if (r < 0) r = r * -1;
	do {
		char c = '0';
		switch (r % base) {
			case 0: c = '0'; break;
			case 1: c = '1'; break;
			case 2: c = '2'; break;
			case 3: c = '3'; break;
			case 4: c = '4'; break;
			case 5: c = '5'; break;
			case 6: c = '6'; break;
			case 7: c = '7'; break;
			case 8: c = '8'; break;
			case 9: c = '9'; break;
			case 10: c = 'a'; break;
			case 11: c = 'b'; break;
			case 12: c = 'c'; break;
			case 13: c = 'd'; break;
			case 14: c = 'e'; break;
			case 15: c = 'f'; break;
		}
		a += c;
		r -= r % base;
		r /= base;
	} while (r > 0);
	if (x < 0) a += '-';
	for(int i = a.length();i > 0;i--) {
		out += a[i-1];
	}
	return out;
}

// Whitespace trimmer, not yet working.
inline std::string trimStr(const std::string& Src, const std::string& c = " \r\n") {
	// Nothing here yet.
#ifdef DBOUT
	dbfile << "Skipping trim!\n";
#endif
	return Src;
}
