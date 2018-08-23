#!/bin/bash


#root -l -b -q 'fitTemplates_opttest.cc++("","PFIso/p_{T}",0.0,1.0,20,1,"new","")'
#root -l -b -q 'fitTemplates_opttest.cc++("_barrel","PFIso/p_{T}",0.0,1.0,20,1,"new","")'
#root -l -b -q 'fitTemplates_opttest.cc++("_endcap","PFIso/p_{T}",0.0,1.0,20,1,"new","")'

#root -l -b -q 'fitTemplates_opttest.cc++("","PFIso/p_{T}",0.0,1.0,20,2,"new","")'
#root -l -b -q 'fitTemplates_opttest.cc++("_barrel","PFIso/p_{T}",0.0,1.0,20,2,"new","")'
#root -l -b -q 'fitTemplates_opttest.cc++("_endcap","PFIso/p_{T}",0.0,1.0,20,2,"new","")'

root -l -b -q 'fitTemplates_opttest.cc++("","PFIso/p_{T}",0.0,1.0,20,1,"nominal","")'
root -l -b -q 'fitTemplates_opttest.cc++("_barrel","PFIso/p_{T}",0.0,1.0,20,1,"nominal","")'
root -l -b -q 'fitTemplates_opttest.cc++("_endcap","PFIso/p_{T}",0.0,1.0,20,1,"nominal","")'

root -l -b -q 'fitTemplates_opttest.cc++("","PFIso/p_{T}",0.0,1.0,20,2,"nominal","")'
root -l -b -q 'fitTemplates_opttest.cc++("_barrel","PFIso/p_{T}",0.0,1.0,20,2,"nominal","")'
root -l -b -q 'fitTemplates_opttest.cc++("_endcap","PFIso/p_{T}",0.0,1.0,20,2,"nominal","")'


exit 0

