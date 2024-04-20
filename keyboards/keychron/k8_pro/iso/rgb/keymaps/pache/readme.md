Keychron K8 pro ISO RGB pache
===

# Build

```sh
qmk compile -kb keychron/k8_pro/iso/rgb -km pache
make keychron/k8_pro/iso/rgb:pache
```

# Flash

Official `make keychron/k8_pro/iso/rgb:pache:flash` doesn't work
`mass-erase:force` DFU `-s` option seems to be missing
Use following instead :

```sh
dfu-util -a 0 -d 0483:df11 -s 0x8000000:mass-erase:force:leave -D <fw_bin_file>
```
