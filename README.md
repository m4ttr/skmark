Description
===========
skmark is a netfilter target module and iptables extension.
The module sets the corresponding sock->sk_mark from the matching skb.  

Installation
============
skmark module uses dkms to build the ipt_SKMARK module. 

## Prerequisites:
Debian:
```
apt-get install linux-headers-$(uname -r)
apt-get install dkms 
apt-get install iptables-dev
```
Redhat:
```
yum install kernel-devel
yum install iptables-devel
yum install dkms
```
## Build
```
dkms build .
dkms install skmark/0.3
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
iptables -t mangle -I MARK-EGRESS-TOS -m mark --mark 8 -j TOS --set-tos 8
```

Multi-nic Routing 
```
iptables -I INPUT -i eth1 -j SKMARK --set-mark 100
iptables -t filter −A OUTPUT −m socket −−restore-skmark −j ACCEPT

ip rule add fwmark 100 table TO_ETH1
ip route add default <DEFAULT_GATEWAY> dev eth1 table TO_ETH1

# You may need to turn off rp_filter
```
