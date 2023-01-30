DECLARE SUB drawline (x0%, y0%, x1%, y1%, col%)
RANDOMIZE TIMER
DIM regs&(7): regs&(0) = &H38: CALL int86old(&H10, regs&(), regs&())
DIM board%(64, 64), bmpkind%(256), tempbuf%(64, 64)
DIM tantable&(2048), costable&(2048), sintable&(2048), radarang%(320)
OPEN "tables.dat" FOR BINARY AS #1
FOR z% = 0 TO 2047
   GET #1, , sintable&(z%)
NEXT z%
FOR z% = 0 TO 2047
   GET #1, , tantable&(z%)
NEXT z%
FOR z% = 0 TO 319
   GET #1, , radarang%(z%)
NEXT z%
CLOSE #1
FOR z% = 0 TO 2047
   costable&((z% + 1536) AND 2047) = sintable&(z%)
NEXT z%
FOR z% = 1 TO 31
   bmpkind%(z%) = 1
NEXT z%
FOR z% = 32 TO 255
   bmpkind%(z%) = 2
NEXT z%
bmpkind%(0) = 0
bmpkind%(64) = 1
OPEN "boards.dat" FOR BINARY AS #1
z$ = STRING$(4096, 0)
GET #1, , z$
CLOSE #1
FOR z% = 0 TO 63
   FOR zz% = 0 TO 63
      'board%(z%, zz%) = ASC(MID$(z$, z% * 64 + zz% + 1, 1))
      board%(z%, zz%) = 0
      IF (z% \ 4 = z% / 4) AND (zz% \ 4 = zz% / 4) THEN board%(z%, zz%) = 32
      IF z% = 0 OR z% = 32 OR z% = 63 OR zz% = 0 OR zz% = 32 OR zz% = 63 THEN board%(z%, zz%) = 1
      IF board%(z%, zz%) > 0 THEN
         CALL drawline(z% * 12, zz% * 12, z% * 12 + 11, zz% * 12, board%(z%, zz%))
         CALL drawline(z% * 12 + 11, zz% * 12, z% * 12 + 11, zz% * 12 + 11, board%(z%, zz%))
         CALL drawline(z% * 12 + 11, zz% * 12 + 11, z% * 12, zz% * 12 + 11, board%(z%, zz%))
         CALL drawline(z% * 12, zz% * 12 + 11, z% * 12, zz% * 12, board%(z%, zz%))
      END IF
   NEXT zz%
NEXT z%

DO
   DO
      posx& = INT(32768 * RND)
      posy& = INT(32768 * RND)
   LOOP WHILE board%(posx& \ 512, posy& \ 512) <> 0
   ang% = INT(2048 * RND)
   FOR i% = 0 TO 319
      angle% = (radarang%(i%) + ang% + 2048) AND 2047
      tan1& = tantable&((2560 - angle%) AND 2047)
      tan2& = tantable&(angle%)
      xdir& = 512 - ((angle% + 512) AND 1024)
      ydir& = 512 - (angle% AND 1024)
      alg1% = 0
      alg2% = 0
      yl0& = 2147483647
      newyl0& = yl0&
      IF tan1& <= -2048 OR tan1& >= 2048 THEN
         alg1% = 1
         x& = posx& AND &HFE00
         IF xdir& = -512 THEN x& = x& OR 511
         increment& = (xdir& * 65536) \ tan1&
         x& = x& + xdir&
         y& = (((x& - posx&) * 65536) \ tan1&) + posy&
         x1& = x&: y1& = y&: inc1& = increment&
         IF x& >= 0 AND y& >= 0 AND x& < 32768 AND y& < 32768 THEN
            IF board%(x& \ 512, y& \ 512) <> 1 THEN
               DO
                  x& = x& + xdir&
                  y& = y& + increment&
                  IF x& < 0 OR y& < 0 OR x& > 32767 OR y& > 32767 THEN y& = -1: EXIT DO
               LOOP WHILE board%(x& \ 512, y& \ 512) <> 1
            END IF
         END IF
         IF y& <> -1 THEN
            yl0& = ((x& - posx&) * costable&(ang%) + (y& - posy&) * sintable&(ang%)) \ 256&
            realx& = x&
            realy& = y&
         END IF
      END IF
      IF tan2& <= -2048 OR tan2& >= 2048 THEN
         alg2% = 1
         y& = posy& AND &HFE00
         IF ydir& = -512 THEN y& = y& OR 511
         increment& = (ydir& * 65536) \ tan2&
         y& = y& + ydir&
         x& = (((y& - posy&) * 65536) \ tan2&) + posx&
         x2& = x&: y2& = y&: inc2& = increment&
         IF x& >= 0 AND y& >= 0 AND x& < 32768 AND y& < 32768 THEN
            IF board%(x& \ 512, y& \ 512) <> 1 THEN
               DO
                  y& = y& + ydir&
                  x& = x& + increment&
                  IF x& < 0 OR y& < 0 OR x& > 32767 OR y& > 32767 THEN x& = -1: EXIT DO
               LOOP WHILE board%(x& \ 512, y& \ 512) <> 1
            END IF
         END IF
         IF x& <> -1 THEN
            newyl0& = ((x& - posx&) * costable&(ang%) + (y& - posy&) * sintable&(ang%)) \ 256&
            IF newyl0& < yl0& THEN
               realx& = x&
               realy& = y&
            END IF
         END IF
      END IF
      CALL drawline(realx& / 42.67, realy& / 42.67, posx& / 42.67, posy& / 42.67, i% AND 255)
      stat1% = 0
      stat2% = 0
      IF ABS(inc1&) < ABS(inc2&) THEN
         WHILE (((stat1% OR stat2%) AND 1) = 0)
            IF x1& >= 0 AND y1& >= 0 AND x1& < 32768 AND y1& < 32768 THEN
               stat1% = bmpkind%(board%(x1& \ 512, y1& \ 512))
               IF stat1% = 2 THEN tempbuf%(x1& \ 512, y1& \ 512) = 1
               x1& = x1& + xdir&
               y1& = y1& + inc1&
               IF ((y2& > y1&) <> (ydir& > 0)) THEN
                  IF x2& >= 0 AND y2& >= 0 AND x2& < 32768 AND y2& < 32768 THEN
                     stat2% = bmpkind%(board%(x2& \ 512, y2& \ 512))
                     IF stat2% = 2 THEN tempbuf%(x2& \ 512, y2& \ 512) = 1
                     x2& = x2& + inc2&
                     y2& = y2& + ydir&
                  END IF
               END IF
            END IF
         WEND
      ELSE
         WHILE (((stat1% OR stat2%) AND 1) = 0)
            IF x2& >= 0 AND y2& >= 0 AND x2& < 32768 AND y2& < 32768 THEN
               stat2% = bmpkind%(board%(x2& \ 512, y2& \ 512))
               IF stat2% = 2 THEN tempbuf%(x2& \ 512, y2& \ 512) = 1
               x2& = x2& + inc2&
               y2& = y2& + ydir&
               IF ((x1& > x2&) <> (xdir& > 0)) THEN
                  IF x1& >= 0 AND y1& >= 0 AND x1& < 32768 AND y1& < 32768 THEN
                     stat1% = bmpkind%(board%(x1& \ 512, y1& \ 512))
                     IF stat1% = 2 THEN tempbuf%(x1& \ 512, y1& \ 512) = 1
                     x1& = x1& + xdir&
                     y1& = y1& + inc1&
                  END IF
               END IF
            END IF
         WEND
      END IF
   NEXT i%
   FOR x% = 0 TO 63
      FOR y% = 0 TO 63
         IF tempbuf%(x%, y%) > 0 THEN
            tempbuf%(x%, y%) = 0
            CALL drawline(x% * 12 + 1, y% * 12 + 1, x% * 12 + 11, y% * 12 + 11, 15)
            CALL drawline(x% * 12 + 11, y% * 12 + 1, x% * 12 + 1, y% * 12 + 11, 15)
         END IF
      NEXT y%
   NEXT x%
   DO: LOOP WHILE INKEY$ = ""
LOOP

SUB drawline (x0%, y0%, x1%, y1%, col%)
   DEF SEG = &HA000
   xdim% = ABS(x1% - x0%)
   ydim% = ABS(y1% - y0%)
   IF xdim% >= ydim% THEN
      IF x1% < x0% THEN SWAP x0%, x1%: SWAP y0%, y1%
      p& = (y0% AND 63) * 1024& + x0%
      pag% = y0% \ 64
      OUT &H3CD, pag%
      cnt% = 0
      IF y1% > y0% THEN pinc& = 1024 ELSE pinc& = -1024
      FOR zx% = 0 TO xdim%
         POKE p&, col%
         cnt% = cnt% + ydim%
         IF cnt% >= xdim% THEN
            cnt% = cnt% - xdim%
            p& = p& + pinc&
            IF p& < 0 THEN pag% = pag% - 1: OUT &H3CD, pag%: p& = p& + 65536
            IF p& > 65535 THEN pag% = pag% + 1: OUT &H3CD, pag%: p& = p& - 65536
         END IF
         p& = p& + 1
      NEXT zx%
   END IF
   IF xdim% < ydim% THEN
      IF y1% < y0% THEN SWAP x0%, x1%: SWAP y0%, y1%
      p& = (y0% AND 63) * 1024& + x0%
      pag% = y0% \ 64
      OUT &H3CD, pag%
      cnt% = 0
      IF x1% > x0% THEN pinc& = 1 ELSE pinc& = -1
      FOR zx% = 0 TO ydim%
         POKE p&, col%
         cnt% = cnt% + xdim%
         IF cnt% >= ydim% THEN
            cnt% = cnt% - ydim%
            p& = p& + pinc&
         END IF
         p& = p& + 1024
         IF p& < 0 THEN pag% = pag% - 1: OUT &H3CD, pag%: p& = p& + 65536
         IF p& > 65535 THEN pag% = pag% + 1: OUT &H3CD, pag%: p& = p& - 65536
      NEXT zx%
   END IF
END SUB

