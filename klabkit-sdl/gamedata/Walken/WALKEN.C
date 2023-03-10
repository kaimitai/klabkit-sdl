#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <dos.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#define dataport 0x330
#define statport 0x331
#define numwalls 80
#define numoptions 3
#define numkeys 10

static char credits[numwalls+1] =
{
	0,
	1,1,1,1,1,1,1,1,1,2,1,1,1,1,2,2,2,2,1,1,
	1,2,2,2,2,2,0,0,0,0,1,1,1,0,1,1,1,1,1,1,
	1,1,1,1,2,2,2,2,1,1,1,2,2,2,2,2,2,2,2,2,
	1,1,1,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0
};
static unsigned int pic, walseg[numwalls];
static int lborder[numwalls], rborder[numwalls];
static int sortang[512], sorti[512], totalsortcnt, sortcnt;
static char sortbnum[512], passthrough[256], bmpkind[256];
static unsigned int mposx[512], mposy[512], mnum;
static unsigned int mgolx[512], mgoly[512];
static unsigned int moldx[512], moldy[512];
static int mshock[512], vidmode, lside, rside, scrsize;
static int htot, hstr, vtot, vstr;
static unsigned char option[numoptions], keydefs[numkeys];
static unsigned char mstat[512], mshot[512], mboard[64][64];
static unsigned char tempbuf[4096], board[64][64], palette[768];
static unsigned int height[360], linum[360], walnum[360], sprinum[360];
static int lincalc[360], numline, lastline, linplc[360], linbits[360];
static long tantable[2048], sintable[2048];
static int radarang[360], pageoffset;
static unsigned int times90[256], less64inc[64];
static unsigned char readch, oldreadch, keystatus[256], extended;
static int clockspeed, statusbar, statusbargoal;
static unsigned long totalclock;
void interrupt far keyhandler(void);
void (interrupt far *oldkeyhandler)();
int musicsource, midiscrap, midiinst = 0;
unsigned long musicstatus, count, countstop, nownote;
unsigned int numnotes, speed, drumstat, numchans, firstime = 1;
static unsigned long *note, chanage[18];
static unsigned char inst[256][32], databuf[2048];
static unsigned char chanfreq[18], chantrack[18];
static unsigned char trinst[16], trquant[16], trchan[16];
static unsigned char trprio[16], trvol[16];
static unsigned int pcfreq[63] =
{

	0,
	65,69,73,78,82,87,92,98,104,110,117,123,
	131,139,147,156,165,175,185,196,208,220,233,247,
	262,277,294,311,330,349,370,392,415,440,466,494,
	523,554,587,622,659,698,740,784,831,880,932,988,
	1047,1109,1175,1245,1319,1397,1480,1568,1661,1760,1864,1976,
	2094
};
static unsigned int adlibfreq[63] =
{
	0,
	2390,2411,2434,2456,2480,2506,2533,2562,2592,2625,2659,2695,
	3414,3435,3458,3480,3504,3530,3557,3586,3616,3649,3683,3719,
	4438,4459,4482,4504,4528,4554,4581,4610,4640,4673,4707,4743,
	5462,5483,5506,5528,5552,5578,5605,5634,5664,5697,5731,5767,
	6486,6507,6530,6552,6576,6602,6629,6658,6688,6721,6755,6791,
	7510
};
void interrupt far ksmhandler(void);
void (interrupt far *oldhand)();
static unsigned char *snd, speechstatus;
void interrupt far divzerohandler(void);
void (interrupt far *olddivzerohandler)(void);
static int boardnum, oldlife, life, death;
static int lifevests, lightnings, firepowers, keys;
static int flowerpos, kenpos, ballpos;
static int youbulnum, bulnum, bulang[32], bulkind[32];
static unsigned int bulx[32], buly[32];
static unsigned long bulstat[32], lastbulshoot;
static unsigned int startx, starty, posx, posy, oldposx, oldposy;
static int numboards, ang, startang, angvel, vel, maxvel;
static int posz, hvel, rogermode;
static unsigned char memtype[numwalls];
static unsigned emmpageframe, emmtotalpages, emmavailpages, emmhandle;
static unsigned convavailpages, convwalls, oldlogpage, oldphyspage;

main(int argc,char **argv)
{
	char filename[15];
	int i, j, k, m, n, x, y;
	unsigned int l, newx, newy, plcx, plcy;
	long tanz;
	time_t tnow;

	loadtables();
	vidmode = option[0];
	musicsource = (option[2]-1);
	if (musicsource == -1)
		musicsource = 0;
	speechstatus = option[1];
	time(&tnow);
	srand((unsigned)tnow);
	if ((note = (unsigned long *)halloc(8192,4)) == 0)
	{
		printf("Music memory allocation of 8K failed.");
		exit(-1);
	}
	if ((snd = (unsigned char *)halloc(64000,1)) == 0)
	{
		printf("Speech memory allocation of 64K failed.");
		exit(-1);
	}
	oldlogpage = -1;
	oldphyspage = -1;
	if ((pic = convallocate(numwalls<<8)) == -1)
	{
		convwalls = ((convavailpages&0xff00)>>8);
		pic = convallocate(convavailpages&0xff00);
		if ((emmhandle = emmallocate(((numwalls-convwalls)+3)>>2)) == -1)
		{
			printf("Not enough memory\n\n");
			printf("Conventional memory - %ld bytes.\n",(((long)convavailpages)<<4));
			printf("EMS memory          - %ld bytes.\n",(((long)emmavailpages)<<14));
			exit(-1);
		}
		l = pic;
		for(i=0;i<convwalls;i++)
		{
			memtype[i] = 255;
			walseg[i] = l;
			l += 256;
		}
		for(i=convwalls;i<numwalls;i++)
		{
			memtype[i] = ((i-convwalls)>>2);
			walseg[i] = emmpageframe+(((i-convwalls)&3)<<8);
		}
	}
	else
	{
		convwalls = numwalls;
		convavailpages = (numwalls<<8);
		l = pic;
		for(i=0;i<numwalls;i++)
		{
			memtype[i] = 255;
			walseg[i] = l;
			l += 256;
		}
	}
	l = 0;
	for(i=0;i<256;i++)
	{
		times90[i] = l;
		l += 90;
	}
	less64inc[0] = 16384;
	for(i=1;i<64;i++)
		less64inc[i] = 16384 / i;
	_asm \
	{
		mov ax, 0x13
		int 0x10
	}
	if (vidmode == 1)
		setmode360240();
	outp(0x3c4,1); outp(0x3c5,inp(0x3c5)|32);           //screen off
	outp(0x3d4,0x13); outp(0x3d5,45);                   //Bytes per Line
	outp(0x3c4,0x4); outp(0x3c5,inp(0x3c5)&(255-8));    //Chain 4 off
	outp(0x3d4,0x17); outp(0x3d5,inp(0x3d5)|64);        //Word Mode
	outp(0x3d4,0x14); outp(0x3d5,inp(0x3d5)&(255-64));  //Double Word Mode
	inp(0x3da); outp(0x3c0,0x30); outp(0x3c0,0x71);     //Fix for Line Compare
	loadwalls();
	speed = 240;
	for(i=0;i<256;i++)
		keystatus[i] = 0;
	oldkeyhandler = _dos_getvect(0x9);
	_disable(); _dos_setvect(0x9, keyhandler); _enable();
	olddivzerohandler = _dos_getvect(0);
	_disable(); _dos_setvect(0, divzerohandler); _enable();
	introduction();
	while (keystatus[keydefs[9]] == 0)
	{
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
			mov di, pageoffset
			mov cx, 5400
			mov ax, 0x0707
			rep stosw
			mov cx, 5400
			mov ax, 0x0808
			rep stosw
		}
		if (death < 63)
		{
			_asm \
			{
				mov dx, 0x3c8
				mov al, 0
				out dx, al
				inc dx
				mov bx, 0
				mov ch, death
startfade:
				mov al, palette[bx]
				mul ch
				mov cl, 6
				shr ax, cl
				out dx, al
				inc bx
				cmp bx, 768
				jb startfade
			}
			posz+=2;
			if (posz > 64)
				posz = 64;
			if (angvel < 0)
				angvel -= 20;
			if (angvel > 0)
				angvel += 20;
			ang = (ang+angvel)&2047;
			death -= 2;
			if (death <= 0)
			{
				death = 0;
				_asm \
				{
					mov dx, 0x3c8
					mov al, 0
					out dx, al
					inc dx
					mov bx, 0
fixpalette:
					mov al, palette[bx]
					out dx, al
					inc bx
					cmp bx, 768
					jb fixpalette
				}
				if (lifevests > 0)
				{
					death = 63;
					life = 63;
					statusbardraw((lifevests<<5),31);
					drawlife();
					lifevests--;
					musicon();
					posz = 32;
					posx = startx;
					posy = starty;
					ang = startang;
					angvel = 0;
					vel = 0;
					hvel = 0;
				}
				else
					introduction();
			}
		}
		for(i=0;i<bulnum;i++)
		{
			if (bulkind[i] == 1)
				checkobj(bulx[i],buly[i],posx,posy,ang,39);
			if (bulkind[i] == 2)
				checkobj(bulx[i],buly[i],posx,posy,ang,57);
			if (bulkind[i] == 1)
				if (bulstat[i]+(120+(lightnings<<5)) < totalclock)
				{
					bulnum--;
					youbulnum--;
					bulx[i] = bulx[bulnum];
					buly[i] = buly[bulnum];
					bulang[i] = bulang[bulnum];
					bulstat[i] = bulstat[bulnum];
					bulkind[i] = bulkind[bulnum];
				}
			if (bulkind[i] == 2)
				if (bulstat[i]+240 < totalclock)
				{
					bulnum--;
					bulx[i] = bulx[bulnum];
					buly[i] = buly[bulnum];
					bulang[i] = bulang[bulnum];
					bulstat[i] = bulstat[bulnum];
					bulkind[i] = bulkind[bulnum];
				}
		}
		picrot(posx,posy,posz,ang);
		sortcnt = 0;
		_asm \
		{
			mov dx, 0x3da
retrace:
			in al, dx
			test al, 8
			jnz retrace

			mov dx, 0x3d4
			mov al, 0xc
			out dx, al
			inc dx
			mov al, 11
			cmp pageoffset, 32768
			jb skipotherpage
			add al, 128
skipotherpage:
			cmp vidmode, 0
			jne skipnot320200a
			add al, 7
skipnot320200a:
			out dx, al
			dec dx
			mov al, 0xd
			out dx, al
			inc dx
			mov al, 64
			cmp vidmode, 0
			jne skipnot320200b
			add al, 13
skipnot320200b:
			out dx, al
			mov dx, 0x3da
notretrace:
			in al, dx
			test al, 8
			jz notretrace
			xor pageoffset, 0x8000
		}
		if (totalclock == 0)
		{
			outp(0x3c4,1); outp(0x3c5,inp(0x3c5)&223);
		}
		if (keystatus[51] > 0)
		{
			midiinst = (midiinst + 29) % 30;
			setmidiinsts();
			keystatus[51] = 0;
		}
		if (keystatus[52] > 0)
		{
			midiinst = (midiinst + 1) % 30;
			setmidiinsts();
			keystatus[52] = 0;
		}
		if (keystatus[19] > 0)
			rogermode = rogermode ^ 1;
		if ((keystatus[28] > 0) && (statusbargoal == statusbar))
		{
			if (vidmode == 0)
				statusbargoal = (399+335)-statusbar;
			else
				statusbargoal = (479+415)-statusbar;
		}
		if (statusbar != statusbargoal)
		{
			if (statusbargoal < statusbar)
			{
				statusbar -= (clockspeed>>1);
				if (statusbar <= statusbargoal)
				{
					statusbar = statusbargoal;
					scrsize -= 2880;
				}
			}
			else
			{
				statusbar += (clockspeed>>1);
				if (statusbar >= statusbargoal)
				{
					statusbar = statusbargoal;
					scrsize += 2880;
				}
			}
			linecompare(statusbar);
		}
		if ((keystatus[keydefs[5]] > 0) && (death == 63))
		{
			if ((youbulnum < firepowers) && (lastbulshoot+240-(lightnings<<5) < totalclock))
			{
				bulx[bulnum] = posx;
				buly[bulnum] = posy;
				bulang[bulnum] = ang;
				bulstat[bulnum] = totalclock;
				bulkind[bulnum] = 1;
				lastbulshoot = totalclock;
				youbulnum++;
				bulnum++;
				ksay("shoot.wav");
			}
			if (firepowers == 0)
				ksay("notnow.wav");
		}
		for(i=0;i<bulnum;i++)
		{
			x = (int)((clockspeed*sintable[(bulang[i]+512)&2047])>>13);
			y = (int)((clockspeed*sintable[bulang[i]])>>13);
			for(m=0;m<4;m++)
				if (bulstat[i] > 0)
				{
					bulx[i] += x;
					buly[i] += y;
					if ((bulkind[i] == 2) && (death == 63) && (m < 4))
					{
						l = labs((long)bulx[i]-(long)posx)+labs((long)buly[i]-(long)posy);
						if (l < 768)
						{
							life -= (1+((768-(int)l)>>6));
							if (life <= 0)
							{
								life = 0;
								drawlife();
								death = 62;
								angvel = (rand()&32)-16;
								ksay("death.wav");
								musicoff();
							}
							else
							{
								drawlife();
								angvel = (rand()&32)-16;
								ksay("ouch.wav");
							}
							bulnum--;
							if (bulkind[i] == 1)
								youbulnum--;
							bulx[i] = bulx[bulnum];
							buly[i] = buly[bulnum];
							bulang[i] = bulang[bulnum];
							bulstat[i] = bulstat[bulnum];
							bulkind[i] = bulkind[bulnum];
							m = 4;
						}
					}
					j = mboard[bulx[i]>>10][buly[i]>>10];
					if ((j > 0) && (j != 68) && (bulkind[i] == 1) && (m < 4))
					{
						l = 0;
						for(k=0;k<mnum;k++)
							if (labs((long)bulx[i]-(long)mposx[k])+labs((long)buly[i]-(long)mposy[k]) < 768)
							{
								mshot[k]--;
								if (mshot[k] > 0)
								{
									if (mstat[k] == 54)
										mshock[k] += 30;
									else
										mshock[k] += 60;
									l |= 2;
								}
								if (mshot[k] == 0)
								{
									mboard[moldx[k]>>10][moldy[k]>>10] = 0;
									mboard[mgolx[k]>>10][mgoly[k]>>10] = 0;
									checkobj(mposx[k],mposy[k],posx,posy,ang,44);
									mnum--;
									moldx[k] = moldx[mnum];
									moldy[k] = moldy[mnum];
									mposx[k] = mposx[mnum];
									mposy[k] = mposy[mnum];
									mgolx[k] = mgolx[mnum];
									mgoly[k] = mgoly[mnum];
									mstat[k] = mstat[mnum];
									mshot[k] = mshot[mnum];
									mshock[k] = mshock[mnum];
									l |= 1;
								}
							}
						if (l > 0)
						{
							bulnum--;
							if (bulkind[i] == 1)
								youbulnum--;
							bulx[i] = bulx[bulnum];
							buly[i] = buly[bulnum];
							bulang[i] = bulang[bulnum];
							bulstat[i] = bulstat[bulnum];
							bulkind[i] = bulkind[bulnum];
							m = 4;
							if (death == 63)
							{
								if ((l&1) > 0)
									ksay("blowup.wav");
								if ((l&2) > 0)
									ksay("hitandy.wav");
							}
						}
					}
					j = board[bulx[i]>>10][buly[i]>>10];
					if (bmpkind[j] != 0)
					{
						if ((bulkind[i] == 1) && (j == 5) && (m < 4) && (death == 63))
						{
							life--;
							if (life <= 0)
							{
								life = 0;
								drawlife();
								death = 62;
								angvel = (rand()&32)-16;
								ksay("death.wav");
								musicoff();
							}
							else
							{
								drawlife();
								angvel = (rand()&32)-16;
								ksay("ouch.wav");
							}
							bulnum--;
							if (bulkind[i] == 1)
								youbulnum--;
							bulx[i] = bulx[bulnum];
							buly[i] = buly[bulnum];
							bulang[i] = bulang[bulnum];
							bulstat[i] = bulstat[bulnum];
							bulkind[i] = bulkind[bulnum];
							m = 4;
						}
						if ((bulkind[i] == 1) && (j == 52) && (death == 63))
							ksay("bullseye.wav");
						if ((passthrough[j] != 0) && (j != 40) && (m < 4))
							if ((bmpkind[j] == 1) || ((((bulx[i]&1023)<<2) > lborder[j]) && (((bulx[i]&1023)<<2) < rborder[j]) && (((buly[i]&1023)<<2) > lborder[j]) && (((buly[i]&1023)<<2) < rborder[j])))
							{
								bulnum--;
								if (bulkind[i] == 1)
									youbulnum--;
								bulx[i] = bulx[bulnum];
								buly[i] = buly[bulnum];
								bulang[i] = bulang[bulnum];
								bulstat[i] = bulstat[bulnum];
								bulkind[i] = bulkind[bulnum];
								m = 4;
							}
						if ((j == 40) && (m < 4))
						{
							bulang[i] = ((bulang[i]+1024)&2047);
							while ((board[bulx[i]>>10][buly[i]>>10]&127) == 40)
							{
								bulx[i] -= x;
								buly[i] -= y;
							}
							m = 4;
							if (bulkind[i] == 1)
							{
								if (death == 63)
									ksay("bounce.wav");
								bulstat[i] = totalclock;
							}
						}
					}
				}
		}
		for(i=0;i<mnum;i++)
			if (labs((long)mposx[i]-(long)posx)+labs((long)mposy[i]-(long)posy) < 16384)
			{
				if (mstat[i] == 66)                 //Ken speed
					j = (clockspeed<<2)+(clockspeed<<1);
				if (mstat[i] == 38)                 //Green monster speed
					j = (clockspeed<<3)+(clockspeed<<1);
				if (mstat[i] == 54)                 //Andy speed
					j = (clockspeed<<3)+(clockspeed<<2);
				if (mstat[i] == 68)                 //Ball speed
					j = (clockspeed<<3);

				if (mshock[i] > 0)
				{
					mshock[i] -= clockspeed;
					if (mshock[i] < 0)
						mshock[i] = 0;
				}
				else
				{
					if (mgolx[i] > mposx[i])
					{
						mposx[i] += j;
						if (mposx[i] > mgolx[i])
						mposx[i] = mgolx[i];
					}
					if (mgolx[i] < mposx[i])
					{
						mposx[i] -= j;
						if (mposx[i] < mgolx[i])
							mposx[i] = mgolx[i];
					}
					if (mgoly[i] > mposy[i])
					{
						mposy[i] += j;
						if (mposy[i] > mgoly[i])
							mposy[i] = mgoly[i];
					}
					if (mgoly[i] < mposy[i])
					{
						mposy[i] -= j;
						if (mposy[i] < mgoly[i])
							mposy[i] = mgoly[i];
					}
				}
				if ((labs((long)posx-(long)mposx[i])+labs((long)posy-(long)mposy[i]) < 768) && (death == 63) && (mshock[i] == 0))
				{
					life -= (1+(clockspeed>>3));
					if (life <= 0)
					{
						life = 0;
						drawlife();
						death = 62;
						angvel = (rand()&32)-16;
						ksay("death.wav");
						musicoff();
					}
					else
					{
						drawlife();
						angvel = (rand()&32)-16;
						ksay("ouch.wav");
					}
				}
				if ((mposx[i] == mgolx[i]) && (mposy[i] == mgoly[i]) && (mshock[i] == 0))
				{
					if (board[mposx[i]>>10][mposy[i]>>10] == 73)
					{
						mboard[moldx[i]>>10][moldy[i]>>10] = 0;
						mboard[mgolx[i]>>10][mgoly[i]>>10] = 0;
						if (mstat[i] == 68)
							checkobj(mposx[i],mposy[i],posx,posy,ang,72);
						mnum--;
						moldx[i] = moldx[mnum];
						moldy[i] = moldy[mnum];
						mposx[i] = mposx[mnum];
						mposy[i] = mposy[mnum];
						mgolx[i] = mgolx[mnum];
						mgoly[i] = mgoly[mnum];
						mstat[i] = mstat[mnum];
						mshot[i] = mshot[mnum];
						mshock[i] = mshock[mnum];
						ksay("fall.wav");
					}
					else
					{
						x = (mposx[i]>>10);
						y = (mposy[i]>>10);
						if ((rand()&1) == 0)
						{
							if ((posx < mposx[i]) && (passthrough[board[x-1][y]] == 0) && (board[x-1][y] != 23) && (board[x-1][y] < 128) && (mboard[x-1][y] == 0))
								mgolx[i] = mposx[i]-1024;
							if ((posx > mposx[i]) && (passthrough[board[x+1][y]] == 0) && (board[x+1][y] != 23) && (board[x+1][y] < 128) && (mboard[x+1][y] == 0))
								mgolx[i] = mposx[i]+1024;
						}
						else
						{
							if ((posy < mposy[i]) && (passthrough[board[x][y-1]] == 0) && (board[x][y-1] != 23) && (board[x][y-1] < 128) && (mboard[x][y-1] == 0))
								mgoly[i] = mposy[i]-1024;
							if ((posy > mposy[i]) && (passthrough[board[x][y+1]] == 0) && (board[x][y+1] != 23) && (board[x][y+1] < 128) && (mboard[x][y+1] == 0))
								mgoly[i] = mposy[i]+1024;
						}
						mboard[moldx[i]>>10][moldy[i]>>10] = 0;
						mboard[mposx[i]>>10][mposy[i]>>10] = mstat[i];
						mboard[mgolx[i]>>10][mgoly[i]>>10] = mstat[i];
						moldx[i] = mposx[i];
						moldy[i] = mposy[i];
					}
				}
				j = rand()&2047;
				if (mstat[i] == 38)
					j &= 1023;
				if (mstat[i] == 54)
					j &= 511;
				if ((mstat[i] != 68) && (j < clockspeed) && (bulnum < 32) && (mshock[i] == 0))
				{
					bulx[bulnum] = mposx[i];
					buly[bulnum] = mposy[i];
					k = 512;
					if (mposx[i] != posx)
					{
						tanz = (((((long)mposy[i]-(long)posy)<<12)/((long)mposx[i]-(long)posx))<<4);
						if (tanz < 0)
							k = 768;
						else
							k = 256;
						for (m=128;m>0;m>>=1)
						{
							if (tantable[k] < tanz)
								k += m;
							else
								k -= m;
						}
					}
					if (mposy[i] > posy)
						k += 1024;
					bulang[bulnum] = ((k+2016+(rand()&63))&2047);
					bulstat[bulnum] = totalclock;
					if (mstat[i] == 54)
						bulstat[bulnum] += 240;
					bulkind[bulnum] = 2;
					bulnum++;
				}
			}
		if ((keystatus[keydefs[2]]) && (angvel > -40) && (death == 63))
			angvel -= 14;
		if ((keystatus[keydefs[3]]) && (angvel < 40) && (death == 63))
			angvel += 14;
		if (angvel < 0)
		{
			angvel += 8;
			if (angvel > 0)
				angvel = 0;
		}
		if (angvel > 0)
		{
			angvel -= 8;
			if (angvel < 0)
				angvel = 0;
		}
		if ((rogermode > 0) && (death == 63) && (keystatus[keydefs[2]] == 0) && (keystatus[keydefs[3]] == 0))
			angvel = 0;
		ang = (ang+2048+((angvel*clockspeed)>>3))&2047;
		if ((keystatus[30] > 0) && (hvel > -5) && (death == 63))
			hvel-=2;
		if ((keystatus[44] > 0) && (hvel < 5) && (death == 63))
			hvel+=2;
		if (hvel < 0)
			hvel++;
		if (hvel > 0)
			hvel--;
		posz += ((hvel*clockspeed)>>3);
		if (posz < 8)
		{
			posz = 8;
			hvel = 0;
		}
		if (posz > 56)
		{
			posz = 56;
			hvel = 0;
		}
		if ((keystatus[keydefs[6]] > 0) && (death == 63))
		{
			if (keys > 0)
			{
				x = (posx>>10);
				y = (posy>>10);
				if (board[x-1][y] == 21)
				{
					board[x-1][y] = 0;
					ksay("unlock.wav");
				}
				if (board[x+1][y] == 21)
				{
					board[x+1][y] = 0;
					ksay("unlock.wav");
				}
				if (board[x][y-1] == 21)
				{
					board[x][y-1] = 0;
					ksay("unlock.wav");
				}
				if (board[x][y+1] == 21)
				{
					board[x][y+1] = 0;
					ksay("unlock.wav");
				}
			}
			if (keys == 0)
				ksay("notnow.wav");
		}
		maxvel = 150;
		if (keystatus[keydefs[4]] > 0)
			maxvel = 300;
		if ((keystatus[keydefs[0]]) && (death == 63))
		{
			vel += 40;
			if (vel > maxvel)
				vel = maxvel;
		}
		if ((keystatus[keydefs[1]]) && (death == 63))
		{
			vel -= 40;
			if (vel < -maxvel)
				vel = -maxvel;
		}
		if (vel < 0)
		{
			vel += 25;
			if (vel > 0)
				vel = 0;
		}
		if (vel > 0)
		{
			vel -= 25;
			if (vel < 0)
				vel = 0;
		}
		if (vel != 0)
		{
			oldposx = posx;
			oldposy = posy;
			posx += (int)(((long)vel*clockspeed*sintable[(ang+512)&2047])>>19);
			posy += (int)(((long)vel*clockspeed*sintable[ang])>>19);
			newx = posx, newy = posy;
			if (((newx-128)&0xfc00) < ((oldposx-128)&0xfc00))
			{
				plcx = ((oldposx-128)&0xfc00)+128;
				plcy = oldposy+(int)((((long)oldposx-(long)plcx)*tantable[ang])>>16);
				if (passthrough[board[(plcx>>10)-1][(plcy-128)>>10]] != 0)
					posx = ((oldposx-128)&0xfc00) + 128;
				if (passthrough[board[(plcx>>10)-1][(plcy+128)>>10]] != 0)
					posx = ((oldposx-128)&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy-128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 128;
				if (passthrough[board[(posx+128)>>10][(posy-128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy+128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 895;
				if (passthrough[board[(posx+128)>>10][(posy+128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 895;
			}
			if (((newx+128)&0xfc00) > ((oldposx+128)&0xfc00))
			{
				plcx = ((oldposx+128)&0xfc00)+895;
				plcy = oldposy+(int)((((long)oldposx-(long)plcx)*tantable[ang])>>16);
				if (passthrough[board[(plcx>>10)+1][(plcy-128)>>10]] != 0)
					posx = ((oldposx+128)&0xfc00) + 895;
				if (passthrough[board[(plcx>>10)+1][(plcy+128)>>10]] != 0)
					posx = ((oldposx+128)&0xfc00) + 895;
				if (passthrough[board[(posx-128)>>10][(posy-128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 128;
				if (passthrough[board[(posx+128)>>10][(posy-128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy+128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 895;
				if (passthrough[board[(posx+128)>>10][(posy+128)>>10]] != 0)
					posy = (oldposy&0xfc00) + 895;
			}
			if (((newy-128)&0xfc00) < ((oldposy-128)&0xfc00))
			{
				plcy = ((oldposy-128)&0xfc00)+128;
				plcx = oldposx+(int)((((long)oldposy-(long)plcy)*tantable[(2560-ang)&2047])>>16);
				if (passthrough[board[(plcx-128)>>10][(plcy>>10)-1]] != 0)
					posy = ((oldposy-128)&0xfc00) + 128;
				if (passthrough[board[(plcx+128)>>10][(plcy>>10)-1]] != 0)
					posy = ((oldposy-128)&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy-128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy+128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 128;
				if (passthrough[board[(posx+128)>>10][(posy-128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 895;
				if (passthrough[board[(posx+128)>>10][(posy+128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 895;
			}
			if (((newy+128)&0xfc00) > ((oldposy+128)&0xfc00))
			{
				plcy = ((oldposy+128)&0xfc00)+895;
				plcx = oldposx+(int)((((long)oldposy-(long)plcy)*tantable[(2560-ang)&2047])>>16);
				if (passthrough[board[(plcx-128)>>10][(plcy>>10)+1]] != 0)
					posy = ((oldposy+128)&0xfc00) + 895;
				if (passthrough[board[(plcx+128)>>10][(plcy>>10)+1]] != 0)
					posy = ((oldposy+128)&0xfc00) + 895;
				if (passthrough[board[(posx-128)>>10][(posy-128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 128;
				if (passthrough[board[(posx-128)>>10][(posy+128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 128;
				if (passthrough[board[(posx+128)>>10][(posy-128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 895;
				if (passthrough[board[(posx+128)>>10][(posy+128)>>10]] != 0)
					posx = (oldposx&0xfc00) + 895;
			}
		}
		x = (posx>>10);
		y = (posy>>10);
		i = (board[x][y]&127);
		if ((i == 32) || ((keystatus[42] > 0) && (keystatus[18] > 0) && (keystatus[54] > 0)))
		{
			lifevests++;
			if (lifevests > 4)
				lifevests = 4;
			if (i == 32)
				board[x][y] = 0;
			if (death == 63)
				ksay("getstuff.wav");
			statusbardraw((lifevests<<5),32);
		}
		if ((i == 36) || ((keystatus[42] > 0) && (keystatus[38] > 0) && (keystatus[54] > 0)))
		{
			lightnings++;
			if (lightnings > 6)
				lightnings = 6;
			if (i == 36)
				board[x][y] = 0;
			if (death == 63)
				ksay("getstuff.wav");
			statusbardraw(305-((lightnings<<5)-(lightnings<<3)),36);
		}
		if ((i == 39) || ((keystatus[42] > 0) && (keystatus[33] > 0) && (keystatus[54] > 0)))
		{
			firepowers++;
			if (firepowers > 6)
				firepowers = 6;
			if (i == 39)
				board[x][y] = 0;
			if (death == 63)
				ksay("getstuff.wav");
			statusbardraw(316-((firepowers<<5)-(firepowers<<3)),39);
		}
		if ((i == 45) || ((keystatus[42] > 0) && (keystatus[37] > 0) && (keystatus[54] > 0)))
		{
			keys++;
			if (i == 45)
				board[x][y] = 0;
			if (death == 63)
				ksay("getstuff.wav");
			statusbardraw(0,45);
		}
		if (((i == 46) || (i == 59) || (i == 60)) || ((keystatus[42] > 0) && (keystatus[31] > 0) && (keystatus[54] > 0)))
		{
			life += 5;
			if (i == 46)
				life += 5;
			if (i == 60)
				life += 15;
			if (life > 63)
				life = 63;
			drawlife();
			if ((i == 46) || (i == 59) || (i == 60))
				board[x][y] = 0;
			if (death == 63)
				ksay("getstuff.wav");
		}
		if ((i == 63) || ((keystatus[42] > 0) && (keystatus[48] > 0) && (keystatus[54] > 0)))
		{
			ksay("congrats.wav");
			musicoff();
			if (boardnum < numboards-1)
			{
				boardnum++;
				statusbardraw(0,31);
				loadboard();
				filename[0] = 'W', filename[1] = 'A', filename[2] = 'L';
				filename[3] = 'S', filename[4] = 'N', filename[5] = 'G';
				filename[6] = (boardnum/10)+48, filename[7] = (boardnum%10)+48;
				filename[8] = '.', filename[9] = 'K', filename[10] = 'S';
				filename[11] = 'M', filename[12] = 0;
				loadmusic(filename);
				musicon();
			}
			else
				keystatus[keydefs[9]] = 1;
		}
		if ((i == 49) && (death == 63))
		{
			life -= (1+(clockspeed>>3));
			if (life <= 0)
			{
				life = 0;
				drawlife();
				death = 62;
				angvel = (rand()&32)-16;
				ksay("death.wav");
				musicoff();
			}
			else
			{
				drawlife();
				angvel = (rand()&32)-16;
				ksay("hitfan.wav");
			}
		}
		if ((i == 73) && (abs(512-(posx&1023))+abs(512-(posy&1023)) < 384) && (death == 63))
		{
			life = 0;
			drawlife();
			death = 62;
			angvel = 0;
			ksay("fall.wav");
			musicoff();
		}
		if ((keystatus[42] == 0) || (keystatus[54] == 0))
		{
			if (keystatus[keydefs[8]] > 0)
			{
				_asm xor pageoffset, 0x8000
				for(i=lside;i<rside;i++)
					height[i] = 0;
				spridraw((int)180-64,(int)120-64,(int)128,(int)79);
				_asm xor pageoffset, 0x8000
				j = 0;
				while ((j == 0) && (keystatus[1] == 0) && (keystatus[57] == 0))
				{
					for(i=2;i<10;i++)
						if (keystatus[i] > 0)
						{
							savegame(i-2);
							j = 1;
						}
				}
			}
			if (keystatus[keydefs[7]] > 0)
			{
				_asm xor pageoffset, 0x8000
				for(i=lside;i<rside;i++)
					height[i] = 0;
				spridraw((int)180-64,(int)120-64,(int)128,(int)78);
				_asm xor pageoffset, 0x8000
				j = 0;
				while ((j == 0) && (keystatus[1] == 0) && (keystatus[57] == 0))
				{
					for(i=2;i<10;i++)
						if (keystatus[i] > 0)
						{
							loadgame(i-2);
							j = 1;
						}
				}
			}
		}
		totalclock += clockspeed;
		clockspeed = 0;
	}
	_disable(); _dos_setvect(0x9, oldkeyhandler); _enable();
	_disable(); _dos_setvect(0, olddivzerohandler); _enable();
	musicoff();
	if (musicsource == 1)
	{
		midiscrap = 256;
		while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
			midiscrap--;
		outp(statport,0xff);
		midiscrap = 256;
		while ((midiscrap > 0) && ((inp(dataport) & 0xfe) != 0))
			midiscrap--;
	}
	if ((pic = convdeallocate(pic)) == -1)
	{
		printf("Conventional memory not released!\n");
		exit(-1);
	}
	if (convwalls < numwalls)
		if (emmdeallocate() == -1)
		{
			printf("EMS memory not released!");
			exit(-1);
		}
	_asm \
	{
		mov ax, 0x3
		int 0x10
	}
}

picrot(posx, posy, posz, ang)
unsigned int posx, posy;
int posz, ang;
{
	char stat1, stat2;
	int i, j, k, x, y, yf0, picplc, yc0;
	int linesatonce, yy, endyy, temp, angle, xc, yc;
	long x1, y1, x2, y2, yinc, xinc, xdir, ydir, tan1, tan2;
	unsigned int yl0, picseg;

	_asm \
	{
		mov ax, 0xa000
		mov es, ax
	}
	if (vidmode == 0)
	{
		lincalc[0] = 20;
		lincalc[1] = 276;
		lincalc[2] = 340;
		lincalc[3] = 148;
		lincalc[4] = 308;
		linbits[0] = 0;
		linbits[1] = 0;
		linbits[2] = 0;
		linbits[3] = 128;
		linbits[4] = 32;
		lastline = 5;
	}
	else
	{
		lincalc[0] = 0;
		lincalc[1] = 256;
		lincalc[2] = 320;
		lincalc[3] = 352;
		lincalc[4] = 356;
		lincalc[5] = 357;
		lincalc[6] = 358;
		lincalc[7] = 359;
		lincalc[8] = 128;
		lincalc[9] = 288;
		lincalc[10] = 336;
		lincalc[11] = 354;
		linbits[0] = 0;
		linbits[1] = 0;
		linbits[2] = 0;
		linbits[3] = 0;
		linbits[4] = 0;
		linbits[5] = 0;
		linbits[6] = 0;
		linbits[7] = 0;
		linbits[8] = 128;
		linbits[9] = 32;
		linbits[10] = 16;
		linbits[11] = 2;
		lastline = 12;
	}
	numline = 0;
	while (numline < lastline)
	{
		i = lincalc[numline];
		angle = (radarang[i]+ang+2048)&2047;
		tan1 = tantable[(2560-angle)&2047];
		tan2 = tantable[angle];
		if (tan1 != 0)
		{
			x1 = (((long)(posx&0xfc00))<<6);
			yinc = tan2;
			xdir = 65536;
			if (((angle+512)&1024) > 0)
			{
				xdir = -65536;
				yinc = -yinc;
				x1 += 65535;
			}
			y1 = ((x1>>6)-(long)posx);
			if ((tan2 > -65536) && (tan2 < 65536))
				y1 = ((y1*tan2)>>10);
			else
				y1 = ((y1*(tan2>>4))>>6);
			y1 += (((long)posy)<<6);
		}
		if (tan2 != 0)
		{
			y2 = (((long)(posy&0xfc00))<<6);
			xinc = tan1;
			ydir = 65536;
			if ((angle&1024) > 0)
			{
				ydir = -65536;
				xinc = -xinc;
				y2 += 65535;
			}
			x2 = ((y2>>6)-(long)posy);
			if ((tan1 > -65536) && (tan1 < 65536))
				x2 = ((x2*tan1)>>10);
			else
				x2 = ((x2*(tan1>>4))>>6);
			x2 += (((long)posx)<<6);
		}
		stat1 = 0;
		stat2 = 0;
		_asm \
		{
			mov ax, xdir[0]
			mov dx, xdir[2]
			add x1[0], ax
			adc x1[2], dx
			mov ax, yinc[0]
			mov dx, yinc[2]
			add y1[0], ax
			adc y1[2], dx
			mov ax, xinc[0]
			mov dx, xinc[2]
			add x2[0], ax
			adc x2[2], dx
			mov ax, ydir[0]
			mov dx, ydir[2]
			add y2[0], ax
			adc y2[2], dx
		}
		if (tan2 == 0)
			_asm \
			{
begingetline1:
				mov ax, x1[2]
				mov cl, 6
				shl ax, cl
				mov si, y1[2]
				add si, ax
				mov bl, byte ptr ds:board[si]
				mov byte ptr ds:tempbuf[si], 1
				and bx, 127
				mov al, byte ptr ds:bmpkind[bx]
				mov stat1, al
				cmp al, 1
				je endcheck1
				mov ax, xdir[0]
				mov dx, xdir[2]
				add x1[0], ax
				adc x1[2], dx
				jmp begingetline1
endcheck1:
			}
		else if (tan1 == 0)
			_asm \
			{
begingetline2:
				mov ax, x2[2]
				mov cl, 6
				shl ax, cl
				mov si, y2[2]
				add si, ax
				mov bl, byte ptr ds:board[si]
				mov byte ptr ds:tempbuf[si], 1
				and bx, 127
				mov al, byte ptr ds:bmpkind[bx]
				mov stat2, al
				cmp al, 1
				je endcheck2
				mov ax, ydir[0]
				mov dx, ydir[2]
				add y2[0], ax
				adc y2[2], dx
				jmp begingetline2
endcheck2:
			}
		else if (labs(yinc) < labs(xinc))
			while (((stat1|stat2)&1) == 0)
			{
				if ((x1<x2) != (xdir<0))
					_asm \
					{
						mov ax, x1[2]
						mov cl, 6
						shl ax, cl
						mov si, y1[2]
						add si, ax
						mov bl, byte ptr ds:board[si]
						mov byte ptr ds:tempbuf[si], 1
						and bx, 127
						mov al, byte ptr ds:bmpkind[bx]
						mov stat1, al
						cmp al, 1
						je skipcheck11
						mov ax, xdir[0]
						mov dx, xdir[2]
						add x1[0], ax
						adc x1[2], dx
						mov ax, yinc[0]
						mov dx, yinc[2]
						add y1[0], ax
						adc y1[2], dx
skipcheck11:
					}
				else
					_asm \
					{
						mov ax, x2[2]
						mov cl, 6
						shl ax, cl
						mov si, y2[2]
						add si, ax
						mov bl, byte ptr ds:board[si]
						mov byte ptr ds:tempbuf[si], 1
						and bx, 127
						mov al, byte ptr ds:bmpkind[bx]
						mov stat2, al
						cmp al, 1
						je skipcheck12
						mov ax, xinc[0]
						mov dx, xinc[2]
						add x2[0], ax
						adc x2[2], dx
						mov ax, ydir[0]
						mov dx, ydir[2]
						add y2[0], ax
						adc y2[2], dx
skipcheck12:
					}
			}
		else
			while (((stat1|stat2)&1) == 0)
			{
				if ((y2<y1) != (ydir<0))
					_asm \
					{
						mov ax, x2[2]
						mov cl, 6
						shl ax, cl
						mov si, y2[2]
						add si, ax
						mov bl, byte ptr ds:board[si]
						mov byte ptr ds:tempbuf[si], 1
						and bx, 127
						mov al, byte ptr ds:bmpkind[bx]
						mov stat2, al
						cmp al, 1
						je skipcheck21
						mov ax, xinc[0]
						mov dx, xinc[2]
						add x2[0], ax
						adc x2[2], dx
						mov ax, ydir[0]
						mov dx, ydir[2]
						add y2[0], ax
						adc y2[2], dx
skipcheck21:
					}
				else
					_asm \
					{
						mov ax, x1[2]
						mov cl, 6
						shl ax, cl
						mov si, y1[2]
						add si, ax
						mov bl, byte ptr ds:board[si]
						mov byte ptr ds:tempbuf[si], 1
						and bx, 127
						mov al, byte ptr ds:bmpkind[bx]
						mov stat1, al
						cmp al, 1
						je skipcheck22
						mov ax, xdir[0]
						mov dx, xdir[2]
						add x1[0], ax
						adc x1[2], dx
						mov ax, yinc[0]
						mov dx, yinc[2]
						add y1[0], ax
						adc y1[2], dx
skipcheck22:
					}
			}
		if (stat1 == 1)
		{
			yl0 = (int)(((((x1>>6)-(long)posx)>>2)*sintable[(ang+512)&2047]+(((y1>>6)-(long)posy)>>2)*sintable[ang])>>16);
			j = ((int)(board[x1>>16][y1>>16]-1)&127);
			picplc = (int)((y1>>4)&0xfff);
			if (((angle+512)&2047) >= 1024)
				picplc ^= 0xfff;
			linplc[i] = (((unsigned)(x1>>16))<<6)+(unsigned)(y1>>16);
		}
		if (stat2 == 1)
		{
			yl0 = (int)(((((x2>>6)-(long)posx)>>2)*sintable[(ang+512)&2047]+(((y2>>6)-(long)posy)>>2)*sintable[ang])>>16);
			j = ((int)(board[x2>>16][y2>>16]-1)&127);
			picplc = (int)((x2>>4)&0xfff);
			if (angle < 1024)
				picplc ^= 0xfff;
			linplc[i] = (((unsigned)(x2>>16))<<6)+(unsigned)(y2>>16)+4096;
		}
		height[i] = 0;
		if (yl0 != 0)
			height[i] = 40960/yl0;
		linum[i] = picplc;
		walnum[i] = j;
		if (linbits[numline] > 1)
		{
			if ((linplc[i-linbits[numline]] == linplc[i]) && (linbits[numline] <= 16))
			{
				for(j=(i-linbits[numline]+1);j<i;j++)
					linplc[j] = -1;
				for(j=(linbits[numline]>>1);j>0;j>>=1)
					for(k=(i-linbits[numline])+j;k<i;k+=j)
						if (linplc[k] == -1)
						{
							height[k] = ((height[k-j]+height[k+j])>>1);
							linum[k] = ((linum[k-j]+linum[k+j])>>1);
							walnum[k] = walnum[i];
							linplc[k] = linplc[i];
						}
			}
			else
			{
				linbits[lastline] = (linbits[numline]>>1);
				lincalc[lastline] = i-linbits[lastline];
				lastline++;
			}
			if ((linplc[i+linbits[numline]] == linplc[i]) && (linbits[numline] <= 16))
			{
				for(j=(i+linbits[numline]-1);j>i;j--)
					linplc[j] = -1;
				for(j=(linbits[numline]>>1);j>0;j>>=1)
					for(k=(i+linbits[numline])-j;k>i;k-=j)
						if (linplc[k] == -1)
						{
							height[k] = ((height[k-j]+height[k+j])>>1);
							linum[k] = ((linum[k-j]+linum[k+j])>>1);
							walnum[k] = walnum[i];
							linplc[k] = linplc[i];
						}
			}
			else
			{
				linbits[lastline] = (linbits[numline]>>1);
				lincalc[lastline] = i+linbits[lastline];
				lastline++;
			}
		}
		numline++;
	}
	outp(0x3c4,0x2);
	for(i=lside;i<rside;i++)
		linum[i] &= 0xfc0;
	for(i=lside;i<rside;i++)
	{
		yf0 = height[i];
		picplc = linum[i];
		picseg = walseg[walnum[i]];
		if (memtype[walnum[i]] != 255)
			setpages(memtype[walnum[i]],0);
		outp(0x3c5,1<<(i&3));
		linesatonce = 0;
		if (((i&3) < 3) && (linum[i] == linum[i+1]) && (linplc[i] == linplc[i+1]))
		{
			outp(0x3c5,3<<(i&3));
			linesatonce = 1;
		}
		if (((i&3) < 2) && (linesatonce == 1) && (linum[i] == linum[i+2]) && (linplc[i] == linplc[i+2]))
		{
			outp(0x3c5,7<<(i&3));
			linesatonce = 2;
		}
		if (((i&3) == 0) && (linesatonce == 2) && (linum[i] == linum[i+3]) && (linplc[i] == linplc[i+3]))
		{
			outp(0x3c5,15);
			linesatonce = 3;
		}
		if ((yf0 > 0) && (yf0 < 64))
		{
			yy = 120-((yf0*posz)>>6);
			yy = times90[yy]+(i>>2)+pageoffset;
			_asm \
			{
				mov bx, yf0
				shl bx, 1
				mov cx, less64inc[bx]
				mov si, picplc
				mov dx, cx
				shr dx, 1
				mov bh, 0
				mov di, yy
				push ds
				mov ds, picseg
start1:
				mov bl, dh
				mov al, ds:[si+bx]
				stosb
				add di, 89
				add dx, cx
				cmp dh, 63
				jbe start1
				pop ds
			}
		}
		else if ((yf0 >= 64) && (yf0 < 120))
		{
			yy = 120-((yf0*posz)>>6);
			yy = times90[yy]+(i>>2)+pageoffset;
			_asm \
			{
				mov dx, 0
				mov si, picplc
				mov bx, yf0
				mov cl, 3
				shl bx, cl
				mov cx, si
				add cx, 64
				mov di, yy
				push ds
				mov ds, picseg
start2:
				lodsw
				cmp si, cx
				jbe start2a
				jmp endit2
start2a:
				mov dh, 0
				add dx, bx
				cmp dh, 3
				je start2b
				ja start2c
				mov es:[di], al
				mov es:[di+90], ah
				add di, 180
				jmp start2
start2b:
				mov es:[di], al
				mov es:[di+90], al
				mov es:[di+180], ah
				add di, 270
				jmp start2
start2c:
				mov es:[di], al
				mov es:[di+90], al
				mov es:[di+180], ah
				mov es:[di+270], ah
				add di, 360
				jmp start2
endit2:
				pop ds
			}
		}
		else if (yf0 >= 120)
		{
			yy = 120-(((long)yf0*posz)>>6);
			endyy = yy + yf0;
			if (yy < 0)
			{
				_asm \
				{
					mov ax, yy
					neg ax
					mov cl, 6
					rol ax, cl
					mov dx, ax
					and al, 0xc0
					and dx, 0x0f
					mov cx, yf0
					div cx
					add picplc, ax          ;dividend
					mov ax, yf0
					sub ax, dx              ;remainder
					mov yc0, ax
				}
				yy = (i>>2)+pageoffset;
			}
			else
			{
				yc0 = yf0-1;
				yy = times90[yy]+(i>>2)+pageoffset;
			}
			if (endyy <= 240)
			{
				_asm \
				{
					mov dx, yc0
					mov si, picplc
					mov cx, si
					and cx, 0x0fc0
					add cx, 64
					mov bx, yf0
					mov di, yy
					mov ax, bx
					dec ax
				}
				if (yf0 < 128)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart3:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart3
						mov dx, 0
start3:
						lodsw
						cmp si, cx
						jb start3a
						mov ah, 8
						je start3a
						jmp endit3
start3a:
						mov dh, 0
						add dx, bx
						cmp dh, 3
						je start3b
						ja start3c
						mov es:[di], al
						mov es:[di+90], ah
						add di, 180
						jmp start3
start3b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						add di, 270
						jmp start3
start3c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						mov es:[di+270], ah
						add di, 360
						jmp start3
endit3:
						pop ds
					}
				else if (yf0 < 192)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart4:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart4
						mov dx, 0
start4:
						lodsw
						cmp si, cx
						jb start4a
						mov ah, 8
						je start4a
						jmp endit4
start4a:
						mov dh, 0
						add dx, bx
						cmp dh, 5
						je start4b
						ja start4c
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						mov es:[di+270], ah
						add di, 360
						jmp start4
start4b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						add di, 450
						jmp start4
start4c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						mov es:[di+450], ah
						add di, 540
						jmp start4
endit4:
						pop ds
					}
				else if (yf0 < 256)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart5:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart5
						mov dx, 0
start5:
						lodsw
						cmp si, cx
						jb start5a
						mov ah, 8
						je start5a
						jmp endit5
start5a:
						mov dh, 0
						add dx, bx
						cmp dh, 7
						je start5b
						ja start5c
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						mov es:[di+450], ah
						add di, 540
						jmp start5
start5b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						mov es:[di+360], ah
						mov es:[di+450], ah
						mov es:[di+540], ah
						add di, 630
						jmp start5
start5c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						mov es:[di+360], ah
						mov es:[di+450], ah
						mov es:[di+540], ah
						mov es:[di+630], ah
						add di, 720
						jmp start5
endit5:
						pop ds
					}
				else
					_asm \
					{
						push ds
						mov ds, picseg
						lodsb
						jmp start6b
start6:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						add di, 360
						sub dx, 256
						js start6c
						or dh, dh
						jnz start6
start6b:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns start6b
start6c:
						lodsb
						add dx, bx
						cmp si, cx
						jbe start6
						pop ds
					}
			}
			else
			{
				_asm \
				{
					mov dx, yc0
					mov si, picplc
					mov cx, pageoffset
					add cx, scrsize
					mov bx, yf0
					mov di, yy
				}
				if (yf0 < 128)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart7:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart7
						mov dx, 0
start7:
						lodsw
						cmp di, cx
						jbe start7a
						jmp endit7
start7a:
						mov dh, 0
						add dx, bx
						cmp dh, 3
						je start7b
						ja start7c
						mov es:[di], al
						mov es:[di+90], ah
						add di, 180
						jmp start7
start7b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						add di, 270
						jmp start7
start7c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						mov es:[di+270], ah
						add di, 360
						jmp start7
endit7:
						pop ds
					}
				else if (yf0 < 192)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart8:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart8
						mov dx, 0
start8:
						lodsw
						cmp di, cx
						jbe start8a
						jmp endit8
start8a:
						mov dh, 0
						add dx, bx
						cmp dh, 5
						je start8b
						ja start8c
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], ah
						mov es:[di+270], ah
						add di, 360
						jmp start8
start8b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						add di, 450
						jmp start8
start8c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						mov es:[di+450], ah
						add di, 540
						jmp start8
endit8:
						pop ds
					}
				else if (yf0 < 256)
					_asm \
					{
						inc cx
						shl bx, 1
						shl bx, 1
						shl bx, 1
						push ds
						mov ds, picseg
						lodsb
prestart9:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns prestart9
						mov dx, 0
start9:
						lodsw
						cmp di, cx
						jbe start9a
						jmp endit9
start9a:
						mov dh, 0
						add dx, bx
						cmp dh, 7
						je start9b
						ja start9c
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], ah
						mov es:[di+360], ah
						mov es:[di+450], ah
						add di, 540
						jmp start9
start9b:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						mov es:[di+360], ah
						mov es:[di+450], ah
						mov es:[di+540], ah
						add di, 630
						jmp start9
start9c:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						mov es:[di+360], ah
						mov es:[di+450], ah
						mov es:[di+540], ah
						mov es:[di+630], ah
						add di, 720
						jmp start9
endit9:
						pop ds
					}
				else
					_asm \
					{
						push ds
						mov ds, picseg
						lodsb
						jmp start10b
start10:
						mov es:[di], al
						mov es:[di+90], al
						mov es:[di+180], al
						mov es:[di+270], al
						add di, 360
						cmp di, cx
						jae endit10
						sub dx, 256
						js start10c
						or dh, dh
						jnz start10
start10b:
						mov es:[di], al
						add di, 90
						sub dx, 64
						jns start10b
start10c:
						lodsb
						add dx, bx
						jmp start10
endit10:
						pop ds
					}
			}
		}
		i += linesatonce;
	}
	kenpos++;
	if (kenpos == 68)
		kenpos = 65;
	ballpos++;
	if (ballpos == 72)
		ballpos = 68;
	for(i=0;i<mnum;i++)
	{
		xc = (mposx[i]>>10);
		yc = (mposy[i]>>10);
		if (tempbuf[(xc<<6)+yc] != 0)
		{
			if (mboard[xc][yc] == 66)
				checkobj(mposx[i],mposy[i],posx,posy,ang,kenpos);
			if (mboard[xc][yc] == 68)
				checkobj(mposx[i],mposy[i],posx,posy,ang,ballpos);
			if (mboard[xc][yc] == 38)
			{
				if (mshock[i] > 0)
					checkobj(mposx[i],mposy[i],posx,posy,ang,61);
				else
					checkobj(mposx[i],mposy[i],posx,posy,ang,38);
			}
			if (mboard[xc][yc] == 54)
			{
				if (mshock[i] > 0)
					checkobj(mposx[i],mposy[i],posx,posy,ang,56);
				else
				{
					j = 54;
					if ((posx > mposx[i]) == (posy > mposy[i]))
					{
						if ((mgolx[i] < moldx[i]) || (mgoly[i] > moldy[i]))
							j = 53;
						if ((mgolx[i] > moldx[i]) || (mgoly[i] < moldy[i]))
							j = 55;
						if ((posx < mposx[i]) && (posy < mposy[i]))
							j = 108 - j;
					}
					if ((posx > mposx[i]) != (posy > mposy[i]))
					{
						if ((mgolx[i] < moldx[i]) || (mgoly[i] < moldy[i]))
							j = 53;
						if ((mgolx[i] > moldx[i]) || (mgoly[i] > moldy[i]))
							j = 55;
						if ((posx > mposx[i]) && (posy < mposy[i]))
							j = 108 - j;
					}
					checkobj(mposx[i],mposy[i],posx,posy,ang,j);
				}
			}
		}
	}
	flowerpos++;
	if (flowerpos == 52)
		flowerpos = 49;
	for(xc=0;xc<64;xc++)
		for(yc=0;yc<64;yc++)
			if (tempbuf[(xc<<6)+yc] != 0)
			{
				tempbuf[(xc<<6)+yc] = 0;
				i = (board[xc][yc]&127);
				if (bmpkind[i] == 2)
				{
					if (i == 49)
						checkobj((xc<<10)+512,(yc<<10)+512,posx,posy,ang,flowerpos);
					else
						checkobj((xc<<10)+512,(yc<<10)+512,posx,posy,ang,i);
				}
			}
	totalsortcnt = sortcnt;
	for(xc=0;xc<totalsortcnt;xc++)
	{
		temp = 0;
		for(yc=0;yc<sortcnt;yc++)
			if (sorti[yc] < sorti[temp])
				temp = yc;
		spridraw(sortang[temp]-(sorti[temp]>>1),120-((sorti[temp]*posz)>>6),sorti[temp],sortbnum[temp]);
		sortang[temp] = sortang[sortcnt-1];
		sorti[temp] = sorti[sortcnt-1];
		sortbnum[temp] = sortbnum[sortcnt-1];
		sortcnt--;
	}
}

spridraw(x, y, siz, walnum)
int x, y, siz, walnum;
{
	int i, j, k, xc, picplc, yc0, linesatonce;
	unsigned int picseg, plc;

	_asm \
	{
		mov ax, 0xa000
		mov es, ax
	}
	if (memtype[walnum-1] != 255)
		setpages(memtype[walnum-1],0);
	picseg = walseg[walnum-1];
	picplc = 0;
	j = lborder[walnum-1];
	k = rborder[walnum-1];
	outp(0x3c4,0x2);
	xc = 0;
	if (siz < 64)
	{
		plc = times90[y]+(x>>2)+pageoffset;
		for(i=0;i<64;i++)
		{
			if (xc >= 64)
			{
				xc -= 64;
				x++;
				plc += ((x&3) == 0);
				if ((picplc >= j) && (picplc <= k) && (x >= lside) && (x < rside) && (siz >= height[x]))
				{
					outp(0x3c5,1<<(x&3));
					_asm \
					{
						mov bx, siz
						shl bx, 1
						mov cx, less64inc[bx]
						mov si, picplc
						mov dx, cx
						shr dx, 1
						mov bh, 0
						mov di, plc
						push ds
						mov ds, picseg
startspri1:
						mov bl, dh
						mov al, ds:[si+bx]
						cmp al, 252
						jae skipitspri1
						mov es:[di], al
skipitspri1:
						add di, 90
						add dx, cx
						cmp dh, 63
						jbe startspri1
						pop ds
					}
				}
			}
			picplc += 64;
			xc += siz;
		}
	}
	else if (siz >= 64)
	{
		i = y+siz;
		yc0 = siz-1;
		if (y < 0)
		{
			_asm \
			{
				mov ax, y
				neg ax
				mov cl, 6
				rol ax, cl
				mov dx, ax
				and al, 0xc0
				and dx, 0x0f
				mov cx, siz
				div cx
				add picplc, ax           ;dividend
				mov ax, siz
				sub ax, dx
				mov yc0, ax              ;remainder
			}
			y = 0;
		}
		if (i < 240)
		{
			for(i=x;i<x+siz;i++)
			{
				if ((i >= lside) && (i < rside))
				{
					if ((siz >= height[i]) && (picplc >= j) && (picplc <= k))
						sprinum[i] = picplc;
					else
						sprinum[i] = 65535;
				}
				xc += 64;
				if (xc >= siz)
				{
					xc -= siz;
					picplc += 64;
				}
			}
			if ((x+siz) < rside)
				sprinum[x+siz] = 65534;
			for(i=x;i<x+siz;i++)
				if ((i >= lside) && (i < rside) && (sprinum[i] != 65535))
				{
					picplc = sprinum[i];
					outp(0x3c5,1<<(i&3));
					linesatonce = 0;
					if (((i&3) < 3) && (sprinum[i] == sprinum[i+1]))
					{
						outp(0x3c5,3<<(i&3));
						linesatonce = 1;
					}
					if (((i&3) < 2) && (linesatonce == 1) && (sprinum[i] == sprinum[i+2]))
					{
						outp(0x3c5,7<<(i&3));
						linesatonce = 2;
					}
					if (((i&3) == 0) && (linesatonce == 2) && (sprinum[i] == sprinum[i+3]))
					{
						outp(0x3c5,15);
						linesatonce = 3;
					}
					plc = times90[y]+(i>>2)+pageoffset;
					if (siz < 128)
						_asm \
						{
							mov dx, yc0
							mov si, picplc
							mov cx, si
							and cx, 0x0fc0
							add cx, 65
							mov bx, siz
							shl bx, 1
							shl bx, 1
							shl bx, 1
							mov di, plc
							push ds
							mov ds, picseg
startspri2:
							lodsw
							cmp si, cx
							jb startspri2a
							mov ah, 255
							je startspri2a
							jmp enditspri2
startspri2a:
							mov dh, 0
							add dx, bx
							cmp dh, 3
							je startspri2b
							ja startspri2c
							cmp al, 252
							jae skipspri2a
							mov es:[di], al
skipspri2a:
							cmp ah, 252
							jae skipspri2b
							mov es:[di+90], ah
skipspri2b:
							add di, 180
							jmp startspri2
startspri2b:
							cmp al, 252
							jae skipspri2c
							mov es:[di], al
							mov es:[di+90], al
skipspri2c:
							cmp ah, 252
							jae skipspri2d
							mov es:[di+180], ah
skipspri2d:
							add di, 270
							jmp startspri2
startspri2c:
							cmp al, 252
							jae skipspri2e
							mov es:[di], al
							mov es:[di+90], al
skipspri2e:
							cmp ah, 252
							jae skipspri2f
							mov es:[di+180], ah
							mov es:[di+270], ah
skipspri2f:
							add di, 360
							jmp startspri2
enditspri2:
							pop ds
						}
					else if (siz < 192)
						_asm \
						{
							mov dx, yc0
							mov si, picplc
							mov cx, si
							and cx, 0x0fc0
							add cx, 65
							mov bx, siz
							shl bx, 1
							shl bx, 1
							shl bx, 1
							mov di, plc
							push ds
							mov ds, picseg
startspri3:
							lodsw
							cmp si, cx
							jb startspri3a
							mov ah, 255
							je startspri3a
							jmp enditspri3
startspri3a:
							mov dh, 0
							add dx, bx
							cmp dh, 5
							je startspri3b
							ja startspri3c
							cmp al, 252
							jae skipspri3a
							mov es:[di], al
							mov es:[di+90], al
skipspri3a:
							cmp ah, 252
							jae skipspri3b
							mov es:[di+180], ah
							mov es:[di+270], ah
skipspri3b:
							add di, 360
							jmp startspri3
startspri3b:
							cmp al, 252
							jae skipspri3c
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
skipspri3c:
							cmp ah, 252
							jae skipspri3d
							mov es:[di+270], ah
							mov es:[di+360], ah
skipspri3d:
							add di, 450
							jmp startspri3
startspri3c:
							cmp al, 252
							jae skipspri3e
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
skipspri3e:
							cmp ah, 252
							jae skipspri3f
							mov es:[di+270], ah
							mov es:[di+360], ah
							mov es:[di+450], ah
skipspri3f:
							add di, 540
							jmp startspri3
enditspri3:
							pop ds
						}
					else if (siz < 256)
						_asm \
						{
							mov dx, yc0
							mov si, picplc
							mov cx, si
							and cx, 0x0fc0
							add cx, 65
							mov bx, siz
							shl bx, 1
							shl bx, 1
							shl bx, 1
							mov di, plc
							push ds
							mov ds, picseg
startspri4:
							lodsw
							cmp si, cx
							jb startspri4a
							mov ah, 255
							je startspri4a
							jmp enditspri4
startspri4a:
							mov dh, 0
							add dx, bx
							cmp dh, 7
							je startspri4b
							ja startspri4c
							cmp al, 252
							jae skipspri4a
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
skipspri4a:
							cmp ah, 252
							jae skipspri4b
							mov es:[di+270], ah
							mov es:[di+360], ah
							mov es:[di+450], ah
skipspri4b:
							add di, 540
							jmp startspri4
startspri4b:
							cmp al, 252
							jae skipspri4c
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
							mov es:[di+270], al
skipspri4c:
							cmp ah, 252
							jae skipspri4d
							mov es:[di+360], ah
							mov es:[di+450], ah
							mov es:[di+540], ah
skipspri4d:
							add di, 630
							jmp startspri4
startspri4c:
							cmp al, 252
							jae skipspri4e
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
							mov es:[di+270], al
skipspri4e:
							cmp ah, 252
							jae skipspri4f
							mov es:[di+360], ah
							mov es:[di+450], ah
							mov es:[di+540], ah
							mov es:[di+630], ah
skipspri4f:
							add di, 720
							jmp startspri4
enditspri4:
							pop ds
						}
					else
						_asm \
						{
							mov di, plc
							mov si, picplc
							mov cx, si
							and cx, 0x0fc0
							add cx, 64
							mov dx, yc0
							mov bx, siz
							push ds
							mov ds, picseg
							lodsb
startspri5:
							cmp al, 252
							jae skipspri5a
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
							mov es:[di+270], al
skipspri5a:
							add di, 360
							sub dx, 256
							js startspri5c
							or dh, dh
							jnz startspri5
startspri5b:
							cmp al, 252
							jae skipspri5b
							mov es:[di], al
skipspri5b:
							add di, 90
							sub dx, 64
							jns startspri5b
startspri5c:
							lodsb
							add dx, bx
							cmp si, cx
							jbe startspri5
							pop ds
						}
					i += linesatonce;
				}
		}
		else
		{
			for(i=x;i<x+siz;i++)
			{
				if ((i >= lside) && (i < rside))
				{
					if ((siz >= height[i]) && (picplc >= j) && (picplc <= k))
						sprinum[i] = picplc;
					else
						sprinum[i] = 65535;
				}
				xc += 64;
				if (xc >= siz)
				{
					xc -= siz;
					picplc += 64;
				}
			}
			if ((x+siz) < rside)
				sprinum[x+siz] = 65534;
			for(i=x;i<x+siz;i++)
				if ((i >= lside) && (i < rside) && (sprinum[i] != 65535))
				{
					picplc = sprinum[i];
					outp(0x3c5,1<<(i&3));
					linesatonce = 0;
					if (((i&3) < 3) && (sprinum[i] == sprinum[i+1]))
					{
						outp(0x3c5,3<<(i&3));
						linesatonce = 1;
					}
					if (((i&3) < 2) && (linesatonce == 1) && (sprinum[i] == sprinum[i+2]))
					{
						outp(0x3c5,7<<(i&3));
						linesatonce = 2;
					}
					if (((i&3) == 0) && (linesatonce == 2) && (sprinum[i] == sprinum[i+3]))
					{
						outp(0x3c5,15);
						linesatonce = 3;
					}
					plc = times90[y]+(i>>2)+pageoffset;
					if (siz < 128)
						_asm \
						{
							mov di, plc
							mov si, picplc
							mov cx, pageoffset
							add cx, scrsize
							mov dx, yc0
							mov bx, siz
							push ds
							mov ds, picseg
							lodsb
beginspri6:
							cmp al, 252
							jae skipspri6a
							mov es:[di], al
skipspri6a:
							add di, 90
							cmp di, cx
							jae endspri6
							sub dx, 64
							jns beginspri6
							add dx, bx
							lodsb
							jmp beginspri6
endspri6:
							pop ds
						}
					else if (siz < 192)
						_asm \
						{
							mov di, plc
							mov si, picplc
							mov cx, pageoffset
							add cx, scrsize
							mov dx, yc0
							mov bx, siz
							push ds
							mov ds, picseg
							lodsb
beginspri7:
							cmp al, 252
							jae skipspri7a
							mov es:[di], al
skipspri7a:
							add di, 90
							cmp di, cx
							jae endspri7
							sub dx, 64
							jns beginspri7
							add dx, bx
							lodsb
							cmp al, 252
							jae skipspri7b
							mov es:[di], al
							mov es:[di+90], al
skipspri7b:
							add di, 90
							sub dx, 64
							jmp skipspri7a
endspri7:
							pop ds
						}
					else if (siz < 256)
						_asm \
						{
							mov di, plc
							mov si, picplc
							mov cx, pageoffset
							add cx, scrsize
							mov dx, yc0
							mov bx, siz
							push ds
							mov ds, picseg
							lodsb
beginspri8:
							cmp al, 252
							jae skipspri8a
							mov es:[di], al
skipspri8a:
							add di, 90
							cmp di, cx
							jae endspri8
							sub dx, 64
							jns beginspri8
							add dx, bx
							lodsb
							cmp al, 252
							jae skipspri8b
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
skipspri8b:
							add di, 180
							sub dx, 128
							jmp skipspri8a
endspri8:
							pop ds
						}
					else
						_asm \
						{
							mov di, plc
							mov si, picplc
							mov cx, pageoffset
							add cx, scrsize
							mov dx, yc0
							mov bx, siz
							push ds
							mov ds, picseg
							lodsb
startspri9:
							cmp al, 252
							jae skipspri9a
							mov es:[di], al
							mov es:[di+90], al
							mov es:[di+180], al
							mov es:[di+270], al
skipspri9a:
							add di, 360
							cmp di, cx
							jae enditspri9
							sub dx, 256
							js startspri9c
							or dh, dh
							jnz startspri9
startspri9b:
							cmp al, 252
							jae skipspri9b
							mov es:[di], al
skipspri9b:
							add di, 90
							sub dx, 64
							jns startspri9b
startspri9c:
							lodsb
							add dx, bx
							jmp startspri9
enditspri9:
							pop ds
						}
					i += linesatonce;
				}
		}
	}
}

statusbardraw(x, walnum)
int x, walnum;
{
	int i, xc, picplc;
	unsigned int picseg, plc;

	_asm \
	{
		mov ax, 0xa000
		mov es, ax
	}
	if (memtype[walnum-1] != 255)
		setpages(memtype[walnum-1],0);
	picseg = walseg[walnum-1];
	picplc = 0;
	outp(0x3c4,0x2);
	xc = 0;
	plc = (x>>2);
	if (vidmode == 1)
		plc += 5;
	for(i=0;i<32;i++)
	{
		outp(0x3c5,1<<(x&3));
		_asm \
		{
			mov di, plc
			mov si, picplc
			mov cx, si
			add cx, 64
			mov dx, 32
			mov bx, dx
			shr dx, 1
			push ds
			mov ds, picseg
			jmp beginbar1
skipbar1a:
			add dx, 64
			add di, 90
beginbar1:
			lodsb
			sub dx, bx
			jns beginbar1
			cmp si, cx
			jae endbar1
			cmp al, 252
			jae skipbar1a
			mov es:[di], al
			jmp skipbar1a
endbar1:
			pop ds
		}
		picplc += 128;
		x++;
		plc += ((x&3) == 0);
	}
}

loadboard()
{
	int i, j, k, fil;

	if ((fil = open("boards.dat",O_BINARY|O_RDWR,S_IREAD)) != -1)
	{
		numboards = (int)(filelength(fil)>>12);
		lseek(fil,((long)boardnum)<<12,SEEK_SET);
		read(fil,&board[0],4096);
		close(fil);
		if (memtype[numwalls-1] != 255)
			setpages(memtype[numwalls-1],0);
		_asm \
		{
			cld
			mov bx, numwalls
			dec bx
			shl bx, 1
			mov ax, walseg[bx]
			mov es, ax
			mov si, offset board
			mov ax, seg board
			mov di, 0
			mov cx, 2048
			push ds
			mov ds, ax
			rep movsw
			pop ds
		}
		mnum = 0;
		for(i=0;i<63;i++)
			for(j=0;j<63;j++)
			{
				mboard[i][j] = 0;
				k = board[i][j];
				if ((k == 66) || (k == 38) || (k == 54) || (k == 68))
				{
					mposx[mnum] = (i<<10)+512;
					mposy[mnum] = (j<<10)+512;
					mgolx[mnum] = mposx[mnum];
					mgoly[mnum] = mposy[mnum];
					moldx[mnum] = mposx[mnum];
					moldy[mnum] = mposy[mnum];
					mstat[mnum] = board[i][j];
					mshock[mnum] = 0;
					if (board[i][j] == 66)
						mshot[mnum] = 1;
					if (board[i][j] == 38)
						mshot[mnum] = 2;
					if (board[i][j] == 54)
						mshot[mnum] = 5;
					mnum++;
					mboard[i][j] = k;
					board[i][j] = 0;
				}
				if ((board[i][j] >= 252) && (board[i][j] <= 255))
				{
					posx = (i<<10)+512;
					posy = (j<<10)+512;
					ang = ((board[i][j]-252)<<9);
					startx = posx;
					starty = posy;
					startang = ang;
					board[i][j] = 0;
				}
			}
		posz = 32;
		angvel = 0;
		vel = 0;
		hvel = 0;
		for(i=0;i<32;i++)
			bulstat[i] = 0;
		lastbulshoot = 0;
		bulnum = 0;
		youbulnum = 0;
		keys = 0;
		death = 63;
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

loadtables()
{
	int fil, i;

	if ((fil = open("tables.dat",O_BINARY|O_RDWR,S_IREAD)) != -1)
	{
		read(fil,&sintable[0],8192);
		read(fil,&tantable[0],8192);
		read(fil,&radarang[0],720);
		read(fil,&htot,2);
		read(fil,&hstr,2);
		read(fil,&vtot,2);
		read(fil,&vstr,2);
		read(fil,&option[0],numoptions);
		read(fil,&keydefs[0],numkeys);
		close(fil);
	}
}

int ksay(filename)
char filename[80];
{
	int infile;
	unsigned int length;
	unsigned long addr;
	void far *addre = (void far *)snd;

	if (speechstatus == 0)
		return(-1);
	if ((infile = open(filename, O_BINARY, S_IREAD)) == -1)
		return(-1);
	read(infile, snd, 44);
	if (reset_dsp() == 0)
	{
		addr = ((long)FP_SEG(addre)<<4) + (long)FP_OFF(addre);
		if (((addr&65535)^65535) < 32000)
		{
			length = read(infile, &snd[32000], 32000);
			addr += 32000;
		}
		else
			length = read(infile, &snd[0], 32000);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,0xd1);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,0x40);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,165);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,0x14);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,(length-1)&255);
		while ((inp(0x22c) & 0x80) != 0); outp(0x22c,((length-1)>>8)&255);
		outp(0xa,0x5);
		outp(0xc,0);
		outp(0xb,0x49);
		outp(0x2,(int)(addr&255)); outp(0x2,(int)((addr>>8)&255));
		outp(0x3,(length-1)&255); outp(0x3,((length-1)>>8)&255);
		outp(0x83,(int)((addr>>16)&255));
		outp(0xa,0x1);
	}
	close(infile);
	return(0);
}

reset_dsp()
{
	_asm \
	{
 lb1: mov dx, 0x226
		mov al, 0x1
		out dx, al
		mov cx, 128
 lab1:in al, dx
		loop lab1
		xor al, al
		out dx, al
		mov cx, 128
 lab2:in al, dx
		loop lab2
		mov dx, 0x22e
		mov cx, 0xffff
 lb2: in al, dx
		and al, 0x80
		cmp al, 0x80
		jz lb3
		loop lb2
		jmp lb1
 lb3:
	}
	if (inp(0x22a) == 0xaa)
		return(0);
	else
		return(-1);
}

void interrupt far divzerohandler()
{
	_asm \
	{
		mov ax, 3
		int 0x10
		mov ax, 0xb800
		mov es, ax
		mov di, 0
		mov al, 90
		stosb
		mov al, 69
		stosb
		mov al, 82
		stosb
		mov al, 79
		stosb
	}
}

checkobj(x,y,posx,posy,ang,num)
int x, y, posx, posy, ang;
unsigned char num;
{
	int angle, siz, ysiz;

	if (((posx|1023) != (x|1023)) || ((posy|1023) != (y|1023)))
	{
		siz = (int)((((long)(x-posx)>>2)*sintable[(ang+512)&2047]+((long)(y-posy)>>2)*sintable[ang])>>16);
		if (siz != 0)
		{
			ysiz = (int)((((long)(x-posx)>>2)*sintable[ang]-((long)(y-posy)>>2)*sintable[(ang+512)&2047])>>16);
			angle = 180-(int)((180*(long)ysiz)/(long)siz);
			siz = 40960 / siz;
			if (((angle+(siz>>1)) >= lside) && ((angle-(siz>>1)) < rside) && (siz > 0) && (siz < 1024))
			{
				sortang[sortcnt] = angle;
				sorti[sortcnt] = siz;
				sortbnum[sortcnt] = num;
				sortcnt++;
			}
		}
	}
}

linecompare(lin)
unsigned int lin;
{
	outp(0x3d4,0x18); outp(0x3d5,lin&255);
	outp(0x3d4,0x7); outp(0x3d5,(inp(0x3d5)&239)|((lin&256)>>4));
	outp(0x3d4,0x9); outp(0x3d5,(inp(0x3d5)&191)|((lin&512)>>3));
}

drawlife()
{
	if (life < 0)
		life = 0;
	if (life > 63)
		life = 63;
	_asm \
	{
		mov dx, 0x3c4
		mov al, 0x2
		out dx, al
		inc dx
		mov al, 0x6
		out dx, al
	}
	if (life < oldlife)
		_asm \
		{
			cld
			mov ax, 0xa000
			mov es, ax
			mov di, life
			mov cx, oldlife
			sub cx, di
			add di, 8
			cmp vidmode, 0
			je skipdrawlife1
			add di, 5
skipdrawlife1:
			mov bx, cx
			mov dx, di
			mov al, 0
			rep stosb
			mov cx, bx
			mov di, dx
			add di, 90
			rep stosb
		}
	if (life > oldlife)
		_asm \
		{
			cld
			mov ax, 0xa000
			mov es, ax
			mov di, oldlife
			mov cx, life
			sub cx, di
			add di, 8
			cmp vidmode, 0
			je skipdrawlife2
			add di, 5
skipdrawlife2:
			mov bx, cx
			mov dx, di
			mov al, 252
			rep stosb
			mov cx, bx
			mov di, dx
			add di, 90
			rep stosb
		}
	oldlife = life;
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

emmallocate(needpages)
unsigned needpages;
{
	int snot;

	_asm \
	{
		mov al, 0x67
		mov ah, 0x35
		int 0x21
		cmp word ptr es:[10], 0x4d45
		jnz emmerror1
		cmp word ptr es:[12], 0x584d
		jnz emmerror1
		cmp word ptr es:[14], 0x5858
		jnz emmerror1
		cmp word ptr es:[16], 0x3058
		jnz emmerror1
		mov ah, 0x40
		int 0x67
		or ah, ah
		jnz emmerror1
		mov ah, 0x46
		int 0x67
		or ah, ah
		jnz emmerror1
		cmp al, 0x30
		jb emmerror1
		mov ah, 0x41
		int 0x67
		or ah, ah
		jnz emmerror1
		mov emmpageframe, bx
		mov ah, 0x42
		int 0x67
		or ah, ah
		jnz emmerror1
		mov emmtotalpages, dx
		mov emmavailpages, bx
		or bx, bx
		jz emmerror1
		mov ah, 0x43
		mov bx, needpages
		int 0x67
		or ah, ah
		jnz emmerror1
		mov snot, dx
		jmp doneemm1
emmerror1:
		mov snot, -1
doneemm1:
	}
	return(snot);
}

emmdeallocate()
{
	int snot;

	_asm \
	{
		mov snot, 0
		mov dx, emmhandle
		mov ah, 0x45
		int 0x67
		or ah, ah
		jnz emmerror2
		jmp doneemm2
emmerror2:
		mov snot, -1
doneemm2:
	}
	return(snot);
}

setpages(logpage,physpage)
unsigned logpage, physpage;
{
	int snot;

	if ((logpage == oldlogpage) && (physpage == oldphyspage))
		return(0);
	oldlogpage = logpage;
	oldphyspage = physpage;
	_asm \
	{
		mov snot, 0
		mov bx, logpage
		mov al, physpage
		mov dx, emmhandle
		mov ah, 0x44
		int 0x67
		or ah, ah
		jz doneemm3
		mov snot, -1
doneemm3:
	}
	return(snot);
}

loadwalls()
{
	unsigned char tempbuf2[512], m, n;
	int i, j, k, fil, c1, c2, c3, cnt, bufleng;

	if ((fil = open("walls.kzp",O_BINARY|O_RDWR,S_IREAD)) != -1)
	{
		passthrough[0] = 0;
		bmpkind[0] = 0;
		read(fil,&tempbuf[0],numwalls);
		for(i=0;i<numwalls;i++)
		{
			passthrough[i+1] = (tempbuf[i]&1);
			bmpkind[i+1] = 1+((tempbuf[i]&2)>>1);
		}
		for(i=128;i<256;i++)
		{
			passthrough[i] = 0;
			bmpkind[i] = bmpkind[i&127];
		}
		i = 0;
		cnt = 0;
		bufleng = 1;
		_asm cld
		while ((i < numwalls) && (bufleng > 0))
		{
			bufleng = read(fil,&tempbuf2[0],512);
			for(k=0;k<bufleng;k+=2)
			{
				m = tempbuf2[k];
				n = tempbuf2[k+1];
				_asm \
				{
					mov ax, seg tempbuf
					mov es, ax
					mov di, cnt
					add di, offset tempbuf
					mov cl, n
					mov ch, 0
					add cnt, cx
					inc cx
					mov al, m
					rep stosb
				}
				if (cnt >= 4096)
				{
					cnt = 0;
					if (bmpkind[i] == 2)
					{
						j = 0;
						while ((j < 4096) && (tempbuf[j] == 255))
							j++;
						lborder[i] = (j&0xfc0);
						j = 4095;
						while ((j > 0) && (tempbuf[j] == 255))
							j--;
						rborder[i] = (j&0xfc0);
					}
					else
					{
						lborder[i] = 0;
						rborder[i] = 4095;
					}
					if (memtype[i] != 255)
						setpages(memtype[i],0);
					_asm \
					{
						mov bx, i
						shl bx, 1
						mov ax, walseg[bx]
						mov es, ax
						mov si, offset tempbuf
						mov ax, seg tempbuf
						mov di, 0
						mov cx, 4096
						push ds
						mov ds, ax
						rep movsb
						pop ds
					}
					i++;
				}
			}
		}
		close(fil);
		c1 = 0, c2 = 0, c3 = 0;
		outp(0x3c8,0);
		for(i=0;i<252;i++)
		{
			outp(0x3c9,(c1<<6)/6);
			outp(0x3c9,(c2<<6)/7);
			outp(0x3c9,(c3<<6)/6);
			c1++;
			if (c1 == 6)
			{
				c1 = 0;
				c2++;
				if (c2 == 7)
				{
					c2 = 0;
					c3++;
				}
			}
		}
		for(i=0;i<12;i++)
			outp(0x3c9,63);
		outp(0x3c7,0);
		for(i=0;i<768;i++)
			palette[i] = inp(0x3c9);
	}
}

convallocate(numparagraphs)
unsigned numparagraphs;
{
	unsigned segnum;

	_asm \
	{
		mov ah, 0x48
		mov bx, numparagraphs
		int 0x21
		mov segnum, ax
		jnc convallocated
		mov segnum, -1
		mov convavailpages, bx
convallocated:
	}
	return(segnum);
}

convdeallocate(segnum)
unsigned segnum;
{
	_asm \
	{
		mov ah, 0x49
		mov es, segnum
		int 0x21
		mov segnum, 0
		jnc convdeallocated
		mov segnum, -1
convdeallocated:
	}
	return(segnum);
}

loadgame(gamenum)
int gamenum;
{
	char filename[20];
	int i, fil;

	filename[0] = 'S', filename[1] = 'A', filename[2] = 'V';
	filename[3] = 'G', filename[4] = 'A', filename[5] = 'M';
	filename[6] = 'E', filename[7] = gamenum+48;
	filename[8] = '.', filename[9] = 'D', filename[10] = 'A';
	filename[11] = 'T', filename[12] = 0;
	if((fil=open(filename,O_BINARY|O_RDONLY,S_IREAD))==-1)
		return(-1);
	read(fil,&board[0][0],4096);
	read(fil,&mboard[0][0],4096);
	read(fil,&boardnum,2);
	read(fil,&life,2);
	read(fil,&death,2);
	read(fil,&lifevests,2);
	read(fil,&lightnings,2);
	read(fil,&firepowers,2);
	read(fil,&keys,2);
	read(fil,&flowerpos,2);
	read(fil,&kenpos,2);
	read(fil,&ballpos,2);
	read(fil,&rogermode,2);
	read(fil,&statusbar,2);
	read(fil,&statusbargoal,2);
	read(fil,&posx,2);
	read(fil,&posy,2);
	read(fil,&posz,2);
	read(fil,&ang,2);
	read(fil,&startx,2);
	read(fil,&starty,2);
	read(fil,&startang,2);
	read(fil,&angvel,2);
	read(fil,&vel,2);
	read(fil,&hvel,2);
	read(fil,&oldposx,2);
	read(fil,&oldposy,2);
	read(fil,&youbulnum,2);
	read(fil,&bulnum,2);
	read(fil,&bulang[0],bulnum<<1);
	read(fil,&bulkind[0],bulnum<<1);
	read(fil,&bulx[0],bulnum<<1);
	read(fil,&buly[0],bulnum<<1);
	read(fil,&bulstat[0],bulnum<<2);
	read(fil,&lastbulshoot,4);
	read(fil,&mnum,2);
	read(fil,&mposx[0],mnum<<1);
	read(fil,&mposy[0],mnum<<1);
	read(fil,&mgolx[0],mnum<<1);
	read(fil,&mgoly[0],mnum<<1);
	read(fil,&moldx[0],mnum<<1);
	read(fil,&moldy[0],mnum<<1);
	read(fil,&mstat[0],mnum);
	read(fil,&mshock[0],mnum<<1);
	read(fil,&mshot[0],mnum);
	read(fil,&totalclock,4);
	musicoff();
	read(fil,&musicstatus,4);
	if (musicstatus != 0)
	{
		filename[0] = 'W', filename[1] = 'A', filename[2] = 'L';
		filename[3] = 'S', filename[4] = 'N', filename[5] = 'G';
		filename[6] = (boardnum/10)+48, filename[7] = (boardnum%10)+48;
		filename[8] = '.', filename[9] = 'K', filename[10] = 'S';
		filename[11] = 'M', filename[12] = 0;
		loadmusic(filename);
		musicon();
	}
	read(fil,&clockspeed,2);
	read(fil,&count,4);
	read(fil,&countstop,4);
	read(fil,&nownote,4);
	read(fil,&chanage,18<<2);
	read(fil,&chanfreq,18);
	close(fil);
	if (memtype[numwalls-1] != 255)
		setpages(memtype[numwalls-1],0);
	_asm \
	{
		cld
		mov bx, numwalls
		dec bx
		shl bx, 1
		mov ax, walseg[bx]
		mov es, ax
		mov si, offset board
		mov ax, seg board
		mov di, 0
		mov cx, 2048
		push ds
		mov ds, ax
		rep movsw
		pop ds
	}
	scrsize = 21600;
	if (vidmode == 0)
		scrsize = 19800;
	if ((vidmode == 0) && (statusbargoal > 400))
	{
		statusbar -= 80;
		statusbargoal -= 80;
		if (statusbargoal < 399)
			scrsize -= 2880;
	}
	if ((vidmode == 1) && (statusbargoal < 400))
	{
		scrsize += 2880;
		statusbar += 80;
		if (statusbargoal < 479)
			scrsize -= 2880;
	}
	linecompare(statusbar);
	for(i=0;i<320;i+=32)
		statusbardraw(i,31);
	oldlife = 0;
	drawlife();
	oldlife = 63;
	drawlife();
	for(i=1;i<=lifevests;i++)
		statusbardraw((i<<5),32);
	for(i=1;i<=lightnings;i++)
		statusbardraw(305-((i<<5)-(i<<3)),36);
	for(i=1;i<=firepowers;i++)
		statusbardraw(316-((i<<5)-(i<<3)),39);
	if (keys > 0)
		statusbardraw(0,45);
}

savegame(gamenum)
int gamenum;
{
	char filename[20];
	int fil;

	filename[0] = 'S', filename[1] = 'A', filename[2] = 'V';
	filename[3] = 'G', filename[4] = 'A', filename[5] = 'M';
	filename[6] = 'E', filename[7] = gamenum+48;
	filename[8] = '.', filename[9] = 'D', filename[10] = 'A';
	filename[11] = 'T', filename[12] = 0;
	if((fil=open(filename,O_BINARY|O_CREAT|O_WRONLY,S_IWRITE))==-1)
		return(-1);
	write(fil,&board[0][0],4096);
	write(fil,&mboard[0][0],4096);
	write(fil,&boardnum,2);
	write(fil,&life,2);
	write(fil,&death,2);
	write(fil,&lifevests,2);
	write(fil,&lightnings,2);
	write(fil,&firepowers,2);
	write(fil,&keys,2);
	write(fil,&flowerpos,2);
	write(fil,&kenpos,2);
	write(fil,&ballpos,2);
	write(fil,&rogermode,2);
	write(fil,&statusbar,2);
	write(fil,&statusbargoal,2);
	write(fil,&posx,2);
	write(fil,&posy,2);
	write(fil,&posz,2);
	write(fil,&ang,2);
	write(fil,&startx,2);
	write(fil,&starty,2);
	write(fil,&startang,2);
	write(fil,&angvel,2);
	write(fil,&vel,2);
	write(fil,&hvel,2);
	write(fil,&oldposx,2);
	write(fil,&oldposy,2);
	write(fil,&youbulnum,2);
	write(fil,&bulnum,2);
	write(fil,&bulang[0],bulnum<<1);
	write(fil,&bulkind[0],bulnum<<1);
	write(fil,&bulx[0],bulnum<<1);
	write(fil,&buly[0],bulnum<<1);
	write(fil,&bulstat[0],bulnum<<2);
	write(fil,&lastbulshoot,4);
	write(fil,&mnum,2);
	write(fil,&mposx[0],mnum<<1);
	write(fil,&mposy[0],mnum<<1);
	write(fil,&mgolx[0],mnum<<1);
	write(fil,&mgoly[0],mnum<<1);
	write(fil,&moldx[0],mnum<<1);
	write(fil,&moldy[0],mnum<<1);
	write(fil,&mstat[0],mnum);
	write(fil,&mshock[0],mnum<<1);
	write(fil,&mshot[0],mnum);
	write(fil,&totalclock,4);
	write(fil,&musicstatus,4);
	write(fil,&clockspeed,2);
	write(fil,&count,4);
	write(fil,&countstop,4);
	write(fil,&nownote,4);
	write(fil,&chanage,18<<2);
	write(fil,&chanfreq,18);
	close(fil);
}

introduction()
{
	unsigned int plc;
	int lasti, i, j, x, y;

	outp(0x3c4,1); outp(0x3c5,inp(0x3c5)|32);
	lside = 0;
	rside = 360;
	scrsize = 21600;
	lasti = 0;
	i = 0;
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
		mov di, pageoffset
		mov cx, 32768
		mov ax, 0x0000
		rep stosw
	}
	if (vidmode == 0)
	{
		outp(0x3d4,0xc); outp(0x3d5,7);
		outp(0x3d4,0xd); outp(0x3d5,13);
	}
	loadmusic("intro.ksm");
	clockspeed = 0;
	totalclock = 0;
	plc = 0;
	if (vidmode == 0)
		plc = 1805;
	musicon();
	ksay("welcome.wav");
	for(i=0;i<360;i++)
		height[i] = 0;
	outp(0x3c4,1); outp(0x3c5,inp(0x3c5)&223);
	while ((keystatus[1] == 0) && (keystatus[57] == 0) && (keystatus[28] == 0))
	{
		outp(0x3c8,0);
		j = (((int)labs((totalclock%120)-60))>>3);
		_asm \
		{
			mov dx, 0x3da
retrace1:
			in al, dx
			test al, 8
			jnz retrace1
retrace2:
			in al, dx
			test al, 8
			jz retrace2
		}
		outp(0x3c9,j);
		outp(0x3c9,j);
		outp(0x3c9,j);
		totalclock += clockspeed;
		if (totalclock > 5760)
			totalclock -= 5760;
		clockspeed = 0;
		lasti = i;
		i = (int)(totalclock>>2);
		if ((i >= 0) && (i < 240))
		{
			if (lasti >= 240)
			{
				plc = 0;
				if (vidmode == 0)
					plc += 1805;
				outp(0x3d4,0xc); outp(0x3d5,(plc>>8)&255);
				outp(0x3d4,0xd); outp(0x3d5,plc&255);
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
					mov cx, 32768
					mov ax, 0x0000
					rep stosw
				}
				linecompare(511);
				pageoffset = 0;
			}
			spridraw(180-(i>>1),120-(i>>1),i,64);
		}
		if ((i >= 240) && (i < 480))
		{
			if (lasti < 240)
			{
				pageoffset = 21600;
				spridraw(80,20,200,5);
				plc = 0;
				if (vidmode == 0)
					plc += 1805;
			}
		}
		if ((i >= 480) && (i < 720))
		{
			plc += 90;
			if (vidmode == 0)
				if (plc > 23405)
					plc = 23405;
			if (vidmode == 1)
				if (plc > 21600)
					plc = 21600;
			_asm \
			{
				mov bx, plc
				mov dx, 0x3d4
				mov al, 0xc
				out dx, al
				inc dx
				mov al, bh
				out dx, al
				dec dx
				mov al, 0xd
				out dx, al
				inc dx
				mov al, bl
				out dx, al
			}
		}
		if ((i >= 720) && (i < 960))
		{
			if (lasti < 720)
			{
				plc = 21600;
				if (vidmode == 0)
					plc += 1805;
				outp(0x3d4,0xc); outp(0x3d5,(plc>>8)&255);
				outp(0x3d4,0xd); outp(0x3d5,plc&255);
				ksay("blowup.wav");
				spridraw(80,20,200,44);
			}
			j = rand()&127;
			spridraw((rand()%(360-j)),(rand()%(240-j)),j,44);
		}
		if ((i >= 960) && (i < 1200))
		{
			if (lasti < 960)
			{
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
					mov cx, 10800
					mov ax, 0x0000
					rep stosw
				}
				pageoffset = 0;
				if (vidmode == 0)
				{
					x = 0;
					y = 0;
				}
				else
				{
					x = 20;
					y = 20;
				}
				spridraw(256+x,136+y,64,76);
				j = 1;
				while (j < 80)
				{
					if (credits[j] == 1)
					{
						spridraw(x,y,32,j);
						x += 32;
						if (x >= 320)
						{
							x -= 320;
							y += 32;
						}
					}
					j++;
				}
				plc = 480;
				if (vidmode == 0)
					plc -= 80;
			}
			plc -= 4;
			if ((plc > 0) && (plc < 512))
				linecompare(plc);
			else
				linecompare(1);
		}
		if ((i >= 1200) && (i < 1440))
		{
			if (lasti < 1200)
			{
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
					mov di, 21600
					mov cx, scrsize
					shr cx, 1
					mov ax, 0x0000
					rep stosw
				}
				plc = 21600;
				outp(0x3d4,0xc); outp(0x3d5,(plc>>8)&255);
				outp(0x3d4,0xd); outp(0x3d5,plc&255);
				pageoffset = 21600;
				if (vidmode == 0)
				{
					x = 0;
					y = 0;
				}
				else
				{
					x = 20;
					y = 20;
				}
				spridraw(256+x,136+y,64,77);
				j = 1;
				while (j < 80)
				{
					if (credits[j] == 2)
					{
						spridraw(x,y,32,j);
						x += 32;
						if (x >= 320)
						{
							x -= 320;
							y += 32;
						}
					}
					j++;
				}
				plc = 1;
			}
			plc += 4;
			if ((plc > 0) && (plc < 512))
				linecompare(plc);
		}
	}
	outp(0x3c4,1); outp(0x3c5,inp(0x3c5)|32);
	keystatus[57] = 0;
	keystatus[28] = 0;
	keystatus[1] = 0;
	musicoff();
	if (vidmode == 0)
	{
		lside = 20;
		rside = 340;
		scrsize = 19800;
	}
	else
	{
		lside = 0;
		rside = 360;
		scrsize = 21600;
	}
	outp(0x3c8,0);
	outp(0x3c9,0);
	outp(0x3c9,0);
	outp(0x3c9,0);
	_asm \
	{
		mov ax, 0xa000
		mov es, ax
		cld
		mov di, 0
		mov cx, 32768
		mov ax, 0x0000
		rep stosw
	}
	boardnum = 0;
	loadboard();
	pageoffset = 35648;
	sortcnt = 0;
	oldlife = 0;
	life = 63;
	death = 63;
	lifevests = 0;
	lightnings = 0;
	firepowers = 0;
	flowerpos = 49;
	kenpos = 66;
	ballpos = 68;
	loadmusic("walsng00.ksm");
	musicon();
	rogermode = 0;
	totalclock = 0;
	clockspeed = 0;
	statusbar = 335;
	if (vidmode == 1)
		statusbar += 80;
	statusbargoal = statusbar;
	linecompare(statusbar);
	for(i=0;i<320;i+=32)
		statusbardraw(i,31);
	drawlife();
}

loadmusic(filename)
char filename[80];
{
	char buffer[256];
	int infile, i, j;

	if (firstime == 1)
	{
		if (musicsource == 1)
		{
			midiscrap = 256;
			while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
				midiscrap--;
			outp(statport,0x3f);
			midiscrap = 32000;
			while ((midiscrap > 0) && ((inp(dataport) & 0xfe) != 0))
				midiscrap--;
		}
		if (musicsource == 2)
		{
			if((infile=open("insts.dat",O_BINARY|O_RDONLY,S_IREAD))==-1)
			{
				printf("I cannot load insts.dat!");
				exit(0);
			}
			for(i=0;i<256;i++)
			{
				read(infile,&buffer,33);
				for(j=0;j<20;j++)
					inst[i][j] = buffer[j];
				inst[i][20] = 0;
				for(j=21;j<32;j++)
					inst[i][j] = buffer[j-1];
			}
			close(infile);
			numchans = 9;
			outdata(0,0x1,0);  //clear test stuff
			outdata(0,0x4,0);  //reset
			outdata(0,0x8,0);  //2-operator synthesis
			firstime = 0;
		}
	}
	if ((strstr(filename,".KSM") == 0) && (strstr(filename,".ksm") == 0))
		strcat(filename,".KSM");
	if((infile=open(filename,O_BINARY|O_RDONLY,S_IREAD))==-1)
	{
		printf("I cannot load %s.",filename);
		exit(0);
	}
	read(infile,&trinst[0],16);
	read(infile,&trquant[0],16);
	read(infile,&trchan[0],16);
	read(infile,&trprio[0],16);
	read(infile,&trvol[0],16);
	read(infile,&numnotes,2);
	read(infile,&note[0],numnotes<<2);
	close(infile);
	numchans = 9-trchan[11]*3;
	if (musicsource == 1)
		setmidiinsts();
	if (musicsource == 2)
	{
		if (trchan[11] == 0)
		{
			drumstat = 0;
			outdata(0,0xbd,drumstat);
		}
		if (trchan[11] == 1)
		{
			setinst(0,6,0,63-trvol[11],0xd6,0x68,0,0,10,0xd6,0x68,0,4); //bass
			setinst(0,7,0,63-trvol[12],0xd8,0x4f,0,0,63-trvol[14],0xf8,0xff,0,14); //snare & hihat
			setinst(0,8,0,63-trvol[15],0xf5,0xc8,0,0,63-trvol[13],0xd6,0x88,0,0);  //topsymb & tom
			outdata(0,0xa0+6,600&255);
			outdata(0,0xb0+6,(600>>8)&223);
			outdata(0,0xa0+7,400&255);
			outdata(0,0xb0+7,(400>>8)&223);
			outdata(0,0xa0+8,5510&255);
			outdata(0,0xb0+8,(5510>>8)&223);
			drumstat = 32;
			outdata(0,0xbd,drumstat);
		}
	}
}

outdata(synth,index,data)
unsigned char synth,index,data;
{
	if (synth == 1)
		_asm mov dx, 0x38a ; get the right 3812 address
	else
		_asm mov dx, 0x388 ; get the left 3812 address
	_asm \
	{
		mov ax, index    ; get the index value
		out dx, al       ; output to both chips
		in al, dx        ; slow down for the index to settle
		in al, dx
		in al, dx
		in al, dx
		in al, dx
		inc dx           ; move to the data register
		mov ax, data
		out dx, al       ; write the data out
		mov cx, 33       ; load the loop count
 lbl1:in al, dx
		loop lbl1
	}
}

musicon()
{
	char ch;
	unsigned int num;
	int i, j, k, offs;
	unsigned char instbuf[11];

	for(i=0;i<numchans;i++)
	{
		chantrack[i] = 0;
		chanage[i] = 0;
	}
	j = 0;
	for(i=0;i<16;i++)
		if ((trchan[i] > 0) && (j < numchans))
		{
			k = trchan[i];
			while ((j < numchans) && (k > 0))
			{
				chantrack[j] = i;
				k--;
				j++;
			}
		}
	for(i=0;i<numchans;i++)
	{
		if (musicsource == 2)
		{
			for(j=0;j<11;j++)
				instbuf[j] = inst[trinst[chantrack[i]]][j+21];
			instbuf[1] = ((instbuf[1]&192)|(63-trvol[chantrack[i]]));
			setinst(0,i,instbuf[0],instbuf[1],instbuf[2],instbuf[3],instbuf[4],instbuf[5],instbuf[6],instbuf[7],instbuf[8],instbuf[9],instbuf[10]);
		}
		chanfreq[i] = 0;
	}
	k = 0;
	musicstatus = 0;
	count = (note[k]>>12)-1;
	countstop = (note[k]>>12)-1;
	nownote = note[k];
	musicstatus = 1;
	num = (unsigned)((1193181.7 / (double)speed)+.5);
	outp(0x43,54); outp(0x40,num&255); outp(0x40,num>>8);
	oldhand = _dos_getvect(0x8);
	_disable(); _dos_setvect(0x8, ksmhandler); _enable();
}

musicoff()
{
	int i;

	outp(0x43,54); outp(0x40,255); outp(0x40,255);
	_disable(); _dos_setvect(0x8, oldhand); _enable();
	if (musicsource == 0)
		outp(97, inp(97) & 0xfc);
	if (musicsource == 1)
		for(i=0;i<numchans;i++)
		{
			midiscrap = 256;
			while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
				midiscrap--;
			if (midiscrap > 0)
			{
				if (i < 6)
					outp(dataport,0x90);
				else
					outp(dataport,0x91);
			}
			midiscrap = 256;
			while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
				midiscrap--;
			if (midiscrap > 0)
				outp(dataport,chanfreq[i]+35);
			midiscrap = 256;
			while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
				midiscrap--;
			if (midiscrap > 0)
				outp(dataport,0);
		}
	if (musicsource == 2)
		for(i=0;i<numchans;i++)
		{
			outdata(0,0xa0+i,0);
			outdata(0,0xb0+i,0);
		}
	musicstatus = 0;
}

void interrupt far ksmhandler()
{
	int i, j, quanter, bufnum, chan, drumnum, freq;
	long temp;

	clockspeed++;
	count++;
	if (count >= countstop)
	{
		bufnum = 0;
		while (count >= countstop)
		{
			if (musicsource == 0)
			{
				if ((((note[nownote]>>8)&15) == 0) && ((note[nownote]&64) > 0))
				{
					databuf[bufnum] = (unsigned char)(note[nownote]&63);
					bufnum++;
				}
			}
			if (musicsource > 0)
			{
				if (((note[nownote]&255) >= 1) && ((note[nownote]&255) <= 61))
				{
					i = 0;
					while (((chanfreq[i] != (note[nownote]&63)) || (chantrack[i] != ((note[nownote]>>8)&15))) && (i < numchans))
						i++;
					if (i < numchans)
					{
						if (musicsource == 1)
						{
							if (i < 6)
								databuf[bufnum] = (unsigned char)(0x90);
							else
								databuf[bufnum] = (unsigned char)(0x91);
							bufnum++;
							databuf[bufnum] = (unsigned char)(note[nownote]&63)+35; bufnum++;
							databuf[bufnum] = (unsigned char)0; bufnum++;
						}
						if (musicsource == 2)
						{
							databuf[bufnum] = (unsigned char)(0xa0+i); bufnum++;
							databuf[bufnum] = (unsigned char)(adlibfreq[note[nownote]&63]&255); bufnum++;
							databuf[bufnum] = (unsigned char)(0xb0+i); bufnum++;
							databuf[bufnum] = (unsigned char)((adlibfreq[note[nownote]&63]>>8)&223); bufnum++;
						}
						chanfreq[i] = 0;
						chanage[i] = 0;
					}
				}
				else if (((note[nownote]&255) >= 65) && ((note[nownote]&255) <= 125))
				{
					if (((note[nownote]>>8)&15) < 11)
					{
						temp = 0;
						i = numchans;
						for(j=0;j<numchans;j++)
							if ((countstop - chanage[j] >= temp) && (chantrack[j] == ((note[nownote]>>8)&15)))
							{
								temp = countstop - chanage[j];
								i = j;
							}
						if (i < numchans)
						{
							if (musicsource == 1)
							{
								if (i < 6)
									databuf[bufnum] = (unsigned char)(0x90);
								else
									databuf[bufnum] = (unsigned char)(0x91);
								bufnum++;
								databuf[bufnum] = (unsigned char)(note[nownote]&63)+35; bufnum++;
								databuf[bufnum] = (unsigned char)64; bufnum++;
							}
							if (musicsource == 2)
							{
								databuf[bufnum] = (unsigned char)(0xa0+i); bufnum++;
								databuf[bufnum] = (unsigned char)0; bufnum++;
								databuf[bufnum] = (unsigned char)(0xb0+i); bufnum++;
								databuf[bufnum] = (unsigned char)0; bufnum++;
								databuf[bufnum] = (unsigned char)(0xa0+i); bufnum++;
								databuf[bufnum] = (unsigned char)(adlibfreq[note[nownote]&63]&255); bufnum++;
								databuf[bufnum] = (unsigned char)(0xb0+i); bufnum++;
								databuf[bufnum] = (unsigned char)((adlibfreq[note[nownote]&63]>>8)|32); bufnum++;
							}
							chanfreq[i] = note[nownote]&63;
							chanage[i] = countstop;
						}
					}
					else
					{
						if (musicsource == 1)
						{
							databuf[bufnum] = (unsigned char)(0x92), bufnum++;
							switch((note[nownote]>>8)&15)
							{
								case 11: drumnum = 38; break;
								case 12: drumnum = 43; break;
								case 13: drumnum = 64; break;
								case 14: drumnum = 50; break;
								case 15: drumnum = 48; break;
							}
							databuf[bufnum] = (unsigned char)drumnum; bufnum++;
							databuf[bufnum] = (unsigned char)64; bufnum++;
						}
						if (musicsource == 2)
						{
							freq = adlibfreq[note[nownote]&63];
							switch((note[nownote]>>8)&15)
							{
								case 11: drumnum = 16; chan = 6; freq -= 2048; break;
								case 12: drumnum = 8; chan = 7; freq -= 2048; break;
								case 13: drumnum = 4; chan = 8; break;
								case 14: drumnum = 2; chan = 8; break;
								case 15: drumnum = 1; chan = 7; freq -= 2048; break;
							}
							databuf[bufnum] = (unsigned char)(0xa0+chan); bufnum++;
							databuf[bufnum] = (unsigned char)(freq&255); bufnum++;
							databuf[bufnum] = (unsigned char)(0xb0+chan); bufnum++;
							databuf[bufnum] = (unsigned char)((freq>>8)&223); bufnum++;
							databuf[bufnum] = (unsigned char)(0xbd); bufnum++;
							databuf[bufnum] = (unsigned char)(drumstat&(255-drumnum)); bufnum++;
							drumstat |= drumnum;
							databuf[bufnum] = (unsigned char)(0xbd); bufnum++;
							databuf[bufnum] = (unsigned char)(drumstat); bufnum++;
						}
					}
				}
			}
			nownote++;
			if (nownote >= numnotes)
			{
				nownote = 0;
				count = (note[nownote]>>12)-1;
			}
			quanter = (240/trquant[(note[nownote]>>8)&15]);
			countstop = (((note[nownote]>>12)+(quanter>>1)) / quanter) * quanter;
		}
		if (musicsource == 0)
		{
			j = 0;
			for(i=0;i<bufnum;i++)
				if (databuf[i] > j)
					j = databuf[i];
			if (j == 0)
				outp(97,inp(97)&0xfc);
			if (j > 0)
			{
				outp(67,182);
				outp(66,(unsigned)(1193280L / pcfreq[j])&255);
				outp(66,(unsigned)(1193280L / pcfreq[j])>>8);
				outp(97,inp(97)|0x3);
			}
		}
		if (musicsource == 1)
			for(i=0;i<bufnum;i++)
			{
				midiscrap = 256;
				while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
					midiscrap--;
				if (midiscrap > 0)
					outp(dataport,databuf[i]);
			}
		if (musicsource == 2)
			for(i=0;i<bufnum;i+=2)
				_asm \
				{
					mov dx, 0x388             ; get the left 3812 address
					mov bx, i
					mov ax, databuf[bx]       ; get the index value
					out dx, al                ; output to both chips
					in al, dx                 ; slow down for the index to settle
					in al, dx
					in al, dx
					in al, dx
					in al, dx
					inc dx                    ; move to the data register
					inc bx
					mov ax, databuf[bx]
					out dx, al                ; write the data out
					mov cx, 33                ; load the loop count
			 lbl2:in al, dx
					loop lbl2
				}
	}
	outp(0x20,0x20);
}

setinst(synth,chan,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10)
int chan;
unsigned char synth,v0,v1,v2,v3,v4,v5,v6,v7,v8,v9,v10;
{
	int offs;

	outdata(synth,0xa0+chan,0);
	outdata(synth,0xb0+chan,0);
	outdata(synth,0xc0+chan,v10);
	if (chan == 0)
		offs = 0;
	if (chan == 1)
		offs = 1;
	if (chan == 2)
		offs = 2;
	if (chan == 3)
		offs = 8;
	if (chan == 4)
		offs = 9;
	if (chan == 5)
		offs = 10;
	if (chan == 6)
		offs = 16;
	if (chan == 7)
		offs = 17;
	if (chan == 8)
		offs = 18;
	outdata(synth,0x20+offs,v5);
	outdata(synth,0x40+offs,v6);
	outdata(synth,0x60+offs,v7);
	outdata(synth,0x80+offs,v8);
	outdata(synth,0xe0+offs,v9);
	offs+=3;
	outdata(synth,0x20+offs,v0);
	outdata(synth,0x40+offs,v1);
	outdata(synth,0x60+offs,v2);
	outdata(synth,0x80+offs,v3);
	outdata(synth,0xe0+offs,v4);
}

setmidiinsts()
{
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,0xc0);
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,midiinst);
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,0xc1);
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,midiinst);
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,0xc2);
	midiscrap = 256;
	while ((midiscrap > 0) && ((inp(statport) & 0x40) != 0))
		midiscrap--;
	if (midiscrap > 0)
		outp(dataport,14);
}
