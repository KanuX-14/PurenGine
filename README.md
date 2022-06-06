# PurenGine
PG is a UI engine, written in pure C. PurenGine use the SDL2 engine to render the application.

## Dependencies

- Debian
```bash
sudo apt-get install git make libsdl2-dev libsdl2-ttf-dev
```

- Arch Linux
```bash
sudo pacman -S --needed git make sdl2 sdl2_ttf
```

- Fedora
```bash
sudo dnf install git make SDL2 SDL2-devel SDL2_ttf SDL2_ttf-devel
```

- Other distributions
```bash
git clone https://github.com/libsdl-org/SDL.git \
cd SDL \
mkdir build \
cd build \
../configure \
make -j$(nproc) \
sudo make install
```

## Build

- Install without root:
```bash
git clone --recursive https://github.com/KanuX-14/PurenGine.git \
cd PurenGine \
make && make install
```
- Install with root (leave PG in /usr/local/bin):
```bash
git clone --recursive https://github.com/KanuX-14/PurenGine.git \
cd PurenGine \
make && sudo make install
```

## References

[SDL](https://github.com/libsdl-org/SDL)
[SDL_ttf](https://github.com/libsdl-org/SDL_ttf)
[SDL Wiki](https://wiki.libsdl.org)

SDL2 is available in most of devices. For the moment, only Linux devices are supported.
