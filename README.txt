io42io3
generic segatools I/O DLL for using a physical IO4 board for games that expect IO3 input
2024 Haruka
Licensed under the GPLv3.

Specialized for SDEJ, but can be used for any game.

--- Usage ---

Place io42io3.dll, io42io3.ini and hidapi.dll in your game directory and edit segatools.ini to add a section:

[XXXXio]
path=io42io3.dll

where XXXX is dependent on your game/segatools version (kantaiio, fateio, ...)

See io42io3.example_kca.ini for an example config (FGO io4 to KCA io3)

--- Compiling ---

have msys2 installed at the default location and run compile.bat