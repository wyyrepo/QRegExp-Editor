#!/bin/bash

for i in "16" "24" "32" "48" "64" "128" "256"
do
    convert -background none qregexp-editor-logo.svg -resize $i qregexp-editor-logo-${i}.png;
done
exit 0
