/*******************************
 * MAUVE Registration Frontend *
 *                             *
 * Begun 2017/04/17            *
 *******************************/
#define PROGRAM_MAIN "mauve-register"

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Input_Choice.H>
#include <ctime>

Fl_Preferences *opts = new Fl_Preferences(Fl_Preferences::USER,"skr",PROGRAM_MAIN);


#include "skr_gui.h"
#include "skr_sql.h"

void widgetActivator(Fl_Check_Button* w,void* target) {
	Fl_Input*t = (Fl_Input*)target;
	if (w->value()) {
		t->activate();
		std::cout << "Active\n";
	} else {
		t->deactivate();
		std::cout << "Inactive\n";
	}
}

void widgetActivatorI(Fl_Input_Choice* w,void* target) {
	Fl_Input*t = (Fl_Input*)target;
	if (w->menubutton()->value() == 0) {
		t->activate();
		std::cout << "Active\n";
	} else {
		t->deactivate();
		std::cout << "Inactive\n";
	}
}

void cb_close(Fl_Widget* w,void *o) {
	exit(0);
}

#define ROWH 37
#define COLR 240
#define TOPMAR 10
#define RIGHT 630
#define BUTCOL 75
#define BUTWID 70

void cb_servpick(Fl_Light_Button* b,void *o) {
	std::cout << "Start";
	Fl_Window *d = new Fl_Window(400,300,"Server Login");
	Fl_Input *server = new Fl_Input(90,TOPMAR,200,32,"Address:");
	server->value(skr::sqserv.c_str());
	Fl_Input *dbname = new Fl_Input(90,TOPMAR + ROWH,200,32,"Database:");
	dbname->value(skr::sqbase.c_str());
	Fl_Input *user = new Fl_Input(90,TOPMAR + (ROWH * 2),200,32,"User:");
	user->value(skr::squser.c_str());
	Fl_Check_Button *usepass = new Fl_Check_Button(20,TOPMAR + (ROWH * 3),100,32,"Use Password");
	Fl_Button *c = new Fl_Button(390 - BUTCOL,TOPMAR + (ROWH * 5),BUTWID,32,"Cancel");
	Fl_Button *s = new Fl_Button(390 - (BUTCOL * 2),TOPMAR + (ROWH * 5),BUTWID,32,"Save");
	d->end();
	d->set_modal();
	b->value(0);
	d->show();
	std::cout << "End";
}

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(640,480,PROGRAM_MAIN);
	bool success = setSQLbase();
	Fl_Light_Button *servbut = new Fl_Light_Button(50,TOPMAR,300,32,"Server");
	servbut->selection_color(FL_GREEN);
	if (skr::sqserv.empty() || skr::sqbase.empty() || skr::sqbase == "x") {
		string labtext = "Choose Server/Base";
		servbut->copy_label(labtext.c_str());
		servbut->value(0);
	} else {
		string labtext = "Server: " + skr::sqbase + "@" + skr::sqserv;
		servbut->copy_label(labtext.c_str());
		servbut->value(1);
	}
	servbut->callback((Fl_Callback*)cb_servpick);
  Fl_Input *dateField = new Fl_Input(50,TOPMAR + ROWH,100,32,"Date:");
	Fl_Input *timeField = new Fl_Input(200,TOPMAR + ROWH,100,32,"Time:");
	time_t now = time(0);
	tm * t = localtime(&now);
	char buffer[11];
	strftime(buffer,11,"%F",t);
	dateField->value(buffer);
	strftime(buffer,11,"%T",t);
	timeField->value(buffer);
	timeField->deactivate();
//	Fl_Check_Button *mailed = new Fl_Check_Button(50,45,100,32,"Email contact");
//	mailed->when(FL_WHEN_CHANGED);
//	mailed->callback((Fl_Callback*)widgetActivator,timeField);
//  Fl_Box *box = new Fl_Box(20,80,300,60,"Contact type");
//	box->box(FL_UP_BOX);
//  box->labelfont(FL_BOLD+FL_ITALIC);
//  box->labelsize(20);
//  box->labeltype(FL_SHADOW_LABEL);
	Fl_Input_Choice *contyp = new Fl_Input_Choice(410,TOPMAR + ROWH,90,32,"Contact type:");
	contyp->add("Email"); // email
	contyp->add("Phone"); // phone call
	contyp->add("Mail"); // mail
	contyp->add("Visit"); // visit
	contyp->value("Phone");
	contyp->callback((Fl_Callback*)widgetActivatorI,timeField);
	int leaderID = 0;
	Fl_Light_Button *leader = new Fl_Light_Button(10,TOPMAR + (2 * ROWH),120,32,"Team Leader");
	Fl_Output *leadname = new Fl_Output(190,TOPMAR + (2 * ROWH),330,32,"Name:");
	Fl_Output *address = new Fl_Output(90,TOPMAR + (3 * ROWH),280,32,"Address");
	Fl_Button *addbut = new Fl_Button(380,TOPMAR + (3 * ROWH),60,32,"Edit");
	Fl_Input *phone = new Fl_Input(90,TOPMAR + (4 * ROWH),120,32,"Phone #:");
	Fl_Input *emla = new Fl_Input(280,TOPMAR + (4 * ROWH),180,32,"Email:");
	Fl_Light_Button *churchset = new Fl_Light_Button(20,TOPMAR + (5 * ROWH),60,32,"Set");
	Fl_Output *churchname = new Fl_Output(150,TOPMAR + (5 * ROWH),180,32,"Church:");
	Fl_Output *churchaddy = new Fl_Output(420,TOPMAR + (ROWH * 5),200,32,"Address:");
	Fl_Output *churchphon = new Fl_Output(90,TOPMAR + (ROWH * 6),120,32,"Phone #:");
	Fl_Input *tsize = new Fl_Input(300,TOPMAR + (ROWH * 6),60,32,"Team Size:");
	Fl_Output *date1 = new Fl_Output(90,TOPMAR + (ROWH * 7),60,32,"1");
	Fl_Output *date2 = new Fl_Output(90,TOPMAR + (ROWH * 8),60,32,"2");
	Fl_Output *date3 = new Fl_Output(90,TOPMAR + (ROWH * 9),60,32,"3");
	Fl_Button *dc1 = new Fl_Button(160,TOPMAR + (ROWH * 7),32,32);
	Fl_Button *dc2 = new Fl_Button(160,TOPMAR + (ROWH * 8),32,32);
	Fl_Button *dc3 = new Fl_Button(160,TOPMAR + (ROWH * 9),32,32);
	Fl_Button *savebut = new Fl_Button(RIGHT - (BUTCOL * 3),TOPMAR + (ROWH * 11),BUTWID,32,"Register");
	Fl_Button *clrbut = new Fl_Button(RIGHT - (BUTCOL * 4),TOPMAR + (ROWH * 11),BUTWID,32,"Clear");
	Fl_Button *quitbut = new Fl_Button(RIGHT - (BUTCOL * 1),TOPMAR + (ROWH * 11),BUTWID,32,"Exit");
	quitbut->callback(cb_close);

  window->end();
  window->show(argc, argv);
  return Fl::run();
}

