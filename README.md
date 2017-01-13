# libmysofa

## Introduction

This is a simple set of C functions to read AES SOFA files, if they contain HRTFs
stored according to the AES69-2015 standard [http://www.aes.org/publications/standards/search.cfm?docID=99].

## Compile

Enter following commands

> cd build

> cmake -DCMAKE_BUILD_TYPE=Debug ..

> make all test

## Usage 

Libmysofa has two main function calls.

To read a SOFA file call 

```
#include <mysofa.h>

struct MYSOFA_HRTF *hrtf = mysofa_load("file.sofa", &err);
```

To free the HRTF structure, call:
```
mysofa_free(hrtf);
```

In order to make the use of SOFA files even more easy, we have added some functional calls. 
The following functional checks for a typical parameter set with in the SOFA file. More precisely, for a SimpleFreeFieldHRIR data in the time domain. If the file does not match those properties, the result is not MYSOFA_OK (0).
```
mysofa_check(hrtf) == MYSOFA_OK
```
In a SOFA file, coordinate can be stored spherical or carthesian. The following call converts all coordinates to the spherical format.
```
mysofa_tospherical(hrtf)
```
The FIR filter in a SOFA file are not sorted at all. Thus, looking for filter parameters has a complexity of O(N). The following function call sorts the filter according to the coordinates (using a 3D Morton mapping). The result is a MYSOFA_LOOKUP structure.
```
struct MYSOFA_LOOKUP lookup = mysofa_sort(hrtf);
```
To find some a nearby filter for given coordinates with O(log n) complexity, call 
```
int filter_index = mysofa_lookup(lookup, double *coordinate
```
The lookup structure is freed with
```
mysofa_lookup_free(lookup);
```

## Disclaimer

The SOFA files are from https://www.sofaconventions.org/, Piotr Majdak <piotr@majdak.com>.
The source code is by Christian Hoene <christian.hoene@symonics.com>, Symonics GmbH, and available under Apache 2.0 license.
This work has been funded by Germman Federal Ministry of Education and Research, funding code 01IS14027A.


