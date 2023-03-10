#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <io.h>
#include <fcntl.h>
#include <sys\types.h>
#include <sys\stat.h>
#define numoptions 3
#define numkeys 10


static unsigned char readch, oldreadch, keystatus[256], extended;
static char keystring[10][37] =
{
	"Press a key to move you FORWARD.    ",
	"Press a key to move you BACKWARD.   ",
	"Press a key to turn you LEFT.       ",
	"Press a key to turn you RIGHT.      ",
	"Press a key to make you RUN.        ",
	"Press a key to make you FIRE.       ",
	"Press a key to make you UNLOCK DOOR.",
	"Press a key to LOAD GAME.           ",
	"Press a key to SAVE GAME.           ",
	"Press a key to QUIT GAME.           "
};
static int htot, hstr, vtot, vstr;
unsigned char option[numoptions], keys[numkeys];
void interrupt far keyhandler(void);
void (interrupt far *oldkeyhandler)();

main()
{
	int i, j, k, optionum;
	char ch, col;

	loadsetup();
	drawoptions();
	optionum = 0;
	while (ch != 27)
	{
		for(i=0;i<numoptions;i++)
		{
			if (i == optionum)
				col = 75;
			else
				col = 67;
			if (i == 0)
			{
				if (option[i] == 0)
					printstr(25,3,"320*200 resolution                              ",col);
				if (option[i] == 1)
					printstr(25,3,"360*240 resolution (multi-sync monitor required)",col);
			}
			if (i == 1)
			{
				if (option[i] == 0)
					printstr(25,4,"No digitized sound           ",col);
				if (option[i] == 1)
					printstr(25,4,"Sound Blaster digitized sound",col);
			}
			if (i == 2)
			{
				if (option[i] == 0)
					printstr(25,5,"No background music          ",col);
				if (option[i] == 1)
					printstr(25,5,"PC speaker background music  ",col);
				if (option[i] == 2)
					printstr(25,5,"MPU-401 MIDI background music",col);
				if (option[i] == 3)
					printstr(25,5,"Adlib background music       ",col);
			}
		}
		ch = getch();
		if (ch == 0)
		{
			ch = getch();
			if ((ch == 72) && (optionum > 0))
				optionum--;
			if ((ch == 80) && (optionum < numoptions-1))
				optionum++;
			if ((ch == 75) && (option[optionum] > 0))
				option[optionum]--;
			if ((ch == 77) && ((option[optionum] < 1) || ((optionum == 2) && (option[optionum] < 3))))
				option[optionum]++;
			ch = 0;
		}
		if ((ch == 'k') || (ch == 'K'))
		{
			oldkeyhandler = _dos_getvect(0x9);
			_disable(); _dos_setvect(0x9, keyhandler); _enable();
			for(j=0;j<numkeys;j++)
			{
				for(i=0;i<256;i++)
					keystatus[i] = 0;
				printstr(20,16,&keystring[j][0],79);
				k = 0;
				i = 0;
				while (k == 0)
				{
					i = ((i+1)&255);
					if ((i != 0) && (i != 0xaa) && (keystatus[i] != 0))
					{
						k = 1;
						if ((j > 0) && (keys[j-1] == i))
							k = 0;
						keys[j] = i;
					}
				}
			}
			printstr(20,16,"                                    ",79);
			_disable(); _dos_setvect(0x9, oldkeyhandler); _enable();
		}
		if ((ch == 'a') || (ch == 'A'))
		{
			printstr(20,8, "Controls: (ESC to go back to this menu)",78);
			printstr(20,9, "Up            - move screen up.",79);
			printstr(20,10,"Down          - move screen down.",79);
			printstr(20,11,"Left          - move screen left.",79);
			printstr(20,12,"Right         - move screen right.",79);
			printstr(20,13,"Shift + Up    - shrink screen vertically.",79);
			printstr(20,14,"Shift + Down  - expand screen vertically.",79);
			printstr(20,15,"Shift + Left  - shrink screen horizontally.",79);
			printstr(20,16,"Shift + Right - expand screen horizontally.",79);
			printstr(20,17,"Press any key to start adjusting:",78);
			while (kbhit() == 0);
			_asm \
			{
				mov ax, 0x13
				int 0x10
			}
			outp(0x3d4,0x13); outp(0x3d5,45);                   //Bytes per Line
			outp(0x3c4,0x4); outp(0x3c5,inp(0x3c5)&(255-8));    //Chain 4 off
			outp(0x3d4,0x17); outp(0x3d5,inp(0x3d5)|64);        //Word Mode
			outp(0x3d4,0x14); outp(0x3d5,inp(0x3d5)&(255-64));  //Double Word Mode
			setmode360240();
			_asm \
			{
				mov dx, 0x3c4
				mov al, 0x2
				out dx, al
				inc dx
				mov al, 0xff
				out dx, al
				mov ax, 0xa000
				mov es, ax
				cld
				mov di, 0
				mov cx, 21600
				mov ax, 0x0101
				rep stosw
				mov di, 0
				mov ax, 0x0f0f
				mov cx, 45
				rep stosw
				mov di, 21510
				mov cx, 45
				rep stosw
				mov al, 0x1
				out dx, al
				mov al, 0x0f
				mov di, 0
				mov cx, 240
leftside:
				mov es:[di], al
				add di, 90
				loop leftside
				mov al, 0x8
				out dx, al
				mov al, 0x0f
				mov di, 89
				mov cx, 240
rightside:
				mov es:[di], al
				add di, 90
				loop rightside
			}
			ch = 0;
			while (ch != 27)
			{
				if (kbhit() != 0)
				{
					ch = getch();
					if (ch == 0)
					{
						ch = getch();
						_asm \
						{
							mov ax, 0x40
							mov es, ax
							mov di, 0x17
							mov al, es:[di]
							and al, 3
							mov ah, al
							mov k, ax
						}
						if (k == 0)
						{
							if (ch == 75)
								hstr++, setmode360240();
							if (ch == 77)
								hstr--, setmode360240();
							if (ch == 72)
								vstr+=2, setmode360240();
							if (ch == 80)
								vstr-=2, setmode360240();
						}
						if (k > 0)
						{
							if (ch == 75)
								htot++, setmode360240();
							if (ch == 77)
								htot--, setmode360240();
							if (ch == 72)
								vtot++, setmode360240();
							if (ch == 80)
								vtot--, setmode360240();
						}
					}
				}
			}
			drawoptions();
			ch = 0;
		}
		if (ch == 13)
			savesetup();
	}
	_asm \
	{
		mov ax, 0x3
		int 0x10
	}
}

loadsetup()
{
	int fil, i, j;

	if ((fil = open("tables.dat",O_BINARY|O_RDONLY,S_IREAD)) == -1)
	{
		printf("Cannot load options");
		exit(0);
	}
	lseek(fil,8192+8192+720,SEEK_SET);
	read(fil,&htot,2);
	read(fil,&hstr,2);
	read(fil,&vtot,2);
	read(fil,&vstr,2);
	read(fil,&option[0],numoptions);
	read(fil,&keys[0],numkeys);
	close(fil);
}

savesetup()
{
	int fil, i, j;

	if ((fil = open("tables.dat",O_BINARY|O_WRONLY,S_IWRITE)) == -1)
	{
		printf("Cannot save options");
		exit(0);
	}
	lseek(fil,8192+8192+720,SEEK_SET);
	write(fil,&htot,2);
	write(fil,&hstr,2);
	write(fil,&vtot,2);
	write(fil,&vstr,2);
	write(fil,&option[0],numoptions);
	write(fil,&keys[0],numkeys);
	close(fil);
}

printchr(x,y,character,attribute,len)
int x, y, len;
char character, attribute;
{
	int pos;

	pos = (y*80+x)<<1;
	_asm \
	{
		mov ax, 0xb800
		mov es, ax
		mov di, pos
		mov al, character
		mov ah, attribute
		mov cx, len
		cld
		rep stosw
	}
}

printstr(x,y,string,attribute)
int x, y;
char string[81], attribute;
{
	char character;
	int i, pos;

	pos = (y*80+x)<<1;
	i = 0;
	while (string[i] != 0)
	{
		character = string[i];
		_asm \
		{
			mov ax, 0xb800
			mov es, ax
			mov di, pos
			mov al, character
			mov ah, attribute
			stosw
		}
		i++;
		pos+=2;
	}
}

void interrupt far keyhandler()
{
	oldreadch = readch;
	_asm \
	{
		in al, 0x60
		mov readch, al
		in al, 0x61
		or al, 0x80
		out 0x61, al
		and al, 0x7f
		out 0x61, al
	}
	if (readch == 0xe0)
		extended = 128;
	else
	{
		if (oldreadch != readch)
			keystatus[(readch&127)+extended] = ((readch>>7)^1);
		extended = 0;
	}
	_asm \
	{
		mov al, 0x20
		out 0x20, al
	}
}

setmode360240()
{
	int i;

	outp(0x3d4,0x11); outp(0x3d5,inp(0x3d5)&127);       //Unlock indeces 0-7
	outp(0x3d4,0x3); outp(0x3d5,inp(0x3d5)|128);        //Unlock V. retraces
	outp(0x3d4,0x0); outp(0x3d5,htot&255);
	outp(0x3d4,0x1); outp(0x3d5,90);
	outp(0x3d4,0x2); outp(0x3d5,89);
	outp(0x3d4,0x3); outp(0x3d5,inp(0x3d5)|26);
	outp(0x3d4,0x4); outp(0x3d5,hstr&255);
	outp(0x3d4,0x5); outp(0x3d5,inp(0x3d5)&127);
	outp(0x3d4,0x6); outp(0x3d5,vtot&255);
	outp(0x3d4,0x7);
	i = ((inp(0x3d5)&82)|8);
	i |= ((vstr&512)>>2);
	i |= ((vstr&256)>>6);
	i |= ((vtot&512)>>4);
	i |= ((vtot&256)>>8);
	outp(0x3d5,i);
	outp(0x3d4,0x9); outp(0x3d5,inp(0x3d5)&223);
	outp(0x3d4,0x12); outp(0x3d5,224);
	outp(0x3d4,0x15); outp(0x3d5,223);
	outp(0x3d4,0x16); outp(0x3d5,inp(0x3d5)|96);
	outp(0x3d4,0x10); outp(0x3d5,vstr&255);
	outp(0x3d4,0x3); outp(0x3d5,inp(0x3d5)&127);        //Relock V. retraces
	outp(0x3d4,0x11); outp(0x3d5,inp(0x3d5)|128);       //Relock indeces 0-7
}

drawoptions()
{
	int i;

	_asm \
	{
		mov ax, 0x3
		int 0x10
	}
	outp(0x3d4,0x0e); outp(0x3d5,255);
	printchr(0,0,218,78,1);
	printchr(1,0,196,78,78);
	printchr(79,0,191,78,1);
	for(i=1;i<24;i++)
	{
		printchr(0,i,179,78,1);
		printchr(1,i,32,78,78);
		printchr(79,i,179,78,1);
	}
	printchr(0,24,192,78,1);
	printchr(1,24,196,78,78);
	printchr(79,24,217,78,1);
	printstr(34,1,"Walken Setup",79);
	printstr(2,3,"1) Graphics screen  -",77);
	printstr(2,4,"2) Digitized sound  -",77);
	printstr(2,5,"3) Music source     -",77);
	printstr(20,18,"To adjust 360*240 mode,  press A.",71);
	printstr(20,19,"To define your own keys, press K.",71);
	printstr(20,20,"Press UP/DOWN arrows to select option.",71);
	printstr(20,21,"Press LEFT/RIGHT arrows to change option.",71);
	printstr(20,22,"Press ENTER to save current setup.",71);
	printstr(20,23,"Press ESC to exit.",71);
}
