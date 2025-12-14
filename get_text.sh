#!/bin/bash

url="$1"
out="input.txt"

curl -L "$url" | lynx -stdin -dump -nolist > "$out"