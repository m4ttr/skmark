Description
===========
somark is a netfilter target module and iptables extension.
The module sets the corresponding sock->sk_mark from the matching skb.  

Installation
============
somark module uses dkms to build the ipt_SKMARK module. 

## Prerequisites:
```
apt-get install linux-headers-$(uname -r)
apt-get install dkms 
```

## Build
```
dkms build .
dkms install somark/0.1
```

## Iptable extension
```
cd ./extension
sudo make
sudo make install
```

Usage
=====
DSCP or ToS bit reflection

```
# Create a new change to manage setting egress TOS
iptables -N MARK-EGRESS-TOS -t mangle

# Match (0x08)  8 Maximize-Throughput and set sk_mark 0 on the connections socket
iptables -I INPUT -m tos --tos 8 -j SKMARK --set-mark 8

# Restore the socket mark and reset ToS bit.
iptables -t filter −A OUTPUT −m socket −−restore-skmark −j MARK-EGRESS-TOS
iptables -t mangle -I MARK-EGRESS-TOS -m mark --mark 5 -j TOS --set-tos 5 
```
