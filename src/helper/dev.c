#include <string.h>
#include <stdio.h>
#include "helper/dev.h"

void dev_pup_stach(vm_info_t *vm)
{
	char outfile[1024] = {0};
	sprintf(outfile, "%s/stack.txt", DEV_OUTPUT_DIR);

	FILE *fd = fopen(outfile, "w+");
	fwrite(vm->cpu->mem->buffer, 1, vm->cpu->mem->memSize, fd);
	fclose(fd);
}