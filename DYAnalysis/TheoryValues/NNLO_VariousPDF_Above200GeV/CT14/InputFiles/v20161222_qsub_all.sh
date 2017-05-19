#!/bin/bash
qsub -N M200to220_DY b_v20161222_M200to220_DY_NNLO_CT14nnlo.sh
qsub -N M220to243_DY b_v20161222_M220to243_DY_NNLO_CT14nnlo.sh
qsub -N M243to273_DY b_v20161222_M243to273_DY_NNLO_CT14nnlo.sh
qsub -N M273to320_DY b_v20161222_M273to320_DY_NNLO_CT14nnlo.sh
qsub -N M320to380_DY b_v20161222_M320to380_DY_NNLO_CT14nnlo.sh
qsub -N M380to440_DY b_v20161222_M380to440_DY_NNLO_CT14nnlo.sh
qsub -N M440to510_DY b_v20161222_M440to510_DY_NNLO_CT14nnlo.sh
qsub -N M510to600_DY b_v20161222_M510to600_DY_NNLO_CT14nnlo.sh
qsub -N M600to700_DY b_v20161222_M600to700_DY_NNLO_CT14nnlo.sh
qsub -N M700to830_DY b_v20161222_M700to830_DY_NNLO_CT14nnlo.sh
qsub -N M830to1000_DY b_v20161222_M830to1000_DY_NNLO_CT14nnlo.sh
qsub -N M1000to1200_DY b_v20161222_M1000to1200_DY_NNLO_CT14nnlo.sh
qsub -N M1200to1500_DY b_v20161222_M1200to1500_DY_NNLO_CT14nnlo.sh
qsub -N M1500to2000_DY b_v20161222_M1500to2000_DY_NNLO_CT14nnlo.sh
qsub -N M2000to3000_DY b_v20161222_M2000to3000_DY_NNLO_CT14nnlo.sh

echo "job submission is finished"

