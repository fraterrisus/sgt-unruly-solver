TIMES=1000

for BOARD in "8x8de" "8x8dn" "10x10de" "10x10dn" "14x14de" "14x14dn"
do
  rm -f times
  i=0
  while
    [[ $i -lt ${TIMES} ]]
  do
    i=$((i+1))
    sgt-unruly --generate 1 ${BOARD} | ./solver | tail -1 | cut -d' ' -f 3 >> times
  done
  echo -n "${BOARD}: "
  paste -s -d+ times | bc
done
