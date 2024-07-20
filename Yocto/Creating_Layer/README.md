# Creating new Layer and recipe

The OpenEmbedded build system supports organizing Metadata into multiple layers. Layers allow you to isolate different types of customizations from each other.

## Create New Layer

The bitbake-layers script with the create-layer subcommand simplifies creating a new custom layer.

```bash
bitbake-layers create-layer meta-AliLayer
```
You will notice that the command created a layer with the following:
```bash
meta-AliLayer/
├── conf
│   └── layer.conf
├── COPYING.MIT
├── README
└── recipes-example
    └── example
        └── example_0.1.bb
```
- `Priority` is set to **6** by default.

## Creating New Recipe
File structure for simple recipe
```bash
recipes-Ali/
└── helloworld
    ├── files
    │   └── helloworld.c
    └── helloworld_0.1.bb
```

### Recipe File
```bash
DESCRIPTION = "SIMPLE HELLOWORLD RECIPE"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://$(COREBASE)/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://helloworld.c"

S = "${WORKDIR}"

do_compile() {
    ${CC} ${LDFLAGS} helloworld.c -o helloworld
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 helloworld ${D}${bindir}
}
```


## Adding Layer

Enable the layer by adding it to `bblayers.conf`, so the OpenEmbedded build system can use this new layer.

```bash
bitbake-layers add-layer meta-AliLayer
```
- The layer has been added to the **bblayers.conf**
```bash
BBLAYERS ?= " \
  /home/aly/YOCTO/poky/meta \
  /home/aly/YOCTO/poky/meta-poky \
  /home/aly/YOCTO/poky/meta-yocto-bsp \
  /home/aly/YOCTO/poky/yoctoTest/workspace \
  /home/aly/YOCTO/poky/meta-AliLayer \
  "
  ```