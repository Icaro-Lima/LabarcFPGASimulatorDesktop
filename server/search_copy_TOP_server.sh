shopt -s expand_aliases
source ~/.bashrc
./search_copy_TOP_server &
disown -h %1
