#!/usr/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m'
DIR_APP="/usr/bin"
deps=(qemu-system-x86 qemu-ui-gtk grub-mkrescue)
missing=()
SCRIPT_DIR="$(cd -- "$(dirname "$0")" && pwd)"

for dep in "${deps[@]}"; do
    if ! whereis -b "$dep" | grep -q '/'; then
        missing+=("$dep")
    fi
done
install() {
if [ ${#missing[@]} -ne 0 ]; then
    echo -e "${RED}Dependencies missing:${NC} ${missing[*]}"
    exit 1
fi
    echo -e "${YELLOW}installing grub-qemu...${NC}"
    if [ ! -f "$SCRIPT_DIR/grub-qemu" ]; then
        echo -e "${RED} grub-qemu not found in current directory!${NC}"
        exit 1
    fi

	  if [ -f "$DIR_APP/grub-qemu" ]; then
	      sudo rm "$DIR_APP/grub-qemu"
  	fi
	  chmod a+x "$SCRIPT_DIR/grub-qemu"
	  sudo cp "$SCRIPT_DIR/grub-qemu" "$DIR_APP"
  	echo -e "${GREEN} grub-qemu Installed!${NC}"
}

uninstall() {
    sudo rm -f "$DIR_APP/grub-qemu" &> /dev/null
    echo -e "${GREEN} grub-qemu Deleted!${NC}"
}

if [ "$1" == "install" ]; then
    install
elif [ "$1" == "uninstall" ]; then
    uninstall
else
    echo -e "${RED} Options $0 {install|uninstall}${NC}"
    exit 1
fi
