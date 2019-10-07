#!/bin/bash

# shift the ttbar xsec up (for syst)
root -l -b -q estimateBkg.cc+'("mass",1)'
root -l -b -q estimateBkg.cc+'("mass3bins",1)'
root -l -b -q estimateBkg.cc+'("pt",1)'
root -l -b -q estimateBkg.cc+'("pt1560",1)'
root -l -b -q estimateBkg.cc+'("rap1560",1)'
root -l -b -q estimateBkg.cc+'("rap60120",1)'
root -l -b -q estimateBkg.cc+'("phistar",1)'
root -l -b -q estimateBkg.cc+'("phistar1560",1)'

# shift the ttbar xsec down (for syst)
root -l -b -q estimateBkg.cc+'("mass",-1)'
root -l -b -q estimateBkg.cc+'("mass3bins",-1)'
root -l -b -q estimateBkg.cc+'("pt",-1)'
root -l -b -q estimateBkg.cc+'("pt1560",-1)'
root -l -b -q estimateBkg.cc+'("rap1560",-1)'
root -l -b -q estimateBkg.cc+'("rap60120",-1)'
root -l -b -q estimateBkg.cc+'("phistar",-1)'
root -l -b -q estimateBkg.cc+'("phistar1560",-1)'

# variation of the RR value
root -l -b -q estimateBkg.cc+'("mass",0,1)'
root -l -b -q estimateBkg.cc+'("mass3bins",0,1)'
root -l -b -q estimateBkg.cc+'("pt",0,1)'
root -l -b -q estimateBkg.cc+'("pt1560",0,1)'
root -l -b -q estimateBkg.cc+'("rap1560",0,1)'
root -l -b -q estimateBkg.cc+'("rap60120",0,1)'
root -l -b -q estimateBkg.cc+'("phistar",0,1)'
root -l -b -q estimateBkg.cc+'("phistar1560",0,1)'

# nominal
root -l -b -q estimateBkg.cc+'("mass")'
root -l -b -q estimateBkg.cc+'("mass3bins")'
root -l -b -q estimateBkg.cc+'("pt")'
root -l -b -q estimateBkg.cc+'("pt1560")'
root -l -b -q estimateBkg.cc+'("rap1560")'
root -l -b -q estimateBkg.cc+'("rap60120")'
root -l -b -q estimateBkg.cc+'("phistar")'
root -l -b -q estimateBkg.cc+'("phistar1560")'

root -l -b -q drawplots.C'("mass")'
root -l -b -q drawplots.C'("mass3bins")'
root -l -b -q drawplots.C'("pt")'
root -l -b -q drawplots.C'("pt1560")'
root -l -b -q drawplots.C'("rap1560")'
root -l -b -q drawplots.C'("rap60120")'
root -l -b -q drawplots.C'("phistar")'
root -l -b -q drawplots.C'("phistar1560")'

