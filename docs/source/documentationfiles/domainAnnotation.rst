.. _domainAnnotation:

*****************
Domain annotation
*****************

To annotate sequences with domains, different programs and databases exist. Our programs mainly support Pfam domains. This section will give you a short overview on how to annoate your protein sequences with pfam domains. The standard program to annotate your proteome with pfam domains is pfam_scan.pl which is provided by the Pfam database.

============
Requirements
============

 * pfam_scan.pl (http://ftp.ebi.ac.uk/pub/databases/Pfam/Tools/)
 * HMMER (http://hmmer.org/download.html)
 * BioPerl (https://bioperl.org/)
 

 
======================
Preparing the database
======================

This steps needs to be done only once when you downloaded a new version of the HMM Models from Pfam. 

Downloaded the Pfam-A.hmm.gz and the Pfam-A.hmm.dat.gz file from the Pfam release you want to use: http://ftp.ebi.ac.uk/pub/databases/Pfam/releases/. Then, you will need to run the following commands to create a database that is usable by pfam_scan.pl/hmmscan

.. code-block:: bash
  
  # unzip the downloaded files
  gunzip Pfam-A.hmm.gz
  gunzip Pfam-A.hmm.dat.gz

  # use hmmpress from the HMMER suite to prepare the databse for hmmscan
  hmmpress Pfam-A.hmm

These steps will create some additional files.

====================
Running pfam_scan.pl
====================

For pfam_scan.pl to work properly you will need to:

  1. Have the HMMER binaries in your PATH variable for example by manipulating PATH
  
    .. code-block:: bash
  
     export PATH=<path to directory containing HMMER executables>:$PATH
  
  2. Set the PERL5LIB variable to include the pfam_scan.pl directory
  
   .. code-block:: bash
  
     export PERL5LIB=<path to PfamScan directory>:$PERL5LIB


You can then call pfam_scan.pl

.. code-block:: bash
  
  pfam_scan.pl -fasta <sequence file> -dir <directory containing the hmm files> -outfile <output file>
 
 
