
CC = g++
WRQ = `fltk-config --cxxflags` -lfltk -lm
CFLAGS = `mysql_config --cflags --libs`
CROSSER = `fltk-config --ldstaticflags`
WCC = i686-w64-mingw32-g++-win32

all: register

register: register.cpp skr_gui.h skr_sql.h skr_f.h nocurse.h mysadtry.h parrot.h
#	$(WCC) register.cpp -o ../register.exe $(WRQ) $(CFLAGS) $(CROSSER)
	$(CC) register.cpp -o ../register $(WRQ) -lXext -lX11 $(CFLAGS)
