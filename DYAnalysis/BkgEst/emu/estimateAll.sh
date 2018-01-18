#!/bin/bash


root -l -b -q estimateBkg.cc+'("mass")'
root -l -b -q estimateBkg.cc+'("pt")'
root -l -b -q estimateBkg.cc+'("rap1560")'
root -l -b -q estimateBkg.cc+'("rap60120")'
root -l -b -q estimateBkg.cc+'("phistar")'

# root -l -b -q drawplots.C'("mass")'
# root -l -b -q drawplots.C'("pt")'
# root -l -b -q drawplots.C'("rap1560")'
# root -l -b -q drawplots.C'("rap60120")'
# root -l -b -q drawplots.C'("phistar")'
