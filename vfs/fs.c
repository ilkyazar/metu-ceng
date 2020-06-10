#include "fs.h"
#include "ext2_fs/ext2.h"

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// for my pathwalk function
#define BASE_OFFSET 1024
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

int init_fs(const char *image_path) {
  current_fs = initialize_ext2(image_path);
  current_sb = current_fs->get_superblock(current_fs);
  return !(current_fs && current_sb);
}

struct file *openfile(char *path) {
  struct file *f = malloc(sizeof(struct file));
  f->f_path = malloc(strlen(path) + 1);
  strcpy(f->f_path, path);
  struct dentry *dir = pathwalk(path);
  if (!dir) {
    return NULL;
  }
  f->f_inode = dir->d_inode;
  free(dir);
  if (f->f_inode->f_op->open(f->f_inode, f)) {
    return NULL;
  }
  return f;
}

int closefile(struct file *f) {
  if (f->f_op->release(f->f_inode, f)) {
    printf("Error closing file\n");
  }
  free(f);
  f = NULL;
  return 0;
}

int readfile(struct file *f, char *buf, int size, loffset_t *offset) {
  if (*offset >= f->f_inode->i_size) {
    return 0;
  }
  if (*offset + size >= f->f_inode->i_size) {
    size = f->f_inode->i_size - *offset;
  }
  // May add llseek call
  return f->f_op->read(f, buf, size, offset);
}

struct dentry *pathwalk(char *path) {
  /* Allocates and returns a new dentry for a given path */
  printf("in pathwalk\n");
  struct dentry* dentry;
  dentry = malloc(sizeof(struct dentry));

  int fd = myfs.file_descriptor;
  struct ext2_super_block ext2_sb;
  int block_size;
  struct ext2_group_desc group_desc;
  struct ext2_inode ext2_root_inode;
  struct inode* root_inode;
  
  /* Read super block */
  lseek(fd, BASE_OFFSET, SEEK_SET);
  read(fd, &ext2_sb, sizeof(ext2_sb));
  printf("read super blk\n");

  block_size = 1024 << ext2_sb.s_log_block_size;

  /* Read group descriptor */
  lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
  read(fd, &group_desc, sizeof(group_desc));
  printf("read group descr\n");

  /* Read root inode, always number 2 */
  lseek(fd, BLOCK_OFFSET(group_desc.bg_inode_table) + (2 - 1)*sizeof(struct ext2_inode), SEEK_SET);
  read(fd, &ext2_root_inode, sizeof(struct ext2_inode));
  printf("read root inode\n");

  root_inode = malloc(sizeof(struct inode));
  root_inode->i_ino = 2;
  
  //memcpy(&root_inode->i_block, &ext2_root_inode.i_block, sizeof root_inode->i_block);
  for(int i = 0; i < 15; i++) {
    root_inode->i_block[i] = ext2_root_inode.i_block[i];
  }
  
  char* dir = strtok(path, "/");

  dentry->d_name = dir;
  dentry = my_lookup(root_inode, dentry);
  dir = strtok(NULL, "/");

  while (dir != NULL) {
    dentry->d_name = dir;
    dentry->d_parent = dentry;
    dentry = my_lookup(dentry->d_inode, dentry);
    dir = strtok(NULL, "/");
  }

  //dentry->d_inode filled in lookup
  //dentry->d_name = path; :S
  //dentry->d_flags = 

  //dentry->d_parent = 

  //dentry->d_sb = 
  //dentry->d_fsdata = ??
  printf("returning dentry\n");
  //printf("name of parent: %s\n", dentry->d_parent->d_name);
  return dentry;
}

int statfile(struct dentry *d_entry, struct kstat *k_stat) {
  return d_entry->d_inode->i_op->getattr(d_entry, k_stat);
}
