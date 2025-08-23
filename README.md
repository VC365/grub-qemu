About
-----
grub-qemu is a lightweight program for a full-fledged preview of grub. When I was using grub-emu on Fedora, it wasn't very good.
 I was looking for something lightweight while still having my system's grub theme loaded automatically, when the idea for this project came to my mind.

![preview](https://raw.githubusercontent.com/VC365/grub-qemu/master/dood/نماگرفت.png)
* QEMU has a problem with the 1366x768 resolution, which has been fixed in grub-qemu and the resolution is now 1360x768.

Installation
------------

### Requirements

* `qemu-system-x86`
* `qemu-desktop`
* `grub`
  
### Using the installer script

```bash
chmod a+x installer.sh
./installer.sh install
```
### Using AUR

```bash
paru -S grub-qemu
# or
yay -S grub-qemu
```
#### Manual Installation
```bash
git clone https://aur.archlinux.org/grub-qemu.git
cd grub-qemu
makepkg -si
```
## Usage

```console
grub-qemu --help
Usage: grub-qemu [--qemu "Argument"] [--custom-grub "Directory"] [--enable-kvm]
 [--disable-full-screen] [--disable-theme] [-v] [-h]
Options:
  -q, --qemu "Argument"           Use custom argument for QEMU.
  -c, --custom-grub "Directory"   Use custom grub.
  --enable-kvm                      Enable KVM for QEMU.
  --disable-full-screen             Disable full screen mode.
  --disable-theme                   Disable theme grub.

  -h, --help                        Show this help message.
  -v, --version                     Show version.
                 Credit : VC365 (https://github.com/VC365) 
```
## Author

[VC365](https://github.com/VC365)
