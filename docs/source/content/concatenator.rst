.. _concatenator:

************
concatenator
************

Concatenates alignments into a single alignment!

=========
Use cases
=========

* A typical case is to concatenate alignments of 1-1 orthologs to create a large alignment that will be used to reconstruct a species phylogeny

=======
Options
=======

In this section you can find a number of options that can be used.

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

.. option:: -l, --lenient

    If a sequence is missing the it will be filled with gaps.

.. option:: -p <STRINGS>, --pattern <STRINGS>

    For each sequence to concatenate a pattern is provided that is found in the sequence name

.. option:: -D CHAR, --delimiter CHAR
   
   Delimiter to use, if none is provided the whole sequence name is used

.. option:: -o, --out <FILE>

    The file to write the output to. If none is given, the the output will be printed to the terminal.


============
Simple Usage
============


Often you have all the files in a directory and you want to 

.. code-block:: bash

    $ concatenator -i aln1.fa aln2.fa -o concatenatedAln.fa
