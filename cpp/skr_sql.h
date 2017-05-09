/*******************************************
 * MySQL Functions for SANDRA and IdealJob *
 *                                         *
 * Begun 2011-03-30                        *
 *******************************************/
#ifndef SKRSQL_H
#define SKRSQL_H
#include <string> // We'll be using strings
using std::string;
#include <stdio.h> // We'll be using standard I/O
#include <mysql/mysql.h> // We'll be accessing a database
#include "skr_f.h" // We'll be doing file operations I've done before
// Prototypes for functions used in this header
string trimWhite(string);
int findC(const char,const string);
void diefrom(const string);

#define SQLS "localhost" // In case no server is supplied
// open a debug file where I can write errors
std::fstream sqfile("debug-sql.txt", std::ios::out | std::ios::app);

// In my namespace, these prototypes live...
namespace skr {
	MYSQL_RES* sendSQLquery(const string,const string,const unsigned int);
	unsigned int trySQLaction(const string);
	unsigned long uSQLvalue(const string, const string);
	string sSQLvalue(const string, const string);
	bool bSQLvalue(const string,const string);
	int iSQLvalue(const string,const string);
	// Variables used globally within this namespace:
	string sqpass = "";
	string squser = "";
	string sqserv = "localhost";
	string sqbase = "default";
	bool usepass = false;
	bool logged = false;
	string token = "00000000";
}

// cleanBaseMeta(url or base name)
// returns cleaned base name or url
string cleanBaseMeta(string in) { // for database URLs and base names only.
	// valid chars:
	string vc = "1234567890QqWwEeRrTtYyUuIiOoPpAaSsDdFfGgHhJjKkLlZzXxCcVvBbNnMm`:/&-_.#@";
	string out = ""; // output string, starts empty
	int inlen = in.length(); // max for loop, length of input
	for (int i = 0;i < inlen;i++) { // for each character in input...
		int pa = findC(in[i],vc); // Search vc for this character
		if(pa > -1) { // If it is found...
			out.append(in,i,1); // add it to the output
		} // otherwise, discard it.
	} // done with input
	return out; // return input with invalid characters removed
} // end cleanBaseMeta

#if SKR_GUI==3
wxConfigBase *pConfig;
#endif
// setSQLbase()
// returns success, T/F, after setting variables
bool setSQLbase() { // Set the dbname
#ifndef PROGRAM_MAIN
#define PROGRAM_MAIN "undefined-main" // If I'm the main (WHY!?!?)
#endif
#ifdef DBOUT // Using debug file
	if(debugmin > 7) sqfile << "\n\n " << PROGRAM_MAIN << ":setSQLbase(): "; sqfile.flush(); // log function entry
#endif // End using debug file
	int success = true; // Start OK
	char input[128] = SQLS;
#if SKR_GUI==2
	char *prefval;
#endif
	char dbname[128] = "x"; // database name
	enum { urllen = 128 };
	skr::sqserv.reserve( urllen );
	skr::sqbase.reserve( urllen );
#ifdef DBOUT // Using debug file
	if(debugmin > 5) sqfile << "reading server... "; sqfile.flush();
#endif // End using debug file
#if SKR_GUI==3
	string confitem = "";
	confitem = std::string(pConfig->Read(wxT("/SQL/Server"),SQLS).mb_str());
	std::cout << "Server is..." << confitem << "...\n";
	skr::sqserv.assign(confitem);
	confitem = std::string(pConfig->Read(wxT("/SQL/Base"),"x").mb_str());
	std::cout << "Base is..." << confitem << "...\n";
	skr::sqbase.assign(confitem);
	confitem = std::string(pConfig->Read(wxT("/SQL/User"),"").mb_str());
	std::cout << "User is..." << confitem << "...\n";
	skr::squser.assign(confitem);
#else
	std::ifstream file;
#if SKR_GUI==2
	if (opts->entryExists("SQL/server")){
		char success = opts->get(const_cast<const char*>("SQL/server"),prefval,const_cast<const char*>(SQLS),128);
		std::cout << "Result: " << int(success) << "\n";
		skr::sqserv.assign(prefval);
		std::cout << "Server:" << skr::sqserv << "\n";
	} else {
		std::cout << "Entries: " << opts->entries() << "  ... ";
		std::cout << "No server stored. Falling back...\n";
#endif
	file.open("sqlserver.is", std::ios::in); // input file
	if(file) {
		file.read(input,128); // read server from file
#ifdef DBOUT // Using debug file
		if(debugmin > 8) sqfile << "{" << input << ")"; sqfile.flush();
#endif // End using debug file
	} // No server is okay, because we fall back to localhost.
	file.close();
	skr::sqserv.assign(input);
#if SKR_GUI==2
	}
#endif
#ifdef DBOUT // Using debug file
	if(debugmin > 5) sqfile << "reading basename... "; sqfile.flush();
#endif // End using debug file
#if SKR_GUI==2
	if (opts->entryExists("SQL/base")){
		char success = opts->get(const_cast<const char*>("SQL/base"),prefval,"",128);
		std::cout << "Result: " << int(success) << "\n";
		skr::sqbase.assign(prefval);
		std::cout << "Base:" << skr::sqbase << "\n";
	} else {
		std::cout << "Entries: " << opts->entries() << "  ... ";
		std::cout << "No database stored. Falling back...\n";
#endif
	file.open("sqlbase.is", std::ios::in); // input file
	if(file) {
		file.read(dbname,128); // read database name from file
#ifdef DBOUT // Using debug file
		if(debugmin > 8) sqfile << "(" << dbname << ")"; sqfile.flush();
#endif // End using debug file
	} else { // No basename is not okay.
		success = false; // trip the flag to die
	}
	skr::sqbase.assign(dbname);
	file.close();
#if SKR_GUI==2
	}
#endif
#ifdef DBOUT // Using debug file
	if(debugmin > 5) sqfile << "reading user... "; sqfile.flush();
#endif // End using debug file
#if SKR_GUI==2
	if (opts->entryExists("SQL/user")){
		char success = opts->get(const_cast<const char*>("SQL/user"),prefval,"",128);
		std::cout << "Result: " << int(success) << "\n";
		skr::squser.assign(prefval);
		std::cout << "User:" << skr::squser << "\n";
	} else {
		std::cout << "Entries: " << opts->entries() << "  ... ";
		std::cout << "No username stored. Falling back...\n";
#endif
	file.open("sqluser.is", std::ios::in); // input file
	if(file) {
		file.read(input,128); // read server from file
#ifdef DBOUT // Using debug file
		if(debugmin > 8) sqfile << "{" << input << ")"; sqfile.flush();
#endif // End using debug file
	} // No user is okay, because we fall back to empty.
	skr::squser.assign(input);
	file.close();
#if SKR_GUI==2
	}
	opts->flush();
#endif
#ifdef DBOUT // Using debug file
	if(debugmin > 5) sqfile << "cleaning variables... "; sqfile.flush();
#endif // End using debug file
#endif // End not using GUI/config
	skr::sqbase.assign(trimWhite(skr::sqbase)); // clean up the name of the database
	skr::sqserv.assign(cleanBaseMeta(skr::sqserv)); // clean up the server address
#ifdef DBOUT // Using debug file
	if(debug > 7) sqfile << " <=\n"; sqfile.flush();
#endif // End using debug file
	return success;
} // end setSQLbase

bool setSQLbase(const string key,const string value) {
	if(key == "base") {
		skr::sqbase.assign(cleanBaseMeta(value));
#if SKR_GUI==2
		opts->set("SQL/base",skr::sqbase.c_str());
#else
		std::ofstream file("sqlbase.is", std::ios::out); // output file
		if(file) {
			file << skr::sqbase; // write base to file
			#ifdef DBOUT // Using debug file
			if(debugmin > 8) sqfile << "{" << value << ")"; sqfile.flush();
			#endif // End using debug file
		} else { // couldn't write file.
			std::cout << "Error opening file sqlbase.is for writing.\n";
			return false;
		}
		file.close();
#endif
	} else if (key == "server") {
		skr::sqserv.assign(cleanBaseMeta(value));
#if SKR_GUI==2
		opts->set("SQL/serv",skr::sqserv.c_str());
#else
		std::ofstream file("sqlserver.is", std::ios::out); // output file
		if(file) {
			file << skr::sqserv; // write base to file
			#ifdef DBOUT // Using debug file
			if(debugmin > 8) sqfile << "{" << value << ")"; sqfile.flush();
			#endif // End using debug file
		} else { // couldn't write file.
			std::cout << "Error opening file sqlserver.is for writing.\n";
			return false;
		}
		file.close();
#endif
	} else if(key == "user") {
		skr::squser.assign(trimWhite(value));
#if SKR_GUI==2
		opts->set("SQL/user",skr::squser.c_str());
#else
		std::ofstream file("sqluser.is", std::ios::out); // output file
		if(file) {
			file << skr::squser; // write base to file
			#ifdef DBOUT // Using debug file
			if(debugmin > 8) sqfile << "{" << value << ")"; sqfile.flush();
			#endif // End using debug file
		} else { // couldn't write file.
			std::cout << "Error opening file sqluser.is for writing.\n";
			return false;
		}
		file.close();
#endif
	} else if(key == "pass") {
		skr::sqpass.assign(value);
	} else {
		return false;
	}
	return true;
}

// stripCGIsession(url)
// Finds the 8-character session ID and copies it to the token variable in namespace skr::
void stripCGIsession(const string in) {
	if(in.find("session=") != string::npos) { // If session marker is present...
		int startst = in.find("session=") + 8; // mark start...
		if(in.length() >= (unsigned int)(startst + 8)) skr::token = in.substr(startst,8); // and copy
	}
}

// trySQLaction(query string)
// For performing actions that do not return data but an error number.
unsigned int skr::trySQLaction(const string my_query) {
/* Warning. This function assumes correct escaping of
 *	dangerous characters/clauses. Do not send unfiltered
 *	input to this function.
 */
	const char* thequery = my_query.c_str();
	MYSQL mysql;

	mysql_init(&mysql);
//	if(!mysql_real_connect(&mysql,SQLS,squser.c_str(),sqpass.c_str(),"sandra",0,NULL,0)) {
	// Connect to server
	if(!mysql_real_connect(&mysql,sqserv.c_str(),squser.c_str(),sqpass.c_str(),sqbase.c_str(),0,NULL,0)) {
		fprintf(stderr,"\nFailed to connect to database: Error: %s\n",mysql_error(&mysql));
		return false;
	} else { // if no error, perform the query:
		mysql_query(&mysql,thequery);
		return mysql_errno(&mysql); // return the error number
	}
}

// sendSQLquery(query,debug message printable operation,length of query)
MYSQL_RES* skr::sendSQLquery(const string my_query, const string operation, const unsigned int len = 0) {
/* Warning. This function assumes correct escaping of
 *	dangerous characters/clauses. Do not send unfiltered
 *	input to this function.
 */
#ifdef DBOUT // using debug file
	if(debugmin > 5) sqfile << "sSQLq is " << operation << " ";
	if(debugmin > 8) sqfile << "with " << my_query;
	if(debugmin > 5) sqfile << "\n"; sqfile.flush();
#endif // Not using debug file
	const char* thequery = my_query.c_str();
	MYSQL mysql;
//	MYSQL_ROW row;
	MYSQL_RES* result;
//	unsigned int num_fields;
//	unsigned int i;

	mysql_init(&mysql);
//	if(!mysql_real_connect(&mysql,SQLS,squser.c_str(),sqpass.c_str(),"sandra",0,NULL,0)) {
	if(!mysql_real_connect(&mysql,sqserv.c_str(),squser.c_str(),sqpass.c_str(),sqbase.c_str(),0,NULL,0)) {
		fprintf(stderr,"\nFailed to connect to database: Error: %s\n",mysql_error(&mysql));
		return NULL;
	} else {
		int err = 0;
		if(len > 0) { // If length is given, use real
			err = mysql_real_query(&mysql,thequery,len);
			if(debug > 9) sqfile << " (r) ";
		} else { // otherwise, use the more restrictive version
			err = mysql_query(&mysql,thequery);
			if(debug > 9) sqfile << " (n) ";
		}
		if(err != 0) {
#ifdef DBOUT // Using debug file
			if(debug > 7) sqfile << "sendSQLquery(): " << skr::timestamp() << mysql_error(&mysql) << "\n";
#endif // Not debug file
			string s = "\nMySQL Query Failed while "; s += operation; s += "!";
#ifdef SKR_ALLOW_DEBUG
			sqfile << skr::timestamp() << "Failed while " << operation << ". Q: " << thequery << " - Error: " << mysql_error(&mysql) << "\n";
#endif // end debug stuff
			mysql_close(&mysql);
#ifdef SKR_MUSTFAILGRACEFULLY // Do I have to keep going on error?
			std::cout << "<br />Oops. I maked a boo-boo. Sorry.<br />\n"
			return NULL;
#else // okay to choke and die
			diefrom(s,2);
#endif // endif must fail gracefully
		} else { // If no errors...
			result = mysql_store_result(&mysql);
			mysql_close(&mysql);
			return result;
		}
	}
	return NULL;
}

// uSQLvalue(query,operation for debug messages)
// For queries expecting one result, as an unsigned long
unsigned long skr::uSQLvalue(const string q, const string op) {
	unsigned long r = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	result = sendSQLquery(q,op);
	if ((row = mysql_fetch_row(result)) != NULL) {
		if(row[0] != NULL) { r = atoul(row[0]); } else { fun_err = 1; }
	} else { fun_err = 2; }
	if(result != NULL) mysql_free_result(result);
	return r;
}

// sSQLvalue(query,operation for debug messages)
// For queries resulting in one value, a string
string skr::sSQLvalue(const string q, const string op) {
	string r = "";
	MYSQL_RES* result;
	MYSQL_ROW row;
	result = sendSQLquery(q,op);
	if ((row = mysql_fetch_row(result)) != NULL) {
		if(row[0] != NULL) { r = row[0]; } else { fun_err = 1; }
	} else { fun_err = 2; }
	if(result != NULL) mysql_free_result(result);
	return r;
}

// bSQLvalue(query,operation)
// For queries resulting in one boolean value
bool skr::bSQLvalue(const string q,const string op) {
	bool r = false;
	MYSQL_RES* result;
	MYSQL_ROW row;
	result = sendSQLquery(q,op);
	if ((row = mysql_fetch_row(result)) != NULL) {
		if(row[0] != NULL) { r = atoi(row[0]); } else { fun_err = 1; }
	} else { fun_err = 2; }
	if(result != NULL) mysql_free_result(result);
	return r;
}

// Same as above, but expects query to result in an integer (+/-)
int skr::iSQLvalue(const string q,const string op) {
	int r = 0;
	MYSQL_RES* result;
	MYSQL_ROW row;
	result = sendSQLquery(q,op);
	if ((row = mysql_fetch_row(result)) != NULL) {
		if(row[0] != NULL) { r = atoi(row[0]); } else { fun_err = 1; }
	} else { fun_err = 2; }
	if(result != NULL) mysql_free_result(result);
	return r;
}

#endif
