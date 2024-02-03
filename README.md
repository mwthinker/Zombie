zombie::Zombie [![CI build](https://github.com/mwthinker/Zombie/actions/workflows/ci.yml/badge.svg)](https://github.com/mwthinker/Zombie/actions/workflows/ci.yml) [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
======
![image](https://user-images.githubusercontent.com/3457668/116273499-14b62300-a782-11eb-8179-c66399824f6c.png)

A 2D-shooter zombie game.

## Prerequisites
Git and CMake must be installed.

Clone vcpkg and set enviroment variable VCPKG_ROOT to the root folder of the cloned vcpkg folder.

```bash
git clone https://github.com/microsoft/vcpkg.git
```

Or follow the instruction in https://github.com/microsoft/vcpkg#using-vcpkg-with-cmake.

## Build
```bash
git clone https://github.com/microsoft/Zombie.git
cd Zombie
mkdir build
cd build
cmake ..
make
```

Or use CMake gui and generate the build system of your choice.

### Release/Debug build
Either do it via the IDE or run 

```bash
cd build
# Debug
cmake -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .
# Or Release
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

If you have acess to the private repo run:
```bash
cd build
# Debug
cmake -DZombieData_PrivateRepo_SSH=1 -DCMAKE_BUILD_TYPE=Debug .. && cmake --build .
# Or Release
cmake -DZombieData_PrivateRepo_SSH=1 -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

## Open source
The project is under the MIT license.
