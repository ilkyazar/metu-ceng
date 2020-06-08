#ifndef _EXT2_H_
#define _EXT2_H_

#include "ext2_fs.h"
#include "vfs/fs.h"
#include <stdio.h>

struct file_system_type myfs;

struct file_system_type *initialize_ext2(const char *image_path);

/** This function assumes that only the inode number field
*   ( i_ino ) of the passed in inode i is valid and the function
*   reads and populates the remaining fields of i.
**/
void my_read_inode(struct inode *i);

/** This function fills in the fields of kstatfs struct stats with
 * the information from the superblock sb . Returns zero for
 * successful operations.
 */
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

/** This function reads the file pointed by inode i and fills the 
*   file struct f. It returns zero for successful operation.
**/
int my_open(struct inode *i, struct file *f);

/** This function is called when a file is closed.  It performs 
*  clean up operations if necessary. Returns zero for successful operation.
**/
int my_release(struct inode *i, struct file *f);

/** This function assumes that only the name field ( d_name ) of the 
*   directory entry dir is valid and searches for it in the directory 
*   pointed by inode i. If a matching directory entry is found, 
*   it fills the rest of the directory entry dir. It returns 
*   the pointer to the filled directory entry.
**/
struct dentry *my_lookup(struct inode *i, struct dentry *dir);

int my_readlink(struct dentry *dir, char *buf, int len);
int my_readdir(struct inode *i, filldir_t callback);

/** This function fills in the fields of kstat structure, 
*   stats with the information from the object pointed
*   by the directory entry dir. It returns zero for successful operation.
**/
int my_getattr(struct dentry *dir, struct kstat *stats);

struct super_block* my_get_superblock(struct file_system_type *current_fs);
#endif
