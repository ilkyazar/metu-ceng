#include "ext2.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BASE_OFFSET 1024 /* location of the superblock in the first group */

struct super_operations s_op;
struct inode_operations i_op;
struct file_operations f_op;

char fs_name[] = "ext2";

/* Implement functions in s_op, i_op, f_op here */

struct file_system_type *initialize_ext2(const char *image_path) {
  /* fill super_operations s_op */
  /* fill inode_operations i_op */
  /* fill file_operations f_op */
  
  /* for example:
      s_op = (struct super_operations){
        .read_inode = your_read_inode_function,
        .statfs = your_statfs_function,
      };
  */

  s_op = (struct super_operations) {
        .read_inode = my_read_inode,
        .statfs = my_statfs,
      };

  f_op = (struct file_operations) {
        .llseek = my_llseek,
        .read = my_read,
        .open = my_open,
        .release = my_release,
      };

  i_op = (struct inode_operations) {
        .lookup = my_lookup,
        .readlink = my_readlink,
        .readdir = my_readdir,
        .getattr = my_getattr,
  };

  myfs.name = fs_name;
  myfs.file_descriptor = open(image_path, O_RDONLY);

  /* assign get_superblock function
     for example:
        myfs.get_superblock = your_get_superblock;
  */
  
  myfs.get_superblock = my_get_superblock;

  return &myfs;
}

// s_op
void my_read_inode(struct inode *i) {
    struct super_block* super_block;
    
}

int my_statfs(struct super_block *sb, struct kstatfs *stats) {

}

// f_op

loffset_t* my_llseek(struct file *f, loffset_t o, int whence) {
    loffset_t file_position;

    switch(whence) {
      case 0: /* SEEK_SET */
        file_position = o;
        break;

      case 1: /* SEEK_CUR */
        file_position = f->f_pos + o;
        break;

      case 2: /* SEEK_END */
        file_position = f->f_inode->i_size + o;
        break;

      default:
        return -1;
    }

    if (file_position > 0) {
      f->f_pos = file_position;
      return file_position;
    }
    else {
      return -1;
    }

}

ssize_t my_read(struct file *f, char *buf, size_t len, loffset_t *o) {

    // For example, consider our file->f_pos was 100, and the given offset o is 10.
    // According to the explanation, we should set f_pos to 10 and then read len bytes,
    // resulting f_pos = 10+len, which might be less than 100 (initial f_pos).
    // New f_pos can be less than initial f_pos.

    ssize_t bytes_read = 0;
    f->f_pos = o;

    if (f->f_pos > f->f_inode->i_size) {
      return -1;
    }

    if (f->f_pos + len > f->f_inode->i_size) {
      bytes_read = f->f_inode->i_size - f->f_pos;
    }
    else {
      bytes_read = len;
    }

    // update the offset of file
    f->f_pos += bytes_read;

    return bytes_read;
}

int my_open(struct inode *i, struct file *f) {

}

int my_release(struct inode *i, struct file *f) {

}

// i_op

struct dentry *my_lookup(struct inode *i, struct dentry *dir) {

}

int my_readlink(struct dentry *dir, char *buf, int len) {

}

int my_readdir(struct inode *i, filldir_t callback) {

}

int my_getattr(struct dentry *dir, struct kstat *stats) {

}

struct super_block* my_get_superblock(struct file_system_type *current_fs) {
  printf("this is my get superblock function\n");
  printf(current_fs->name);
  printf("\n");
  //printf(current_fs->file_descriptor);
  printf("\n");
  return current_fs->get_superblock;
}