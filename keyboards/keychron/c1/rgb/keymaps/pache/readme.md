
# Instructions

## Compile

In `qmk_firmware/` :

```sh
qmk compile -kb keychron/c1/rgb -km pache
```

## Flash

In `sonix-flasher/` (`qmk_firmware/../sonix-flasher/`) :

```sh
venv . activate
fbs run
```
