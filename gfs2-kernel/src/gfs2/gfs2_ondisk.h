/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2005 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 */

#ifndef __GFS2_ONDISK_DOT_H__
#define __GFS2_ONDISK_DOT_H__

#define GFS2_MAGIC		0x01161970
#define GFS2_BASIC_BLOCK	512
#define GFS2_BASIC_BLOCK_SHIFT	9

/* Lock numbers of the LM_TYPE_NONDISK type */

#define GFS2_MOUNT_LOCK		0
#define GFS2_LIVE_LOCK		1
#define GFS2_TRANS_LOCK		2
#define GFS2_RENAME_LOCK	3

/* Format numbers for various metadata types */

#define GFS2_FORMAT_NONE	0
#define GFS2_FORMAT_SB		100
#define GFS2_FORMAT_RG		200
#define GFS2_FORMAT_RB		300
#define GFS2_FORMAT_DI		400
#define GFS2_FORMAT_IN		500
#define GFS2_FORMAT_LF		600
#define GFS2_FORMAT_JD		700
#define GFS2_FORMAT_LH		800
#define GFS2_FORMAT_LD		900
#define GFS2_FORMAT_LB		1000
#define GFS2_FORMAT_EA		1100
#define GFS2_FORMAT_ED		1200
#define GFS2_FORMAT_UT		1300
#define GFS2_FORMAT_QC		1400
/* These are format numbers for entities contained in files */
#define GFS2_FORMAT_RI		1500
#define GFS2_FORMAT_DE		1600
#define GFS2_FORMAT_QU		1700
/* These are part of the superblock */
#define GFS2_FORMAT_FS		1801
#define GFS2_FORMAT_MULTI	1900

/*
 * An on-disk inode number
 */

#define gfs2_inum_equal(ino1, ino2) \
	(((ino1)->no_formal_ino == (ino2)->no_formal_ino) && \
	((ino1)->no_addr == (ino2)->no_addr))

struct gfs2_inum {
	__be64 no_formal_ino;
	__be64 no_addr;
};

/*
 * Generic metadata head structure
 * Every inplace buffer logged in the journal must start with this.
 */

#define GFS2_METATYPE_NONE	0
#define GFS2_METATYPE_SB	1
#define GFS2_METATYPE_RG	2
#define GFS2_METATYPE_RB	3
#define GFS2_METATYPE_DI	4
#define GFS2_METATYPE_IN	5
#define GFS2_METATYPE_LF	6
#define GFS2_METATYPE_JD	7
#define GFS2_METATYPE_LH	8
#define GFS2_METATYPE_LD	9
#define GFS2_METATYPE_LB	10
#define GFS2_METATYPE_EA	11
#define GFS2_METATYPE_ED	12
#define GFS2_METATYPE_UT	13
#define GFS2_METATYPE_QC	14

struct gfs2_meta_header {
	__be32 mh_magic;
	__be32 mh_type;
	__be64 mh_blkno;	/* ?? */ /* Was generation number in gfs1 */
	__be32 mh_format;
	__be32 __pad;		/* Was incarnation number in gfs1 */
};

/*
 * super-block structure
 *
 * It's probably good if SIZEOF_SB <= GFS2_BASIC_BLOCK (512 bytes)
 *
 * Order is important, need to be able to read old superblocks to do on-disk
 * version upgrades.
 */

/* Address of superblock in GFS2 basic blocks */
#define GFS2_SB_ADDR		128

/* The lock number for the superblock (must be zero) */
#define GFS2_SB_LOCK		0

/* Requirement:  GFS2_LOCKNAME_LEN % 8 == 0
   Includes: the fencing zero at the end */
#define GFS2_LOCKNAME_LEN	64

struct gfs2_sb {
	struct gfs2_meta_header sb_header;

	__be32 sb_fs_format;
	__be32 sb_multihost_format;
	__u32  __pad0;	/* Was superblock flags in gfs1 */

	__be32 sb_bsize;
	__be32 sb_bsize_shift;
	__u32 __pad1;	/* Was journal segment size in gfs1 */

	struct gfs2_inum sb_master_dir; /* Was jindex dinode in gfs1 */
	struct gfs2_inum __pad2, __pad3; /* Was rindex,root dinodes in gfs1 */

	char sb_lockproto[GFS2_LOCKNAME_LEN];
	char sb_locktable[GFS2_LOCKNAME_LEN];
	/* In gfs1, quota and license dinodes followed */
};

/*
 * resource index structure
 */

struct gfs2_rindex {
	__be64 ri_addr;	/* grp block disk address */
	__be32 ri_length;	/* length of rgrp header in fs blocks */
	__u32 __pad;

	__be64 ri_data0;	/* first data location */
	__be32 ri_data;	/* num of data blocks in rgrp */

	__be32 ri_bitbytes;	/* number of bytes in data bitmaps */

	__u8 ri_reserved[64];
};

/*
 * resource group header structure
 */

/* Number of blocks per byte in rgrp */
#define GFS2_NBBY		4
#define GFS2_BIT_SIZE		2
#define GFS2_BIT_MASK		0x00000003

#define GFS2_BLKST_FREE		0
#define GFS2_BLKST_USED		1
#define GFS2_BLKST_INVALID	2
#define GFS2_BLKST_DINODE	3

#define GFS2_RGF_JOURNAL	0x00000001
#define GFS2_RGF_METAONLY	0x00000002
#define GFS2_RGF_DATAONLY	0x00000004
#define GFS2_RGF_NOALLOC	0x00000008

struct gfs2_rgrp {
	struct gfs2_meta_header rg_header;

	__be32 rg_flags;
	__be32 rg_free;
	__be32 rg_dinodes;

	__u8 rg_reserved[92]; /* Several fields from gfs1 now reserved */
};

/*
 * quota structure
 */

struct gfs2_quota {
	__be64 qu_limit;
	__be64 qu_warn;
	__be64 qu_value;
};

/*
 * dinode structure
 */

#define GFS2_MAX_META_HEIGHT	10
#define GFS2_DIR_MAX_DEPTH	17

#define DT2IF(dt) (((dt) << 12) & S_IFMT)
#define IF2DT(sif) (((sif) & S_IFMT) >> 12)

/* Dinode flags */
#define GFS2_DIF_JDATA			0x00000001
#define GFS2_DIF_EXHASH			0x00000002
#define GFS2_DIF_UNUSED			0x00000004  /* only in gfs1 */
#define GFS2_DIF_EA_INDIRECT		0x00000008
#define GFS2_DIF_DIRECTIO		0x00000010
#define GFS2_DIF_IMMUTABLE		0x00000020
#define GFS2_DIF_APPENDONLY		0x00000040
#define GFS2_DIF_NOATIME		0x00000080
#define GFS2_DIF_SYNC			0x00000100
#define GFS2_DIF_SYSTEM			0x00000200 /* New in gfs2 */
#define GFS2_DIF_TRUNC_IN_PROG		0x20000000 /* New in gfs2 */
#define GFS2_DIF_INHERIT_DIRECTIO	0x40000000
#define GFS2_DIF_INHERIT_JDATA		0x80000000

struct gfs2_dinode {
	struct gfs2_meta_header di_header;

	struct gfs2_inum di_num;

	__be32 di_mode;	/* mode of file */
	__be32 di_uid;	/* owner's user id */
	__be32 di_gid;	/* owner's group id */
	__be32 di_nlink;	/* number of links to this file */
	__be64 di_size;	/* number of bytes in file */
	__be64 di_blocks;	/* number of blocks in file */
	__be64 di_atime;	/* time last accessed */
	__be64 di_mtime;	/* time last modified */
	__be64 di_ctime;	/* time last changed */
	__be32 di_major;	/* device major number */
	__be32 di_minor;	/* device minor number */

	/* This section varies from gfs1. Padding added to align with
         * remainder of dinode
	 */
	__be64 di_goal_meta;	/* rgrp to alloc from next */
	__be64 di_goal_data;	/* data block goal */
	__u32 __pad[2];

	__be32 di_flags;	/* GFS2_DIF_... */
	__be32 di_payload_format;  /* GFS2_FORMAT_... */
	__u16 __pad1;	/* Was ditype in gfs1 */
	__be16 di_height;	/* height of metadata */
	__u32 __pad2;	/* Unused incarnation number from gfs1 */

	/* These only apply to directories  */
	__u16 __pad3;	/* Padding */
	__be16 di_depth;	/* Number of bits in the table */
	__be32 di_entries;	/* The number of entries in the directory */

	struct gfs2_inum __pad4; /* Unused even in current gfs1 */

	__be64 di_eattr;	/* extended attribute block number */

	__u8 di_reserved[56];
};

/*
 * directory structure - many of these per directory file
 */

#define GFS2_FNAMESIZE		255
#define GFS2_DIRENT_SIZE(name_len) ((sizeof(struct gfs2_dirent) + (name_len) + 7) & ~7)

struct gfs2_dirent {
	struct gfs2_inum de_inum;
	__be32 de_hash;
	__be32 de_rec_len;
	__u8 de_name_len;
	__u8 de_type;
	__u16 __pad1;
	__u32 __pad2;
};

/*
 * Header of leaf directory nodes
 */

struct gfs2_leaf {
	struct gfs2_meta_header lf_header;

	__be16 lf_depth;		/* Depth of leaf */
	__be16 lf_entries;		/* Number of dirents in leaf */
	__be32 lf_dirent_format;	/* Format of the dirents */
	__be64 lf_next;			/* Next leaf, if overflow */

	__u8 lf_reserved[32];
};

/*
 * Extended attribute header format
 */

#define GFS2_EA_MAX_NAME_LEN	255
#define GFS2_EA_MAX_DATA_LEN	65536

#define GFS2_EATYPE_UNUSED	0
#define GFS2_EATYPE_USR		1
#define GFS2_EATYPE_SYS		2

#define GFS2_EATYPE_LAST	2
#define GFS2_EATYPE_VALID(x)	((x) <= GFS2_EATYPE_LAST)

#define GFS2_EAFLAG_LAST	0x01	/* last ea in block */

struct gfs2_ea_header {
	__be32 ea_rec_len;
	__be32 ea_data_len;
	__u8 ea_name_len;	/* no NULL pointer after the string */
	__u8 ea_type;		/* GFS2_EATYPE_... */
	__u8 ea_flags;		/* GFS2_EAFLAG_... */
	__u8 ea_num_ptrs;
	__u32 __pad;
};

/*
 * Log header structure
 */

#define GFS2_LOG_HEAD_UNMOUNT	0x00000001	/* log is clean */

struct gfs2_log_header {
	struct gfs2_meta_header lh_header;

	__be64 lh_sequence;	/* Sequence number of this transaction */
	__be32 lh_flags;	/* GFS2_LOG_HEAD_... */
	__be32 lh_tail;		/* Block number of log tail */
	__be32 lh_blkno;
	__be32 lh_hash;
};

/*
 * Log type descriptor
 */

#define GFS2_LOG_DESC_METADATA	300
/* ld_data1 is the number of metadata blocks in the descriptor.
   ld_data2 is unused. */

#define GFS2_LOG_DESC_REVOKE	301
/* ld_data1 is the number of revoke blocks in the descriptor.
   ld_data2 is unused. */

struct gfs2_log_descriptor {
	struct gfs2_meta_header ld_header;

	__be32 ld_type;		/* GFS2_LOG_DESC_... */
	__be32 ld_length;	/* Number of buffers in this chunk */
	__be32 ld_data1;	/* descriptor-specific field */
	__be32 ld_data2;	/* descriptor-specific field */

	__u8 ld_reserved[32];
};

/*
 * Inum Range
 * Describe a range of formal inode numbers allocated to
 * one machine to assign to inodes.
 */

#define GFS2_INUM_QUANTUM	1048576

struct gfs2_inum_range {
	__be64 ir_start;
	__be64 ir_length;
};

/*
 * Statfs change
 * Describes an change to the pool of free and allocated
 * blocks.
 */

struct gfs2_statfs_change {
	__be64 sc_total;
	__be64 sc_free;
	__be64 sc_dinodes;
};

/*
 * Unlinked Tag
 * Describes an allocated inode that isn't linked into
 * the directory tree and might need to be deallocated.
 */

#define GFS2_UTF_UNINIT		0x00000001

struct gfs2_unlinked_tag {
	struct gfs2_inum ut_inum;
	__be32 ut_flags;	/* GFS2_UTF_... */
	__u32 __pad;
};

/*
 * Quota change
 * Describes an allocation change for a particular
 * user or group.
 */

#define GFS2_QCF_USER		0x00000001

struct gfs2_quota_change {
	__be64 qc_change;
	__be32 qc_flags;	/* GFS2_QCF_... */
	__be32 qc_id;
};

/* Translation functions */

extern void gfs2_inum_in(struct gfs2_inum *no, char *buf);
extern void gfs2_inum_out(struct gfs2_inum *no, char *buf);
extern void gfs2_meta_header_in(struct gfs2_meta_header *mh, char *buf);
extern void gfs2_meta_header_out(struct gfs2_meta_header *mh, char *buf);
extern void gfs2_sb_in(struct gfs2_sb *sb, char *buf);
extern void gfs2_sb_out(struct gfs2_sb *sb, char *buf);
extern void gfs2_rindex_in(struct gfs2_rindex *ri, char *buf);
extern void gfs2_rindex_out(struct gfs2_rindex *ri, char *buf);
extern void gfs2_rgrp_in(struct gfs2_rgrp *rg, char *buf);
extern void gfs2_rgrp_out(struct gfs2_rgrp *rg, char *buf);
extern void gfs2_quota_in(struct gfs2_quota *qu, char *buf);
extern void gfs2_quota_out(struct gfs2_quota *qu, char *buf);
extern void gfs2_dinode_in(struct gfs2_dinode *di, char *buf);
extern void gfs2_dinode_out(struct gfs2_dinode *di, char *buf);
extern void gfs2_dirent_in(struct gfs2_dirent *de, char *buf);
extern void gfs2_dirent_out(struct gfs2_dirent *de, char *buf);
extern void gfs2_leaf_in(struct gfs2_leaf *lf, char *buf);
extern void gfs2_leaf_out(struct gfs2_leaf *lf, char *buf);
extern void gfs2_ea_header_in(struct gfs2_ea_header *ea, char *buf);
extern void gfs2_ea_header_out(struct gfs2_ea_header *ea, char *buf);
extern void gfs2_log_header_in(struct gfs2_log_header *lh, char *buf);
extern void gfs2_log_header_out(struct gfs2_log_header *lh, char *buf);
extern void gfs2_log_descriptor_in(struct gfs2_log_descriptor *ld, char *buf);
extern void gfs2_log_descriptor_out(struct gfs2_log_descriptor *ld, char *buf);
extern void gfs2_inum_range_in(struct gfs2_inum_range *ir, char *buf);
extern void gfs2_inum_range_out(struct gfs2_inum_range *ir, char *buf);
extern void gfs2_statfs_change_in(struct gfs2_statfs_change *sc, char *buf);
extern void gfs2_statfs_change_out(struct gfs2_statfs_change *sc, char *buf);
extern void gfs2_unlinked_tag_in(struct gfs2_unlinked_tag *ut, char *buf);
extern void gfs2_unlinked_tag_out(struct gfs2_unlinked_tag *ut, char *buf);
extern void gfs2_quota_change_in(struct gfs2_quota_change *qc, char *buf);
extern void gfs2_quota_change_out(struct gfs2_quota_change *qc, char *buf);

/* Printing functions */

extern void gfs2_inum_print(struct gfs2_inum *no);
extern void gfs2_meta_header_print(struct gfs2_meta_header *mh);
extern void gfs2_sb_print(struct gfs2_sb *sb);
extern void gfs2_rindex_print(struct gfs2_rindex *ri);
extern void gfs2_rgrp_print(struct gfs2_rgrp *rg);
extern void gfs2_quota_print(struct gfs2_quota *qu);
extern void gfs2_dinode_print(struct gfs2_dinode *di);
extern void gfs2_dirent_print(struct gfs2_dirent *de, char *name);
extern void gfs2_leaf_print(struct gfs2_leaf *lf);
extern void gfs2_ea_header_print(struct gfs2_ea_header *ea, char *name);
extern void gfs2_log_header_print(struct gfs2_log_header *lh);
extern void gfs2_log_descriptor_print(struct gfs2_log_descriptor *ld);
extern void gfs2_inum_range_print(struct gfs2_inum_range *ir);
extern void gfs2_statfs_change_print(struct gfs2_statfs_change *sc);
extern void gfs2_unlinked_tag_print(struct gfs2_unlinked_tag *ut);
extern void gfs2_quota_change_print(struct gfs2_quota_change *qc);

#endif /* __GFS2_ONDISK_DOT_H__ */
