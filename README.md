# zhm5-randomiser-rewritten
ZHM5-Randomizer-Rewritten is an item randomizer for Hitman 2 (2018), based on [pawREP's work](https://github.com/pawREP/ZHM5Randomizer), big thanks to him, that allows randomization of world, hero, npc and stash inventories through different randomisation strategies.

The Reason i did a full rewrite was because i had serveral design disagreements to his and it made it hard for me to expand the mod in the ways i wanted.

I am planning on expanding it more in the future and if you have suggestions of any kind, feel free to create an issue.

Grab the latest [Release](https://github.com/BieHDC/zhm5-randomiser-rewritten/releases) now!

---
## Hints
If no "ZHM5Randomozer.ini" is present, the mod will use default settings.

To set a different strategy for a specific item pusher, edit the corrosponding line in the `ZHM5Randomizer.ini` file.

The list of available Strategies and their effects are listed in [`strategy.c:43`](/src/strategy.c#L43).

---
## Install
To play this mod, place the `DINPUT8.dll` in `HITMAN2/Retail`. Optionally place `ZHM5Randomizer.ini` next to it.

If you are using Proton, add the following launch parameters `WINEDLLOVERRIDES="dinput8=n,b" %command%`.

---
## In case you speak python
There is this file called [`convertrepositorytocfile.py`](/src/resources/convertrepositorytocfile.py)
If you know how to do this better, please create a PR with the fix, and check first if the output is correct.

---
* Version 1.0:
  * Full release
	
* Version 0.9:
  * Fully working, needs a little refactoring.