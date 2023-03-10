										 Controls
								ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
								³ +         - Faster ³
								³ -         - Slower ³
								³ SPACE BAR - Pause  ³
								³ ESC       - Quit   ³
								ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ




ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³            ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿            ³
³            ³    K e n ' s   L a b y r i n t h    N o t e s :    ³            ³
³            ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ            ³
³                                                                              ³
³    Started:   Monday, October 12, 1992                                       ³
³    Revised:   Friday, January 15, 1993                                       ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          1.  History:                                                        ³
³                                                                              ³
³     On June 9, 1992, I had first seen the shareware version of Wolfenstein.  ³
³ Since my brother (Alan) was playing Wolfenstein constantly, I decided to win ³
³ over his time by creating a game similar to it, except my own.  So on June   ³
³ 16, 1992, I started, never knowing that the game would turn into what it is  ³
³ today.  The first thing I did was to figure out how to draw variable size    ³
³ vertical lines from a 64*64 bitmap.  Then, I studied about how I could       ³
³ project this onto the screen.  Next, I decided to store four mathematical    ³
³ pictures in a file, with the fifth one being my picture.  The number of      ³
³ pictures has been increasing steadily over the past four months, currently   ³
³ at 192.  The next big task to solve was how to clip movement so you do not   ³
³ go through walls.  After some time, I decided to start some of the many      ³
³ algorithms that draw planes on the screen quickly and correctly. This was    ³
³ one of the toughest parts of the program, yet after a week of working on the ³
³ problem, I finally solved it.  In October, I was constantly adding the       ³
³ smaller features, such as a life bar, 8 monsters, 2 hazards (fans & holes) 3 ³
³ types of food, 2 keys, 4 magical items (2 potions and capes), the status     ³
³ bar, the floor map automatically taking the place of a reserved bitmap,      ³
³ Adlib music, 360*240 resolution support, Sound Blaster digitized sound       ³
³ support, death animation, introduction screen, custom setup by means of a    ³
³ separate setup program, and animations at the ends of floors.  So far there  ³
³ are 27 floors, each having its own original song.  I thank Andrew Cotter for ³
³ creating 22 out of the 27 current floors, for drawing just about half of all ³
³ of the art, and for sharing his many ideas that are currently in the game.   ³
³                                                                              ³
³    Here are some of the more recent additions:                               ³
³    ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                               ³
³ - XMS extended memory support.                                               ³
³ - Introduction screen in the GIF format.                                     ³
³ - WALLS.KZP now uses LZW compression rather than Run-Length Encoding.        ³
³ - 3d-shading.                                                                ³
³ - New weapon selection keys.                                                 ³
³ - Strafe key.                                                                ³
³ - Doors that you can open and close.                                         ³
³ - New type of secret - Shootable walls, not just walk-through walls.         ³
³ - Much better art job.                                                       ³
³ - Story screen.                                                              ³
³ - Cool readme program. (I hope you understand this one)                      ³
³ - Up to 16 warp zones per level.                                             ³
³ - Final boss                                                                 ³
³ - High score table                                                           ³
³                                                                              ³
³    Additions since the First Shareware Release (as of 1/1/93)                ³
³    ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                ³
³                                                                              ³
³ - Joystick support                                                           ³
³ - Coins & Soda machines                                                      ³
³ - Improved art on several enemies                                            ³
³ - A few more songs to make an original song for each level.                  ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          2.  Story:                                                          ³
³                                                                              ³
³     The fate of the Planet Earth is in your hands!  The scientific community ³
³ on the planet Zogar has been searching the universe for the alien race most  ³
³ able to survive adversity. Some have been extremely intelligent, but were    ³
³ lacking in physical skills.  Others were great warriors but did not match    ³
³ the Zogarians in intellect.  You will be judged by your performance in a     ³
³ unique labyrinth on Zogar, one which will test your physical and mental      ³
³ aptitude (you know, like one of those Idaho tests, but with music in the     ³
³ background).  So far, not one civilization has been deemed worthy by         ³
³ King Xenhai.  Should you fail, well, let's just say that Earth will be blown ³
³ into a million pieces, each the size of a pea.                               ³
³                                                                              ³
³     Your objective is simple:  Find your way out of the labyrinth.  On top   ³
³ of the labyrinth is the spectators' box, where Xenhai has been watching.  If ³
³ you make it out, then Earth, and all life on it,  will be saved.             ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          3.  Requirements:                                                   ³
³                                                                              ³
³    - Any IBM PC compatible computer such as 8086,80286,80386,80486.          ³
³          (80386 or better strongly recommended for best game speed)          ³
³    - At least 965K of memory, with at least 210K in conventional, and an     ³
³          XMS or EMS driver already loaded.                                   ³
³    - VGA display card with VGA monitor.                                      ³
³    - Approximately 926K of free hard disk space.                             ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          4.  Installation:                                                   ³
³                                                                              ³
³    Make a directory on the hard drive, then simply use the DOS "COPY"        ³
³ command to copy all the files on the disk into the directory on the hard     ³
³ drive.  Then type "SETUP" in the directory on the hard drive to set the      ³
³ computer up with the correct hardware and key definitions.  After saving     ³
³ the custom setup, type "LAB3D" to play the game.                             ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          5.  Controls:                                                       ³
³                                                                              ³
³    Before you play, be sure to use the setup program in the same directory   ³
³ by typing "SETUP".   If you do not define keys, here is a list of the        ³
³ default keys that I find easy to use:                                        ³
³                                                                              ³
³    Move FORWARD                     - Up arrow                               ³
³    Move BACKWARD                    - Down arrow                             ³
³    Turn LEFT                        - Left arrow                             ³
³    Turn RIGHT                       - Right arrow                            ³
³    RUN                              - Left shift                             ³
³    STRAFE (walk sideways)           - Right ctrl                             ³
³    SHOOT                            - Left ctrl                              ³
³    Weapon 1 (red FIREBALLS)         - F1                                     ³
³    Weapon 2 (green BOUNCY-BULLETS)  - F2                                     ³
³    Weapon 3 (HEAT-SEEKING MISSILES) - F3                                     ³
³    UNLOCK DOOR                      - Space bar                              ³
³    STAND HIGH                       - "A"                                    ³
³    STAND LOW                        - "Z"                                    ³                      
³    RAISE/LOWER STATUS BAR           - Enter (above right shift key)          ³
³    PAUSE GAME                       - "P"                                    ³
³    LOAD GAME                        - "L"                                    ³
³    SAVE GAME                        - "S"                                    ³
³    QUIT GAME                        - ESCape                                 ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          6.  About the things seen in the game:                              ³
³                                                                              ³
³     THINGS THAT HELP YOU:                                                    ³
³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                  ³
³    ÃÄ Bullets - You can get up to six bullet powers of each kind of bullet.  ³
³    ³  ³     The more bullet powers you have, the more often you can          ³
³    ³  ³     shoot.                                                           ³
³    ³  ÃÄ Normal bullets (red) - Just a simple straight-flying bullet.        ³
³    ³  ÀÄ Bouncy bullets (green) - These bullets bounce off all walls that    ³
³    ³        you cannot normally walk through.                                ³
³    ÃÄ Lightnings - You can get up to six lightnings.  The more lightnings    ³
³    ³     you have, the farther each bullet will travel.                      ³
³    ÃÄ Extra Lives - You can get up to 4 extra lives.  When you die, the      ³
³    ³     floor does NOT reset.  However you do go back to the starting       ³
³    ³     position.  If you have no extra lives when you die, the game is     ³
³    ³     over. (surprised?)                                                  ³
³    ÃÄ Keys - There are two keys possible for each level.  Both disappear     ³
³    ³  ³     after each level is completed.                                   ³
³    ³  ÃÄ Gold Key - Unlocks any doors with the gold "Insert Key" sign.       ³
³    ³  ÀÄ Silver Key - Unlocks any doors with the silver "Insert Key" sign.   ³
³    ÃÄ Life bar - On the top of the status bar, there is a row of small white ³
³    ³  ³     boxes.  If all of the boxes are white then you are perfectly     ³
³    ³  ³     healthy.  When all boxes turn black, you die.                    ³
³    ³  ÃÄ Fries (yellow lines in a teacup) - adds 5 white boxes of life.      ³
³    ³  ÃÄ Meals (meat and a glass) - adds 10 white boxes of life.             ³
³    ³  ÀÄ First-aid kits - adds 20 white boxes of life.                       ³
³    ÃÄ Coins - Coins are camouflaged well with the floors and hard to see.    ³
³    ³     If you do come across one, press the space bar to pick it up.       ³
³    ÃÄ Soda Machine - Scattered around various boards.  After inserting a     ³
³    ³     coin by pressing space bar next to one, you may get more life,      ³
³    ³     or either potion, depending on what you're lacking.                 ³
³    ÀÄ Magical items - These items last for only 30 seconds, then they        ³
³       ³     disappear.  If you have two of the same type of magical item     ³
³       ³     at the same time, then you will have 60 seconds of use, for      ³
³       ³     three - 90 seconds of use, etc.                                  ³
³       ÃÄ Purple potions - Hits monsters with more power every time they are  ³
³       ³     shot, and allows red and orange bullets to pass through walls    ³
³       ³     that you can't walk through.                                     ³
³       ÃÄ Gray cloak - Enemies and their bullets pass right through           ³
³       ³     you.  The only way to die with these is to fall down a hole.     ³
³       ÀÄ Blue cloak - Similar to the gray cloak except not only are you      ³
³             invincible, but you also kill all monsters on contact!           ³
³                                                                              ³
³                                                                              ³
³      THINGS THAT HURT YOU:                                                   ³
³    ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ                                                  ³
³    ÃÄ Stationary objects:                                                    ³
³    ³  ÃÄ Fans - Quickly eat away at your life if you stand in them.          ³
³    ³  ÀÄ Holes - Instantly kill you if you fall down them.                   ³
³    ÃÄ Moving objects:                                                        ³
³    ³  ÃÄ Bats - Kill them with one shot.  They fly up to you and eat away at ³
³    ³  ³     your skin.  They are easy because they cannot shoot.             ³
³    ³  ÃÄ Ken monsters (guy with "K" on shirt) - 1 shot required to kill, the ³
³    ³  ³     slowest and easiest monster to kill.                             ³
³    ³  ÃÄ Skeletons - The only difference from Ken monsters is appearance.    ³
³    ³  ÃÄ Green monsters - 2 shots to kill, slightly faster than a Ken and    ³
³    ³  ³     shoots more often.                                               ³
³    ³  ÃÄ Andy monsters (guy with "A" hidden behind arm on shirt) - 5 shots   ³
³    ³  ³     to kill.  A relatively tough monster.                            ³
³    ³  ÃÄ 8-balls - Virtually impossible to shoot down.  The only reasonable  ³
³    ³  ³     way to kill them is to direct them into a stationary hole.       ³
³    ³  ÃÄ Frozen Ghosts - There are only two ways to kill these beasts.  One  ³
³    ³  ³     is by a hole, and the other- well you can figure it out.         ³
³    ³  ÃÄ Walking holes (holes with eyes in them) - Bullets pass right over   ³
³    ³  ³     them.  The only way to kill these critters is by leading them    ³
³    ³  ³     into a permanent hole.  (In other words, the only way to kill a  ³
³    ³  ³     hole is to have the hole fall down a hole)                       ³
³    ³  ÃÄ Aliens (guys in tiny hovercraft spaceships) - The fastest and best  ³
³    ³  ³      fighters of them all.  10 shots to kill.                        ³
³    ³  ÀÄ End boss - The final challenge!  Nobody knows who it is or what it  ³
³    ³         looks like.  All that we know is that to succeed in your        ³
³    ³         mission, it must be destroyed.                                  ³
³    ³                                                                         ³
³    ÀÄ Shooting special pictures:                                             ³
³       ÃÄ Shooting Ken's picture - Shame on you!  You lose 1 little white box ³
³       ³     if you shoot me!                                                 ³
³       ÃÄ Shooting Andy's picture (sorry, no photo available) - Shame on you! ³
³       ³     An Andy monster will pop out of the wall and attack you.  He     ³
³       ³     will take twice as many shots to kill as a normal Andy monster.  ³
³       ÀÄ Shooting the Advanced Systems picture - Shame on you!  You also     ³
³             lose 1 little white box if you shoot the name of my brother's    ³
³             computer business.                                               ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³          7.  Add-Ons:                                                        ³
³                                                                              ³
³      Buy More Levels for Ken's Labyrinth!                                    ³
³                                                                              ³
³              Board Pack 1 ($15)                                              ³
³                                                                              ³
³                  The Next 10 Levels Plus One BONUS Level Free!               ³
³                  The new levels introduce:                                   ³
³                       New heat seeking missiles,                             ³
³                       More powerful potions,                                 ³
³                       More Decorative Walls,                                 ³         
³                       Working soda machines deliver your wish,               ³
³                       The Powerful Blue Cloak                                ³
³                       and MANY more Hideous Evil Monsters!                   ³
³                                                                              ³
³              Board Pack 2 ($15) You must already own Pack 1 or be getting    ³
³                                 the combo option                             ³
³                                                                              ³
³                  The Next 10 Levels Plus BOSS Level Free!                    ³
³                  Board Pack 2 adds more strategy, more challenging           ³                                                 
³                       levels with tougher mazes, more intelligent            ³
³                       monsters and even more secrets!  Just wait till        ³
³                       the evil boss sees you.                                ³
³                                                                              ³
³              Or Get Both Board Packs for Just $25 !!!                        ³
³                                                                              ³
³                                                                              ³
³        Price list for boards:                                                ³
³                                                                              ³
³            Board Pack 1     $15                                              ³
³            Board Pack 2     $15 (Only if you already own Pack 1)             ³
³            Both Packs 1+2   $25                                              ³
³                                                                              ³
³                                                                              ³
³      Ken's Labyrinth Developers' Toolkit                                     ³
³      (Will be available soon)                                                ³
³                                                                              ³
³              Make Your Own Levels! Create New Challenges!                    ³
³              Change the Evil Monsters!                                       ³
³                                                                              ³
³              You Get: EDITDATA.EXE (for changing the walls and floors)       ³
³                       COMPRESS.EXE (to make your changes work)               ³
³                       SPRIDRAW.EXE (to view the sprites)                     ³
³                       PLAYKSM.EXE (to play LAB3D songs in DOS)               ³
³                                                                              ³
³             But Wait.  There's More!  You get over 25 original songs just    ³
³             waiting to hit the top 40!  Just add lyrics! (or use it as your  ³
³             very own Karaoke Machine!)                                       ³
³                                                                              ³
³                       - All for just $35                                     ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³         8.  Technical support:                                               ³
³                                                                              ³
³    If you have any problems, questions, or ideas about Ken's Labyrinth,      ³
³    you may write us at these addresses or call us at these phone numbers:    ³
³                                                                              ³
³ Ken Silverman                              Andrew Cotter                     ³
³ Advanced Systems                           Assistant Artist/Board Designer   ³
³ Programmer/Artist/Composer                 25 Bow Street                     ³
³ 50 Cindyann Drive                          East Greenwich, RI  02818         ³
³ East Greenwich, RI  02818                  (401) 884-0484                    ³
³ (401) 884-6723                                                               ³
³ E-mail:  ams@lems.brown.edu                                                  ³
³                                                                              ³
³ Please excuse us if there is ever any delay in responses because Ken is      ³
³ going to college next year and Andy has too much homework.                   ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

ÚÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
³                                                                              ³
³         8.  People who need mention yet were still pains in the butt:        ³
³                                                                              ³
³ Alan Silverman        Mahesh Jayaraman                                       ³
³ Advanced Systems      Advanced Systems                                       ³
³                                                                              ³
ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ

