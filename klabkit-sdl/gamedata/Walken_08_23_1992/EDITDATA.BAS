CLS
numwalls% = 64
numboards% = 5
OPTION BASE 0
DIM regs%(7): regs%(0) = 0: CALL int86old(&H33, regs%(), regs%())
DIM pal%(256, 3)
DIM buf%(64, 64)
DIM tempbuf%(64, 64)
DIM walstatus%(64)
boardnum& = 0: wallnum& = 0: anywallstatchg% = 0
OPEN "boards.dat" FOR BINARY AS #1
OPEN "walls.dat" FOR BINARY AS #2
SEEK #2, numwalls% * 4096& + 1
buf$ = STRING$(64, 0)
GET #2, , buf$
FOR z% = 0 TO 63
   walstatus%(z%) = ASC(MID$(buf$, z% + 1, 1))
NEXT z%
SCREEN 13
LOCATE 25, 1: PRINT "Loading data...";
GOSUB setupcolors
LOCATE 25, 1: PRINT "               ";
palval% = 0
trailstatus% = 0
z$ = ""
DO UNTIL z$ = CHR$(27)
   IF z$ <> CHR$(27) THEN GOSUB editboards
   IF z$ <> CHR$(27) THEN GOSUB editwalls
LOOP
IF anywallstatchg% = 1 THEN
   buf$ = ""
   FOR z% = 0 TO 63
      buf$ = buf$ + CHR$(walstatus%(z%))
   NEXT z%
   SEEK #2, numwalls% * 4096& + 1
   PUT #2, , buf$
END IF
CLOSE #1
CLOSE #2
END

editboards:
   GOSUB loadnewboard
   c% = 32: d% = 32
   mousx% = c% * 3: mousy% = d% * 3
   col% = 0
   GOSUB printcol
   DO
      oldmousx% = mousx%: oldmousy% = mousy%
      DO
         LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), 15, BF
         GOSUB mouse
         z$ = UCASE$(INKEY$)
         OUT &H3C8, 252
         WAIT &H3DA, 8
         FOR z% = 1 TO 12
            OUT &H3C9, palval%
         NEXT z%
         palval% = (palval% + 1) AND 63
      LOOP WHILE z$ = "" AND mousx% \ 3 = oldmousx% \ 3 AND mousy% \ 3 = oldymousy% \ 3
      LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), buf%(c%, d%), BF
      IF trailstatus% = 0 THEN LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), buf%(c%, d%), BF
      IF trailstatus% = 1 THEN LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), col%, BF: buf%(c%, d%) = col%
      IF z$ = CHR$(0) + CHR$(72) AND d% > 0 THEN d% = d% - 1: mousy% = mousy% - 3
      IF z$ = CHR$(0) + CHR$(80) AND d% < 63 THEN d% = d% + 1: mousy% = mousy% + 3
      IF z$ = CHR$(0) + CHR$(75) AND c% > 0 THEN c% = c% - 1: mousx% = mousx% - 3
      IF z$ = CHR$(0) + CHR$(77) AND c% < 63 THEN c% = c% + 1: mousx% = mousx% + 3
      IF z$ = CHR$(0) + CHR$(73) AND boardnum& > 0 THEN
         GOSUB testsaveboardchanges
         boardnum& = boardnum& - 1
         GOSUB loadnewboard
      END IF
      IF z$ = CHR$(0) + CHR$(81) AND boardnum& < numboards% - 1 THEN
         GOSUB testsaveboardchanges
         boardnum& = boardnum& + 1
         GOSUB loadnewboard
      END IF
      c% = mousx% \ 3
      d% = mousy% \ 3
      IF z$ = "-" AND col% > 0 THEN col% = col% - 1: GOSUB printsprite
      IF z$ = "+" AND col% < 255 THEN col% = col% + 1: GOSUB printsprite
      IF z$ = " " OR bstatus% > 0 THEN buf%(c%, d%) = col%
      IF z$ = "A" THEN GOSUB addboard
      IF z$ = "T" OR z$ = "t" THEN trailstatus% = 1 - trailstatus%: GOSUB printcol
      IF z$ = "R" THEN
         LOCATE 2, 26: PRINT "Rotate way?";
         DO
            z$ = UCASE$(INKEY$)
         LOOP WHILE z$ = ""
         LOCATE 2, 26: PRINT "           ";
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               tempbuf%(z%, zz%) = buf%(z%, zz%)
            NEXT zz%
         NEXT z%
         IF z$ = CHR$(0) + CHR$(72) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%(z%, (zz% + 63) AND 63)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(80) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%(z%, (zz% + 1) AND 63)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(75) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%((z% + 63) AND 63, zz%)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(77) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%((z% + 1) AND 63, zz%)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
      END IF
      IF z$ = "M" THEN
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               tempbuf%(z%, zz%) = buf%(z%, zz%)
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "P" THEN
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               buf%(z%, zz%) = tempbuf%(z%, zz%)
               LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "C" THEN
         chanfromcol% = buf%(c%, d%)
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               IF buf%(z%, zz%) = chanfromcol% THEN
                  buf%(z%, zz%) = col%
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), col%, BF
               END IF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "F" THEN
         LINE (0, 0)-(191, 191), col%, B
         PAINT (c% * 3 + 1, d% * 3 + 1), col%, col%
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               buf%(z%, zz%) = POINT(z% * 3 + 1, zz% * 3 + 1)
               LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = CHR$(9) THEN col% = buf%(c%, d%): GOSUB printsprite
   LOOP WHILE z$ <> CHR$(27) AND z$ <> CHR$(13)
   GOSUB testsaveboardchanges
RETURN

editwalls:
   GOSUB loadnewall
   DEF SEG = &HA000
   c% = 32: d% = 32
   mousx% = c% * 3: mousy% = d% * 3
   col% = 0
   GOSUB printcol
   DO
      oldmousx% = mousx%: oldmousy% = mousy%
      DO
         WAIT &H3DA, 8
         WAIT &H3DA, 8, 255
         LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), (buf%(c%, d%) + 147) MOD 252, BF
         WAIT &H3DA, 8
         WAIT &H3DA, 8, 255
         LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), buf%(c%, d%), BF
         GOSUB mouse
         z$ = UCASE$(INKEY$)
         OUT &H3C8, 252
         WAIT &H3DA, 8
         FOR z% = 1 TO 12
            OUT &H3C9, palval%
         NEXT z%
         palval% = (palval% + 1) AND 63
      LOOP WHILE z$ = "" AND mousx% \ 3 = oldmousx% \ 3 AND mousy% \ 3 = oldymousy% \ 3
      DEF SEG = 0: keystate% = PEEK(&H417)
      IF trailstatus% = 0 THEN LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), buf%(c%, d%), BF
      IF trailstatus% = 1 THEN LINE (c% * 3, d% * 3)-(c% * 3 + 2, d% * 3 + 2), col%, BF: buf%(c%, d%) = col%
      IF z$ = "R" THEN
         LOCATE 2, 26: PRINT "Rotate way?";
         DO
            z$ = UCASE$(INKEY$)
         LOOP WHILE z$ = ""
         LOCATE 2, 26: PRINT "           ";
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               tempbuf%(z%, zz%) = buf%(z%, zz%)
            NEXT zz%
         NEXT z%
         IF z$ = CHR$(0) + CHR$(72) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%(z%, (zz% + 63) AND 63)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(80) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%(z%, (zz% + 1) AND 63)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(75) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%((z% + 63) AND 63, zz%)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
         IF z$ = CHR$(0) + CHR$(77) THEN
            FOR z% = 0 TO 63
               FOR zz% = 0 TO 63
                  buf%(z%, zz%) = tempbuf%((z% + 1) AND 63, zz%)
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
               NEXT zz%
            NEXT z%
         END IF
      END IF
      IF (keystate% AND 7) = 0 THEN
         IF z$ = CHR$(0) + CHR$(72) AND d% > 0 THEN d% = d% - 1: mousy% = mousy% - 3
         IF z$ = CHR$(0) + CHR$(80) AND d% < 63 THEN d% = d% + 1: mousy% = mousy% + 3
         IF z$ = CHR$(0) + CHR$(75) AND c% > 0 THEN c% = c% - 1: mousx% = mousx% - 3
         IF z$ = CHR$(0) + CHR$(77) AND c% < 63 THEN c% = c% + 1: mousx% = mousx% + 3
      END IF
      IF (keystate% AND 3) > 0 THEN
         GOSUB clearcol
         IF z$ = CHR$(0) + CHR$(72) AND col% > 0 THEN col% = col% - 1
         IF z$ = CHR$(0) + CHR$(80) AND col% < 251 THEN col% = col% + 1
         IF z$ = CHR$(0) + CHR$(75) AND col% > 41 THEN col% = col% - 42
         IF z$ = CHR$(0) + CHR$(77) AND col% < 252 - 42 THEN col% = col% + 42
         GOSUB printcol
      END IF
      IF z$ = "T" OR z$ = "t" THEN trailstatus% = 1 - trailstatus%: GOSUB printcol
      IF z$ = "D" THEN
         LOCATE 2, 26: INPUT "Wall?"; wall%
         IF wall% > 0 AND wall% <= numwalls% THEN
            GOSUB testsavewallchanges
            wallnum& = wall% - 1
            GOSUB loadnewall
         ELSE
            SOUND 60, 1
         END IF
      LOCATE 2, 26: PRINT "            ";
      END IF
      IF z$ = CHR$(0) + CHR$(73) AND wallnum& > 0 THEN
         GOSUB testsavewallchanges
         wallnum& = wallnum& - 1
         GOSUB loadnewall
      END IF
      IF z$ = CHR$(0) + CHR$(81) AND wallnum& < numwalls% - 1 THEN
         GOSUB testsavewallchanges
         wallnum& = wallnum& + 1
         GOSUB loadnewall
      END IF
      c% = mousx% \ 3
      d% = mousy% \ 3
      IF z$ = CHR$(8) THEN col% = 255
      IF z$ = " " OR bstatus% > 0 THEN buf%(c%, d%) = col%
      IF z$ = "I" THEN
         walstatus%(wallnum&) = 1 - walstatus%(wallnum&)
         LOCATE 23, 30: PRINT "Inside =";
         IF walstatus%(wallnum&) = 0 THEN PRINT "on ";
         IF walstatus%(wallnum&) = 1 THEN PRINT "off";
         anywallstatchg% = 1
      END IF
      IF z$ = "A" THEN GOSUB addwall
      IF z$ = "V" THEN GOSUB showallwalls
      IF z$ = "M" THEN
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               tempbuf%(z%, zz%) = buf%(z%, zz%)
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "P" THEN
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               buf%(z%, zz%) = tempbuf%(z%, zz%)
               LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "C" THEN
         chanfromcol% = buf%(c%, d%)
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               IF buf%(z%, zz%) = chanfromcol% THEN
                  buf%(z%, zz%) = col%
                  LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), col%, BF
               END IF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = "F" THEN
         LINE (0, 0)-(191, 191), col%, B
         PAINT (c% * 3 + 1, d% * 3 + 1), col%, col%
         FOR z% = 0 TO 63
            FOR zz% = 0 TO 63
               buf%(z%, zz%) = POINT(z% * 3 + 1, zz% * 3 + 1)
               LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
            NEXT zz%
         NEXT z%
      END IF
      IF z$ = CHR$(9) THEN GOSUB clearcol: col% = buf%(c%, d%): GOSUB printcol
   LOOP WHILE z$ <> CHR$(27) AND z$ <> CHR$(13)
   GOSUB testsavewallchanges
RETURN

mouse:
   regs%(0) = 11: CALL int86old(&H33, regs%(), regs%())
   mousx% = mousx% + regs%(2): mousy% = mousy% + regs%(3)
   IF mousx% < 0 THEN mousx% = 0
   IF mousx% > 191 THEN mousx% = 191
   IF mousy% < 0 THEN mousy% = 0
   IF mousy% > 191 THEN mousy% = 191
   regs%(0) = 5: CALL int86old(&H33, regs%(), regs%())
   bstatus% = regs%(0)
RETURN

printsprite:
   LOCATE 1, 26: PRINT col%; "   ";
   DEF SEG = &HA000
   IF col% > 0 THEN
      SEEK #2, (col% - 1) * 4096& + 1
      tempbuf$ = STRING$(4096, 0)
      GET #2, , tempbuf$
      FOR zz% = 0 TO 63
         p% = zz% * 320 + 240
         FOR z% = 0 TO 63
            POKE p% + z%, ASC(MID$(tempbuf$, z% * 64 + zz% + 1, 1))
         NEXT z%
      NEXT zz%
   ELSE
      LINE (240, 0)-(303, 63), 0, BF
   END IF
RETURN
       
clearcol:
   LOCATE 22, 30: PRINT "TRAIL ";
   IF trailstatus% = 0 THEN PRINT "OFF";  ELSE PRINT "ON ";
   x% = 203 + (col% \ 42) * 3
   y% = 50 + (col% MOD 42) * 3
   PSET (x% + 1, y% + 1), col%
RETURN

printcol:
   LOCATE 1, 26: PRINT col%; "    ";
   LOCATE 22, 30: PRINT "TRAIL ";
   IF trailstatus% = 0 THEN PRINT "OFF";  ELSE PRINT "ON ";
   x% = 203 + (col% \ 42) * 3
   y% = 50 + (col% MOD 42) * 3
   PSET (x% + 1, y% + 1), 251 - col%
RETURN

setupcolors:
   c1% = 0: c2% = 0: c3% = 0
   OUT &H3C8, 0
   FOR z% = 0 TO 251
      OUT &H3C9, INT(c1% * 64 \ 6)
      OUT &H3C9, INT(c2% * 64 \ 7)
      OUT &H3C9, INT(c3% * 64 \ 6)
      c1% = c1% + 1
      IF c1% = 6 THEN c1% = 0: c2% = c2% + 1
      IF c2% = 7 THEN c2% = 0: c3% = c3% + 1
   NEXT z%
   FOR z% = 1 TO 12
      OUT &H3C9, 63
   NEXT z%
   FOR z% = 0 TO 41
      LINE (203, z% * 3 + 50)-(205, z% * 3 + 52), z%, BF
      LINE (206, z% * 3 + 50)-(208, z% * 3 + 52), z% + 42, BF
      LINE (209, z% * 3 + 50)-(211, z% * 3 + 52), z% + 84, BF
      LINE (212, z% * 3 + 50)-(214, z% * 3 + 52), z% + 126, BF
      LINE (215, z% * 3 + 50)-(217, z% * 3 + 52), z% + 168, BF
      LINE (218, z% * 3 + 50)-(220, z% * 3 + 52), z% + 210, BF
   NEXT z%
RETURN
  
testsavewallchanges:
   LOCATE 25, 1: PRINT "Save changes?";
   zx$ = INPUT$(1)
   IF zx$ = "y" OR zx$ = "Y" THEN
      buf$ = ""
      FOR z% = 0 TO 63
         FOR zz% = 0 TO 63
            buf$ = buf$ + CHR$(buf%(z%, zz%))
         NEXT zz%
      NEXT z%
      SEEK #2, wallnum& * 4096& + 1
      PUT #2, , buf$
   END IF
   LOCATE 25, 1: PRINT "             ";
RETURN

testsaveboardchanges:
   LOCATE 25, 1: PRINT "Save changes?";
   zx$ = INPUT$(1)
   IF zx$ = "y" OR zx$ = "Y" THEN
      buf$ = ""
      FOR z% = 0 TO 63
         FOR zz% = 0 TO 63
            buf$ = buf$ + CHR$(buf%(z%, zz%))
         NEXT zz%
      NEXT z%
      SEEK #1, boardnum& * 4096& + 1
      PUT #1, , buf$
   END IF
   LOCATE 25, 1: PRINT "             ";
RETURN

loadnewall:
   LOCATE 24, 30: PRINT "Sprite"; wallnum& + 1;
   LOCATE 23, 30: PRINT "Inside =";
   IF walstatus%(wallnum&) = 0 THEN PRINT "on ";
   IF walstatus%(wallnum&) = 1 THEN PRINT "off";
   SEEK #2, wallnum& * 4096& + 1
   buf$ = STRING$(4096, 0)
   GET #2, , buf$
   FOR zz% = 0 TO 63
      FOR z% = 0 TO 63
         buf%(z%, zz%) = ASC(MID$(buf$, zz% + 64 * z% + 1, 1))
         LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
      NEXT z%
   NEXT zz%
RETURN

loadnewboard:
   LOCATE 24, 30: PRINT "Board"; boardnum& + 1; " ";
   LOCATE 23, 30: PRINT "            ";
   SEEK #1, boardnum& * 4096& + 1
   buf$ = STRING$(4096, 0)
   GET #1, , buf$
   GOSUB printsprite
   FOR zz% = 0 TO 63
      FOR z% = 0 TO 63
         buf%(z%, zz%) = ASC(MID$(buf$, zz% + 64 * z% + 1, 1))
         LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
      NEXT z%
   NEXT zz%
RETURN

showallwalls:
   SEEK #2, 1
   SEEK #1, 1
   regs%(0) = &H2E: CALL int86old(&H10, regs%(), regs%())
   DEF SEG = &HA000
   GOSUB setupcolors
   tempbuf$ = STRING$(4096, 0)
   FOR z% = 0 TO numwalls%
      IF z% < 64 THEN GET #2, , tempbuf$
      IF z% = 64 THEN GET #1, , tempbuf$
      p& = (z% \ 10) * 40960 + (z% MOD 10) * 64&
      FOR zz% = 0 TO 63
         FOR zzz% = 0 TO 63
            newp& = p& + zz% * 640& + zzz%
            OUT &H3CD, newp& \ 65536
            POKE newp& AND 65535, ASC(MID$(tempbuf$, zzz% * 64 + zz% + 1, 1))
         NEXT zzz%
      NEXT zz%
   NEXT z%
   DO
      OUT &H3C8, 252
      WAIT &H3DA, 8
      FOR z% = 1 TO 12
         OUT &H3C9, palval%
      NEXT z%
      palval% = (palval% + 1) AND 63
   LOOP WHILE INKEY$ = ""
   regs%(0) = &H13: CALL int86old(&H10, regs%(), regs%())
   DEF SEG = &HA000
   GOSUB setupcolors
   GOSUB loadnewall
   GOSUB printcol
RETURN

addwall:
   LOCATE 25, 1: PRINT "What wall file"; : INPUT zx$
   OPEN zx$ FOR BINARY AS #3
   LOCATE 25, 1: PRINT STRING$(40, 32);
   LOCATE 25, 1: PRINT "What wall number"; : INPUT zx&
   SEEK #3, zx& * 4096& + 1
   buf$ = STRING$(4096, 0)
   GET #3, , buf$
   CLOSE #3
   CLS
   GOSUB setupcolors
   LOCATE 24, 30: PRINT "Sprite"; wallnum& + 1;
   LOCATE 23, 30: PRINT "Inside =";
   IF walstatus%(wallnum&) = 0 THEN PRINT "on ";
   IF walstatus%(wallnum&) = 1 THEN PRINT "off";
   GOSUB printcol
   FOR z% = 0 TO 63
      FOR zz% = 0 TO 63
         buf%(z%, zz%) = ASC(MID$(buf$, zz% + 64 * z% + 1, 1))
         LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
      NEXT zz%
   NEXT z%
RETURN

addboard:
   LOCATE 25, 1: PRINT "What board file"; : INPUT zx$
   OPEN zx$ FOR BINARY AS #3
   LOCATE 25, 1: PRINT STRING$(40, 32);
   LOCATE 25, 1: PRINT "What board number"; : INPUT zx&
   SEEK #3, zx& * 4096& + 1
   buf$ = STRING$(4096, 0)
   GET #3, , buf$
   CLOSE #3
   CLS
   GOSUB setupcolors
   LOCATE 24, 30: PRINT "Sprite"; wallnum& + 1;
   LOCATE 23, 30: PRINT "Inside =";
   IF walstatus%(wallnum&) = 0 THEN PRINT "on ";
   IF walstatus%(wallnum&) = 1 THEN PRINT "off";
   GOSUB printcol
   FOR z% = 0 TO 63
      FOR zz% = 0 TO 63
         buf%(z%, zz%) = ASC(MID$(buf$, zz% + 64 * z% + 1, 1))
         LINE (z% * 3, zz% * 3)-(z% * 3 + 2, zz% * 3 + 2), buf%(z%, zz%), BF
      NEXT zz%
   NEXT z%
RETURN

