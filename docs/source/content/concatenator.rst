.. _concatenator:

************
concatenator
************

Concatenates alignments into a single alignment!

.. image:: ../images/aln.svg
  :width: 100%
  :alt: Graphical representation of the program

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

    If a sequence is missing the space will be filled with gaps.

.. option:: -o, --out <FILE>

    The file to write the output to. If none is given, the output will be printed to the terminal.

Name detection options
----------------------

To concatenate two alignments, the correct sequences need to be concatenated. By default, a complete match 
of the sequence name is required to determine the match. However, the options below introduce ways to allow 
only partial matches.

.. program:: concatenator

.. option:: -D CHAR, --delimiter CHAR
   
    The first part of the sequence until the first occurrence of the delimiter is used to determine the match.
    If your files look like:

    | >Mnat_123
    | acgtacgt
    | >Znev_456
    | acgtacgt
    | >Csec_789
    | acgtacgt

    then you would use: ``-D _``. The parts used for matching are then Mnat, Znev and Csec.


.. option:: -p <STRINGS>, --pattern <STRINGS>

    For each sequence to be concatenated, a pattern is provided that is found in the sequence name.
    If your files look like:
    
    | >abc_Mnat_123
    | acgtacgt
    | >def_Znev_456
    | acgtacgt
    | >ghi_Csec_789
    | acgtacgt

    then you would use: ``-p Mnat Znev Csec``

.. NOTE::
   Only sequences matching the given patterns will be concatenated. Other sequences will be ignored.

========
Examples
========

You can find the input files in the concatenate example folder. You can execute them and have a look at the input files.

Here are some example where the matching is simply based on the full sequence name.

.. code-block:: bash

    # Concatenate alignments explicitly giving the names
    $ concatenator -i complete/*.fasta

    # Same but providing the directory 
    $ concatenator -d complete -e .fasta


Here are some example where the matching is based on a common prefix. The delimiter determines where the prefix ends.

.. code-block:: bash

    # Concatenate alignments based on their prefix
    concatenator -i prefix/aln1.fasta prefix/aln2.fasta -D _

    # Concatenate alignments based on their prefix allowing for missing sequences
    concatenator -i prefix/aln1.fasta prefix/aln2.fasta prefix/aln3.fasta -D _ -l

Here is an example where given patterns is used to concatenate the sequences

.. code-block:: bash

    # Concatenate alignments based on the patterns given
    concatenator -i pattern/aln1.fasta pattern/aln2.fasta -p seqA seqC seqG
