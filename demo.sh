#!/bin/bash

if [ ! -d 'results' ]; then
    mkdir 'results'
fi

echo $#

if [ $# -ge 2 ]; then
    V=${1}
    E=${2}
else
    V='10'
    E='20'
fi

if [ $# -ge 3 ]; then
    input='results/input'${3}'.txt'
    apout='results/apasp'${3}'.txt'
    fwout='results/floyd'${3}'.txt'
else
    input='results/input.txt'
    apout='results/apasp.txt'
    fwout='results/floyd.txt'
fi

echo ''
echo 'V='${V}', E='${E}
echo -n 'Gererate a new graph at '${input}'...'
rm ${input}
./gen ${V} ${E} >${input}
echo ''
echo ''

echo -n 'Run APASP at '${apout}'...'
rm ${apout}
time ./apasp <${input} >${apout}
echo ''

echo -n 'Run baseline(Floyd-Warshall) at '${fwout}'...'
rm ${fwout}
time ./FW <${input} >${fwout}
echo ''

echo 'Validation check...'
python checker.py ${fwout} ${apout}
echo