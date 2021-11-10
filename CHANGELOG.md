# Changelog

## [0.2.2] - 2021-11-8
### Added
* add support for python 3.6, but cudasim don't support python 3.6.
* add profile tool for all gemm and conv kernels.

## [0.2.1] - 2021-11-8
### Fixed
* Fix some bug of implicit gemm

## [0.2.0] - 2021-11-2
### Addad
* add implicit gemm algorithm for all kind of convolution with kernel volume <= 32. this algorithm is very fast with float16.
* add cuda 11.3 build

### Removed
* remove python 3.6 support