#!/bin/bash
sed -e $'/#include\s*"dsu.cpp"/{r dsu.cpp\nd}' $1 | sed -e '/\/\//d'
