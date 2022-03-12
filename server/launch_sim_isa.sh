shopt -s expand_aliases
source ~/.bashrc
./launch_sims > /dev/null 2>&1 &
disown -h %1
./launch_isa &
disown -h %1
