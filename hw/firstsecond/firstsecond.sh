rm -r /tmp/fifo_one /tmp/fifo_two

gcc first.c -o first
gcc second.c -o second

konsole  -e ./second &
konsole  -e ./first &
