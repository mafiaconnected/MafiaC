# Mafia Connected (Client)
## Developer Guide

### What you'll need
* Git (like [Github Desktop](https://git-scm.com/downloads](https://desktop.github.com/) or just the [basic git](https://git-scm.com/downloads))
* [Visual Studio 2017](https://www.visualstudio.com/downloads/) (With XP Support and C++)
* [DirectX SDK](https://www.microsoft.com/en-gb/download/details.aspx?id=6812)
* [CMake](https://cmake.org/download/)
* [SpiderMonkey](https://github.com/jack9267/SpiderMonkey)
* [Dependencies Source](https://github.com/jack9267/Dependencies) or [Dependencies SDK](https://www.dropbox.com/s/g4xynz3hwlq7nud/Dependencies-31-05-2022.exe?dl=1)
* [Galactic SDK](https://www.dropbox.com/s/if6vi8tux8hy0mb/Galactic%20SDK.exe?dl=1)
* [ModLauncher SDK](https://www.dropbox.com/s/prdanh15gqgcnxe/Mod%20Launcher%20SDK.exe?dl=1)
* [NASM](https://www.nasm.us/pub/nasm/releasebuilds/?C=M;O=D)

### Setup instructions
* Download and/or install all of the above
* Clone the [Client](https://github.com/mafiaconnected/MafiaC) and [Server](https://github.com/mafiaconnected/MafiaC-Server) repos to your PC
* Use the "Update Submodule.bat" tool for the cmake modules
* Use the "Compile_VS2017_XP.bat" bat file in both repo folders to build them

### Notes
* The project is split into "Hacks" which is a rather poorly chosen word to describe the "modules" of the project.
* The "MultiplayerMod" is the main module for the project. Each engine will have a different MultiplayerMod module (like MultiplayerModTwo for Mafia 2)
* In the repo's root folder, there are "Configure" bat files to create the VS solution and cmake files.
* The "Compile" bat files make building easier.
* The "Data" folder contains extra non-essential stuff like Languages, GUI stuff, etc. It's provided in the release as a tar archive.
* The "Update Archive.bat" tool packs the stuff in "Data" to the above tar archive.
* More will be added to this list as I think of them.