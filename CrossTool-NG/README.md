# ToolChain CrossTool-NG
## Installing CrossTool-NG
### :exclamation: Note 
you will need to install these packages 
```bash
sudo apt-get install automake bison git libexpat1-dev libncurses5-dev libsdl1.2-dev libtool \
python3-dev 
```
### Clone the CrossTool-NG Repository
```bash
git clone https://github.com/crosstool-ng/crosstool-ng.git
```
### checkout for a Stable Commit
```bash
git checkout 25f6dae8
```

### Installing the Environment

```bash
./bootstrap    # setup the environment
./configure --enable-local  # The Program will be installed in the current directory
make      # to generate the Makefile for CroosTool-NG
```

## Building the Toolchain
### list all micro-controllers supported
```bash
./ct-ng list-samples
./ct-ng <micro-controler>
```
### Make a custom configuration
```bash
./ct-ng menuconfig
```
### build the target toolchain

```bash
./ct-ng build
```
**It will be installed in ~/x-tools/<micro-controller_name>**





