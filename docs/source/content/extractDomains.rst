.. _extractDomains:

**************
extractDomains
**************

The purpose of this program is to extract the protein sequences of domains. The input is a domain annotation and the sequences from which to extract the domains.


=======
Options
=======


General options
---------------

The general options influence the general behaviour of extractDomains:

.. program:: extractDomains

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -d <FILE>, --domains <FILE>

    The domain annotation file(s) to be used used. These files are created for example by pfam_scan.pl. See :ref:`domainAnnotation` for more information on how to annoate your fasta file with domains.
    
.. option:: -s <FILE>, --sequences <FILE>
    
    The sequence file to use. Should be the same that has been used to create the domain annotation file.

.. option:: -o <FILE>, --out <FILE>

    The output file. In this file the extracted domain sequences will be saved in FASTA format.

.. option:: -n <ARG>, --name <ARG>

    The Pattern to be used for the name of the extracted sequence. The following placeholders can be used:

        * %s The sequence name
        * %b The start position of the domain in the sequence
        * %e The end position of the domain in the sequence
        * %d The domain ID
    
    The default value of this parameters is: (=%s_%b-%e %d)

.. option:: --id <ARG>

   The pfam accessesion number of the protein (sub-)sequences to extract. If it is not provided all domain sequences as described in the domain file will be extracted.

.. option:: --min-length <ARG>

   The minimum length of the sequences that will be extracted. Shorter domain sequences will simply be ignored.
   
   
.. option:: -e, --use-evelope

   If available (for example when using Pfam annotations) the envelope coordinates will be used.

   
============
Simple Usage
============

.. code-block:: bash

    extractDomains -i seqs.fasta -d seqs.pfam -o domains.fa --id PF00042


===============================================
Example Use Case: Building a more sensitive HMM
===============================================

While HMMs are already quite sensitive it is sometimes useful to create a HMM that is better fitting to the species set you want to analyse. 
The general workflow in this case would be:

1. Annotate the proteome(s) of interest and closely related ones with a normal :ref:`domain annotation <domainAnnotation>` program
2. Extract the domain sequences using ``extractDomains``
3. Combine these extracted sequences with the original PFAM seed sequences into a single sequence Set
4. Create a new multiple sequence alignment based on the the set created in the previous step
5. Use the `HMMER <http://hmmer.org/>`_ or `HH-suite <https://github.com/soedinglab/hh-suite>`_ programs to create a new HMM

You can now rescan your proteomes using the new HMM.