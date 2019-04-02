for i in `seq 220 250`; do
   root -l -b -q -t pythia8.C'('${i}')'
done
