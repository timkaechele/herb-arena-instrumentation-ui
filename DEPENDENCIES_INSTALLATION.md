# SDL3 Installation Guide for Ubuntu

Simple guide to install SDL3 (Simple DirectMedia Layer 3) on Ubuntu systems.

---

## Method 1: Package Manager (Ubuntu 25.04+)

For Ubuntu 25.04 and later, SDL3 is available in official repositories:

```bash
sudo apt update
sudo apt install libsdl3-dev
```

---

## Method 2: Build from Source (Ubuntu 24.04 and earlier)

For older Ubuntu versions or to get the latest SDL3 version:

### 1. Install build dependencies

```bash
sudo apt install build-essential cmake git pkg-config
```

### 2. Download and build SDL3

```bash
# Create build directory
mkdir ~/SDL3_build && cd ~/SDL3_build

# Download SDL3 (check https://github.com/libsdl-org/SDL/releases for latest version)
wget https://github.com/libsdl-org/SDL/archive/refs/tags/release-3.2.4.tar.gz
tar -xzf release-3.2.4.tar.gz
cd SDL-release-3.2.4

# Build and install
mkdir build && cd build
cmake ..
cmake --build . -j$(nproc)
sudo cmake --install . --prefix /usr/local

# Update library cache
sudo ldconfig
```

---

## Verification

Check that SDL3 is installed correctly:

```bash
pkg-config --cflags --libs SDL3
```

Expected output:
```
-I/usr/local/include/SDL3 -L/usr/local/lib -lSDL3
```

---

## Troubleshooting

**`pkg-config: command not found`**
```bash
sudo apt install pkg-config
```

**`SDL3/SDL.h: No such file or directory`**
```bash
# Verify installation
ls /usr/local/include/SDL3/
# If missing, rebuild and update cache
sudo ldconfig
```

**`error while loading shared libraries: libSDL3.so`**
```bash
sudo ldconfig
# If still not working:
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
```

**CMake can't find SDL3**
```bash
pkg-config --list-all | grep SDL3
# If nothing appears, rebuild from source
```

---

## Resources

- [SDL3 Documentation](https://wiki.libsdl.org/SDL3/)
- [SDL3 GitHub](https://github.com/libsdl-org/SDL)
- [SDL3 Examples](https://github.com/libsdl-org/SDL/tree/main/examples)
- [SDL Forums](https://discourse.libsdl.org/)
