# Mafia Connected (Client)
## Developer Guide

## Project Structure
The project is split into a heirarchy: Galactic > ModLauncher > Client/Server. 

Galactic and ModLauncher belong to Jack's Mini Network and are private sources, but libs are provided as an SDK so you can use them to build. They are used in all Jack's Mini Network projects (GTAC, MafiaC, TRML, etc) so anything that should be applied universally to all those mods will be added to Galactic or ModLauncher by Jack or any other member of Jack's Mini Network that has access.   
The client is further split into "Hacks" which is a rather poorly chosen word to describe the "modules" of the project. The "MultiplayerMod" hacks are the main module for the project. Each game with a different engine will have a different MultiplayerMod module (like MultiplayerModTwo for Mafia 2, MultiplayerModDE for Definitive Edition, etc)

### What you'll need
* Git (like [Github Desktop](https://desktop.github.com/) or just the [basic git](https://git-scm.com/downloads))
* [Visual Studio](https://www.visualstudio.com/downloads/)
* [DirectX SDK](https://www.microsoft.com/en-gb/download/details.aspx?id=6812)
* [CMake](https://cmake.org/download/)
* [SpiderMonkey](https://github.com/jack9267/SpiderMonkey)
* [Dependencies Source](https://github.com/jack9267/Dependencies) or [Latest Dependencies SDK](https://nightly.jacksmininetwork.co.uk/)
* [Latest Galactic SDK](https://nightly.jacksmininetwork.co.uk/)
* [Latest ModLauncher SDK](https://nightly.jacksmininetwork.co.uk/)
* [NASM](https://www.nasm.us/pub/nasm/releasebuilds/?C=M;O=D)

### Setup instructions
* Download and/or install all of the above
* Clone the [Client](https://github.com/mafiaconnected/MafiaC) and [Server](https://github.com/mafiaconnected/MafiaC-Server) repos to your PC
* Use the "Update Submodule.bat" tool for the cmake modules
* Use the "Compile_VS2017_XP.bat" bat file in both repo folders to build them

### Branch Structure
* `main` contains the current public release as-is, and is never committed to or modified directly.
* `nightly` contains all the working changes for the next update, and is never committed to or modified directly
* Any other branch are stuff that's being worked on, usually PR's.

### Notes
* All changes are handled via pull requests.
* The client is split into "Hacks" which is a rather poorly chosen word to describe the "modules" of the project.
* The "MultiplayerMod" is the main module for the project. Each engine will have a different MultiplayerMod module (like MultiplayerModTwo for Mafia 2)
* In the repo's root folder, there are "Configure" bat files to create the VS solution and cmake files.
* The "Compile" bat files make building easier.
* The "Data" folder contains extra non-essential stuff like Languages, GUI stuff, etc. It's provided in the release as a tar archive.
* The "Update Archive.bat" tool packs the stuff in "Data" to the above tar archive.
* Use Debug or RelWithDebInfo builds to test. Release build will not launch (see next item).
* The public release build is signed with Authenticode, and won't work without being built/signed by jack9267 himself.
* You can use any VS tools to build a test version, but public releases use 2017 tools with XP Support and C++
* More will be added to this list as I think of them.
