root -l -b << EOF

gSystem->Load("/home/kplee/Unfolding/libRooUnfold.so")

.x SysUnc_BinningChoice.C++("v20160708_1st_ApplyAdditionalSF")

.q

EOF
