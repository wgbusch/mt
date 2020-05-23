#!/bin/bash

for i in {0..9}
do 
    echo " Corriendo test ATP_201$i con CMM "
    ./tp ./Tests_ATP/ATP_201$i.in ./Tests_ATP/ATP_201$i-cmm.out 0
    echo " Corriendo test ATP_201$i con WP "
    ./tp ./Tests_ATP/ATP_201$i.in ./Tests_ATP/ATP_201$i-wp.out 1
    echo " Corriendo test ATP_201$i con ELO "
    ./tp ./Tests_ATP/ATP_201$i.in ./Tests_ATP/ATP_201$i-elo.out 2
done
