# Sequence-Fairies

Sequence-Fairies contains several programs that can help with your analyses.

## Programs

Currently this program suite contains the following programs:

* extractDomains: Can extract the domain sequences from proteins sequences
* isoformCleaner: Removes isoforms from a sequence set and keeps only the longest one.
* seqCheck: Can identify and solve problems with sequence files (stops and other unexpected symbols, duplicated names)
* seqExtract: Can extract sequences from files



Requirements
------------

We try to keep the dependencies as little as possible. Current dependencies are:
- cmake (version 3.30 or greater)
- boost (http://www.boost.org)
- compiler with c++20 and modules support
- zlib
- libbz2


You will also need the BioSeqDataLib (https://zivgitlab.uni-muenster.de/domain-world/BioSeqDataLib), but this will be downloaded automatically when you run cmake

Download
--------

```bash
git clone https://zivgitlab.uni-muenster.de/domain-world/sequence-fairies.git
cd  sequence-fairies
```


Installation
------------


Change into the sequence-fairies directory and run the commands listed below. It will automatically download the necessary submodules, so an internet connection and git are necessary.

```bash
cmake -G Ninja -S . -B build
cmake --build build
```

Usage
-----

Please take a look at the online manual (https://domain-world.zivgitlabpages.uni-muenster.de/sequence-fairies/) to get a detailed overview of how to run the programs.


Problems, Bugs & Suggestions
----------------------------

We try our best not to have any bugs in the code, unfortunately some will probably avoid us and will not be detected. If you encounter one, please be so kind and let us know. The easiest way would be to use write an email to domainworld[ at ]uni-muenster.de.

