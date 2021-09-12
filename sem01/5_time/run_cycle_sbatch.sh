#!/bin/bash
module add mpi/openmpi4-x86_64
for p in {1..5}
    do
        echo "Processes count: $p"
        sbatch -n $p --comment="Run cycle time task" run.sh
    done
