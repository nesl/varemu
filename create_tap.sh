tunctl -t tap$1
brctl addif br0 tap$1
ip link set dev tap$1 up
