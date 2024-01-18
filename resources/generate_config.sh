#!/bin/bash
cd ..
git submodule update --init --recursive
cd src/defines/templates/
cp * ../
