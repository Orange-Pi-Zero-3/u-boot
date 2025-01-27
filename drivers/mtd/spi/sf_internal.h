/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * SPI flash internal definitions
 *
 * Copyright (C) 2008 Atmel Corporation
 * Copyright (C) 2013 Jagannadha Sutradharudu Teki, Xilinx Inc.
 */

#ifndef _SF_INTERNAL_H_
#define _SF_INTERNAL_H_

#include <linux/types.h>
#include <linux/compiler.h>

#define SPI_NOR_MAX_ID_LEN	6
#define SPI_NOR_MAX_ADDR_WIDTH	4

struct flash_info {
#if !CONFIG_IS_ENABLED(SPI_FLASH_TINY)
	char		*name;
#endif

	/*
	 * This array stores the ID bytes.
	 * The first three bytes are the JEDIC ID.
	 * JEDEC ID zero means "no ID" (mostly older chips).
	 */
	u8		id[SPI_NOR_MAX_ID_LEN];
	u8		id_len;

	/* The size listed here is what works with SPINOR_OP_SE, which isn't
	 * necessarily called a "sector" by the vendor.
	 */
	unsigned int	sector_size;
	u16		n_sectors;

	u16		page_size;
	u16		addr_width;

	u32		flags;
#define SECT_4K			BIT(0)	/* SPINOR_OP_BE_4K works uniformly */
#define SPI_NOR_NO_ERASE	BIT(1)	/* No erase command needed */
#define SST_WRITE		BIT(2)	/* use SST byte programming */
#define SPI_NOR_NO_FR		BIT(3)	/* Can't do fastread */
#define SECT_4K_PMC		BIT(4)	/* SPINOR_OP_BE_4K_PMC works uniformly */
#define SPI_NOR_DUAL_READ	BIT(5)	/* Flash supports Dual Read */
#define SPI_NOR_QUAD_READ	BIT(6)	/* Flash supports Quad Read */
#define USE_FSR			BIT(7)	/* use flag status register */
#define SPI_NOR_HAS_LOCK	BIT(8)	/* Flash supports lock/unlock via SR */
#define SPI_NOR_HAS_TB		BIT(9)	/*
					 * Flash SR has Top/Bottom (TB) protect
					 * bit. Must be used with
					 * SPI_NOR_HAS_LOCK.
					 */
#define	SPI_S3AN		BIT(10)	/*
					 * Xilinx Spartan 3AN In-System Flash
					 * (MFR cannot be used for probing
					 * because it has the same value as
					 * ATMEL flashes)
					 */
#define SPI_NOR_4B_OPCODES	BIT(11)	/*
					 * Use dedicated 4byte address op codes
					 * to support memory size above 128Mib.
					 */
#define NO_CHIP_ERASE		BIT(12) /* Chip does not support chip erase */
#define SPI_NOR_SKIP_SFDP	BIT(13)	/* Skip parsing of SFDP tables */
#define USE_CLSR		BIT(14)	/* use CLSR command */
#define SPI_NOR_INDIVIDUAL_LOCK BIT(16) /* individual block/sector lock mode */
#define SPI_NOR_HAS_LOCK_HANDLE BIT(17) /* OP/ERASE for lock operation */
#define SPI_NOR_OCTAL_READ	BIT(18) /* flash supports OCTAL read */
#define USE_IO_MODE		BIT(19) /* Address and data line width */
#define USE_RX_DTR              BIT(20) /* flash supports RX DTR mode */
#define USE_TX_DTR              BIT(21) /* flash supports TX DTR mode */
#define USE_DQS                 BIT(22) /* flash supports DQS mode */
#define OCTAL_SPINOR		BIT(23) /* flash supports OCTAL SPINOR */
#define SPI_NOR_STACK_DIE		BIT(24) /* flash supports to use multi dies */
};

extern const struct flash_info spi_nor_ids[];

#define JEDEC_MFR(info)	((info)->id[0])
#define JEDEC_ID(info)		(((info)->id[1]) << 8 | ((info)->id[2]))

/* Send a single-byte command to the device and read the response */
int spi_flash_cmd(struct spi_slave *spi, u8 cmd, void *response, size_t len);

/*
 * Send a multi-byte command to the device and read the response. Used
 * for flash array reads, etc.
 */
int spi_flash_cmd_read(struct spi_slave *spi, const u8 *cmd,
		size_t cmd_len, void *data, size_t data_len);

/*
 * Send a multi-byte command to the device followed by (optional)
 * data. Used for programming the flash array, etc.
 */
int spi_flash_cmd_write(struct spi_slave *spi, const u8 *cmd, size_t cmd_len,
		const void *data, size_t data_len);


/* Get software write-protect value (BP bits) */
int spi_flash_cmd_get_sw_write_prot(struct spi_flash *flash);


#ifdef CONFIG_SPI_FLASH_MTD
int spi_flash_mtd_register(struct spi_flash *flash);
void spi_flash_mtd_unregister(void);
#endif

#define SPINOR_BOOT_PARAM_MAGIC	"NORPARAM"
typedef struct {
	u8			magic[8];
	__s32			readcmd;
	__s32			read_mode;
	__s32			write_mode;
	__s32			flash_size;
	__s32			addr4b_opcodes;
	__s32			erase_size;
	__s32			delay_cycle;/*When the frequency is greater than 60MHZ configured as 1;less than 24MHZ configured as 2;greater 24MHZ and less 60HZ as 3*/
	__s32			lock_flag;
	__s32			frequency;
	unsigned int		sample_delay;
	unsigned int		sample_mode;
	enum spi_nor_protocol	read_proto;
	enum spi_nor_protocol	write_proto;
	u8			read_dummy;
} boot_spinor_info_t;

#endif /* _SF_INTERNAL_H_ */
