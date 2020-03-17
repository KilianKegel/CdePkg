# CdeServices
## Introduction
**CdeServices**, C Development Environment Services, is an UEFI POST driver that provides core
functions and datastructures, that the CdeLib is build on. **CdeServices**' architecture enables
**CdeLib** to run highly optimized in code and data space usage. It minimizes redundant code and
data space consumption linked to each UEFI driver, since CdeLib functions are just wrapper
functions — that invokes CdeServices.<br>
CdeServices contains:
  * `printf()`-family core
  * `scanf()`-family core
  * `realloc()`-family core (malloc(), free(), realloc())
  * `strcmp()`-family core (all str/wcs/mem n i functions)
  * `strcpy()`-family core (all str/wcs/mem n functions)
  * `strpbrk()`/`strspn()`-family core (all `str`/`wcs` `pbrk`- and `spn`- functions)
  * `strtok`()-family core (all `str`/`wcs` `tok`-functions)
  * `CRT0` startup code for DXE, SMM, PEI
  * buffered I/O core (`fread()`/`fwrite()`)
  * locale character tables
  * interface functions to access DXE- and PEI-API
## Implementation

The **CdeServices** drivers are provided for DXE, SMM and PEI in binary format only.

### Status
**CdeServices** is still in state of EVALUATION. 
