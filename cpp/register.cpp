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
                     
int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(640,480,PROGRAM_MAIN);
  Fl_Input *dateField = new Fl_Input(50,10,100,32,"Date:");
	Fl_Input *timeField = new Fl_Input(200,10,100,32,"Time:");
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
	Fl_Input_Choice *contyp = new Fl_Input_Choice(110,47,90,32,"Contact type:");
	contyp->add("Email"); // email
	contyp->add("Phone"); // phone call
	contyp->add("Mail"); // mail
	contyp->add("Visit"); // visit
	contyp->value("Phone");
	contyp->callback((Fl_Callback*)widgetActivatorI,timeField);
	int leaderID = 0;
	Fl_Light_Button *leader = new Fl_Light_Button(210,47,120,32,"Team Leader");
	Fl_Output *leadname = new Fl_Output(390,47,130,32,"Name:");
	Fl_Output *address = new Fl_Output(90,90,280,32,"Address");
	Fl_Button *addbut = new Fl_Button(380,90,60,32,"Edit");
	Fl_Input *phone = new Fl_Input(90,125,120,32,"Phone #:");
	Fl_Input *emla = new Fl_Input(280,125,180,32,"Email:");
	Fl_Light_Button *churchset = new Fl_Light_Button(20,160,60,32,"Set");
	Fl_Output *churchname = new Fl_Output(150,160,180,32,"Church:");
	Fl_Output *churchaddy = new Fl_Output(420,160,200,32,"Address:");
	Fl_Output *churchphon = new Fl_Output(90,195,120,32,"Phone #:");
	Fl_Input *tsize = new Fl_Input(300,195,60,32,"Team Size:");
	Fl_Output *date1 = new Fl_Output(90,245,60,32,"1");
	Fl_Output *date2 = new Fl_Output(90,280,60,32,"2");
	Fl_Output *date3 = new Fl_Output(90,315,60,32,"3");
	Fl_Button *dc1 = new Fl_Button(160,245,32,32);
	Fl_Button *dc2 = new Fl_Button(160,280,32,32);
	Fl_Button *dc3 = new Fl_Button(160,315,32,32);
	Fl_Button *savebut = new Fl_Button(500,315,70,32,"Register");
	Fl_Button *clrbut = new Fl_Button(420,315,70,32,"Clear");

  window->end();
  window->show(argc, argv);
  return Fl::run();
}

