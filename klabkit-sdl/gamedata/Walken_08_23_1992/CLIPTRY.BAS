SCREEN 12
RANDOMIZE TIMER
DIM board%(16, 16)
FOR z% = 0 TO 15
   LINE (z% * 30, 0)-(z% * 30, 479), 7
   LINE (0, z% * 30)-(479, z% * 30), 7
   LINE (z% * 30 + 9, 0)-(z% * 30 + 9, 479), 8
   LINE (0, z% * 30 + 9)-(479, z% * 30 + 9), 8
   LINE (z% * 30 + 20, 0)-(z% * 30 + 20, 479), 8
   LINE (0, z% * 30 + 20)-(479, z% * 30 + 20), 8
NEXT z%
FOR z% = 0 TO 15
   FOR zz% = 0 TO 15
      board%(z%, zz%) = INT(7 * RND)
      IF board%(z%, zz%) > 1 THEN board%(z%, zz%) = 0
      IF z% = 0 OR zz% = 0 OR z% = 15 OR zz% = 15 THEN board%(z%, zz%) = 1
      IF board%(z%, zz%) = 1 THEN LINE (z% * 30 - 9, zz% * 30 - 9)-(z% * 30 + 38, zz% * 30 + 38), 8, BF
   NEXT zz%
NEXT z%
FOR z% = 0 TO 15
   FOR zz% = 0 TO 15
      IF board%(z%, zz%) = 1 THEN LINE (z% * 30, zz% * 30)-(z% * 30 + 29, zz% * 30 + 29), 4, BF
   NEXT zz%
NEXT z%
DO
   x = INT(16 * RND) * 30 + 15
   y = INT(16 * RND) * 30 + 15
LOOP WHILE board%(x \ 30, y \ 30) = 1
PSET (x, y), 15
ang = 0
vel = 25
DO
   ang = INT(360 * RND)
   z$ = INKEY$
   IF z$ = CHR$(0) + CHR$(72) AND vel < .5 THEN vel = vel + .01
   IF z$ = CHR$(0) + CHR$(80) AND vel > 0 THEN vel = vel - .01
   IF z$ = CHR$(0) + CHR$(75) THEN ang = (ang + 355) MOD 360
   IF z$ = CHR$(0) + CHR$(77) THEN ang = (ang + 5) MOD 360
   oldx = x: oldy = y
   x = x + vel * COS(ang * 3.141592 / 180)
   y = y + vel * SIN(ang * 3.141592 / 180)
   done% = 0
   plcx1 = -1: plcy2 = -1
   IF (x - 10) \ 30 < (oldx - 10) \ 30 THEN plcx1 = ((oldx - 10) \ 30) * 30 + 10: plcy1 = oldy + (plcx1 - oldx) * TAN(ang * 3.141592 / 180)
   IF (x + 10) \ 30 > (oldx + 10) \ 30 THEN plcx1 = ((oldx + 10) \ 30) * 30 + 19: plcy1 = oldy + (plcx1 - oldx) * TAN(ang * 3.141592 / 180)
   IF (y - 10) \ 30 < (oldy - 10) \ 30 THEN plcy2 = ((oldy - 10) \ 30) * 30 + 10: plcx2 = oldx + (plcy2 - oldy) / TAN(ang * 3.141592 / 180)
   IF (y + 10) \ 30 > (oldy + 10) \ 30 THEN plcy2 = ((oldy + 10) \ 30) * 30 + 19: plcx2 = oldx + (plcy2 - oldy) / TAN(ang * 3.141592 / 180)
   IF plcx1 <> -1 THEN
      IF x < oldx AND (board%(plcx1 \ 30 - 1, (plcy1 - 11) \ 30) > 0 OR board%(plcx1 \ 30 - 1, (plcy1 + 11) \ 30) > 0) THEN
         x = (oldx \ 30) * 30 + 10: SOUND 262, .1
      ELSEIF x > oldx AND (board%(plcx1 \ 30 + 1, (plcy1 - 11) \ 30) > 0 OR board%(plcx1 \ 30 + 1, (plcy1 + 11) \ 30) > 0) THEN
         x = (oldx \ 30) * 30 + 19: SOUND 262, .1
      END IF
   END IF
   IF plcy2 <> -1 THEN
      IF y < oldy AND (board%((plcx2 - 11) \ 30, plcy2 \ 30 - 1) > 0 OR board%((plcx2 + 11) \ 30, plcy2 \ 30 - 1) > 0) THEN
         y = (oldy \ 30) * 30 + 10: SOUND 330, .1
      ELSEIF y > oldy AND (board%((plcx2 - 11) \ 30, plcy2 \ 30 + 1) > 0 OR board%((plcx2 + 11) \ 30, plcy2 \ 30 + 1) > 0) THEN
         y = (oldy \ 30) * 30 + 19: SOUND 330, .1
      END IF
   END IF
   LINE (oldx, oldy)-(x, y), 15
LOOP

