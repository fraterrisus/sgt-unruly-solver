TIMES=1000

for BOARD in "10x10de" "10x10dn" "14x14de" "14x14dn" "20x20de" "20x20dn"
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
