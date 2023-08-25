.. _isoformCleaner:

**************
isoformCleaner
**************

Often proteomes contain annotated isoforms. While in general this is a good thing, is some cases that might cause problems (e.g. when estimating the quality of a proteome with DOGMA (https://domainworld.uni-muenster.de/programs/dogma/)). In such cases it is a common approach to only keep the longest isoform of a protein and remove the others from the set. Unfortunately, there is no
general agreement on how to mark isoforms. However, this program tries to handle the most common ways.

============
Options
============


General options
---------------

The general options influence the general behaviour of isoformCleaner:

.. program:: isoformCleaner

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -i <FILE>, --in
    
    The sequence file to filter. The format should be FASTA format.

.. option:: -o <FILE>, --out <FILE>

    The output file. If none is provided, the sequences will be printet to the console.
    
.. option:: -s <CHAR>, --split-char <CHAR>

    The split character to use. (default: '-'). Using a split character is the default option. If you use any other available cleaning option, this one will be turned off. If no split character is found
    in the name, a warning message is displayed and the sequence will be kept in the output.

.. option:: --summary

    List the number of sequences in the input, the output and difference between the two.



Regex options
-------------

Regular expressions can help to identify the gene name inside the fasta header of a file.

.. option::  -r <ARG>, --regular <ARG>

    A regular expression that determines the gene name of the isoform. For more information on the allowed C++ regular expression have a look at the following website: http://www.cplusplus.com/reference/regex/ECMAScript/

.. option:: -n,  --name 

    Search name only
    
.. option:: -c, --comment
    
    Search comment only
    
.. option:: -p <ARG>, --preset <ARG>

    Currently we have two presets that can be used to identify gene names.
    Preset regex: Can be either 'flybase' or 'gene'
  

==============
Simple Usage
==============

If you have a fasta file with proteins which isoforms are marked by a name after a split character (e.g. name-RA, name-RB) one can use the split char option to identify the different isoforms.

File: proteome.fa

.. code-block:: none
    
    >seq1-RA
    ThisIsAShortIsoform
    >seq1-RB
    ThisIsALongerIsoformOfTheSameProtein


.. code-block:: bash
    
    $ isoformCleaner -i proteome.fa -s '-' 
    >seq1-RB
    ThisIsALongerIsoformOfTheSameProtein
    
    
In some cases a simple split character is not sufficient. In nthat case maybe a regular expression can help. For some cases we have predefined expressions:

File: regex.fa

.. code-block:: none
    
    >seq1 gene:1
    ThisIsAShortIsoform
    >seq2 gene:1
    ThisIsALongerIsoformOfTheSameProtein
    
.. code-block:: bash
    
    
    $ isoformCleaner -i regex.fa -r "gene[:=]\\s*([\\S]+)[\\s]*" 
    >seq2 gene:1
    ThisIsALongerIsoformOfTheSameProtein

    # The regular expression above is already provided as a preset:
    $ isoformCleaner -i regex.fa -p gene 
    >seq2 gene:1
    ThisIsALongerIsoformOfTheSameProtein
    

    
