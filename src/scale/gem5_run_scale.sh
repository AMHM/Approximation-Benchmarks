#!/bin/bash
echo "Starting scale..."
source $GEM5_DIR/set_gem5_path.sh

BENCHMARK=scale
BENCHMARK_DIR=$BENCHMARK_ROOT/$BENCHMARK

IMAGES_SET=(west lena sailboat) # sign house tree building f16 fruits girl boat max baboon)
SCALEFACTOR=2

VDD_LEVELS=(600 560 520 480 440)
AFB_LEVELS=(0 1 2)
TRIES=1

rm -f -r $BENCHMARK_DIR/runs/*
rm -f -r $BENCHMARK_DIR/outputs/*

for IMAGE in "${IMAGES_SET[@]}"
do
    #----------------------------------------------------------------------NATIVE EXECUTION----------------------------------------------------------------------------
    mkdir -p $BENCHMARK_DIR/runs/$IMAGE-native
    $BENCHMARK_DIR/bin/x86/$BENCHMARK $SCALEFACTOR $BENCHMARK_DIR/inputs/$IMAGE.ppm $BENCHMARK_DIR/outputs/$IMAGE-X$SCALEFACTOR-native.ppm > \
        $BENCHMARK_DIR/runs/$IMAGE-native/$IMAGE-native.out &
    #---------------------------------------------------------------------VANILLA EXECUTION----------------------------------------------------------------------------
    $GEM5_DIR/run_benchmark.sh $BENCHMARK vanilla 1000 1000 1000 0 $IMAGE-vanilla \
        "$SCALEFACTOR $BENCHMARK_DIR/inputs/$IMAGE.ppm $BENCHMARK_DIR/outputs/$IMAGE-X$SCALEFACTOR-vanilla.ppm" &
    #---------------------------------------------------------------------RELAXED EXECUTION----------------------------------------------------------------------------
    for ((try=1;try<=$TRIES;try++))
    do
        for VDD in "${VDD_LEVELS[@]}"
        do  
            for AFB in "${AFB_LEVELS[@]}"
            do   
                $GEM5_DIR/run_benchmark.sh $BENCHMARK relaxed $VDD 1000 1000 $AFB $IMAGE-$VDD-$AFB-relaxed-$try \
                    "$SCALEFACTOR $BENCHMARK_DIR/inputs/$IMAGE.ppm $BENCHMARK_DIR/outputs/$IMAGE-X$SCALEFACTOR-$VDD-$AFB-relaxed-$try.ppm" &
            done
        done
    done
    wait
    #----------------------------------------------------------------------OUTPUT FIDELITY----------------------------------------------------------------------------
    for ((try=1;try<=$TRIES;try++))
    do
        for VDD in "${VDD_LEVELS[@]}"
        do  
            for AFB in "${AFB_LEVELS[@]}"
            do            
                compare -metric PSNR $BENCHMARK_DIR/outputs/$IMAGE-X$SCALEFACTOR-vanilla.ppm \
                    $BENCHMARK_DIR/outputs/$IMAGE-X$SCALEFACTOR-$VDD-$AFB-relaxed-$try.ppm $BENCHMARK_DIR/outputs/diff-$IMAGE-X$SCALEFACTOR-$VDD-$AFB-relaxed-$try.ppm 2> \
                    $BENCHMARK_DIR/outputs/diff-$IMAGE-X$SCALEFACTOR-$VDD-$AFB-relaxed-$try.log &
            done
        done 
    done    
    wait
    #----------------------------------------------------------------------OUTPUT CONVERSION---------------------------------------------------------------------------
    for ppm_FILE in $BENCHMARK_DIR/outputs/*.ppm; do
        bmp_FILE="${ppm_FILE%.ppm}.bmp"
        convert $ppm_FILE $bmp_FILE
    done
    wait
done

echo "Done with scale..."
exit 1
