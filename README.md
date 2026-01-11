# libsawyer

Utilities for [OpenRCT2](https://github.com/OpenRCT2/OpenRCT2) and [OpenLoco](https://github.com/OpenLoco/OpenLoco).

## Building libsawyer

To get started, first build a `libsawyer` binary:
```
cmake -G Ninja -B bin
cmake --build bin
cmake --install bin
```

### Optional modules

Depending on your needs, you may like to disable some modules. Five modules are optional
(`ENABLE_BREAKPAD`, `ENABLE_DISCORD_RPC`, `ENABLE_LIBPNG`, `ENABLE_SCRIPTING`, `ENABLE_TESTS`).
They can be pass to `cmake` via `-D<param>=<bool>`, e.g. `ENABLE_DISCORD_RPC=OFF`.

### Build and run tests
```
cmake --build bin --target tests
./bin/tests
```

## Building fsaw

The `fsaw` utility can be used to inspect, compress and decompress files that use Sawyer encoding,
e.g. Locomotion/OpenLoco save games. It can be compiled as follows:

```
cd tools/fsaw
cmake -G Ninja -B bin
cmake --build bin
./bin/fsw
```

## Building gxc

The `gxc` utility can be used to build and extract graphics files used by OpenRCT2 and OpenLoco.
It can be compiled as follows:

```
cd tools/gxc
cmake -G Ninja -B bin
cmake --build bin
./bin/gxc
```

## Licence
**libsawyer** is licensed under the MIT License.
