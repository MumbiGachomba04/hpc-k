HPC-K (Suite)
=============
The HPC-K suite is a set of Kernels, commonly used in High-Performance
Computing (ie, HPC), put together into a single source code repository. The
suite allows programmers to play with this well-known (or not so well known)
set of kernels. 

Kernel build
------------
The individual kernel compilation happens *in place*. In order to build a
kernel, enter its directory and execute `make`:

```shell
cd <kernel>
make
```

Each kernel directly inherit the configure options from the root directory file
`kernel.mk` (build options), and indirectly from `config.mk` (directories and
variables) and `target.mk` (default rules).

Kernel execution
----------------
All the kernels support a set a common command line options. Specific kernel
options are provided by means of the `-i` command line parameter. You can
execute the resulting binary with the  `-h` in order to check its complete list
of options.

```shell
Usage: <kernel-name> [options]
Where [options] are:
  -h           Display this information (and exit)
  -i <args>    Set kernel input arguments, where:
     <arg-1>   Argument 1 description (default=none)
     <arg-2>   Argument 2 description (default=none)
     ...
     <arg-n>   Argument n description (default=none)
  -v           Display the program version (and exit)
```



