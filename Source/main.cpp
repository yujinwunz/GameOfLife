#include "SDLGUI.h"
#include "AutomataEngine.h"
#include "sandbox.h"
#undef main
using namespace std;



extern TTF_Font *DefaultFont;
SDL_Surface *screen;



bool init(){	//This is the initializer called by main which sets up the environment for SDL and truetype fonts.
	if(SDL_Init(SDL_INIT_EVERYTHING)==-1 || TTF_Init() == -1){
		cout<<"Error initializing.\n";
		return true;
	};
	SDL_EnableKeyRepeat(500,25);
	//make default font a free font
	DefaultFont = TTF_OpenFont("LiberationSans-Bold.ttf",16);	//Decides on a default font.
	return 0;
}
/*
void quitProgram(SDL_Event *e, void *sender);	//foreward declaration for event delegates.
void HAdjust(SDL_Event *e, void *sender);	
void VAdjust(SDL_Event *e, void *sender);	

static bool running = true;

int main(){
	init();
	SDL_Surface *screen = screen = SDL_SetVideoMode(1000,600,32,SDL_DOUBLEBUF|SDL_HWSURFACE);
	//we will create a form, and add some controls to it.
	form mainForm(0,0,1000,1000);

	//some buttons
	Button aButton(&mainForm,0,0,140,30);	//it is important to specify mainForm as aButton's parent
	Button Quit(&mainForm,150,0,100,30);

	//let's add some text to the buttons.
	aButton.text = "A button!"; Quit.text = "Quit";
	//lets attach an event delegate to Quit's left-click event.
	Quit.addHandler(event_Lclick,quitProgram);	//now, when we click "Quit", quitprogram will run.

	//this is a demo!
	HScroll hsDemo(&mainForm,300,0,200,30,0,100,0);
	VScroll vsDemo(&mainForm,300,30,30,200,0,100,50);

	textBox txDemo(&mainForm,600,10,100,30);	txDemo.setText("Hello world");
	label lbDemo(&mainForm,600,60,100,30);

	//controls have tags; void* pointers that can store info for any use.
	//In this demo, delegates HAdjust and VAdjust will access these tags to
	//locate the label "lbDemo", which they will modify.
	hsDemo.tag = vsDemo.tag = (void*)&lbDemo;
	hsDemo.addHandler(event_scroll,HAdjust);
	vsDemo.addHandler(event_scroll,VAdjust);

	radioButton rbDemo1(&mainForm,0,100,150,30);
	radioButton rbDemo2(&mainForm,0,140,150,30,&rbDemo1);//you can specify another radio button for which this 
	radioButton rbDemo3(&mainForm,0,180,150,30,&rbDemo2);//onegroups to. Union-find will happen, dw I've got this.
	rbDemo1.setText("Radiobutton 1");
	rbDemo2.setText("Radiobutton 2");
	rbDemo3.setText("Radiobutton 3");

	//throw a checkbox for good measure
	checkBox cbDemo(&mainForm,0,250,150,30); cbDemo.setText("Checkbox");

	//To make things work, we need the form we created, "mainform".
	//all the event-driven craziness from SDL is handled in form.wait()
	//form.wait() will wait for exactly ONE event before returning execution.
	//you also need to set up a framerate, telling the form which screen to 
	//draw on.
	mainForm.startFramerate(60,screen);
	while(running){
		mainForm.wait();
		SDL_Flip(screen);	//wait() will render to the screen but not flip, providing flexibility.
	}
	cout<<"Demo done\n";
	return 0;
}


void quitProgram(SDL_Event *e, void *sender){
	Button *s = (Button*)sender;	//this delegate is called by a button. Note that runtime errors
	//will occur if the programmer doesn't ensure this.
	running = false;
}
void HAdjust(SDL_Event *e, void *sender){
	HScroll *s = (HScroll*)sender;

	label *labelToModify = (label*)s->tag;	//remember the tag we set programmically? the tag points to
	//the label in the demo.
	labelToModify->text = intToString(s->value);	//intToString is included in SDLGUI.
}


void VAdjust(SDL_Event *e, void *sender){
	VScroll *s = (VScroll*)sender;

	label *labelToModify = (label*)s->tag;
	labelToModify->text = intToString(s->value);
}
*/
static bool running = true;
void Quit(SDL_Event *e,void* p){
	running = false;
}

struct menuTag{
	int *location;
	int value;
};

void menuClick(SDL_Event *e, void *p){
	Button *s = (Button*)p;
	menuTag *me = (menuTag*)s->tag;
	*me->location = me->value;
}

int menuResult(SDL_Surface *s, pair<string,int> *option, int options, string title = ""){
	form menu(0,0,1000,600);
	SDL_Colour background = {0,20,70,255};
	//checkBox c(&menu,100,100,100,100); c.setText( "LOL");
	menu.backColour = menu.foreColour = background;

	menuTag e[10]; Button *b[10];
	int ret = -1;

	for(int i = 0; i < options; i++){
		b[i] = new Button(&menu,400,((title=="")?300:400)-options*30-25+i*60,200,50);
		b[i]->text = option[i].first;
		e[i].location = &ret; e[i].value = option[i].second;
		b[i]->tag = (void*)(&e[i]);
		b[i]->addHandler(event_Lclick,menuClick);

		//style!
		SDL_Color c1 = {10,50,150,255}, c2 = {50,100,255,255};
		b[i]->backColour = c1; b[i]->foreColour = c2;
		if(option[i].second == -1) b[i]->enabled = false;
	}

	//title
	label *titleLabel = NULL;
	TTF_Font *lf = TTF_OpenFont("LiberationSans-Bold.ttf",50);
	if(title!=""){
		titleLabel = new label(&menu,0,50,1000,80);
		titleLabel->text = title;
		titleLabel->setTextColour(255,255,255);
		titleLabel->setFont(lf);
	}
	
	menu.startFramerate(60,s);
	while(ret == -1){
		menu.wait();
		SDL_Flip(s);
	}
	//clean up
	for(int i = 0; i < options; i++) delete b[i];
	if(titleLabel != NULL) delete titleLabel;
	TTF_CloseFont(lf);
	return ret;
}

void main(){
	cout<<0xffffffff<<"\n";
	if(init()) return;
	screen = SDL_SetVideoMode(1000,600,32,SDL_DOUBLEBUF|SDL_HWSURFACE);
	
	
	//main menu loop
	while(running){
		pair<string,int> p[4] = {make_pair("Play",1),make_pair("Sandbox",2),make_pair("Options",3),make_pair("Quit",4)};
		int result = menuResult(screen,p,4,"Main menu");
		if(result == 4) return ;
		else if(result == 2){
			//sandbox
			int width, height;
			pair <string,int> s[6] = {make_pair("10x10",1),make_pair("50x50",3),
				make_pair("100x50",4),make_pair("500x250",6),
				make_pair("1500x1500",7),make_pair("Infinite",8)};
			int w[9] = {-2,10,20,50,100,200,500,1500,-1};
			int y[9] = {-2,10,10,50,50,200,250,1500,-1};
			int condition = menuResult(screen,s,6,"Select a size");
			sandbox(screen,w[condition],y[condition]);
		}else if(result == 1){
			//play
			pair <string,int> s[6] = {make_pair("Singleplayer",1),make_pair("Local",2),make_pair("Online",3)};
		}else if(result==3){
			//options

		}
	}
}