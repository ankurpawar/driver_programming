#ifndef FILEOPR_H
#define FILEOPR_H

struct file_operations fops=
{
	open:open_dev,
	release:close_dev,
	write:write_dev,
};

#endif
