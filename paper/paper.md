---
title: 'sacio: A library for Seismic Analysis Code data files'
tags:
  - C
  - Fortran
  - seismology
  - earth science
  - geophysics
authors:
  - name: Brian Savage
    orcid: 0000-0002-9252-6282
    affiliation: 1
affiliations:
  - name: University of Rhode Island
    index: 1
date: 8 July 2021
bibliography: paper.bib
---

# Summary

Since nearly the inception of digital seismological data, the Seismic Analysis Code (SAC) has been utilized as processing software and a file format [@goldstein1995status; @goldstein2003sac2000; @goldstein2005sac]. Having a well-defined format is essential for the distribution and processing of seismological data.  Easy and quick access to seismological data over the internet or within local networks is important for the development of seismological tools that impact hazards (earthquakes, volcanoes, and tsunami), national security, and geophysical imaging on a variety of length scales. Here we implement an improved and open source version of the sac format with backwards compatibility.

# Statement of need

The sacio library allows reading and writing of binary data files containing time series data, typically ground motion recorded by seismometers.  This library is used in seismological research including, but not limited to, earthquake location [@michael2013precise] and magnitude determination [@miao2007empirical], seismic tomography [@tape2010seismic], earthquake early warning activities [@colombelli2013application], earthquake source process discrimination [@templeton2018seismic], and hazard probability estimates [@al2008improving; @cramer2014update].

A number of deficiencies exist with the original implementation of the routines to read and write SAC binary files, written in Fortran 77 in the 1980s and directly translated into C in the late 1990s.  First, it was tightly coupled with the processing routines included with the SAC program and difficult to use in external programs.  Second, the routines were a closed source and are covered by export restrictions by the US Department of Energy (US-DOE) and Lawrence Livermore National Laboratory (LLNL). Third, the routines and associated meta data in the data files did not meet existing needs of current seismological data sets, including very high samples rates, > 100 Hz, long duration data series (multiple days to months), and dense sampling arrays.

This open source library that reads and writes SAC binary and alphanumeric files fixes these outstanding issues.  The library is fully decoupled and released under an open source license, BSD 2-Clause.  It is straightforward to link this library with existing software projects without need to request the closed source, export-restricted version of SAC.

This library was designed as a drop-in replacement with strong backwards compatibility for the original closed source version to facilitate an easy transition for scientists; it is currently included within the official SAC distribution and used by seismologists globally. Moreover, the library was written to allow simple linking from C (for language interoperability) and Fortran, still used by scientists and seismologists.

Finally, the library adds routines to handle 64-bit metadata, or header values, to the binary file format to allow for very high sampling rates, long duration data series and high precision station locations. 

# Acknowledgments

We acknowledge the contributions of Arthur Snoke for the detailed discussions on version 7 of the header and the support of the SAC user community.

# References
