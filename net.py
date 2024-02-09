"""Host in the middle Mininet setup

┌───────────────┐     ┌───────────────┐      ┌───────────────┐
│               │     │               │      │               │
│               │     │               │      │               │
│   Left Host ◄─┼─────┤► Middle Host ◄├──────┤► Right Host   │
│               │     │               │      │               │
│               │     │               │      │               │
└───────────────┘     └───────────────┘      └───────────────┘

"""

from mininet.topo import Topo
from mininet.net import Mininet
from mininet.cli import CLI

LEFT_HOST_NAME: str = 'lft_h'
MIDDLE_HOST_NAME: str = 'mid_h'
RIGHT_HOST_NAME: str = 'rgt_h'


class HostInTheMiddleTopo(Topo):

    def build(self):

        left_host = self.addHost(LEFT_HOST_NAME)
        middle_host = self.addHost(MIDDLE_HOST_NAME)
        right_host = self.addHost(RIGHT_HOST_NAME)
        self.addLink(left_host, middle_host)
        self.addLink(middle_host, right_host)

def get_interface(host: str, index: int):
    return f"{host}-eth{index}"


def disable_offload(host, offload: str, index: int):
    host.cmd(f"ethtool -K {get_interface(host.name, index)} {offload} off")


def drop_ip(host, index):
    host.cmd(f"ip addr flush dev {get_interface(host.name, index)}")
    host.cmd(f"ip -6 addr flush dev {get_interface(host.name, index)}")


def run():
    net = Mininet(topo=HostInTheMiddleTopo())

    middle_host = net[MIDDLE_HOST_NAME]
    drop_ip(middle_host, 0)
    drop_ip(middle_host, 1)

    for offload in ['tso', 'sg', 'rx', 'tx']:
        for i in [0, 1]:
            disable_offload(middle_host, offload, i)

    left_host = net[LEFT_HOST_NAME]
    disable_offload(left_host, 'rx', 0)
    disable_offload(left_host, 'tx', 0)

    right_host = net[RIGHT_HOST_NAME]
    disable_offload(right_host, 'rx', 0)
    disable_offload(right_host, 'tx', 0)
    
    right_host.cmd(
        f"tcpdump -i rgt_h-eth0 -w outrgt.pcap &")
    left_host.cmd(
        f"tcpdump -i lft_h-eth0 -w outlft.pcap &")
    middle_host.cmd(
        f"tcpdump -i mid_h-eth0 -w outmid0.pcap &")
    middle_host.cmd(
        f"tcpdump -i mid_h-eth1 -w outmid1.pcap &")

    middle_host.cmd(
        f"a/build/pish_man_in_the_middle {get_interface(MIDDLE_HOST_NAME, 0)} {get_interface(MIDDLE_HOST_NAME, 1)} > mmmid.txt &")

    CLI(net)
    

if __name__ == "__main__":
    run()
