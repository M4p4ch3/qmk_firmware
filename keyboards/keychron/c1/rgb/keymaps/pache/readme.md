
# Keychron C1 RGB pache instructions

## Compile

In `qmk_firmware/` :

```sh
qmk compile -kb keychron/c1/rgb -km pache
```

## Flashing

Relying on [Sonix Flasher](https://github.com/SonixQMK/sonix-flasher/releases/tag/v0.2.1)

### Setup

```sh
git clone https://github.com/SonixQMK/sonix-flasher
cd ./sonix-flasher
python -m venv venv_sonix_flasher
source ./venv_sonix_flasher/bin/activate
pip install wheel
pip install -r requirements.txt
```

Install PyQt5 dependencies :

```sh
sudo apt-get install '^libxcb.*-dev' libx11-xcb-dev libglu1-mesa-dev libxrender-dev libxi-dev libxkbcommon-dev libxkbcommon-x11-dev
```

Add udev rule to avoid having to use sudo :

```sh
echo "# Keychron C1 RGB" | sudo tee -a /etc/udev/rules.d/50-qmk.rules
echo 'SUBSYSTEMS=="usb", ATTRS{idVendor}=="0c45", ATTRS{idProduct}=="7040", TAG+="uaccess"' | sudo tee -a /etc/udev/rules.d/50-qmk.rules
sudo udevadm control --reload-rules && sudo udevadm trigger
```

### Flash

```sh
source ./venv_sonix_flasher/bin/activate
fbs run
```

- Make keyboard boot to bootlader (caps lock + F1)
- Refresh keyboard detection
- Ensure :
  - Device : SN32F24x
  - QMK offset : 0x00
- Click `flash QMK`
