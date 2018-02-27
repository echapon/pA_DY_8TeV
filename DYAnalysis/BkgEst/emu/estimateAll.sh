#!/bin/bash

# shift the ttbar xsec up (for syst)
root -l -b -q estimateBkg.cc+'("mass",1)'
root -l -b -q estimateBkg.cc+'("pt",1)'
root -l -b -q estimateBkg.cc+'("rap1560",1)'
root -l -b -q estimateBkg.cc+'("rap60120",1)'
root -l -b -q estimateBkg.cc+'("phistar",1)'

# shift the ttbar xsec down (for syst)
root -l -b -q estimateBkg.cc+'("mass",-1)'
root -l -b -q estimateBkg.cc+'("pt",-1)'
root -l -b -q estimateBkg.cc+'("rap1560",-1)'
root -l -b -q estimateBkg.cc+'("rap60120",-1)'
root -l -b -q estimateBkg.cc+'("phistar",-1)'

# nominal
root -l -b -q estimateBkg.cc+'("mass")'
root -l -b -q estimateBkg.cc+'("pt")'
root -l -b -q estimateBkg.cc+'("rap1560")'
root -l -b -q estimateBkg.cc+'("rap60120")'
root -l -b -q estimateBkg.cc+'("phistar")'

root -l -b -q drawplots.C'("mass")'
root -l -b -q drawplots.C'("pt")'
root -l -b -q drawplots.C'("rap1560")'
root -l -b -q drawplots.C'("rap60120")'
root -l -b -q drawplots.C'("phistar")'

