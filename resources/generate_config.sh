#!/bin/bash
cd ..
git submodule update --init --recursive
cd src/defines/templates/
cp gifnoc-template.h ../config.h
cp laitnedifnoc-template.h ../confidential.h