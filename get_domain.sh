#!/bin/bash

url="$1"
out="domain.txt"

curl -L "$url" | lynx -stdin -dump -nolist > "$out"