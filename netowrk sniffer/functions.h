#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <poll.h>
#include <linux/ip.h>
#include <arpa/inet.h>
#include <stdlib.h>

struct ring{
    struct iovec *rd;
    uint8_t *map;
    struct tpacket_req3 req;
};

void walk_block(struct tpacket_block_desc *pbd);

void parser(struct tpacket3_hdr *ppd);

static char *get_prot(uint8_t prot);

static char *get_ver(uint8_t ver);
