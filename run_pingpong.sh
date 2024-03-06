NUM_EXCHANGES=100
OUTPUT_REPORT_FILE="results/part1_pingpong.csv"
PINGPONG_EXECUTABLE="bin/ping_pong"

# Remove earlier logs 
rm $OUTPUT_REPORT_FILE

# Create an array of message msg_sizes
MSG_SIZES=(1 2 4 8 16 32 48 64 96 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216 33554432 67108864 134217728)

for MSG_SIZE in ${MSG_SIZES[@]}
do
    echo "Running pingpong for message size: $MSG_SIZE"
    mpiexec -np 2 $PINGPONG_EXECUTABLE $MSG_SIZE $NUM_EXCHANGES $OUTPUT_REPORT_FILE
done