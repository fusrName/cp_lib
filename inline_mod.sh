#!/bin/bash
sed -e $'/#include\s*"mod.cpp"/{r mod.cpp\nd}' $1 | sed -e '/\/\//d'
