.. _fileMerger:

**************
fileMerger
**************

The purpose of this program is to merge sequence files into a single file. It will automatically remove duplicate entries where name *and* sequence are the same. It will also throw an error if the same name occurs with 
but having a different sequence.


=======
Options
=======


General options
---------------

The general options influence the general behaviour of fileMerger:

.. program:: fileMerger

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -i, --in <FILES>

    A list of sequence files that will be concatenated.
    
.. option:: -d, --directory <DIRECTORY>
    
    A directory containing the sequence files. Only files with a specified ending (see :option:`--ending`) will be used.

.. option:: -e <STRING>, --ending <STRING>
    
   The file ending to use together with the :option:`--directory` option. (*default: .fa*)

.. option:: -o <FILE>, --out <FILE>

    The output file. In this file all sequences will be saved in FASTA format.

.. option:: -r --rename

    In case a sequence exists with same name but different sequence, a new uniq name will be given

   
============
Simple Usage
============

.. code-block:: bash

    fileMerger -i seqs1.fasta seqs2.fasta -o merged.fa


