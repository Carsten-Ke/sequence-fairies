.. _seqCheck:

********
seqCheck
********



Unfortunately sometimes protein sequences sets may contain stop characters (usually '*' or '.'). In many programs those chars will cause problems. stopCleaner can help you with this. The default settings
will simply remove stop chars at the end of a sequence. But there are other options that allow you to take care of stop chars in the middle of sequences as well.



.. image:: ../images/seqCheck.png
  :width: 100%
  :alt: Graphical representation of the program


.. warning::

    This program allows you to fastly clean a sequence file so that issues that can cause problems are solved. However, you should have a look at what caused the issues to appear in the first place.
    

If you decide to correct all problems, the following order is applied:

1. end stops
2. all other stops
3. strange symbols
4. duplicates

============
Options
============


General options
---------------

The general options influence the general behaviour of stopCleaner:

.. program:: stopCleaner

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -i <FILE>, --in
    
    The sequence file to filter. The format should be FASTA format.

.. option:: -o <FILE>, --out <FILE>

    The output file. If none is provided, the sequences will be printet to the console.

.. option:: -r <FILE>, --report <FILE>
    
    The file to store the report in. If not provided report is printed to console.
    
.. option:: -A, --check-all

    Enables all available checks
    
.. option:: --fix-and-keep

    Fixes all detected problems and keeps all sequences (enables all checks)

.. option:: --fix-and-remove

    Fixes all detected problems by removing all sequences that were detected to have problems. Only exception are end stops which are still only shortened.
    

Stop check options
------------------

Stop signes can appear for normal reasons (e.g. translated last codon) or for problematic reasons (e.g. wrong gene model). Whatever the reason, Stop signs in protein sequences (e.g. '.' or '*') often cause problems for other programs.


.. option:: --check-stops

    This option will check for stop chars
    
.. option:: --fix-ending

   Removes all stops at the end of a sequence. This step is done before the other stop corrections!

.. option:: --remove-stop-genes

    Remove all genes with stops

.. option:: --stop-char 

    The stop chars to use. (*default: .\**)
        
.. option:: --replace-stop

    Replace in sequence stops with ambigious char  ('N' in case of DNA/RNA sequences, 'X' in proteins).

Alphabet check options
----------------------

Sometimes sequences contain rare amino acids (e.g. U - Selenocystein). These symbols will create problems in some problems. This section contains options to either remove sequences containing the sysmbol or replace it with a less problematic symbol. These options here ignore the stop symbols.

.. option:: --check-alphabet

    Check if a usual alphabet is used
    
.. option:: --set-alphabet

    The alphabet to use (protein, DNA or RNA). 
        
.. option:: --replace-char

    Replace weird chars with ambigious one ('N' in case of DNA/RNA sequences, 'X' in proteins).
        
.. option:: --remove-alpha

    Removes sequences with problematic chars


Duplicate check options
-----------------------

Sequence names should be uniq within a file. However, sometimes due to errors in a GFF sequence names can be duplicated because sequence segments are not merged properly.

.. option:: --check-duplications

    Check for sequences with the identical identifiers
    
.. option:: --rename-duplicates

    Renames the duplicate enties. The first sequence found will keep the original name, the second will be rename with <name>-2, the third <name>-3 etc...
    
.. option:: --remove-duplicates

    Removes all but one sequence with the same name. Only the first one encountered is kept.
