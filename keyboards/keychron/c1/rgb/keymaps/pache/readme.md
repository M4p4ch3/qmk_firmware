
# Instructions

## Compile

In `qmk_firmware/` :

```sh
qmk compile -kb keychron/c1/rgb -km pache
```

## Flash

In `sonix-flasher/` (`qmk_firmware/../sonix-flasher/`) :

```sh
. venv/bin/activate
fbs run
```

- Make keyboard boot to bootlader (caps lock + F1)
- Refresh keyboard detection
- Ensure :
  - Device : SN32F24x
  - QMK offset : 0x00
- Click `flash QMK`
