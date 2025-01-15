.. _seqExtract:

**************
seqExtract
**************

seqExtract is a program to extract sequences or subsequences from a given set. For large sequence files it has the option to build a index of a file for faster future access.

.. image:: ../images/seqExtract.png
  :width: 100%
  :alt: Graphical representation of the program


============
Options
============


General options
---------------

The general options influence the general behaviour of seqExtract:

.. program:: seqExtract

.. option:: -h, --help

    Prints a simple help message with a short description of all the available options.

.. option:: -i <FILE>, --in <FILE>
    
    The sequence file
    
.. option:: -I, --index

    An index file is used. If none exists it will be created. If :option:`-F` is not set the extension of the provided sequence file will be removed and replaced with '.sei'.
    
.. option:: -F <FILE>, --indexFile <FILE>

   The index file to use. Will be created if it doesn't exist yet. 
    
    .. note::

	If the sequence file is changed, the index file will need to be deleted so that a new index file will be created. It will not be done automatically.
    
  
.. option:: -l <FILE>, --inputList <FILE>
   
   File containing input files



Output options
---------------

.. option:: -o <FILE>, --out <FILE>

   The output file
   
.. option:: -a, --append

   Appends the sequences to an existing file instead of overwriting it
  
.. option:: -c, --remove-comments

    Remove comments from output sequences
  
.. option:: --extract-order

   Keeps the order given in the extraction line

   
Extract options
---------------

Here are the different options listed which influence which sequences (or subsequences) are extracted from the whole data set.

.. option:: -e <ARG> --extract <ARG>

   The (sub)sequence(s) to extract from the sequence file. Providing only the sequence name will extract the whole sequence. But you can also 
   provide coordinates. For example ``-e mySeq:1-10`` will extract the first ten amino acids of the sequence with name "mySeq". You can
   also merge several coordinates together: ``-e mySeq:1-10,21-30`` will create a sequence of length 20 which contains both sequence parts.
   ``-e mySeq:1-10 mySeq:21-30`` on the other hand will create to separate sequences.
   
.. option::  -N <FILE>, --namesFile <FILE>
   
   File with extraction lines to use. 

.. option:: -d <ARG>, --delim-extract <ARG> 

   The delimiter to use in the extraction file (*default: Tab*)
   
.. option:: -C <ARG>, --column <ARG> 
   
   The column in the extraction file to use (*default: 1*)
   
.. option:: -D <ARG>, --delim-pos  <ARG> 

   The delimiter to use to separate name from positions (*default: ':'*). This might need to be changed
   if you sequence names contain a ":" already.
   
.. option:: -r, --remove

   Removes the sequences with names provided in "-e" instead of extracting them. The whole sequence is removed even if subsections are provided.
   
.. option:: -n <ARG>, --numSeqs <ARG>

   The number of random sequences to extract (*default: 0*)
   
.. option:: -s <ARG>, --seed <ARG>
   
   Seed for random extract function
   
.. option:: -L <ARG>, --length <ARG>

   Length based extraction. Allowed values are '<NUM', '>NUM', '=NUM'. For example the code
   ``-L '>3' '<7'`` would extract all sequences with length 4-6
   
.. option:: -m <ARG>,  --ignore-missing <ARG>
   
   Usually a warning message is shown if a sequence cannot be found. This can be disabled using this option.

  
Modifying options
-----------------

These options modify the sequence that are extracted.

.. option:: -t, --translate

   Translate into amino acid

.. option:: -T <ARG>, --table <ARG> 

   The translation table to use (*default: standard*)
   
.. option:: -R, --revComp
   
    Calculate the reverse complement
    



