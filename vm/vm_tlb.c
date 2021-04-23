// code for manipulating the tlb (including replacement)

#include <types.h>
#include <kern/errno.h>
#include <lib.h>
#include <spl.h>
#include <cpu.h>
#include <spinlock.h>
#include <proc.h>
#include <current.h>
#include <mips/tlb.h>
#include <addrspace.h>
#include <vm.h>
#include <vm_tlb.h>

int 
tlb_get_rr_victim(void){
int victim;
static unsigned int next_victim=0;

victim=next_victim;
next_victim=(next_victim+1)%NUM_TLB;
return victim;
}

void 
insert_in_tlb (vaddr_t faultaddress,  paddr_t paddr) {
	uint32_t ehi, elo;
	int spl;
	int i;
 	spl = splhigh();
  	
	i=tlb_get_rr_victim();

          tlb_read(&ehi, &elo, i);

          ehi = faultaddress;
          elo = paddr | TLBLO_DIRTY | TLBLO_VALID;
          DEBUG(DB_VM, "dumbvm: entry:%d 0x%x -> 0x%x\n",i, faultaddress, paddr);
          tlb_write(ehi, elo, i);
          splx(spl);
}


