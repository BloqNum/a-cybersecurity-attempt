#include "functions.h"

int main(){
    struct tpacket_req3 req;
    struct tpacket_block_desc *pbd = NULL;
    struct ring ring;
    struct pollfd pfd;
    int sockfd;
    int v = TPACKET_V3;
    int block_num = 100;
    int c_block = 0;

    sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(sockfd < 0)
        perror("socket error");

    memset(&req, 0, sizeof(struct tpacket_req3));

    req.tp_block_size = 4096;
    req.tp_frame_size = 1024;
    req.tp_block_nr = block_num;
    req.tp_frame_nr = block_num*4; 

    if(setsockopt(sockfd, SOL_PACKET, PACKET_VERSION, &v, sizeof(v)) < 0)
        perror("setsockopt error");

    if(setsockopt(sockfd, SOL_PACKET, PACKET_RX_RING, &req, sizeof(req)) < 0)
        perror("setsockopt error");

    unsigned int total_size = req.tp_block_size * req.tp_block_nr;

    if((ring.map = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_SHARED, sockfd, 0)) == MAP_FAILED){
        perror("mmap error");
    }
    
    ring.rd = calloc(block_num, sizeof(struct iovec));
    for(int i = 0; i < block_num; i++){
        ring.rd[i].iov_base = ring.map + (i * req.tp_block_size);
        ring.rd[i].iov_len = req.tp_block_size;
    }

    memset(&pfd, 0, sizeof(pfd));

    pfd.fd = sockfd;
    pfd.events = POLLIN;
    pfd.revents = 0;

    while(1){
        if(c_block >= block_num){
            c_block = 0;
        }

        pbd = (struct tpacket_block_desc *) ring.rd[c_block].iov_base;

        if((pbd->hdr.bh1.block_status & TP_STATUS_USER) == 0){
            poll(&pfd, 1, -1);
            continue;
        }

        walk_block(pbd);
        pbd->hdr.bh1.block_status = TP_STATUS_KERNEL;
        c_block++;

    }
    
    return 0;
}
