#include<stdio.h>

void help(){

  printf("This is a bit of a placeholder\n");
  return;

}

void printip(int ip){

  // takes a 32 bit integer and prints it as 4 decimal separate octects

  unsigned char bytes[4];

  bytes[0] = ip & 0xFF; // take first 8 bits AND it with 0xFF to pad the byte
  bytes[1] = (ip >> 8) & 0xFF; // right shift
  bytes[2] = (ip >> 16) & 0xFF;
  bytes[3] = (ip >> 24) & 0xFF;

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
  return (network & netmask) + (~netmask);
}

unsigned int count_set_bits(unsigned int network){

  unsigned int v; // count the number of bits set in v
  unsigned int c; // c accumulates the total bits set in v

  for (c = 0; v; c++)
  {
    v &= v - 1; // clear the least significant bit set
  }

  return v;

}

int main(int argc, char ** argv){

  unsigned int netmask = 0;
  unsigned int cidrValue = 0;
  unsigned int network_addr = 0;
  unsigned int i_address = 0;
  unsigned int firstHost = 0;
  unsigned int lastHost = 0;
  unsigned int broadcast = 0;

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
      cidrValue = count_set_bits(netmask);
    }

    network_addr = calc_network_address(i_address, netmask);

    firstHost = network_addr + 1;
    broadcast = calc_broadcast(i_address, netmask);
    lastHost = broadcast - 1;

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
  }

  return 0;
}
