Guide for FR code set

0. option information : opt20_QCDopt2_histnew

1. Updated files in interface directory
samples_v20180814.h
fakerate.h
defs.h

2. estimateFR
- modified files
estimateFR/Auto_estimateFR_opttest_20180814.sh
estimateFR/Auto_fitTemplates_opttest_20180814.sh
estimateFR/Auto_selectDenAndNumForFR_opttest_new_20180814.sh
estimateFR/estimateFR_opttest_v2.cc
estimateFR/fitTemplates_opttest.cc
estimateFR/selectDenAndNumForFR_opttest.cc

- how to run
./Auto_selectDenAndNumForFR_opttest_new_20180814.sh
./Auto_fitTemplates_opttest_20180814.sh
./Auto_estimateFR_opttest_20180814.sh

3. applyFR
- modified files
Auto_applyFR_20180822.sh
applyFR_20180822.cc
Auto_estimateDijet_opt_20180822.sh
Auto_estimateWjets_opt_20180822.sh
estimateDijet_opt.cc
estimateWjets_opt.cc

- how to run
./Auto_applyFR_20180822.sh
./Auto_estimateDijet_opt_20180822.sh
./Auto_estimateWjets_opt_20180822.sh

- resulted files
applyFR/print_20180822
applyFR/result_20180822
