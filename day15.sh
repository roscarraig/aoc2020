#!/bin/bash

datafile=$1
pos=1
last=-1

rm $datafile
touch $datafile

stow() {
  last=0
  if egrep -q "^$1:" $datafile ; then
    res=`egrep "^$1:" $datafile | cut -f2 -d:`
    let last=$pos-$res
    sed -i "/^$1:.*/d" $datafile
  fi
  echo $1:$2 >> $datafile
}

for x in `echo $2 | sed -e 's/,/ /g'`; do
  stow $x $pos
  let pos+=1
done

while [[ $pos -le 2020 ]]; do
  stow $last $pos
  let pos+=1
done
echo -n "Part 1: "
egrep ":2020$" $datafile | cut -f1 -d:
