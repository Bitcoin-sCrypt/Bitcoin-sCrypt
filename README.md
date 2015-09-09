Bitcoin - sCrypt v1.4.0-beta

## HOW TO COMPILE GITIAN BUILD OF BITCOIN-SCRYPT QT CLIENT FOR WINDOWS

Amended for Bitcoin-sCrypt by Smokeasy from Gavin's Notes to getting gitian builds up and running and Onichan's Guide to compiling on Windows (https://bitcointalk.org/index.php?topic=84984.0).

# Gitian uses a deterministic build process to allow multiple builders to create verifiably identical binaries thereby allowing users to download trusted binaries that are verified by multiple builders.  (http://gitian.org).

# REQUIREMENTS:  You need the right hardware - you need a 64-bit-capable CPU with hardware virtualization support (Intel VT-x or AMD-V). Not all modern CPUs support hardware virtualization.  You probably need to enable hardware virtualization in your machine's BIOS. You need to be running a recent version of 64-bit-Ubuntu because Gitian supports only Ubuntu hosts (so the Windows client must be cross-compiled on Ubuntu). Check the log files if you run into errors.

# First, install dependencies:

1. sudo apt-get install apache2 git apt-cacher-ng python-vm-builder qemu-kvm ruby qemu-utils rubygems zip curl

# Sanity checks:

2. sudo service apt-cacher-ng status   # Should return apt-cacher-ng is running
3. ls -l /dev/kvm   # Should show a /dev/kvm device

# Once you've got the right hardware and software we can compile the Windows client

# Clone local copies of bitcoin-scrypt and gitian source-codes

4. Enter Ubuntu terminal
5. git clone git://github.com/bitcoin-scrypt/bitcoin-scrypt.git bitcoin
6. git clone git://github.com/devrandom/gitian-builder.git gitian
7. mkdir gitian/inputs
8. zip -r gitian/inputs/bitcoin-1.4.0.zip bitcoin
9. cd gitian/inputs

# Fetch and build inputs

10. wget 'http://downloads.sourceforge.net/project/boost/boost/1.49.0/boost_1_49_0.tar.bz2'
11. wget 'http://www.openssl.org/source/openssl-1.0.1g.tar.gz'
12. wget 'http://download.oracle.com/berkeley-db/db-4.8.30.NC.tar.gz'
13. wget 'http://miniupnp.free.fr/files/download.php?file=miniupnpc-1.6.tar.gz' -O miniupnpc-1.6.tar.gz
14. wget 'http://downloads.sourceforge.net/project/libpng/zlib/1.2.7/zlib-1.2.7.tar.gz'
15. wget 'http://sourceforge.net/projects/libpng/files/libpng15/older-releases/1.5.12/libpng-1.5.12.tar.gz'
16. wget 'http://fukuchi.org/works/qrencode/qrencode-3.2.0.tar.bz2'
17. wget 'http://download.qt-project.org/archive/qt/4.7/qt-everywhere-opensource-src-4.7.4.tar.gz'
18. cd ..

# Build Base Virtual Machine (this will take some time so do not quit prematurely)

19. sudo bin/make-base-vm --arch i386

# Build Bitcoin-sCrypt Windows Client

20. sudo bin/gbuild ../bitcoin/contrib/gitian-descriptors/qt-win32.yml
21. cp build/out/qt-win32-4.7.4-gitian.zip inputs
22. sudo bin/gbuild ../bitcoin/contrib/gitian-descriptors/boost-win32.yml
23. cp build/out/boost-win32-1.49.0-gitian2.zip inputs
24. sudo bin/gbuild ../bitcoin/contrib/gitian-descriptors/deps-win32.yml
25. cp build/out/bitcoin-deps-1.4.0.zip inputs
26. sudo bin/gbuild ../bitcoin/contrib/gitian-descriptors/gitian-win32.yml

## The compiled Windows GUI client, daemon and Windows Installer binaries (along with the source-code) will output to gitian/build/out.

================================================================

# SHA256 file checksums for input files:

dd748a7f5507a7e7af74f452e1c52a64e651ed1f7263fce438a06641d2180d3c  boost_1_49_0.tar.bz2
12edc0df75bf9abd7f82f821795bcee50f42cb2e5f76a6a281b85732798364ef  db-4.8.30.NC.tar.gz
aae4c469f5f03e7c180708fc547335ad1e29854bfdda992196e9c39d2447e9f6  libpng-1.5.12.tar.gz
bbd6b756e6af44b5a5b0f9b93eada3fb8922ed1d6451b7d6f184d0ae0c813994  miniupnpc-1.6.tar.gz
53cb818c3b90e507a8348f4f5eaedb05d8bfe5358aabb508b7263cc670c3e028  openssl-1.0.1g.tar.gz
03c4bc7cd9a75747c3815d509bbe061907d615764f2357923f0db948c567068f  qrencode-3.2.0.tar.bz2
97195ebce8a46f9929fb971d9ae58326d011c4d54425389e6e936514f540221e  qt-everywhere-opensource-src-4.7.4.tar.gz
fa9c9c8638efb8cb8ef5e4dd5453e455751e1c530b1595eed466e1be9b7e26c5  zlib-1.2.7.tar.gz


