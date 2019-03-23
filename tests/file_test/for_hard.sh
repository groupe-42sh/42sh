for a in `seq 1 10`;
do
    for b in `seq 1 10`;
    do
        for c in `seq 1 10`;
        do
            echo a
        done
    done
    for z in `seq 1 $b`;
    do
        echo a
    done
done