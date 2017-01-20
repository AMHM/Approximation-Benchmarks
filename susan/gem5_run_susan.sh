#!/bin/bash
echo "Starting susan..."
source $GEM5_DIR/set_gem5_path.sh

BENCHMARK=susan
BENCHMARK_DIR=$BENCHMARK_ROOT/$BENCHMARK

IMAGES_SET=(west lena sailboat) # sign house tree building f16 fruits girl boat max baboon)

VDD_LEVELS=(600 560 520 480 440)
AFB_LEVELS=(0 1 2)
TRIES=1

rm -f -r $BENCHMARK_DIR/runs/*
rm -f -r $BENCHMARK_DIR/outputs/*

for IMAGE in "${IMAGES_SET[@]}"
do
    #----------------------------------------------------------------------NATIVE EXECUTION----------------------------------------------------------------------------
    mkdir -p $BENCHMARK_DIR/runs/$IMAGE-native
        
    $BENCHMARK_DIR/bin/x86/$BENCHMARK $BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-smoothing-native.pgm -s > \
        $BENCHMARK_DIR/runs/$IMAGE-native/$IMAGE-smoothing-native.out &
    $BENCHMARK_DIR/bin/x86/$BENCHMARK $BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-edges-native.pgm -e > \
        $BENCHMARK_DIR/runs/$IMAGE-native/$IMAGE-edges-native.out &
    $BENCHMARK_DIR/bin/x86/$BENCHMARK $BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-corners-native.pgm -c > \
        $BENCHMARK_DIR/runs/$IMAGE-native/$IMAGE-corners-native.out &
    #---------------------------------------------------------------------VANILLA EXECUTION----------------------------------------------------------------------------
    $GEM5_DIR/run_benchmark.sh $BENCHMARK vanilla 1000 1000 1000 0 $IMAGE-smoothing-vanilla \
        "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-smoothing-vanilla.pgm -s" &
    $GEM5_DIR/run_benchmark.sh $BENCHMARK vanilla 1000 1000 1000 0 $IMAGE-edges-vanilla \
        "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-edges-vanilla.pgm -e" &
    $GEM5_DIR/run_benchmark.sh $BENCHMARK vanilla 1000 1000 1000 0 $IMAGE-corners-vanilla \
        "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-corners-vanilla.pgm -c" &
    #---------------------------------------------------------------------RELAXED EXECUTION----------------------------------------------------------------------------
    for ((try=1;try<=$TRIES;try++))
    do
        for VDD in "${VDD_LEVELS[@]}"
        do  
            for AFB in "${AFB_LEVELS[@]}"
            do   
                $GEM5_DIR/run_benchmark.sh $BENCHMARK relaxed $VDD 1000 1000 $AFB $IMAGE-smoothing-$VDD-$AFB-relaxed-$try \
                    "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-smoothing-$VDD-$AFB-relaxed-$try.pgm -s" &
                $GEM5_DIR/run_benchmark.sh $BENCHMARK relaxed $VDD 1000 1000 $AFB $IMAGE-edges-$VDD-$AFB-relaxed-$try \
                    "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-edges-$VDD-$AFB-relaxed-$try.pgm -e" &
                $GEM5_DIR/run_benchmark.sh $BENCHMARK relaxed $VDD 1000 1000 $AFB $IMAGE-corners-$VDD-$AFB-relaxed-$try \
                    "$BENCHMARK_DIR/inputs/$IMAGE.pgm $BENCHMARK_DIR/outputs/$IMAGE-corners-$VDD-$AFB-relaxed-$try.pgm -c" &
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
                compare -metric PSNR $BENCHMARK_DIR/outputs/$IMAGE-smoothing-$VDD-$AFB-relaxed-$try.pgm \
                    $BENCHMARK_DIR/outputs/$IMAGE-smoothing-vanilla.pgm $BENCHMARK_DIR/outputs/diff-$IMAGE-smoothing-$VDD-$AFB-relaxed-$try.pgm 2> \
                    $BENCHMARK_DIR/outputs/diff-$IMAGE-smoothing-$VDD-$AFB-relaxed-$try.log &
                compare -metric PSNR $BENCHMARK_DIR/outputs/$IMAGE-edges-$VDD-$AFB-relaxed-$try.pgm \
                    $BENCHMARK_DIR/outputs/$IMAGE-edges-vanilla.pgm $BENCHMARK_DIR/outputs/diff-$IMAGE-edges-$VDD-$AFB-relaxed-$try.pgm 2> \
                    $BENCHMARK_DIR/outputs/diff-$IMAGE-edges-$VDD-$AFB-relaxed-$try.log &
                compare -metric PSNR $BENCHMARK_DIR/outputs/$IMAGE-corners-$VDD-$AFB-relaxed-$try.pgm \
                    $BENCHMARK_DIR/outputs/$IMAGE-corners-vanilla.pgm $BENCHMARK_DIR/outputs/diff-$IMAGE-corners-$VDD-$AFB-relaxed-$try.pgm 2> \
                    $BENCHMARK_DIR/outputs/diff-$IMAGE-corners-$VDD-$AFB-relaxed-$try.log &
            done
        done 
    done    
    wait
    #----------------------------------------------------------------------OUTPUT CONVERSION---------------------------------------------------------------------------
    for pgm_FILE in $BENCHMARK_DIR/outputs/*.pgm; do
        bmp_FILE="${pgm_FILE%.pgm}.bmp"
        convert $pgm_FILE $bmp_FILE
    done
    wait
done

echo "Done with susan..."
exit 1
