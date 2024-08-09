# Intel(R) Metrics Library for MDAPI

## Introduction

This software is a user mode driver helper library that provides access to GPU performance counters.

## License

Intel(R) Metrics Library for MDAPI is distributed under the MIT License.

You may obtain a copy of the License at:
https://opensource.org/licenses/MIT

## Supported Platforms

- Intel(R) Processors with Gen12 graphics devices
- Intel(R) Processors with Gen11 graphics devices
- Intel(R) Processors with Gen9 graphics devices

## Supported Operating Systems

Intel(R) Metrics Library for MDAPI is supported on Linux family operating systems with minimum kernel version 4.14.

## Build and Install
Not a stand alone software component. Serves as a helper library for particular Intel drivers.
There is no need to build the library as long as it is an integrated part of those Intel drivers.

1\. Download sources.

2\. Install libdrm-dev:

```shell
sudo apt install libdrm-dev
```

3\. Run CMake generation:

```shell
cmake .
```

4\. Build:

```shell
make -j$(nproc)
```

5\. Built library will be here (for 64-bit release Linux):

```shell
(project_root)/dump/linux64/release/metrics_library/libigdml.so
```

6\. To prepare an installation package:

```shell
make package
```

7\. Install:

```shell
sudo dpkg -i intel-metrics-library*.deb
```

*Note: To clear CMake parameters remove CMakeCache.txt, then regenerate.*

## Support

Please file a GitHub issue to report an issue or ask questions.

## How to Contribute

Contributions to the Metrics Library for MDAPI are welcomed and encouraged.
Please see [CONTRIBUTING](CONTRIBUTING.md) for details how to
contribute to the project.

##
___(*) Other names and brands my be claimed as property of others.___
