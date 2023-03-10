#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <io.h>
#include <sys\types.h>
#include <sys\stat.h>
#define numwalls 80

static unsigned char buf1[4096], buf2[8192];

main()
{
	unsigned char oldat, dat;
	int i, j, cnt, bytecount, fil1, fil2;

	if ((fil1 = open("walls.dat",O_BINARY|O_RDWR,S_IREAD)) == -1)
	{
		printf("Unable to open walls.dat\n");
		exit(-1);
	}
	if ((fil2 = open("walls.kzp",O_BINARY|O_CREAT|O_WRONLY,S_IWRITE)) == -1)
	{
		printf("Unable to create walls.kzp\n");
		exit(-1);
	}
	lseek(fil1,((long)numwalls)<<12,SEEK_SET);
	read(fil1,&buf1[0],numwalls);
	lseek(fil1,0,SEEK_SET);
	write(fil2,&buf1[0],numwalls);
	for(j=0;j<numwalls;j++)
	{
		printf("%d ",j);
		read(fil1,&buf1[0],4096);
		bytecount = 0;
		cnt = 0;
		dat = buf1[0];
		for(i=0;i<4096;i++)
		{
			oldat = dat;
			dat = buf1[i];
			if ((oldat != dat) || (cnt == 255))
			{
				buf2[bytecount] = oldat, bytecount++;
				buf2[bytecount] = cnt, bytecount++;
				cnt = 0;
			}
			cnt++;
		}
		buf2[bytecount] = dat, bytecount++;
		buf2[bytecount] = cnt, bytecount++;
		write(fil2,&buf2[0],bytecount);
	}
	close(fil1);
	close(fil2);
}
