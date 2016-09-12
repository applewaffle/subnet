#include<stdio.h>
#include<stdbool.h>

/**
A simple CLI subnet calculator
Copyright (C) 2016 Brandon Scott James

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

**/

void help(){

  printf("Usage: subnet [ip address] {netmask | cidr}\n\
          Example:\t192.168.0.1 255.255.255.0\n\
          \t\t192.168.0.1 /24\n");
  return;

}

void printip(int ip){

  // takes a 32 bit integer and prints it as 4 decimal separate octects

  unsigned char bytes[4];

  // set the byte to the first byte of the ip & 0xFF masks the higher bytes
  bytes[0] = ip;
  bytes[1] = ip >> 8; // right shift by a byte and repeat
  bytes[2] = ip >> 16;
  bytes[3] = ip >> 24;

  printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);

  return;
}

unsigned int dotted_decimal_to_int(char ip[]){

  unsigned char bytes[4] = {0};

  sscanf(ip, "%hhd.%hhd.%hhd.%hhd", &bytes[3], &bytes[2], &bytes[1], &bytes[0]);

  // set 1 byte at a time by left shifting and ORing
  return bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
}

unsigned int cidr_to_mask(unsigned int cidrValue){

  // left shift 1 by 32 - cidr, subtract 1 from the result and XORing
  // it with a mask that has all bits set, yeilds the subnet mask
  return -1 ^ ((1 << (32 - cidrValue)) - 1);
}

unsigned int get_cidr_value(char cidr[]){

  unsigned int cidrValue;

  sscanf(cidr, "/%u", &cidrValue);

  return cidrValue;
}

unsigned int calc_network_address(unsigned int ipaddress, unsigned int netmask){
  return ipaddress & netmask;
}

unsigned int calc_broadcast(unsigned int network, unsigned int netmask){
  return network + (~netmask);
}

unsigned int mask_to_cidr(unsigned int netmask){

  // works by counting the number of bits set in the mask
  // i accumulates the total bits set in v
  unsigned int cidr;

  for (cidr = 0; netmask > 0; cidr++)
  {
    netmask &= (netmask - 1); // clear the least significant bit set
  }

  return cidr;
}

int main(int argc, char ** argv){

  unsigned int netmask = 0;
  unsigned int cidrValue = 0;
  unsigned int network_addr = 0;
  unsigned int i_address = 0;
  unsigned int firstHost = 0;
  unsigned int lastHost = 0;
  unsigned int broadcast = 0;
  unsigned int hostCount = 0;

  if(argc < 3){
    help();
    return 0;
  }else{
    i_address = dotted_decimal_to_int(argv[1]);
    if((char)argv[2][0] == '/'){
      cidrValue = get_cidr_value(argv[2]);
      netmask = cidr_to_mask(cidrValue);
    }else{
      netmask = dotted_decimal_to_int(argv[2]);
      cidrValue = mask_to_cidr(netmask);
    }

    network_addr = calc_network_address(i_address, netmask);

    firstHost = network_addr + 1;
    broadcast = calc_broadcast(network_addr, netmask);
    lastHost = broadcast - 1;
    hostCount = broadcast - firstHost;

  }

  printf("ip address:\t\t");
  printip(i_address);
  printf("subnet:\t\t\t");
  printip(netmask);
  printf("cidr notation:\t\t/%u\n", cidrValue);
  if(cidrValue == 32){
    printf("\n***/32 masks can only be used to describe a single host***\n\n");
  }else if(cidrValue == 31){
    printf("first host:\t\t");
    printip(network_addr);
    printf("last host:\t\t");
    printip(broadcast);
    printf("\n***/31 masks should only be used on point-to-point links***\n\n");
  }else{
    printf("network address:\t");
    printip(network_addr);
    printf("first host:\t\t");
    printip(firstHost);
    printf("last host:\t\t");
    printip(lastHost);
    printf("broadcast:\t\t");
    printip(broadcast);
    printf("hostcount:\t\t%d\n", hostCount);
  }

  return 0;
}
