										 Controls
								旼컴컴컴컴컴컴컴컴컴커
								� +         - Faster �
								� -         - Slower �
								� SPACE BAR - Pause  �
								� ESC       - Quit   �
								읕컴컴컴컴컴컴컴컴컴켸




旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�            旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커            �
�            �    K e n ' s   L a b y r i n t h    N o t e s :    �            �
�            읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸            �
�                                                                              �
�    Started:   Monday, October 12, 1992                                       �
�    Revised:   Friday, January 15, 1993                                       �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          1.  History:                                                        �
�                                                                              �
�     On June 9, 1992, I had first seen the shareware version of Wolfenstein.  �
� Since my brother (Alan) was playing Wolfenstein constantly, I decided to win �
� over his time by creating a game similar to it, except my own.  So on June   �
� 16, 1992, I started, never knowing that the game would turn into what it is  �
� today.  The first thing I did was to figure out how to draw variable size    �
� vertical lines from a 64*64 bitmap.  Then, I studied about how I could       �
� project this onto the screen.  Next, I decided to store four mathematical    �
� pictures in a file, with the fifth one being my picture.  The number of      �
� pictures has been increasing steadily over the past four months, currently   �
� at 192.  The next big task to solve was how to clip movement so you do not   �
� go through walls.  After some time, I decided to start some of the many      �
� algorithms that draw planes on the screen quickly and correctly. This was    �
� one of the toughest parts of the program, yet after a week of working on the �
� problem, I finally solved it.  In October, I was constantly adding the       �
� smaller features, such as a life bar, 8 monsters, 2 hazards (fans & holes) 3 �
� types of food, 2 keys, 4 magical items (2 potions and capes), the status     �
� bar, the floor map automatically taking the place of a reserved bitmap,      �
� Adlib music, 360*240 resolution support, Sound Blaster digitized sound       �
� support, death animation, introduction screen, custom setup by means of a    �
� separate setup program, and animations at the ends of floors.  So far there  �
� are 27 floors, each having its own original song.  I thank Andrew Cotter for �
� creating 22 out of the 27 current floors, for drawing just about half of all �
� of the art, and for sharing his many ideas that are currently in the game.   �
�                                                                              �
�    Here are some of the more recent additions:                               �
�    컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�                               �
� - XMS extended memory support.                                               �
� - Introduction screen in the GIF format.                                     �
� - WALLS.KZP now uses LZW compression rather than Run-Length Encoding.        �
� - 3d-shading.                                                                �
� - New weapon selection keys.                                                 �
� - Strafe key.                                                                �
� - Doors that you can open and close.                                         �
� - New type of secret - Shootable walls, not just walk-through walls.         �
� - Much better art job.                                                       �
� - Story screen.                                                              �
� - Cool readme program. (I hope you understand this one)                      �
� - Up to 16 warp zones per level.                                             �
� - Final boss                                                                 �
� - High score table                                                           �
�                                                                              �
�    Additions since the First Shareware Release (as of 1/1/93)                �
�    컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴                �
�                                                                              �
� - Joystick support                                                           �
� - Coins & Soda machines                                                      �
� - Improved art on several enemies                                            �
� - A few more songs to make an original song for each level.                  �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          2.  Story:                                                          �
�                                                                              �
�     The fate of the Planet Earth is in your hands!  The scientific community �
� on the planet Zogar has been searching the universe for the alien race most  �
� able to survive adversity. Some have been extremely intelligent, but were    �
� lacking in physical skills.  Others were great warriors but did not match    �
� the Zogarians in intellect.  You will be judged by your performance in a     �
� unique labyrinth on Zogar, one which will test your physical and mental      �
� aptitude (you know, like one of those Idaho tests, but with music in the     �
� background).  So far, not one civilization has been deemed worthy by         �
� King Xenhai.  Should you fail, well, let's just say that Earth will be blown �
� into a million pieces, each the size of a pea.                               �
�                                                                              �
�     Your objective is simple:  Find your way out of the labyrinth.  On top   �
� of the labyrinth is the spectators' box, where Xenhai has been watching.  If �
� you make it out, then Earth, and all life on it,  will be saved.             �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          3.  Requirements:                                                   �
�                                                                              �
�    - Any IBM PC compatible computer such as 8086,80286,80386,80486.          �
�          (80386 or better strongly recommended for best game speed)          �
�    - At least 965K of memory, with at least 210K in conventional, and an     �
�          XMS or EMS driver already loaded.                                   �
�    - VGA display card with VGA monitor.                                      �
�    - Approximately 926K of free hard disk space.                             �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          4.  Installation:                                                   �
�                                                                              �
�    Make a directory on the hard drive, then simply use the DOS "COPY"        �
� command to copy all the files on the disk into the directory on the hard     �
� drive.  Then type "SETUP" in the directory on the hard drive to set the      �
� computer up with the correct hardware and key definitions.  After saving     �
� the custom setup, type "LAB3D" to play the game.                             �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          5.  Controls:                                                       �
�                                                                              �
�    Before you play, be sure to use the setup program in the same directory   �
� by typing "SETUP".   If you do not define keys, here is a list of the        �
� default keys that I find easy to use:                                        �
�                                                                              �
�    Move FORWARD                     - Up arrow                               �
�    Move BACKWARD                    - Down arrow                             �
�    Turn LEFT                        - Left arrow                             �
�    Turn RIGHT                       - Right arrow                            �
�    RUN                              - Left shift                             �
�    STRAFE (walk sideways)           - Right ctrl                             �
�    SHOOT                            - Left ctrl                              �
�    Weapon 1 (red FIREBALLS)         - F1                                     �
�    Weapon 2 (green BOUNCY-BULLETS)  - F2                                     �
�    Weapon 3 (HEAT-SEEKING MISSILES) - F3                                     �
�    UNLOCK DOOR                      - Space bar                              �
�    STAND HIGH                       - "A"                                    �
�    STAND LOW                        - "Z"                                    �                      
�    RAISE/LOWER STATUS BAR           - Enter (above right shift key)          �
�    PAUSE GAME                       - "P"                                    �
�    LOAD GAME                        - "L"                                    �
�    SAVE GAME                        - "S"                                    �
�    QUIT GAME                        - ESCape                                 �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          6.  About the things seen in the game:                              �
�                                                                              �
�     THINGS THAT HELP YOU:                                                    �
�    旼컴컴컴컴컴컴컴컴컴컴컴                                                  �
�    쳐 Bullets - You can get up to six bullet powers of each kind of bullet.  �
�    �  �     The more bullet powers you have, the more often you can          �
�    �  �     shoot.                                                           �
�    �  쳐 Normal bullets (red) - Just a simple straight-flying bullet.        �
�    �  읕 Bouncy bullets (green) - These bullets bounce off all walls that    �
�    �        you cannot normally walk through.                                �
�    쳐 Lightnings - You can get up to six lightnings.  The more lightnings    �
�    �     you have, the farther each bullet will travel.                      �
�    쳐 Extra Lives - You can get up to 4 extra lives.  When you die, the      �
�    �     floor does NOT reset.  However you do go back to the starting       �
�    �     position.  If you have no extra lives when you die, the game is     �
�    �     over. (surprised?)                                                  �
�    쳐 Keys - There are two keys possible for each level.  Both disappear     �
�    �  �     after each level is completed.                                   �
�    �  쳐 Gold Key - Unlocks any doors with the gold "Insert Key" sign.       �
�    �  읕 Silver Key - Unlocks any doors with the silver "Insert Key" sign.   �
�    쳐 Life bar - On the top of the status bar, there is a row of small white �
�    �  �     boxes.  If all of the boxes are white then you are perfectly     �
�    �  �     healthy.  When all boxes turn black, you die.                    �
�    �  쳐 Fries (yellow lines in a teacup) - adds 5 white boxes of life.      �
�    �  쳐 Meals (meat and a glass) - adds 10 white boxes of life.             �
�    �  읕 First-aid kits - adds 20 white boxes of life.                       �
�    쳐 Coins - Coins are camouflaged well with the floors and hard to see.    �
�    �     If you do come across one, press the space bar to pick it up.       �
�    쳐 Soda Machine - Scattered around various boards.  After inserting a     �
�    �     coin by pressing space bar next to one, you may get more life,      �
�    �     or either potion, depending on what you're lacking.                 �
�    읕 Magical items - These items last for only 30 seconds, then they        �
�       �     disappear.  If you have two of the same type of magical item     �
�       �     at the same time, then you will have 60 seconds of use, for      �
�       �     three - 90 seconds of use, etc.                                  �
�       쳐 Purple potions - Hits monsters with more power every time they are  �
�       �     shot, and allows red and orange bullets to pass through walls    �
�       �     that you can't walk through.                                     �
�       쳐 Gray cloak - Enemies and their bullets pass right through           �
�       �     you.  The only way to die with these is to fall down a hole.     �
�       읕 Blue cloak - Similar to the gray cloak except not only are you      �
�             invincible, but you also kill all monsters on contact!           �
�                                                                              �
�                                                                              �
�      THINGS THAT HURT YOU:                                                   �
�    旼컴컴컴컴컴컴컴컴컴컴컴                                                  �
�    쳐 Stationary objects:                                                    �
�    �  쳐 Fans - Quickly eat away at your life if you stand in them.          �
�    �  읕 Holes - Instantly kill you if you fall down them.                   �
�    쳐 Moving objects:                                                        �
�    �  쳐 Bats - Kill them with one shot.  They fly up to you and eat away at �
�    �  �     your skin.  They are easy because they cannot shoot.             �
�    �  쳐 Ken monsters (guy with "K" on shirt) - 1 shot required to kill, the �
�    �  �     slowest and easiest monster to kill.                             �
�    �  쳐 Skeletons - The only difference from Ken monsters is appearance.    �
�    �  쳐 Green monsters - 2 shots to kill, slightly faster than a Ken and    �
�    �  �     shoots more often.                                               �
�    �  쳐 Andy monsters (guy with "A" hidden behind arm on shirt) - 5 shots   �
�    �  �     to kill.  A relatively tough monster.                            �
�    �  쳐 8-balls - Virtually impossible to shoot down.  The only reasonable  �
�    �  �     way to kill them is to direct them into a stationary hole.       �
�    �  쳐 Frozen Ghosts - There are only two ways to kill these beasts.  One  �
�    �  �     is by a hole, and the other- well you can figure it out.         �
�    �  쳐 Walking holes (holes with eyes in them) - Bullets pass right over   �
�    �  �     them.  The only way to kill these critters is by leading them    �
�    �  �     into a permanent hole.  (In other words, the only way to kill a  �
�    �  �     hole is to have the hole fall down a hole)                       �
�    �  쳐 Aliens (guys in tiny hovercraft spaceships) - The fastest and best  �
�    �  �      fighters of them all.  10 shots to kill.                        �
�    �  읕 End boss - The final challenge!  Nobody knows who it is or what it  �
�    �         looks like.  All that we know is that to succeed in your        �
�    �         mission, it must be destroyed.                                  �
�    �                                                                         �
�    읕 Shooting special pictures:                                             �
�       쳐 Shooting Ken's picture - Shame on you!  You lose 1 little white box �
�       �     if you shoot me!                                                 �
�       쳐 Shooting Andy's picture (sorry, no photo available) - Shame on you! �
�       �     An Andy monster will pop out of the wall and attack you.  He     �
�       �     will take twice as many shots to kill as a normal Andy monster.  �
�       읕 Shooting the Advanced Systems picture - Shame on you!  You also     �
�             lose 1 little white box if you shoot the name of my brother's    �
�             computer business.                                               �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�          7.  Add-Ons:                                                        �
�                                                                              �
�      Buy More Levels for Ken's Labyrinth!                                    �
�                                                                              �
�              Board Pack 1 ($15)                                              �
�                                                                              �
�                  The Next 10 Levels Plus One BONUS Level Free!               �
�                  The new levels introduce:                                   �
�                       New heat seeking missiles,                             �
�                       More powerful potions,                                 �
�                       More Decorative Walls,                                 �         
�                       Working soda machines deliver your wish,               �
�                       The Powerful Blue Cloak                                �
�                       and MANY more Hideous Evil Monsters!                   �
�                                                                              �
�              Board Pack 2 ($15) You must already own Pack 1 or be getting    �
�                                 the combo option                             �
�                                                                              �
�                  The Next 10 Levels Plus BOSS Level Free!                    �
�                  Board Pack 2 adds more strategy, more challenging           �                                                 
�                       levels with tougher mazes, more intelligent            �
�                       monsters and even more secrets!  Just wait till        �
�                       the evil boss sees you.                                �
�                                                                              �
�              Or Get Both Board Packs for Just $25 !!!                        �
�                                                                              �
�                                                                              �
�        Price list for boards:                                                �
�                                                                              �
�            Board Pack 1     $15                                              �
�            Board Pack 2     $15 (Only if you already own Pack 1)             �
�            Both Packs 1+2   $25                                              �
�                                                                              �
�                                                                              �
�      Ken's Labyrinth Developers' Toolkit                                     �
�      (Will be available soon)                                                �
�                                                                              �
�              Make Your Own Levels! Create New Challenges!                    �
�              Change the Evil Monsters!                                       �
�                                                                              �
�              You Get: EDITDATA.EXE (for changing the walls and floors)       �
�                       COMPRESS.EXE (to make your changes work)               �
�                       SPRIDRAW.EXE (to view the sprites)                     �
�                       PLAYKSM.EXE (to play LAB3D songs in DOS)               �
�                                                                              �
�             But Wait.  There's More!  You get over 25 original songs just    �
�             waiting to hit the top 40!  Just add lyrics! (or use it as your  �
�             very own Karaoke Machine!)                                       �
�                                                                              �
�                       - All for just $35                                     �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�         8.  Technical support:                                               �
�                                                                              �
�    If you have any problems, questions, or ideas about Ken's Labyrinth,      �
�    you may write us at these addresses or call us at these phone numbers:    �
�                                                                              �
� Ken Silverman                              Andrew Cotter                     �
� Advanced Systems                           Assistant Artist/Board Designer   �
� Programmer/Artist/Composer                 25 Bow Street                     �
� 50 Cindyann Drive                          East Greenwich, RI  02818         �
� East Greenwich, RI  02818                  (401) 884-0484                    �
� (401) 884-6723                                                               �
� E-mail:  ams@lems.brown.edu                                                  �
�                                                                              �
� Please excuse us if there is ever any delay in responses because Ken is      �
� going to college next year and Andy has too much homework.                   �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴커
�                                                                              �
�         8.  People who need mention yet were still pains in the butt:        �
�                                                                              �
� Alan Silverman        Mahesh Jayaraman                                       �
� Advanced Systems      Advanced Systems                                       �
�                                                                              �
읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴켸

