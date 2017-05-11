/*******************************
 * MAUVE Registration Frontend *
 *                             *
 * Begun 2017/04/17            *
 *******************************/
#define PROGRAM_MAIN "mauve-register"

#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Light_Button.H>
#include <FL/Fl_Radio_Button.H>
#include <FL/Fl_Input_Choice.H>
#include <ctime>

Fl_Preferences *opts = new Fl_Preferences(Fl_Preferences::USER,"skr",PROGRAM_MAIN);

struct regFields {
	Fl_Output* l; // leader
	Fl_Output* c; // church
	Fl_Input_Choice* t; // contact type
	Fl_Input* d; // date
	Fl_Input* n; // time
	Fl_Input* s; // size
	Fl_Input* p; // phone#
	Fl_Input* e; // email
	Fl_Output* w1; // week1
	Fl_Output* w2; // week2
	Fl_Output* w3; // week3
} rdata;

struct servData {
	Fl_Input* u; // user
	Fl_Input* h; // host
	Fl_Input* b; // base
	Fl_Check_Button* p; // use password
	Fl_Light_Button* l; // light button
} sdata;

#include "skr_gui.h"
#include "skr_sql.h"

int getPass() {
	if (getConf("SQL/usepass","N") == "N") {
std::cout << "No?\n";
		return 1;
	} else {
std::cout << "Yes?\n";
		if (skr::sqpass == "") {
			string labtext = "Enter SQL password for " + skr::squser + ":";
			skr::sqpass = fl_password(flStr(labtext),"");
			if (skr::sqpass == "") {
				return 0;
			} else {
				return 1;
			}
		} else {
			return 1;
		}
	}
}

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

void cb_cancel(Fl_Widget* w,void *o) {
	Fl_Window *d = (Fl_Window*)o;
	d->~Fl_Window();
}

void cb_saveserver(Fl_Button* b,void *w) {
	Fl_Window *d = (Fl_Window*)b->parent();
	Fl_Input* user = sdata.u; // user
	Fl_Input* host = sdata.h; // host
	Fl_Input* base = sdata.b; // base
	Fl_Check_Button* upass = sdata.p; // use password
	Fl_Light_Button* servbut = sdata.l; // light button
	if (host->changed()) {
		std::cout << "Server changed to " << host->value() << ".\n";
		setSQLbase("server",host->value());
	}
	if (base->changed()) {
		std::cout << "Base changed to '" << base->value() << "'.\n";
		setSQLbase("base",base->value());
	}
	if (user->changed()) {
		std::cout << "User changed to '" << user->value() << "'.\n";
		setSQLbase("user",user->value());
	}
	if (upass->changed() || not opts->entryExists("SQL/usepass")) {
		printf("Password changed to %s.\n",(upass->value() ? "1" : "0"));
		setSQLbase("usepass",(upass->value() ? "Y" : "N"));
	}
	if (skr::sqserv.empty() || skr::sqbase.empty() || skr::sqbase == "x") {
		string labtext = "Choose Server/Base";
		servbut->copy_label(labtext.c_str());
		servbut->value(0);
	} else {
		string labtext = "Server: " + skr::sqbase + "@@" + skr::sqserv + " (" + skr::squser + ")";
		servbut->copy_label(labtext.c_str());
		servbut->value(1);
	}
	opts->flush();
	d->~Fl_Window();
}

void cb_store(Fl_Widget* w,void* data) {
	std::cout << "Start";
	fflush(stdout);
	string x[11];
	if (rdata.c->value() == "" || long(rdata.c->value()) < 1) {
		rdata.c->color(FL_RED);
		rdata.c->redraw();
		return;
	} else {
		x[0] = rdata.c->value();
		rdata.c->color(FL_WHITE);
		rdata.c->redraw();
	}
	if (rdata.l->value() == "" || long(rdata.l->value()) < 1) {
		rdata.l->color(FL_RED);
		rdata.l->redraw();
		return;
	} else {
		x[1] = rdata.l->value();
		rdata.l->color(FL_WHITE);
		rdata.l->redraw();
	}
	if (rdata.t->value() == "") {
		rdata.t->color(FL_RED);
		rdata.t->redraw();
		return;
	} else {
		x[2] = rdata.t->value()[0];
		rdata.t->color(FL_WHITE);
		rdata.t->redraw();
	}
	if (rdata.s->value() == "" || long(rdata.s->value()) < 1) {
		rdata.s->color(FL_RED);
		rdata.s->redraw();
		return;
	} else {
		x[3] = rdata.s->value();
		rdata.s->color(FL_WHITE);
		rdata.s->redraw();
	}
	if (rdata.d->value() == "") {
		rdata.d->color(FL_RED);
		rdata.d->redraw();
		return;
	} else {
		x[4] = rdata.d->value();
		rdata.d->color(FL_WHITE);
		rdata.d->redraw();
	}
	if (rdata.n->value() == "") {
		rdata.n->color(FL_RED);
		rdata.n->redraw();
		return;
	} else {
		x[5] = rdata.n->value();
		rdata.n->color(FL_WHITE);
		rdata.n->redraw();
	}
	if (rdata.w1->value() == "" || long(rdata.w1->value()) < 1) {
		rdata.w1->color(FL_RED);
		rdata.w1->redraw();
		return;
	} else {
		x[6] = rdata.w1->value();
		rdata.w1->color(FL_WHITE);
		rdata.w1->redraw();
	}
	if (rdata.w2->value() == "" || long(rdata.w2->value()) < 1) {
		rdata.w2->color(FL_RED);
		rdata.w2->redraw();
		return;
	} else {
		x[7] = rdata.w2->value();
		rdata.w2->color(FL_WHITE);
		rdata.w2->redraw();
	}
	if (rdata.w3->value() == "" || long(rdata.w3->value()) < 1) {
		rdata.w3->color(FL_RED);
		rdata.w3->redraw();
		return;
	} else {
		x[8] = rdata.w3->value();
		rdata.w3->color(FL_WHITE);
		rdata.w3->redraw();
	}
	if (rdata.p->value() == "") {
		rdata.p->color(FL_RED);
		rdata.p->redraw();
		return;
	} else {
		x[9] = sanitizeInput(rdata.p->value(),'#');
		rdata.p->color(FL_WHITE);
		rdata.p->redraw();
	}
	if (rdata.e->value() != "" && countEaches(rdata.e->value()) == 1) {
		x[10] = sanitizeInput(rdata.e->value(),'e');
		rdata.e->color(FL_WHITE);
		rdata.e->redraw();
	} else {
		rdata.e->color(FL_RED);
		rdata.e->redraw();
		return;
	}
	for (int i = 0; i < 11; i++) {
		if (x[i] == "") { std::cout << "*"; }
		std::cout << " " << i << ": " << x[i] << "\n";
	}
	std::cout << "End";
	fflush(stdout);
	int pass = getPass();
}

void cb_pickLead(Fl_Widget* w,void* t) {
	Fl_Button* b = (Fl_Button*)w;
	Fl_Output* target = (Fl_Output*)t;
	if (1) {
		target->value("7"); // until I get the chooser coded
		b->value(1); // succeeded in choosing leader
	} else {
		b->value(0);
	}
	return;
}

void cb_pickChurch(Fl_Widget* w,void* t) {
	Fl_Button* b = (Fl_Button*)w;
	Fl_Output* target = (Fl_Output*)t;
	if (1) {
		target->value("3"); // until I get the chooser coded
		b->value(1); // succeeded in choosing leader
	} else {
		b->value(0);
	}
	return;
}

int x = 0;
const char* y[5] = {"1","3","5","7","9"};



void cb_pickWeek(Fl_Widget* w,void* o) {
	Fl_Button* b = (Fl_Button*)w;
	Fl_Output* t = (Fl_Output*)o;
	t->value(y[x]);
	x++;
	int pass = getPass();
	if (pass) {
		Fl_Window* d = new Fl_Window(400,300,"Select Week");
		Fl_Scroll* s = new Fl_Scroll(0,0,400,265,"Selections");
		Fl_Output* stat = new Fl_Output(0,268,400,32,"Loading...");
		stat->color(FL_GRAY);
		int max = (int)getConf("General/outlookMax","75");
		if (max > 99) { max = 100; }
		char* query;
		sprintf(query,"SELECT id,week,start,finish,weektype FROM weeks WHERE start > CURDATE() LIMIT %i;",max);
std::cout << "sending: " << query << "\n";
		MYSQL_RES* res = skr::sendSQLquery(query,"gathering weeks");
// SELECT count(size) FROM team WHERE arrive >= {start} AND depart <= {finish};
	}
}
static const char *calendar_xpm[] = {
"16 16 9 1",
" c None",
". c #000000",
"+ c #FF0000",
"@ c #000080",
"# c #A0A0A0",
"$ c #0000C0",
"% c #FFFFFF",
"& c #C0C0FF",
"* c #DCDCDC",
"                ",
"&&&&&&&&&&&&&   ",
"&$$$$$$$$$$$$$  ",
"&$&&&&&&&&&&&&& ",
"&$&$$$$$$$$$$$$.",
"&$&$%%%%%%%%%%%.",
"&$&$%+++++++++%.",
"&$&$%%%%%%%%%%%.",
"&$&$%*@*@*@*@*%.",
"&$&$%@*@*@*@*@%.",
"&$&$%*@*@*@*@*%.",
"&$&$%@*@*@*@*@%.",
"&$&$%*@%%%%%%%%.",
" .&$%%%%%%%%%%%.",
"  ..............",
"                "};

#define ROWH 37
#define COLR 240
#define TOPMAR 10
#define RIGHT 630
#define BUTCOL 75
#define BUTWID 70

void cb_servpick(Fl_Light_Button* b,void *o) {
	sdata.l = b;
	Fl_Window *d = new Fl_Window(400,300,"Server Login");
	Fl_Input *server = new Fl_Input(90,TOPMAR,200,32,"Address:");
	server->value(skr::sqserv.c_str());
	sdata.h = server;
	Fl_Input *dbname = new Fl_Input(90,TOPMAR + ROWH,200,32,"Database:");
	dbname->value(skr::sqbase.c_str());
	sdata.b = dbname;
	Fl_Input *user = new Fl_Input(90,TOPMAR + (ROWH * 2),200,32,"User:");
	user->value(skr::squser.c_str());
	sdata.u = user;
	Fl_Check_Button *usepass = new Fl_Check_Button(20,TOPMAR + (ROWH * 3),100,32,"Use Password");
	usepass->value((getConf("SQL/usepass","N") == "Y" ? 1 : 0));
	sdata.p = usepass;
	Fl_Button *c = new Fl_Button(390 - BUTCOL,TOPMAR + (ROWH * 5),BUTWID,32,"Cancel");
	Fl_Button *s = new Fl_Button(390 - (BUTCOL * 2),TOPMAR + (ROWH * 5),BUTWID,32,"Save");
	d->end();
	d->set_modal();
	b->value(0);
	s->callback((Fl_Callback*)cb_saveserver,b);
	c->callback((Fl_Callback*)cb_cancel,d);
	d->show();
}

int main(int argc, char **argv) {
  Fl_Window *window = new Fl_Window(640,480,PROGRAM_MAIN);
	bool success = setSQLbase();
	Fl_Light_Button *servbut = new Fl_Light_Button(50,TOPMAR,350,32,"Server");
	servbut->selection_color(FL_GREEN);
	if (skr::sqserv.empty() || skr::sqbase.empty() || skr::sqbase == "x") {
		string labtext = "Choose Server/Base";
		servbut->copy_label(labtext.c_str());
		servbut->value(0);
	} else {
		string labtext = "Server: " + skr::sqbase + "@@" + skr::sqserv + " (" + skr::squser + ")";
		servbut->copy_label(labtext.c_str());
		servbut->value(1);
	}
	servbut->callback((Fl_Callback*)cb_servpick);
  Fl_Input *dateField = new Fl_Input(50,TOPMAR + ROWH,100,32,"Date:");
	rdata.d = dateField;
	Fl_Input *timeField = new Fl_Input(200,TOPMAR + ROWH,100,32,"Time:");
	rdata.n = timeField;
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
	rdata.t = contyp;
	contyp->add("Email"); // email
	contyp->add("Phone"); // phone call
	contyp->add("Mail"); // mail
	contyp->add("Visit"); // visit
	contyp->value("Phone");
	contyp->callback((Fl_Callback*)widgetActivatorI,timeField);
	Fl_Output *leadID = new Fl_Output(0,TOPMAR + (ROWH * 2),32,32,"");
	rdata.l = leadID;
	Fl_Light_Button *leader = new Fl_Light_Button(35,TOPMAR + (2 * ROWH),120,32,"Team Leader");
	leader->callback((Fl_Callback*)cb_pickLead,leadID);
	Fl_Output *leadname = new Fl_Output(205,TOPMAR + (2 * ROWH),330,32,"Name:");
	Fl_Output *address = new Fl_Output(90,TOPMAR + (3 * ROWH),280,32,"Address");
	Fl_Button *addbut = new Fl_Button(380,TOPMAR + (3 * ROWH),60,32,"Edit");
	Fl_Input *phone = new Fl_Input(90,TOPMAR + (4 * ROWH),120,32,"Phone #:");
	rdata.p = phone;
	Fl_Input *emla = new Fl_Input(280,TOPMAR + (4 * ROWH),180,32,"Email:");
	rdata.e = emla;
	Fl_Output *churchID = new Fl_Output(0,TOPMAR + (ROWH * 5),32,32,"");
	rdata.c = churchID;
	Fl_Light_Button *churchset = new Fl_Light_Button(35,TOPMAR + (5 * ROWH),60,32,"Set");
	churchset->callback((Fl_Callback*)cb_pickChurch,churchID);
	Fl_Output *churchname = new Fl_Output(165,TOPMAR + (5 * ROWH),180,32,"Church:");
	Fl_Output *churchaddy = new Fl_Output(435,TOPMAR + (ROWH * 5),200,32,"Address:");
	Fl_Output *churchphon = new Fl_Output(90,TOPMAR + (ROWH * 6),120,32,"Phone #:");
	Fl_Input *tsize = new Fl_Input(300,TOPMAR + (ROWH * 6),60,32,"Team Size:");
	rdata.s = tsize;
	Fl_Output *date1 = new Fl_Output(90,TOPMAR + (ROWH * 7),60,32,"1");
	rdata.w1 = date1;
	Fl_Output *date2 = new Fl_Output(90,TOPMAR + (ROWH * 8),60,32,"2");
	rdata.w2 = date2;
	Fl_Output *date3 = new Fl_Output(90,TOPMAR + (ROWH * 9),60,32,"3");
	rdata.w3 = date3;
	Fl_Pixmap *p_cal = new Fl_Pixmap(calendar_xpm);
	Fl_Button *dc1 = new Fl_Button(160,TOPMAR + (ROWH * 7),32,32);
	dc1->callback((Fl_Callback*)cb_pickWeek,date1);
	dc1->image(p_cal);
	Fl_Button *dc2 = new Fl_Button(160,TOPMAR + (ROWH * 8),32,32);
	dc2->callback((Fl_Callback*)cb_pickWeek,date2);
	dc2->image(p_cal);
	Fl_Button *dc3 = new Fl_Button(160,TOPMAR + (ROWH * 9),32,32);
	dc3->callback((Fl_Callback*)cb_pickWeek,date3);
	dc3->image(p_cal);
	Fl_Button *savebut = new Fl_Button(RIGHT - (BUTCOL * 3),TOPMAR + (ROWH * 11),BUTWID,32,"Register");
	Fl_Button *clrbut = new Fl_Button(RIGHT - (BUTCOL * 4),TOPMAR + (ROWH * 11),BUTWID,32,"Clear");
	Fl_Button *quitbut = new Fl_Button(RIGHT - (BUTCOL * 1),TOPMAR + (ROWH * 11),BUTWID,32,"Exit");
	quitbut->callback(cb_close);
	savebut->callback((Fl_Callback*)cb_store);

	

  window->end();
  window->show(argc, argv);
  return Fl::run();
}

