if grep -q clwb /proc/cpuinfo
then echo "clwb" | tr -d '\n'
elif grep -q clflushopt /proc/cpuinfo
then echo "clflushopt" | tr -d '\n'
else echo "clflush" | tr -d '\n'
fi
