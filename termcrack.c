#include <stdio.h>
#include <curses.h>
#include <ncurses.h>
#include <string.h>

void clear_line(int curline, char *wordlist[])
	{
	attroff(A_BOLD);
	attroff(A_UNDERLINE);
	mvprintw(curline, 0, "   %s", wordlist[curline]);
	return;
	}

void select_line(int curline, char *wordlist[])
	{
	mvprintw(curline, 0, "-->");
	attron(A_BOLD);
	attron(A_UNDERLINE);
	mvprintw(curline, 3, "%s", wordlist[curline]);
	return;
	}

int main(int argc, char* argv[])
	{
	int numwords;
	int curword;
	char* wordlist[21];
	int matchlist[20];
	char wordbuf[15];
	int inchar;
	int inputdone;
	int xmax;
	int ymax;
	char *prompt;
	int promptlen;

	/*Set up ncurses */
	initscr();
	raw();
	keypad(stdscr, TRUE);

	/* Initialize everything */
	numwords = 0;
	curword = 0;
	memset((void*)wordlist, 0, 21);
	for(inputdone = 0; inputdone < 20; inputdone++)
		{
		matchlist[inputdone] = 0;
		}
	memset((void*)wordbuf, 0, 15);
	inchar = 0;
	inputdone=0;
	getmaxyx(stdscr, ymax, xmax);
	prompt = "New word: ";
	promptlen = strlen(prompt);

#ifdef DEBUG
	noecho();
	getch();
	echo();
#endif

	mvprintw(ymax-2, 0, "Enter r to display, q to exit");
	mvprintw(ymax-1, 0, "%s%15s", prompt, wordbuf);
	refresh();
	while(inputdone == 0)
		{
		mvgetnstr(ymax-1, promptlen, wordbuf,12);
		if(strcmp(wordbuf, "r") == 0)
			{
			inputdone = 1;
			}
		else if(strcmp(wordbuf, "q") == 0)
			{
			inputdone = 2;
			}
		else
			{
			wordlist[curword] = strdup(wordbuf);
			mvprintw(curword, 3, "%s", wordlist[curword]);
			numwords++;
			curword++;
			if(numwords == 20)
				{
				inputdone = 1;
				}
			else
				{
				memset((void*)wordbuf, 0, 15);
				mvprintw(ymax-1, 0, "%s%15s", prompt, wordbuf);
				refresh();
				}
			}
		}
	if(inputdone == 2)
		{
		endwin();
		return(0);
		}

	mvprintw(ymax-2, 0, "                             ");
	memset((void*)wordbuf, 0, 15);
	mvprintw(ymax-1, 0, "Use arrow keys to select a word, q to exit %15s", wordbuf);
	curword = 0;
	select_line(curword, wordlist);
	refresh();
	noecho();

	inchar = 0;
	inputdone=0;
	while((inchar = getch()) != 'q')
		{
		switch(inchar)
			{
			case KEY_UP:
				clear_line(curword, wordlist);
				curword = (curword - 1) % numwords;
				if(curword < 0)
					{
					curword = numwords -1;
					}
				select_line(curword, wordlist);
				break;
			case KEY_DOWN:
				clear_line(curword, wordlist);
				curword = (curword + 1) % numwords;
				if(curword < 0)
					{
					curword = numwords -1;
					}
				select_line(curword, wordlist);
				break;
			default:
				/* Do nothing */
				clear_line(curword, wordlist);
				curword = curword % numwords;
				select_line(curword, wordlist);
			}
		mvprintw(ymax-2, 0, "curword is %d", curword);
		refresh();
		}

	endwin();
	return(0);
	}

