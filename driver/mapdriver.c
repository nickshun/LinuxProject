/****
 * mapdriver.c
 
 * The mapdriver character device driver implementation
 *
 * CREDITS:
 *   o Many parts of the driver code has to be credited to
 *     Ori Pomerantz, in his chardev.c (Copyright (C) 1998-1999)
 *
 *     Source:  The Linux Kernel Module Programming Guide (specifically,
 *              http://www.tldp.org/LDP/lkmpg/2.6/html/index.html)
 */
#include "mapdriver.h"

/* Driver's Status is kept here */
static driver_status_t status =
{
        '0',   /* Starting ASCII char is '0' */
        false, /* Not busy at the beginning */
        {0},   /* buffer */
        NULL,  /* buffer's ptr */
        -1,    /* major */
        -1     /* minor */
};

/* This function is called whenever a process
 * attempts to open the device file
 */
static int device_open(inode, file)
	struct inode* inode;
	struct file*  file;
{
	static int counter = 0;
#ifdef _DEBUG
	printk("device_open(%p,%p)\n", inode, file);
#endif

	/* This is how you get the minor device number in
	 * case you have more than one physical device using
	 * the driver.
	 */
	status.minor = inode->i_rdev >> 8;
	status.minor = inode->i_rdev & 0xFF;

	printk
	(
		"Device: %d.%d, busy: %d\n",
		status.major,
		status.minor,
		status.busy
	);

	/* We don't want to talk to two processes at the
	 * same time
	 */
	if(status.busy)
		return -EBUSY;

	/* If this was a process, we would have had to be
	 * more careful here.
	 *
	 * In the case of processes, the danger would be
	 * that one process might have check busy
	 * and then be replaced by the schedualer by another
	 * process which runs this function. Then, when the
	 * first process was back on the CPU, it would assume
	 * the device is still not open.
	 *
	 * However, Linux guarantees that a process won't be
	 * replaced while it is running in kernel context.
	 *
	 * In the case of SMP, one CPU might increment
	 * busy while another CPU is here, right after
	 * the check. However, in version 2.0 of the
	 * kernel this is not a problem because there's a lock
	 * to guarantee only one CPU will be kernel module at
	 * the same time. This is bad in  terms of
	 * performance, so version 2.2 changed it.
	 */

	status.busy = true;

	/* Initialize the message. */
	sprintf
	(
		status.buf,
		"If I told you once, I told you %d times - %s",
		counter++,
		"Hello, world\n"
	);

	/* The only reason we're allowed to do this sprintf
	 * is because the maximum length of the message
	 * (assuming 32 bit integers - up to 10 digits
	 * with the minus sign) is less than DRV_BUF_SIZE, which
	 * is 80. BE CAREFUL NOT TO OVERFLOW BUFFERS,
	 * ESPECIALLY IN THE KERNEL!!!
	 */

	status.buf_ptr = status.buf;

	return SUCCESS;
}


/* This function is called when a process closes the
 * device file.
 */
static int device_release(inode, file)
	struct inode* inode;
	struct file*  file;
{
#ifdef _DEBUG
	printk ("device_release(%p,%p)\n", inode, file);
#endif

	/* We're now ready for our next caller */
	status.busy = false;

	return SUCCESS;
}

/* This function is called whenever a process which
 * have already opened the device file attempts to
 * read from it.
 */
static ssize_t device_read(file, buffer, length, offset)
	struct file* file;
    char*        buffer;  /* The buffer to fill with data */
    size_t       length;  /* The length of the buffer */
    loff_t*     offset;  /* Our offset in the file */
{	
	int lengthLeft = length;
	
	if (*offset >= size)
        {
                return 0;
        }

        if (*offset + length > size)
        {
                length = size - *offset;
        }
		
	lengthLeft = length;
	
	while(lengthLeft > 0)
	{
		if (status.mapBuffer[*offset] != 0)
		{			
			copy_to_user(buffer + (length - lengthLeft), status.mapBuffer + *offset, 1);
			*offset += 1;
			lengthLeft--;
		}
		else
		{	
			length -= lengthLeft;
			lengthLeft = 0;
		}
	}	

	return length;
}


/* This function is called when somebody tries to write
 * into our device file.
 */
static ssize_t device_write(file, buffer, length, offset)
	struct file* file;
	const char*  buffer;  /* The buffer */
	size_t       length;  /* The length of the buffer */
	loff_t*      offset;  /* Our offset in the file */
{
	if (*offset >= size)
        {
                return 0;       
        }
        
        if (*offset + length >= size)
        {
                length = size - *offset;
        }
	
	copy_from_user(status.mapBuffer + *offset, buffer, length);
	
	*offset += length;
	
	return length;      
}

static loff_t device_lseek(struct file* file, loff_t offset, int orig)
{
	loff_t newPos = 0;
	
	switch(orig)
	{
	case 0:
		newPos = offset;
		break;
	case 1:
		newPos = file->f_pos + offset;
		break;
	case 2:
		newPos = size - offset;
		break;
	}
	
	if (newPos > size)
	{
		newPos = size;
	}
	if (newPos < 0)
	{
		newPos = 0;
	}
	
	file->f_pos = newPos;
	return newPos;
}

int device_ioctl(struct inode* inode, 
		struct file* file, 
		unsigned int ioctl_num, 
		unsigned long ioctl_param)
{
	
	switch(ioctl_num)
	{
	case IOCTL_RESET:
		printk("Ressetting\n");
		break;
	case IOCTL_ZERO:
		printk("Zeroing\n");
		break;
	case IOCTL_VALIDATE:
		printk("Validating\n");
		break;
	}
	
	return SUCCESS;
}



/* Initialize the module - Register the character device */
int
init_module(void)
{
	int i;	
	/* Register the character device (atleast try) */
	if ( register_chrdev
	(
		MAJOR_NUM,
		DEVICE_NAME,
		&Fops
	) == 0)
	{
		status.major = MAJOR_NUM;
	}
	else
	{
		status.major = -1;
	}
	printk("device major num: %d\n\n", status.major);
	/* Negative values signify an error */
	if(status.major < 0)
	{
		printk
		(
			"Sorry, registering the ASCII device failed with %d\n",
			status.major
		);

		return status.major;
	}

	printk
	(
		"Registeration is a success. The major device number is %d.\n",
		status.major
	);

	printk
	(
		"If you want to talk to the device driver,\n" \
		"you'll have to create a device file. \n" \
		"We suggest you use:\n\n" \
		"mknod %s c %d <minor>\n\n" \
		"You can try different minor numbers and see what happens.\n",
		DEVICE_NAME,
		status.major
	);
	
	memset(status.mapBuffer, 0, sizeof(status.mapBuffer));
	
	strncpy(status.mapBuffer, static_buffer, sizeof(static_buffer) - 1);

	return SUCCESS;
}


/* Cleanup - unregister the appropriate file from /proc */
void
cleanup_module(void)
{
	unregister_chrdev(status.major, DEVICE_NAME);
}

/* EOF */
