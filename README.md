# zhm5-randomiser-rewritten
ZHM5-Randomizer-Rewritten is an item randomizer for Hitman 2 (2018) that allows randomization of world, hero, npc and stash inventories through different randomisation strategies.


This is a quick pre-release release since the game got updated the other day and i will release the full source code the next days!


If no "ZHM5Randomozer.ini" is present, the mod will use default settings, see strategy.c:24.


To play this mod, place the DINPUT8.dll in `HITMAN2/Retail`.
If you are using Proton, add the following launch parameters `WINEDLLOVERRIDES="dinput8=n,b" %command%`.


Version 0.9:
	Fully working, needs a little refactoring.