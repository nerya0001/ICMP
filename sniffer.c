#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <netinet/ip_icmp.h>
#include <netinet/ip.h>
#include <netinet/ether.h>
#include <net/if.h>
#include <net/ethernet.h>

#include <linux/if_packet.h>

#include <sys/socket.h>

#include <arpa/inet.h>

int main()
{

    // declaring variables for later use
    struct sockaddr_in src;
    struct sockaddr_in dest;
    char buffer[IP_MAXPACKET];
    int counter = 1;

    // creating a raw socket
    int sock;
    if ((sock = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) < 0)
    {
        perror("socket");
        exit(1);
    }
    printf("waiting for something to sniff......\n");

    // keep siffing until ctrl + c
    while (true)
    {
        bzero(buffer, IP_MAXPACKET);
        int incoming = recvfrom(sock, buffer, ETH_FRAME_LEN, 0, NULL, NULL);
        if (incoming < 0)
        {
            perror("recvfrom");
            exit(1);
        }
        // we jump to the IP header, ETH_HLEN = 14
        struct iphdr *ip_h = (struct iphdr *)(buffer + ETH_HLEN);

        // in the IP header, it's either 5 or 6 bytes in the length field, so multiply by 4 to get the length in bits
        int ip_h_length = ip_h->ihl * 4;
        if (ip_h->protocol == IPPROTO_ICMP)
        {
            // ICMP header starts after the IP header, so we jump there
            struct icmphdr *icmp_h = (struct icmphdr *)(buffer + ETH_HLEN + ip_h_length);

            // Extract and copy the source and destination data from the ICMP packet
            memset(&src, 0, sizeof(src));
            src.sin_addr.s_addr = ip_h->saddr;
            memset(&dest, 0, sizeof(dest));
            dest.sin_addr.s_addr = ip_h->daddr;

            printf("packet num: #%d\n", counter++);
            printf("from: %s\n", inet_ntoa(src.sin_addr));
            printf("to: %s\n", inet_ntoa(dest.sin_addr));
            printf("type: %d\n", icmp_h->type);
            printf("code: %d\n", icmp_h->code);
        }
    }
    return 0;
}
