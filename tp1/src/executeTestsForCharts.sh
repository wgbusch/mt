#!/bin/bash
echo "Corriendo test CMM para test prob 1"
./tp ./tests/test-prob-1.in ./tests/testprob1chartcmm.out 0
echo " Corriendo test wp para test prob 1"
./tp ./tests/test-prob-1.in ./tests/testprob1chartwp.out 1

echo " Corriendo test wpl para test prob 1"
./tp ./tests/test-prob-1.in ./tests/testprob1chartwpl.out 3

echo "Corriendo test CMM para test prob 2"
./tp ./tests/test-prob-2.in ./tests/testprob2chartcmm.out 0
echo " Corriendo test wp para test prob 2"
./tp ./tests/test-prob-2.in ./tests/testprob2chartwp.out 1

echo " Corriendo test wpl para test prob 2"
./tp ./tests/test-prob-2.in ./tests/testprob2chartwpl.out 3

echo "Corriendo test CMM para test 1"
./tp ./tests/test1.in ./tests/test1chartcmm.out 0
echo " Corriendo test wp para test 1"
./tp ./tests/test1.in ./tests/test1chartwp.out 1

echo "Corriendo test CMM para test 2"
./tp ./tests/test2.in ./tests/test2chartcmm.out 0
echo " Corriendo test wp para test 2"
./tp ./tests/test2.in ./tests/test2chartwp.out 1


