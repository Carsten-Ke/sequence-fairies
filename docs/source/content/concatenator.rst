.. _concatenator:

************
concatenator
************

Concatenates alignments into a single alignment!



General options
---------------



.. program:: concatenator

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -i, --in <FILES>

    A list of alignment files that will be concatenated.
    
.. option:: -d, --directory <DIRECTORY>
    
    A directory containing the alignment files. Only files with a specified ending (see :option:`--ending`) will be used.

.. option:: -e <STRING>, --ending <STRING>
    
   The file ending to use together with the :option:`--directory` option. (*default: .fa*)

.. option:: -f, --fill-gaps

    If a sequence is missing the it will be filled with gaps.

.. option:: -o, --out <FILE>

    The file to write the output to. If none is given, the the output will be printed to the terminal.


============
Simple Usage
============


Often you have all the files in a directory and you want to 

.. code-block:: bash

    $ concatenator -i aln1.fa aln2.fa -o concatenatedAln.fa
