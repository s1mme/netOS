typedef unsigned int u32int;
typedef int			 s32int;
typedef int			 size_t;
typedef unsigned short u16int;
typedef unsigned char u8int;
typedef char		s8int;
typedef unsigned long       uint32_t;

typedef uint32_t            uintptr_t;

 u8int network_buffer[0x2000]; // WRAP set?
 uintptr_t network_bufferPointer;
 u8int buffer[0x2000];

 void bufferpointer();			//access from asm

void *memcpy(void *dest,const void *src,size_t n) { //optimized memcpy
  u32int num_dwords = n/4;
  u32int num_bytes = n%4;
  u32int *dest32 = (u32int*)dest;
  u32int *src32 = (u32int*)src;
  u8int *dest8 = ((u8int*)dest)+num_dwords*4;
  u8int *src8 = ((u8int*)src)+num_dwords*4;
  u32int i;

  for (i=0;i<num_dwords;i++) {
    dest32[i] = src32[i];
  }
  for (i=0;i<num_bytes;i++) {
    dest8[i] = src8[i];
  }
  return dest;
}


void bufferpointer()
{
	u32int length = (network_buffer[network_bufferPointer+3] << 8) + network_buffer[network_bufferPointer+2]; // Little Endian
 	

 	
	memcpy(buffer, &network_buffer[network_bufferPointer], length);
 	
 	// packets are DWORD aligned
 	network_bufferPointer = network_bufferPointer +length + 4;
 	network_bufferPointer = (network_bufferPointer + 3) & ~0x3;
 	
	// handle wrap-around
	network_bufferPointer %= 0x2000;
		
 	// set read pointer
 	*((u16int*)(0x145000 + 0x38)) = network_bufferPointer - 0x10; 
	
}
