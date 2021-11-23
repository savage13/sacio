libsacio Documentation
======================

[![Build Status](https://travis-ci.com/savage13/sacio.svg?branch=master)](https://travis-ci.com/savage13/sacio)

Overview
--------

The sacio library, libsacio, provides an interface to read and write sac files and manipulate their contents.

SAC Files
---------
SAC (Seismic Analysis Code) files are binary or alphanumeric data files for storing time series data, primarily ground motion recorded by seismometers. Format of the SAC file consists of a metadata header followed by the ground motion stored as equally or unequally spaced in time. The data section may also be the Fourier Transform of an input ground motion or a 2D data set.


Documentation
-------------

[libsacio Documention](https://savage13.github.io/sacio/)

IRIS-SAC Relationship
---------------------

The code and library here was originally written and licensed under the [2-Clause BSD License](https://choosealicense.com/licenses/bsd-2-clause/) and subsequently included for the [IRIS-SAC distribution](http://ds.iris.edu/ds/nodes/dmc/forms/sac/). This distribution also includes other, essentially, public domain knowledge on the sac format.  The code found here is the same included within the IRIS-SAC distribution, but does **not** included the licensed code from LLNL/IRIS; this is primarily the processing, filtering, and data operations. If you require the SAC program and this functionality, make that request to [IRIS](http://ds.iris.edu/ds/nodes/dmc/forms/sac/)


Examples
--------

Examples of using the library can be found within the [documenation](https://savage13.github.io/sacio/). Of particular note are:

   - [sac_read()](https://savage13.github.io/sacio/html/group__sac.html#gab2623928ccd5cac1a4cc94be5dd89273)
   - [sac_write()](https://savage13.github.io/sacio/html/group__sac.html#gafe8cd1cadc546ea9d67a5ca14fd3886d)
   - [sac_get_string()](https://savage13.github.io/sacio/html/group__sac.html#ga58de657b18177e79b5fa001b21c55e32) / [sac_set_string()](https://savage13.github.io/sacio/html/group__sac.html#ga9acd9d129945c8fdf5a21434ce1d3359)
   - [sac_get_int()](https://savage13.github.io/sacio/html/group__sac.html#ga65ddb9d01a8d1ea66bc7ad1024b30534) / [sac_set_int()](https://savage13.github.io/sacio/html/group__sac.html#ga65ddb9d01a8d1ea66bc7ad1024b30534)
   - [sac_get_float()](https://savage13.github.io/sacio/html/group__sac.html#ga94ddfd21929cd9cb6faa508d8b6d1460) / [sac_set_float()](https://savage13.github.io/sacio/html/group__sac.html#gaa5cd583512409156b09d6a4b0ec4b683)

Documentation for each function includes a selection of example usage (these also function as tests).

Within the test directory `t` are a set of example programs that use the library.  The best programs to get started are:

   - t/iotest.c - Reading of all data files and accessing header and data values
   - t/cut.c    - Reading data files with a imposed time window (cut)
   - t/cutim.c  - Windowing data within memory (cut)
   - t/compat.c - Usage of the library with original sacio library function names
   - t/ver.c    - Examples of behavior differences between v6 and v7 SAC header files

### Read, Change and Write a sac file

Compile and run the example below using:

   gcc -I. -o example example.c libsacio_bsd.a
   ./example t/imp.sac

The file `t/imp.sac` is an impulse function and distributed with `sacio`.

```c

#include <stdio.h>
#include <sacio.h>

int
main(int argc, char *argv[]) {
    // Variable Declaration
    int i;
    int nerr = 0;
    char org[128] = {0};
    char id[64] = {0};
    double delta;
    sac *s = NULL;

    if(argc < 2) {
        printf("Usage: example file.sac\n");
        return -1;
    }

    // Read a SAc file, could be evenly or unevenly spaced
    s = sac_read(argv[1], &nerr);
    if(nerr != 0) {
        printf("Error: %d reading file\n", nerr);
        return -1;
    }

    // Get a floating point value: delta
    sac_get_float(s, SAC_DELTA, &delta);

    // Print out some header values
    printf("delta: %f\n", delta);
    printf("npts:  %d\n", s->h->npts);
    printf("comps: %d\n", sac_comps(s));

    // Print out the first 5 data values
    for(i = 0; i < 5; i++) {
        printf("y[%4d]  %f\n", i, s->y[i]);
    }

    // Set some header character string values
    sac_set_string(s, SAC_NET, "IU");
    sac_set_string(s, SAC_STA, "BORG");
    sac_set_string(s, SAC_LOC, "00");
    sac_set_string(s, SAC_CHA, "BHZ");

    // Set the origin time to 1994/160 00:33:16.123
    sac_set_time(s, timespec64_from_yjhmsf(1994, 160, 0, 33, 16, 123));

    // Format the origin time in absolute time
    sac_fmt(org, sizeof(org), "%TO", s);
    printf("Origin Time: %s [Absolute]\n", org);

    // Output the Station id: NET.STA.LOC.CHA
    sac_fmt(id, sizeof(id), "%Z", s);
    printf("Station ID: %s\n", id);

    // Write out the modified sac file
    sac_write(s, "output.sac", &nerr);

    return 0;
}
```


Downloading and installing
--------------------------

### Downloading 

This library can be downloaded directly by either going to **Code->Download Zip** or by using git as:

    git clone https://github.com/savage13/sacio.git

This library is self contained and does not require any additional dependencies. 
 
### Compiling

Once downloaded, the library can be compiled from within the `sacio` or `sacio-master` directory using:
  
    ./configure
    make 
    
### Installation

Installation to the default location `/usr/local` can be completed using:

    make install

This will install `libsacio_bsd.a` into `/usr/local/lib/libsacio_bsd.a` and 
`sacio.h` and `timespec.h` into the `/usr/local/inclucde/sacio` directory.  Creating 
a symbolic link to this library will assist in existing programs that require
the sacio library, e.g.:

    ln -s /usr/local/lib/libsacio_bsd.a /usr/local/lib/libsacio.a

Passing the `--prefix` option to the configure command allows a different installation location.  

### Testing

Tests for the library can be run if desired using

    make test
  

Please report issues to the project.

If you would like to contribute to the project please file Pull Requests and/or create issues for discussion at the libsacio project.

Included Libraries
------------------

- 64-bit time https://github.com/evalEmpire/y2038 (MIT License)
- GeographicLib https://geographiclib.sourceforge.io/ version 1.49 (MIT/X11 License)

License
-------

The code here is licensed under the 2-Clause BSD License, except where specified.
