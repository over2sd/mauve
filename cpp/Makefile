
CC = g++
WRQ = `fltk-config --cxxflags` -lfltk -lXext -lX11 -lm
CFLAGS = `mysql_config --cflags --libs`

all: register

register: register.cpp skr_gui.h skr_sql.h skr_f.h nocurse.h mysadtry.h parrot.h
	$(CC) register.cpp -o register $(WRQ) $(CFLAGS)

