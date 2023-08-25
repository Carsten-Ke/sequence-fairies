.. _dsmReader:

**********
dsmReader
**********

Small program that can extract similarity values from a Domain Similarity Matrix (DSM). DSMs can be obtained from the following Website: https://domainworld.uni-muenster.de/data/dsm/

General options
---------------

The general options influence the general behaviour of dsmReader:

.. program:: dsmReader

.. option:: -h, --help

    Prints a simple help message with a small description of all the available options.

.. option:: -m <FILE>, --matrix <FILE>

    The DSM file.
    
.. option:: --d1 <DOMAIN>
    
    The ID of the first domain

.. option:: --d2 <DOMAIN>
    
    The ID of the second domain

.. option:: -n, --use-negative

    Scale values to include negative values.


============
Simple Usage
============

.. code-block:: bash

    $ dsmReader -m pfam-31.dsm  --d1 PF00001 --d2 PF00002
