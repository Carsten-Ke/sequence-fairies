.. _installation:

************
Installation
************

------------
Requirements
------------

We try to keep the dependencies as few as possible. Current dependencies are:

 * BioSeqDataLib (https://zivgitlab.uni-muenster.de/domain-world/BioSeqDataLib.git) (can be added as git submodule)
 * boost (http://www.boost.org)
 * compiler with c++20 and OpenMP support

--------
Download
--------

You can currently choose between two different installation options: ``git`` and a manual download. Using ``git`` is the recommended way but if that is
not possible you can use the manual way instead. You can download the source code from our gitlab server without the use of git as well.

Git
^^^

The easiest way to download the most recent version of DW-Helper is to use ``git``:

.. code-block:: bash

    git clone https://zivgitlab.uni-muenster.de/domain-world/sequence-fairies.git
    cd  sequence-fairies


------------
Installation
------------

The executables of dw-helper can be compiled using the following command inside the root directory of the project. It will automatically download the necessary
submodules.

.. code-block:: bash
 
  mkdir build && cd build
  cmake ..
  make


This command creates the executables within the build directory. You can copy them to any place.
