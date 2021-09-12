#!/bin/bash
for p in {1..5}
    do
        echo "Processes count: $p"
        mpiexec -np $p --oversubscribe ./a.out
    done
