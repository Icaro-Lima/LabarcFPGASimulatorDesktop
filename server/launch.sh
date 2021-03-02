source /usr/local/altera/altera.sh
n=$(jtagconfig | grep ^.\) | tail -1 | cut -d\) -f1)
for i in $(seq 0 $(expr $n - 1)); do
  ./launch $i &
  disown -h %1
done
