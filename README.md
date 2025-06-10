# Intel(R) Metrics Library for MDAPI

## Introduction

This software is a user mode driver helper library that provides access to GPU performance counters.

## License

Intel(R) Metrics Library for MDAPI is distributed under the MIT License.

You may obtain a copy of the License at:
https://opensource.org/licenses/MIT

## Supported Platforms

- Intel(R) Processors with Xe3 graphics devices (formerly Panther Lake)
- Intel(R) Processors with Xe2 graphics devices (formerly Lunar Lake)
- Intel(R) Processors with Arc graphics devices (formerly Alchemist, Battlemage)
- Intel(R) Processors with Xe graphics devices (formerly Tiger Lake, Rocket Lake, DG1, Alder Lake, Ponte Vecchio, Meteor Lake, Arrow Lake)

Since version 1.0.198, support for Gen9 and Gen11 platforms has been discontinued. For these platforms, use version 1.0.196 or earlier.

## Supported Operating Systems

Intel(R) Metrics Library for MDAPI is supported on Linux family operating systems with minimum kernel version 5.7.

### Detailed minimum version requirements

| Platform | Code Name | Minimum Linux Kernel Version |
|--|--|--|
| Xe3-LPG | Panther Lake | 6.15 |
| Xe2-HPG | Battlemage | 6.11 |
| Xe2-LPG | Lunar Lake | 6.11 |
| Xe-LPG | Arrow Lake-H | 6.9 |
| Xe-LPG | Arrow Lake-S | 6.7 |
| Xe-LPG | Arrow Lake-U | 6.9 |
| Xe-LPG | Meteor Lake | 6.7 |
| Xe-HPG | Alchemist | 6.2 |
| Xe-HPC | Ponte Vecchio | 5.20 |
| Xe-LP | Alder Lake-S | 5.16 |
| Xe-LP | Alder Lake-N | 5.18 |
| Xe-LP | Alder Lake-P | 5.17 |
| Xe-LP | DG1 | 5.16 |
| Xe-LP | Rocket Lake | 5.13 |
| Xe-LP | Tiger Lake | 5.7 |

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
