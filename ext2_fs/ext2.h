#ifndef _EXT2_H_
#define _EXT2_H_

#include "ext2_fs.h"
#include "vfs/fs.h"
#include <stdio.h>

struct file_system_type myfs;

struct file_system_type *initialize_ext2(const char *image_path);

void my_read_inode(struct inode *i);
int my_statfs(struct super_block *sb, struct kstatfs *stats);

/** This function repositions the offset of the file f to o bytes
*   relative to the beginning of the file, the current file offset, 
*   or the end of the file, depending on whether whence is SEEK_SET, 
*   SEEK_CUR, or SEEK_END, respectively. It returns the resulting file 
*   position in the argument result. 
**/
loffset_t* my_llseek(struct file *f, loffset_t o, int whence);

/** This function reads the contents of file f into the buffer buf 
*   provided by the user. The read operation starts from the o byte
*   of the file and reads len bytes. It returns the number of bytes read.
**/
ssize_t my_read(struct file *f, char *buf, size_t len, loffset_t *o);

int my_open(struct inode *i, struct file *f);
int my_release(struct inode *i, struct file *f);

struct dentry *my_lookup(struct inode *i, struct dentry *dir);
int my_readlink(struct dentry *dir, char *buf, int len);
int my_readdir(struct inode *i, filldir_t callback);
int my_getattr(struct dentry *dir, struct kstat *stats);

struct super_block* my_get_superblock(struct file_system_type *current_fs);
#endif
