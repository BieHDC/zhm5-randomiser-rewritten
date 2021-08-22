# zhm5-randomiser-rewritten
ZHM5-Randomizer-Rewritten is an item randomizer for Hitman 2 (2018), based on [pawREP's work](https://github.com/pawREP/ZHM5Randomizer), big thanks to him, that allows randomization of world, hero, npc and stash inventories through different randomisation strategies.

The Reason i did a full rewrite was because i had serveral design disagreements to his and it made it hard for me to expand the mod in the ways i wanted.

I am planning on expanding it more in the future and if you have suggestions of any kind, feel free to create an issue.

Grab the latest [Release](https://github.com/BieHDC/zhm5-randomiser-rewritten/releases) now!

---
## Hints
If no "ZHM5Randomozer.ini" is present, the mod will use default settings.

To set a different strategy for a specific item pusher, edit the corrosponding line in the `ZHM5Randomizer.ini` file.

The list of available Strategies and their effects are listed in [`strategy2.c:35`](/src/strategy2.c#L35).

---
## Install
To play this mod, rename the target DLL to `DINPUT8.dll` and place it in `HITMAN{tm,2}/{Retail,DX12Retail}`. Optionally place `ZHM5Randomizer.ini` next to it.

If you are using Proton, add the following launch parameters `WINEDLLOVERRIDES="dinput8=n,b" %command%`.

---
## In case you speak python
There is this file called `convertrepositorytocfile.py` in `src/resources/REPO_hitman{2,2016}/`
If you know how to do this better, please create a PR with the fix, and check first if the output is correct.

---
* Version 2.0:
  * Hitman 2016 Support added.
  * Cleanup of the source structure.

* Version 1.0:
  * Full release, Hitman 2 fully working.
	
* Version 0.9:
  * Fully working, needs a little refactoring.