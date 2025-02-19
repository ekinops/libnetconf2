# libnetconf2 – The NETCONF protocol library

[![BSD license](https://img.shields.io/badge/License-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![Build](https://github.com/CESNET/libnetconf2/workflows/libnetconf2%20CI/badge.svg)](https://github.com/CESNET/libnetconf2/actions?query=workflow%3A%22libnetconf2+CI%22)
[![Docs](https://img.shields.io/badge/docs-link-blue)](https://netopeer.liberouter.org/doc/libnetconf2/)
[![Coverity](https://scan.coverity.com/projects/7642/badge.svg)](https://scan.coverity.com/projects/7642)
[![Codecov](https://codecov.io/gh/CESNET/libnetconf2/branch/master/graph/badge.svg?token=HpKeW36N9D)](https://codecov.io/gh/CESNET/libnetconf2)

**libnetconf2** is a NETCONF library in C intended for building NETCONF clients
and servers. NETCONF is the [NETwork CONFiguration protocol](http://trac.tools.ietf.org/wg/netconf/trac/wiki)
introduced by IETF.

**libnetconf2** is a NETCONF library in C handling NETCONF authentication and all NETCONF RPC communication both server
and client-side. Note that NETCONF datastore implementation is not a part of this library. The library supports both
NETCONF 1.0 ([RFC 4741](https://tools.ietf.org/html/rfc4741)) as well as NETCONF 1.1
([RFC 6241](https://tools.ietf.org/html/rfc6241)). The main features include:

* NETCONF over SSH ([RFC 4742](https://tools.ietf.org/html/rfc4742), [RFC 6242](https://tools.ietf.org/html/rfc6242)),
  using [libssh](https://www.libssh.org/).
* NETCONF over TLS ([RFC 7589](https://tools.ietf.org/html/rfc7589)), using [OpenSSL](https://www.openssl.org/).
  * DNSSEC SSH Key Fingerprints ([RFC 4255](https://tools.ietf.org/html/rfc4255))
* NETCONF over pre-established transport sessions (using this mechanism the communication can be tunneled through
  sshd(8), for instance).
* NETCONF Call Home ([RFC 8071](https://tools.ietf.org/html/rfc8071)).
* NETCONF Event Notifications ([RFC 5277](https://tools.ietf.org/html/rfc5277)),

**libnetconf2** is maintained and further developed by the [Tools for
Monitoring and Configuration](https://www.liberouter.org/) department of
[CESNET](http://www.ces.net/). Any testing or improving/fixing the library
is welcome. Please inform us about your experiences with using **libnetconf2**
via the [issue tracker](https://github.com/CESNET/libnetconf/issues).

Besides the [**libyang**](https://github.com/CESNET/libyang), **libnetconf2** is
another basic building block for the [**Netopeer2** toolset](https://github.com/CESNET/Netopeer2).
For a reference implementation of NETCONF client and server, check the
**Netopeer2** project.

## Branches

The project uses 2 main branches `master` and `devel`. Other branches should not be cloned. In `master` there are files of the
last official *release*. Any latest improvements and changes, which were tested at least briefly are found in `devel`. On every
new *release*, `devel` is merged into `master`.

This means that when only stable official releases are to be used, either `master` can be used or specific *releases* downloaded.
If all the latest bugfixes should be applied, `devel` branch is the  one to be used. Note that whenever **a new issue is created**
and it occurs on the `master` branch, the **first response will likely be** to use `devel` before any further provided support.

## libnetconf vs libnetconf2

**libnetconf2** was developed with experiences gained from the development
of the [**libnetconf**](https://github.com/CESNET/libnetconf) library, which
is now obsolete and should not be used.

## Packages

Binary RPM or DEB packages of the latest release can be built locally using `apkg`, look into `README` in
the `distro` directory.

## Requirements

* C compiler (gcc >= 4.8.4, clang >= 3.0, ...)
* cmake >= 2.8.12
* [libyang](https://github.com/CESNET/libyang)
* libssh >= 0.7.1 (for SSH support)
  * recommended >= 0.9.0
* OpenSSL (for TLS support)

#### Optional

* libpam (for PAM-based SSH `keyboard-interactive` authentication method)
* libval (only for DNSSEC SSHFP retrieval)
  * [DNSSEC-Tools/dnssec-tools/validator](https://github.com/DNSSEC-Tools/DNSSEC-Tools/tree/master/dnssec-tools/validator)
    part of the DNSSEC-Tools suite
* doxygen (for generating documentation)
* cmocka >= 1.0.1 (for tests only, see [Tests](#Tests))
* valgrind (for enhanced testing)
* gcov (for code coverage)
* lcov (for code coverage)
* genhtml (for code coverage)

## Building

```
$ mkdir build; cd build
$ cmake ..
$ make
# make install
```

The library documentation can be generated directly from the source codes using
Doxygen tool:
```
$ make doc
```

## Build Options

There are various options to change result of building.

### Changing Compiler

Set `CC` environment variable:

```
$ CC=/usr/bin/clang cmake ..
```

### Installation Prefix

By default, the library is installed with the `/usr/local` prefix, to change
it, use the following option:
```
$ cmake -DCMAKE_INSTALL_PREFIX:PATH=/usr ..
```

### Transport Protocol Support

The NETCONF protocol specification allows to use the protocol on top of
several transport protocols. **libnetconf2** provides support for SSH and
TLS transport. By default, both SSH and TLS transport is enabled. Disabling
and enabling both the transport protocols can be made
in the same way. The following command has actually the same effect as
specifying no option since it specifies the default settings.
```
$ cmake -DENABLE_TLS=ON -DENABLE_SSH=ON ..
```

### DNSSEC SSHFP Retrieval

In SSH connections, if the remote NETCONF server supports it and it is
enabled, it is possible to safely retrieve server host key fingerprints
using DNSSEC and automatically consider them to be trusted without any
interaction. Enable it with the following command.
```
$ cmake -DENABLE_DNSSEC=ON ..
```

### Build Modes

There are two build modes:
* Release.
  This generates library for the production use without any debug information.
* Debug.
  This generates library with the debug information and disables optimization
  of the code.

The `Debug` mode is currently used as the default one. to switch to the
`Release` mode, enter at the command line:
```
$ cmake -D CMAKE_BUILD_TYPE:String="Release" ..
```

### Inactive Read Timeout

It is possible to adjust inactive read timeout. It is used when a new message is
being read and no new data had arrived for this amount of seconds. 20 is the default value.

```
$ cmake -D READ_INACTIVE_TIMEOUT:String="20" ..
```

### Active Read Timeout

Active read timeout is used to limit the maximum number of seconds a message is given
to arrive in its entirety once a beginning is read. The default is 300 (5 minutes).

```
$ cmake -D READ_ACTIVE_TIMEOUT:String="300" ..
```

### PSPoll Thread Count

This value limits the maximum number of threads that can concurrently access
(wait for access) a single pspoll structure. To simplify, how many threads could
simultaneously call a function whose parameter is one and the same pspoll structure.
If using **netopeer2-server**, it will warn that this value needs to be adjusted if
too small.

```
$ cmake -D MAX_PSPOLL_THREAD_COUNT:String="6" ..
```

### Code Coverage

Based on the tests run, it is possible to generate code coverage report. But
it must be enabled and these commands are needed to generate the report:
```
$ cmake -DENABLE_COVERAGE=ON ..
$ make
$ make coverage
```

Note that `gcc` compiler is required for this option.

### CMake Notes

Note that, with CMake, if you want to change the compiler or its options after
you already ran CMake, you need to clear its cache first - the most simple way
to do it is to remove all content from the 'build' directory.

## Usage

All public functions are available via 2 headers:
```
#include <nc_server.h>
#include <nc_client.h>
```

You need to include either one if implementing a NETCONF server or a NETCONF client,
respectively.

To compile your program with libnetconf2, it is necessary to link it with it using the
following linker parameters:
```
-lnetconf2
```

## Examples

See [examples](examples) directory for an example client and server.

## Tests

The repository includes several tests built with [cmocka](https://cmocka.org/).
The tests can be found in `tests` subdirectory and they are designed for
checking library functionality after code changes.

The tests are by default built in the `Debug` build mode by running
```
$ make
```

In case of the `Release` mode, the tests are not built by default (it requires
additional dependency), but it can be enabled via cmake option:
```
$ cmake -DENABLE_TESTS=ON ..
```

Note that if the necessary [cmocka](https://cmocka.org/) headers are not present
in the system include paths, tests are not available despite the build mode or
cmake's options.

Tests can be run by the make's `test` target:
```
$ make test
```

