#!/bin/sh

MODE="Debug"
GENERATOR=""
ADDITIONAL=""

echo "[builder] Removing old release"

rm -rf ProgramRelease

echo "[builder] Creating neccessary dirs"

mkdir ProgramRelease
mkdir ProgramRelease/build

echo "[builder] Preparing cmake"

NEXT_GEN=0
NEXT_MODE=0;
NEXT_ADD=0
for var in "$@"
do
    if [[ NEXT_GEN -eq 1 ]];
        then
            GENERATOR="$var"
            NEXT_GEN=0
    fi
    if [[ NEXT_MODE -eq 1 ]];
        then
            MODE="$var"
            NEXT_MODE=0
    fi
    if [[ NEXT_ADD -eq 1 ]];
        then
            ADDITIONAL="$var"
            NEXT_ADD=0
    fi
    if [[ $var == *"-m"* ]];
        then
            NEXT_MODE=1            
    fi
    if [[ $var == "-g" ]];
        then
            NEXT_GEN=1
    fi
    if [[ $var == "-a" ]];
        then
            NEXT_ADD=1
    fi
done

echo "[builder] Configurating cmake"

if [[ -z "$GENERATOR" ]];
then 
    cmake -S./ -B./ProgramRelease/build $ADDITIONAL
else
    cmake -S./ -B./ProgramRelease/build -G "$GENERATOR" $ADDITIONAL
fi

echo "[builder] Building"

cmake --build  ProgramRelease/build --config "$MODE" --target ALL_BUILD

echo "[builder] Copying files"
echo "[builder] Copying core"
cp ProgramRelease/build/configurator/"$MODE"/Configurator* ProgramRelease/
cp ProgramRelease/build/archiver/"$MODE"/archiver* ProgramRelease/
cp ProgramRelease/build/external_libs/lib_lz4/"$MODE"/lz4* ProgramRelease/

rm ProgramRelease/*.pdb
rm ProgramRelease/*.lib
rm ProgramRelease/*.exp

echo "[builder] Copying patcher"
cp -R patcher ProgramRelease/
echo "[builder] Copying patcher dependencies"
echo "[builder] Copying lz4"
cp ProgramRelease/build/external_libs/lib_lz4/"$MODE"/lz4_static* ProgramRelease/patcher
echo "[builder] Copying patcher archiver"
cp ProgramRelease/build/archiver/"$MODE"/archiver_static* ProgramRelease/patcher
rm ProgramRelease/patcher/*.pdb
echo "[builder] Copying headers"
mkdir ProgramRelease/archiver
cp archiver/*.h ProgramRelease/archiver

mkdir ProgramRelease/external_libs
mkdir ProgramRelease/external_libs/lib_lz4
cp external_libs/lib_lz4/*.h ProgramRelease/external_libs/lib_lz4

mkdir ProgramRelease/shared
cp shared/*.h ProgramRelease/shared


echo "[builder] Clean up"

rm -rf ProgramRelease/build

echo "[builder] Creating changelog.txt"

git log --pretty="%ai %aN - %s" > ProgramRelease/program/changelog.txt
echo "[builder] All done"