export PATH=/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/bin:$PATH
export LD_LIBRARY_PATH=/cvmfs/cms.cern.ch/slc6_amd64_gcc530/external/lhapdf/6.1.6-ikhhed/lib/:$LD_LIBRARY_PATH
export LHAPDF_DATA_PATH=/afs/cern.ch/work/e/echapon/public/xFitter/deps/lhapdf/share/LHAPDF/
# source /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.8/x86_64-slc6/setup.sh
# source /afs/cern.ch/sw/lcg/external/gcc/4.8/x86_64-slc6-gcc48-opt/setup.sh
# source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.04.18/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
. /cvmfs/sft.cern.ch/lcg/contrib/gcc/4.9/x86_64-slc6-gcc49-opt/setup.sh
. /cvmfs/sft.cern.ch/lcg/app/releases/ROOT/6.06.08/x86_64-slc6-gcc49-opt/root/bin/thisroot.sh

echo "setup done for gcc, ROOT, LHAPDF"
echo "WARNING this setup works but does not allow to compile / run macros needing LHAPDF"
