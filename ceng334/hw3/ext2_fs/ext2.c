#include "ext2.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BASE_OFFSET 1024 /* location of the superblock in the first group */
#define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)
#define INODE_OFFSET(inode_offset, inode) (inode_offset + ((inode)-1)*INODE_SIZE)

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
    struct ext2_inode ext2_i;
    printf("in my read inode\n");
    int fd = myfs.file_descriptor;

    struct ext2_super_block sb;
    /* Read super block */
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &sb, sizeof(sb));
    int block_size = 1024 << sb.s_log_block_size;
    struct ext2_group_desc group_desc;
    printf("read super\n");
    
    /* Read group descriptor */
    lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
    read(fd, &group_desc, sizeof(group_desc));
    printf("read group\n");

    lseek(fd, BLOCK_OFFSET(group_desc.bg_inode_table) + (i->i_ino - 1)*sizeof(struct ext2_inode), SEEK_SET);
    read(fd, &ext2_i, sizeof(struct ext2_inode));

    i->i_mode = ext2_i.i_mode;
    i->i_nlink = ext2_i.i_links_count;
    i->i_uid = ext2_i.i_uid;
    i->i_gid = ext2_i.i_gid;
    i->i_size = ext2_i.i_size;
    i->i_atime = ext2_i.i_atime;
    i->i_mtime = ext2_i.i_mtime;
    i->i_ctime = ext2_i.i_mtime;
    i->i_blocks = ext2_i.i_blocks;
    memcpy(&i->i_block, &ext2_i.i_block, sizeof i->i_block);
    i->i_op = &i_op;
    i->f_op = &f_op;
    //i->i_sb = 
    //i->i_state =
    i->i_flags = ext2_i.i_flags;
    printf("read to i\n");
    
}

int my_statfs(struct super_block *sb, struct kstatfs *stats) {
    printf("in my statfs function\n");
    //stats->name;
    stats->f_magic = sb->s_magic;
    stats->f_bsize = sb->s_blocksize;

    // not sure: 
    stats->f_blocks = sb->s_blocks_count;
    stats->f_bfree = sb->s_free_blocks_count;
    stats->f_inodes = sb->s_inodes_count;
    stats->f_finodes = sb->s_free_inodes_count;

    stats->f_inode_size = sb->s_inode_size;
    stats->f_minor_rev_level = sb->s_minor_rev_level;
    stats->f_rev_level = sb->s_rev_level;
    
    //stats->f_namelen;

    printf("need to check if successful\n");
    return 0;
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

    printf("in my read function\n");

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

    printf("should read into buffer");

    return bytes_read;
}

int my_open(struct inode *i, struct file *f) {
    printf("in my open function\n");
    f->f_inode->i_ino = i->i_ino;
    f->f_inode->i_mode = i->i_mode;
    f->f_inode->i_nlink = i->i_nlink;

    f->f_inode->i_uid = i->i_uid;
    f->f_inode->i_gid = i->i_gid;
    f->f_inode->i_size = i->i_size;

    f->f_inode->i_atime = i->i_atime;
    f->f_inode->i_mtime = i->i_mtime;
    f->f_inode->i_ctime = i->i_ctime;

    f->f_inode->i_blocks = i->i_blocks;
    f->f_inode->i_block[15] = i->i_block;

    f->f_inode->i_op = i->i_op;
    f->f_inode->f_op = i->f_op;
    f->f_inode->i_sb = i->i_sb;

    f->f_inode->i_state = i->i_state;
    f->f_inode->i_flags = i->i_flags;

    printf("need to check if successful");
    return 0;
}

int my_release(struct inode *i, struct file *f) {
    // You should deallocate any memory you allocated if you can
    // (i.e. it not relevant after the file is closed)

    // not sure
    printf("in my release function\n");

    free(f->f_inode->i_ino);
    free(f->f_inode->i_mode);
    free(f->f_inode->i_nlink);

    free(f->f_inode->i_uid);
    free(f->f_inode->i_gid);
    free(f->f_inode->i_size);

    free(f->f_inode->i_atime);
    free(f->f_inode->i_mtime);
    free(f->f_inode->i_ctime);

    free(f->f_inode->i_blocks);
    free(f->f_inode->i_block);

    free(f->f_inode->i_op);
    free(f->f_inode->f_op);
    free(f->f_inode->i_sb);

    free(f->f_inode->i_state);
    free(f->f_inode->i_flags);

    free(f->f_inode);

    return 0;
}

// i_op

struct dentry *my_lookup(struct inode *i, struct dentry *dir) {
    struct ext2_super_block ext2_sb;
    struct ext2_dir_entry *ext2_dir_entry;
    int fd = myfs.file_descriptor;
    void *block;
    int block_size;
    struct inode* inode;
    inode = malloc(sizeof(struct inode));

    /* Read super block */
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &ext2_sb, sizeof(ext2_sb));

    block_size = 1024 << ext2_sb.s_log_block_size;
    block = malloc(block_size);

    for (int inode_block_nr = 0; inode_block_nr < 15; inode_block_nr++) {
        
        lseek(fd, BLOCK_OFFSET(i->i_block[inode_block_nr]), SEEK_SET);
		    read(fd, block, block_size);

        /* set the entry in the directory */
        ext2_dir_entry = (struct ext2_dir_entry *) block;

        if (strcmp(ext2_dir_entry->name, dir->d_name) == 0) {
            printf("name found\n");
            printf("ext2 dentry inode: %d\n", ext2_dir_entry->inode);
            inode->i_ino = ext2_dir_entry->inode;
            printf("set inode nr: %d\n", inode->i_ino);
            my_read_inode(inode);

            //dir->d_flags = ext2_dir_entry->
            printf("gonna set d inode\n");
            dir->d_inode = inode;
            printf("d inode is set\n");
            printf("d inode nr is: %d\n", dir->d_inode->i_ino);
            //dir->d_parent will be filled in pathwalk
            //dir->d_sb = 
            //dir->d_fsdata = ??

            //free(name);
            free(block);
            printf("returning dir\n");
            return dir;
        }
    }

    printf("returning null from lookup\n");
    return NULL;
}

int my_readlink(struct dentry *dir, char *buf, int len) {

}

int my_readdir(struct inode *i, filldir_t callback) {
    printf("in my readdir\n");
    struct ext2_super_block ext2_sb;
    struct ext2_dir_entry *ext2_dir_entry;
    int fd = myfs.file_descriptor;
    void *block;
    int block_size;
    struct inode* inode;
    inode = malloc(sizeof(struct inode));
    int total_nr_of_entries = 0;

    /* Read super block */
    lseek(fd, BASE_OFFSET, SEEK_SET);
    read(fd, &ext2_sb, sizeof(ext2_sb));

    block_size = 1024 << ext2_sb.s_log_block_size;
    block = malloc(block_size);

    for (int inode_block_nr = 0; inode_block_nr < 15; inode_block_nr++) {
        
        lseek(fd, BLOCK_OFFSET(i->i_block[inode_block_nr]), SEEK_SET);
		    read(fd, block, block_size);

        /* set the entry in the directory */
        ext2_dir_entry = (struct ext2_dir_entry *) block;

        if (ext2_dir_entry->name_len > 0) {
          callback(ext2_dir_entry->name, ext2_dir_entry->name_len, i->i_ino);
          total_nr_of_entries++;
        }
        
    }
    return total_nr_of_entries;
}

int my_getattr(struct dentry *dir, struct kstat *stats) {

    stats->ino = dir->d_inode->i_ino;
    stats->mode = dir->d_inode->i_mode;
    stats->nlink = dir->d_inode->i_nlink;

    //stats->uid = getuid();
    stats->uid = dir->d_inode->i_uid;
    //stats->gid = getgid();
    stats->gid = dir->d_inode->i_gid;
    stats->size = dir->d_inode->i_size;

    //stats->atime = time(NULL);
    stats->atime = dir->d_inode->i_atime;
    //stats->mtime = time(NULL);
    stats->mtime = dir->d_inode->i_mtime;
    stats->ctime = dir->d_inode->i_ctime;

    /*struct ext2_super_block sb;
    int block_size = 1024 << sb.s_log_block_size;

    stats->blksize = block_size;*/

    //stats->blksize = dir->d_inode->i_size / dir->d_inode->i_blocks;

    stats->blksize = dir->d_sb->s_blocksize;

    stats->blocks = dir->d_inode->i_blocks;

    printf("should check if successful and block size");
    return 0;
}

struct super_block* my_get_superblock(struct file_system_type *current_fs) {

  int fd = current_fs->file_descriptor;
  struct ext2_super_block ext2_sb;
  
  struct super_block* sb;
  sb = malloc(sizeof(struct super_block));

  struct ext2_group_desc group_desc;

  struct ext2_inode root_inode;

  struct dentry dir_entry;

  int block_size = 1024 << ext2_sb.s_log_block_size;

  /* Read super block */
  lseek(fd, BASE_OFFSET, SEEK_SET);
  read(fd, &ext2_sb, sizeof(struct ext2_super_block));

  sb->s_inodes_count = ext2_sb.s_inodes_count;
  sb->s_blocks_count = ext2_sb.s_blocks_count;
  sb->s_free_blocks_count = ext2_sb.s_free_blocks_count;
  sb->s_free_inodes_count = ext2_sb.s_free_inodes_count;
  sb->s_first_data_block = ext2_sb.s_first_data_block;

  // not sure
  sb->s_blocksize = block_size;
  sb->s_blocksize_bits = ext2_sb.s_log_block_size;
  
  sb->s_blocks_per_group = ext2_sb.s_blocks_per_group;
  sb->s_inodes_per_group = ext2_sb.s_inodes_per_group;
  sb->s_minor_rev_level = ext2_sb.s_minor_rev_level;
  sb->s_rev_level = ext2_sb.s_rev_level;

  // not sure
  sb->s_first_ino = EXT2_GOOD_OLD_FIRST_INO;


  sb->s_inode_size = sizeof(struct ext2_inode);
  //sb->s_block_group_nr = 
  //sb->s_maxbytes = 

  sb->s_type = current_fs;

  sb->s_op = &s_op; 

  //sb->s_flags = 

  sb->s_magic = ext2_sb.s_magic;

  
  /* Read group descriptor */
  lseek(fd, BASE_OFFSET + block_size, SEEK_SET);
  read(fd, &group_desc, sizeof(group_desc));

  /* Read root inode, always number 2 */
  lseek(fd, BLOCK_OFFSET(group_desc.bg_inode_table)+(2-1)*sizeof(struct ext2_inode), SEEK_SET);
  read(fd, &root_inode, sizeof(struct ext2_inode));

  /* Construct dentry for the root inode */

  //dir_entry.d_flags = 
  dir_entry.d_inode = &root_inode;
  //dir_entry.d_parent = 
  //dir_entry.d_name = 
  dir_entry.d_sb = sb;
  //dir_entry.d_fsdata = 
  
  sb->s_root = &dir_entry;

  //sb->s_fs_info =

  return sb;
}