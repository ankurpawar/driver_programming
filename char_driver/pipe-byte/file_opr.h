#ifndef FILEOPR_H
#define FILEOPR_H

struct file_operations fops=
{
	open:open_pipe,
	release:close_pipe,
	write:write_pipe,
	read:read_pipe,
};

#endif
