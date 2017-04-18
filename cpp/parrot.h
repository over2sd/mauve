/***************************************************
 * Web Page Segment Inserter (Parrot Module)       *
 * (c) Copyright 2005 by Lincoln Sayger            *
 *                                                 *
 * 07.07.30 - Updated to currect compiler standard.*
 * 05.05.23 - Begun                                *
 ***************************************************/

#ifndef PARROT_H
#define PARROT_H

#include <fstream>
#include <iostream>
#include <cstdlib>

void parrot(const char* flnm) {
	std::ifstream polly(flnm, std::ios::binary | std::ios::in);
	polly.seekg(0,std::ios::beg);
	if (polly.bad() || !polly) {
		std::cout << "File error encountered with \"" << flnm << "\".  Please contact the system administrator!\n";
		return;
	}
	char printthis;
	polly.read(&printthis,sizeof(char));
	while (!polly.eof()) {
		std::cout << printthis;
		polly.read(&printthis,sizeof(char));
	}
	polly.close();
}

void insertingparrot(const char* flnm, const string s, const unsigned int m = 0) {
	unsigned int n = 0;
	std::ifstream polly(flnm, std::ios::binary | std::ios::in);
	polly.seekg(0,std::ios::beg);
	if (polly.bad() || !polly) {
		std::cout << "File error encountered with \"" << flnm << "\".  Please contact the system administrator!\n";
		return;
	}
	char printthis;
	polly.read(&printthis,sizeof(char));
	while (!polly.eof()) {
		if(printthis == '%') {
			polly.read(&printthis,sizeof(char));
			switch (printthis) {
				case 's':
				if(m > n || m == 0) {
					std::cout << s;
					n++;
				} else {
					std::cout << "%" << printthis;
				} break;
				case '%': std::cout << "%"; break;
				default: std::cout << "%" << printthis;
			}
		} else {
			std::cout << printthis;
		}
		polly.read(&printthis,sizeof(char));
	}
	polly.close();
}

#endif

