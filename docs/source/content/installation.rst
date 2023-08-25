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
 * compiler with c++11 and OpenMP support

--------
Download
--------

You can currently choose between two different installation options: ``git`` and a manual download. Using ``git`` is the recommended way but if that is
not possible you can use the manual way instead. You can download the source code from our gitlab server without the use of git as well.

Git
^^^

The easiest way to download the most recent version of DW-Helper is to use ``git``:

.. code-block:: bash

    git clone https://zivgitlab.uni-muenster.de/domain-world/dw-helper.git
    cd dw-helper
    git submodule init
    git submodule update


If you want to update to a newer version you can simply run the following command:

.. code-block:: bash

  git pull
  git submodule update --recursive --remote

Do not forget to recompile the program after this step.

------------
Installation
------------

The executables of dw-helper can be compiled using the following command inside the root directory of the project.

.. code-block:: bash
 
  mkdir build
  cd build
  cmake ..
  make

This command creates the executables within the build directory.
