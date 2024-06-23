
# Lily 58

Insctructions for Lily58 Pache keymap

# Compile

In `qmk_firmware/` :

```sh
qmk compile -e CONVERT_TO=promicro_rp2040 -kb lily58/light -km pache
```

# Flash

- Plug the keyboard in, while the button is being held down
- Browe detected `` USB device
- Paste `lily58_light_pache_promicro_rp2040.uf2` file

