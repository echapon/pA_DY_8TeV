#!/bin/bash

#root -l -b -q 'fitTemplates.cc++("_pt","p_{T}",15.0,500.0)'
#root -l -b -q 'fitTemplates.cc++("_pt_barrel","p_{T}",15.0,500.0)'
#root -l -b -q 'fitTemplates.cc++("_pt_endcap","p_{T}",15.0,500.0)'
#root -l -b -q 'fitTemplates.cc++("_eta","#eta",-2.5,2.5)'
root -l -b -q 'fitTemplates.cc++("","PFIso/p_{T}",0.0,1.0)'
root -l -b -q 'fitTemplates.cc++("_barrel","PFIso/p_{T}",0.0,1.0)'
root -l -b -q 'fitTemplates.cc++("_endcap","PFIso/p_{T}",0.0,1.0)'

exit 0

