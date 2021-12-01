#ifndef LOAD_H
#define LOAD_H

#include <string>
#include <ncurses.h>
#include <assert.h>
#include <stdlib.h>

class bar{
	private:
		int _line, _witdth, _objects, maxx, _current, _y, maxy;
		int * written;
	public:
		WINDOW *win, *writeWin;
		bar(int line, int width, int objects, int *wr=NULL, WINDOW *w = NULL);
		bar(int line, bool all, int objects, int back=20, int *wr=NULL, WINDOW *w = NULL);
		std::string construct(float progress, std::string end);
		void updateObjects(int objects);
		void updateLine(int line);
		void updateBack(int back);
		void update(std::string end);
		void write(std::string wr);
		void update();
		void finished();
		void finished(std::string);
		void clear();
		void clear(std::string);
		void end();
};

bar::bar(int line, int width, int objects, int *wr, WINDOW *w)
	:_line(line),
       	_witdth(width),
       	_objects(objects),
	_current(0),
	writeWin(w),
	written(wr)
{
	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);
	int y;
	y = getcury(stdscr);
	_y = y;
	if(writeWin == NULL)
		_line = (line + y);
	else
		_line = (maxy-(line+1));
	win = newwin(1, maxx, _line, 0);
}

bar::bar(int line, bool all, int objects, int back, int *wr, WINDOW *w)
	:_line(line),
       	_objects(objects),
	_current(0),
	writeWin(w),
	written(wr)
{
	initscr();
	maxx = getmaxx(stdscr);
	maxy = getmaxy(stdscr);
	_witdth = maxx-back;
	int y;
	y = getcury(stdscr);
	_y = y;
	if(writeWin == NULL)
		_line = (line + y);
	else
		_line = (maxy-(line+1));
	//window, (height, width, y, x)
	win = newwin(1, maxx, _line, 0);
}

void bar::write(std::string wr){
	assert(writeWin != NULL);
	if(*written >= getmaxy(writeWin)){
		wclear(writeWin);
		wrefresh(writeWin);
		(*written) = 0;
	}
	(*written)++;
	wprintw(writeWin, wr.c_str());
	wrefresh(writeWin);
}

void bar::updateObjects(int objects){
	_objects = objects;
}

void bar::updateLine(int line){
	assert(writeWin == NULL);	
	_line = line + _y;
	mvwin(win, _line, 0);
	wrefresh(win);
}

void bar::updateBack(int back){
	_witdth = maxx-back;
}

void bar::update(){
	update("");
}

void bar::update(std::string end){
	_current++;
	if(_current <= _objects){
	std::string ending = "|"+std::to_string(_current)+"/"+std::to_string(_objects)+"|"+end;
	float progress = (float)_current/(float)_objects;
	std::string toPrint = construct(progress, ending);
	clear(toPrint);
	}
	else
		finished();
}

void bar::clear(){
	wclear(win);
	wrefresh(win);
}

void bar::clear(std::string put){
	wclear(win);
	waddstr(win, put.c_str());
	wrefresh(win);
}

void bar::finished(std::string toPrint){
	clear(toPrint);
	//getch();
}

void bar::finished(){
	clear("FINISHED!");
	//getch();
}

void bar::end(){
	delwin(win);
}

std::string bar::construct(float progress, std::string end){
	std::string output;
	output += "[";
	int pos = _witdth * progress;
	for (int i = 0; i < _witdth; ++i) {
		if (i < pos) output += "=";
		else if (i == pos) output += ">";
		else output += " ";
	}
	output += end;
	return output;
}


class load{
	private:
		//std::vector<bar*> bars;
		bar ** bars;
		WINDOW *writeWin;
		int _bars;
		int written;
	public:
		load(int barAmt, bool write=false);
		load(int barAmt, int objects, bool write=false);
		void end();
		bar *getBar(int b){
			assert(b < _bars);
			return bars[b];
		}
};

load::load(int barAmt, bool write)
	:_bars(barAmt),
	written(0)
{
	bars = (bar**)malloc(sizeof(bar*)*barAmt);
	initscr();
	if(write == true){
		int width = getmaxx(stdscr);
		int maxy = getmaxy(stdscr);
		int height = maxy-(barAmt+1);
		//window, (height, width, y, x)
		writeWin = newwin(height, width, 0, 0);
	}
	else
		writeWin = NULL;

	for(int i = 0; i < barAmt; i++){
		bars[i] = new bar(i, true, 30, &written, writeWin);
	}
}

load::load(int barAmt, int objects, bool write)
	:_bars(barAmt),
	written(0)
{
	bars = (bar**)malloc(sizeof(bar*)*barAmt);	
	initscr();
	if(write == true){
		int width = getmaxx(stdscr);
		int maxy = getmaxy(stdscr);
		int height = maxy-(barAmt+1);
		//window, (height, width, y, x)
		writeWin = newwin(height, width, 0, 0);
	}
	else
		writeWin = NULL;
	for(int i = 0; i < barAmt; i++){
		bars[i] = new bar(i, true, objects, &written, writeWin);
	}
}


void load::end(){
	for(int i = 0; i < _bars; i++){
		bars[i]->end();
		free(bars[i]);
	}
	free(bars);
	endwin();
}

#endif
