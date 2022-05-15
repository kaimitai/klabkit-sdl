# KKIT/SDL - Unofficial Ken's Labyrinth Editor's Toolkit

Welcome to the KKIT/SDL code repository and release page. The current version of the editor is 0.1c. The code is standard C++20, and the project files were created using Microsoft Visual Studio Community 2022. You can compile the code from source, or get the latest precompiled win-x64 distribution under the [repository releases](https://github.com/kaimitai/klabkit-sdl/releases/). \
Make sure to read the included documentation (readme.html in the docs folder) for a detailed overview.
<br></br>
### Editor Capabilities
The editor in its current state is fairly complete, but I cannot guarantee that there are no bugs. We support editing all versions of Ken's Labyrinth, including the early tech demo "Walken". As of now, the editor has the following functionality:
<br></br>
### Level Editing
The main part of the editor is of course level editing. The "Boards" screen presents all the levels in a file in a graphical way, indicating which properties any map tile has. The board is presented on a zoomable grid, with copy/paste capabilities and several logical operations. The editor is mostly mouse-driven, but supports keyboard shortcuts for most operations.
<br></br>
We can save the boards to compressed and uncompressed formats, and we also support our own XML format - which allows users to more easily compare file versions, use version control systems to track file history, and collaborate on level pack projects.

![alt text](./klabkit-sdl/docs/images/00_board_editor.png)
###### The editor will show all the level and tile information
<br></br>
### Graphics viewer, exporter and importer
In the Gfx screen, the tile metadata can be viewed and edited. The bitmaps themselves can be exported to - and imported from - BMP files. We also support our own XML format for tiles.
<br></br>
![alt text](./klabkit-sdl/docs/images/01_gfx_editor.png)
###### The gfx screen showing some tiles from Ken's Labyrinth v2.1
<br></br>
### Command-line interface
We also include a command-line tool, KKIT/CLI, which can decompress and compress the rest of the game data, like music and sounds. This tool also supports all versions.
<br></br>
"Ken's Labyrinth" Copyright (c) 1992-1993 Ken Silverman \
Ken Silverman's official web site: "http://www.advsys.net/ken"
