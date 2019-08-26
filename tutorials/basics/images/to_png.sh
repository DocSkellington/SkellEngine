#!/usr/bin/env bash

for file in *.tex
do
    filename="${file%.*}"
    pdflatex ${file}
    convert -compose copy -density 300 -alpha on "${filename}.pdf" "${filename}.png"
done