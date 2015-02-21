#ifndef FILEOPR_H
#define FILEOPR_H

struct file_operations fops=
{
	open:open_dev,
	release:close_dev,
	write:write_dev,
	read:read_dev,
	llseek:llseek_dev,
	unlocked_ioctl:ioctl_dev
};

#endif
