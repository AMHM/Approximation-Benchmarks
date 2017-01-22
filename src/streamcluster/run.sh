#!/bin/sh
outfn=streamcluster_output.txt

#./../../bin/x86/streamcluster 2 5 1 10 10 5 none /../../bin/x86/$outfn 1 #Run fast
./../../bin/x86/streamcluster 10 20 64 8192 8192 1000 none ../../bin/x86/$outfn 1