#!/bin/bash
sed -e $'/#include\s*"dsu2.cpp"/{r dsu2.cpp\nd}' $1 | sed -e '/\/\//d'
