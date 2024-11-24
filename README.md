# Picocalc simple mp3 player

## toolchain

```
arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi
```

YAHAL

https://github.com/cuu/YAHAL.git branch picocalc

## How to compile

set **YAHAL_DIR** to the path of your local YAHAL location

set **arm-gnu-toolchain-13.3.rel1-x86_64-arm-none-eabi/bin** in your $PATH 

then
 
```
mkdir build
cd build
cmake ..
make
```

