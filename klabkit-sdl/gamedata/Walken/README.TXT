11/02/1999:

About this release:

   The history of 3D gaming on the PC platform is always an entertaining
subject to think about.  Many people consider Wolfenstein 3D to be the first
"good" 3D game worth mentioning.  "Ken's Labyrinth" was in fact the first
Wolfenstein clone.
   Back in 1992, I had just finished my junior year in high school.  I
thought that trying to create the same kind of 3D environment as Wolfenstein
3D would be a fun challenge.  Even though the game wasn't a success, I still
get a kick out of how many people still remember the name "Ken's Labyrinth".
   This release is for historical purposes only - for those fans that like
to see what a work in progress looks like.  In no way is this intended to be
competitive with today's games!

Some things you should notice while playing the demo:

* The cheat keys in this version work just like in the later releases except
     you don't need to specify anything on the command line to get into cheat
     mode.
* Spinning sprites haven't been programmed yet (fans & warps)
* See-through walls haven't been programmed yet (no doors)
* There is no key for walking sideways (strafe key)
* Horizontal and vertical walls are not different shades
* Only 1 weapon (fire balls)
* No shootable walls

------------------------------------------------------------------------------
05/31/2009: Additional notes on source code / editing kit:

EDITDATA.BAS: Ken's Labyrinth art (WALLS.DAT) and map (BOARDS.DAT) editor.
   The editor is incompatible, but the keys are mostly the same as the
   version from KLABKIT.ZIP. You must use Microsoft QuickBasic 4.5 (or later)
   and load the QuickLibrary (QB.QLB) for mouse support (Example:
   "qb /l qb.qlb editdata.bas"). It will not run in the stipped-down version
   called QBasic. The most important keys inside EDITDATA.BAS are:
      * PGUP/PGDN to change map or art tile.
      * Enter to switch between map (BOARDS.DAT) and art (WALLS.DAT) mode.
   See EDITDATA.TXT in KLABKIT.ZIP for a more info of the controls.

WALLS.DAT: I included this file because I never wrote a decompressor for
   WALLS.KZP -> WALLS.DAT. WALKEN used simple RLE compression in WALLS.KZP.

COMPRESS.C/COMPRESS.EXE: The Ken's Labyrinth game executable uses the
   compressed walls files: WALLS.KZP. You must run COMPRESS.EXE to see any
   art changes that you make in EDITDATA.BAS. I did not use compression for
   BOARDS at the time. The version in WALKEN has no command line options.
   Simply run it and it will convert WALLS.DAT to WALLS.KZP.

WALKEN.C: Walken source code. Made for 16-bit MSC 6.00a (old Dos compiler).

SETUP.C: Source code to the WALKEN setup program.

------------------------------------------------------------------------------

Credits for this pre-release demo:
   Ken Silverman: 100% programming, some of the maps, 2/3 of the art, music
   Andy Cotter: Most of the maps, 1/3 of the art, some of the .WAV's

-Ken Silverman (official web page: http://advsys.net/ken)
