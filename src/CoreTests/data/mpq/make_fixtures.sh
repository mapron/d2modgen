#!/bin/sh
# Rebuilds the MPQ fixtures used by StormStorageTest.
# Needs smpq (StormLib MPQ archiving utility): apt install smpq / AUR smpq.
set -e
cd "$(dirname "$0")"

rm -f d2data.mpq d2exp.mpq patch_d2.mpq

smpq -c d2data.mpq weapons.txt
smpq -R d2data.mpq weapons.txt 'data\global\excel\weapons.txt'

# StormStorage requires both patch archives to exist; empty ones are enough.
smpq -c d2exp.mpq
smpq -c patch_d2.mpq
