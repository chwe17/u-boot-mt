/*
 * (C) Copyright 2012 Stephen Warren
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#ifndef __CONFIG_H
#define __CONFIG_H

#include <linux/sizes.h>
#include <configs/autoconf.h>
#include <asm/arch/mt_reg_base.h>

//#define DEBUG

/**********************************************************************************************
 *                                      ARM Cortex A7
 **********************************************************************************************/
#if 1
/*
 *  Iverson 20140326 : 
 *      We can disable dcache for accessing non-cachable address instead of using mmu page table to provide noncachable address.
 */
/* Cache */
// Iverson debug
#define CONFIG_SYS_DCACHE_OFF
#endif

/* Machine ID */
#define CONFIG_MACH_TYPE                    7623

/**********************************************************************************************
 *                                          Memory
 **********************************************************************************************/
/* Memory layout */
/* DRAM definition */
/* 
 * Iverson 20140521 : We detect ram size automatically.
 *      CONFIG_SYS_SDRAM_SIZE define max uboot size.
 *      The max size that auto detection support is 256MB.
 */
#define CONFIG_NR_DRAM_BANKS		        1
#define CONFIG_SYS_SDRAM_BASE		        0x80000000

/* Code Layout */
//#define CONFIG_SYS_TEXT_BASE		        0x80000000
#define CONFIG_SYS_TEXT_BASE		        0x81E00000

/* Uboot definition */
#define CONFIG_SYS_UBOOT_BASE		        CONFIG_SYS_TEXT_BASE
#define CONFIG_SYS_UBOOT_MAX_SIZE           SZ_2M
#define CONFIG_SYS_INIT_SP_ADDR             (CONFIG_SYS_TEXT_BASE + \
                                                CONFIG_SYS_UBOOT_MAX_SIZE - \
                                                GENERATED_GBL_DATA_SIZE)

#define CONFIG_SYS_MALLOC_LEN               SZ_32M

/* RichOS memory partitions */
#define CONFIG_SYS_DECOMP_ADDR              0x80008000
#define CONFIG_SYS_LOAD_ADDR                0x84000000
#define CONFIG_SYS_IMAGE_HDR_ADDR           CONFIG_SYS_LOAD_ADDR

/* Linux DRAM definition */
#define CONFIG_LOADADDR			            CONFIG_SYS_LOAD_ADDR

/*
 * For booting Linux, the board info and command line data
 * have to be in the first 64 MB of memory, since this is
 * the maximum mapped by the Linux kernel during initialization.
 */
#define CONFIG_SYS_BOOTM_LEN	            0x4000000

/**********************************************************************************************
 *                                           Board
 **********************************************************************************************/

/* Board */
#if defined(MT7623_FPGA_BOARD)
#define MACH_FPGA   1
#endif
#define CONFIG_ADD_MTK_HEADER               "y"

#define CONFIG_BOARD_LATE_INIT

/**********************************************************************************************
 *                                          Devices
 **********************************************************************************************/

/********************** Flash *************************/
#if defined(ON_BOARD_SPI_FLASH_COMPONENT)
/* SPI Flash Configuration */

/* Parallel Nor Flash */
#define CONFIG_SYS_NO_FLASH

/* SPI Flash */
#define CONFIG_SPI_FLASH

#define CONFIG_SF_DEFAULT_BUS               0
#define CONFIG_SF_DEFAULT_CS                0
#define CONFIG_SF_DEFAULT_SPEED             1000000
#define CONFIG_SF_DEFAULT_MODE              SPI_MODE_3


#define CONFIG_CMD_SF

#define CONFIG_ENV_IS_IN_SPI_FLASH

#define CONFIG_ENV_SIZE                     SZ_16K
#define CONFIG_ENV_SECT_SIZE                0x40000
#define CONFIG_ENV_OFFSET                   0x0000

#define CONFIG_SF_LINUX_BLOCK               3

#define ENV_BOOT_WRITE_IMAGE \
    "boot_wr_img=image_blks 1" \
    ";sf update ${loadaddr} " __stringify(CONFIG_SF_LINUX_BLOCK) " ${img_blks}\0"
#define ENV_BOOT_READ_IMAGE \
    "boot_rd_img=sf read ${loadaddr} " __stringify(CONFIG_SF_LINUX_BLOCK) " 64" \
    ";image_blks 1;sf read ${loadaddr} " __stringify(CONFIG_SF_LINUX_BLOCK) " ${img_blks}\0"
#elif defined(ON_BOARD_NAND_FLASH_COMPONENT)
#define CONFIG_SYS_NO_FLASH

/* NAND Flash Configuration */
#define CONFIG_SYS_MAX_NAND_DEVICE	        1	
#define CONFIG_SYS_NAND_BASE                NFI_BASE
#define CONFIG_SYS_MAX_FLASH_SECT			256	
#define CONFIG_SYS_MAX_FLASH_BANKS			1
#define CONFIG_CMD_NAND
//#define CONFIG_MTD_DEBUG
//#define CONFIG_MTD_DEBUG_VERBOSE  7
#define CONFIG_CMD_MTDPARTS	1
#define CONFIG_MTD_PARTITIONS	1
#define CONFIG_ENV_IS_IN_NAND
#define CONFIG_MTD_NAND_VERIFY_WRITE		1

#if defined (CONFIG_MTK_MLC_NAND_SUPPORT)
#define CONFIG_ENV_SIZE                     SZ_4K
#define CONFIG_ENV_OFFSET                   0x2000000
#define CONFIG_NAND_UBOOT_OFFSET               0x1000000
#define CONFIG_NAND_LINUX_OFFSET               0x4000000
#define CONFIG_MAX_NAND_PAGE_SIZE		8192
#define CONFIG_MAX_NAND_BLOCK_SIZE		2097152
#define CONFIG_MAX_UBOOT_SIZE			393216	//262144
#else
#define CONFIG_ENV_SIZE                     SZ_4K
#define CONFIG_ENV_OFFSET                   0xC0000
#define CONFIG_NAND_UBOOT_OFFSET            0x40000
#define CONFIG_NAND_LINUX_OFFSET            0x140000
#define CONFIG_NAND_RECOVERY_OFFSET         0x2140000
#define CONFIG_MAX_NAND_PAGE_SIZE       	2048
#define CONFIG_MAX_NAND_BLOCK_SIZE      	131072
#define CONFIG_MAX_UBOOT_SIZE   			393216	//262144
#endif

#define ENV_BOOT_WRITE_IMAGE \
    "boot_wr_img=image_check; if test ${img_result} = good; then image_blks " __stringify(CONFIG_MAX_NAND_PAGE_SIZE) \
    " ${filesize};nand erase " __stringify(CONFIG_NAND_LINUX_OFFSET) " ${img_align_size}" \
    ";nand write ${loadaddr} " __stringify(CONFIG_NAND_LINUX_OFFSET) " ${img_align_size}; fi\0" 

#define ENV_BOOT_READ_IMAGE \
    "boot_rd_img=nand read ${loadaddr} " __stringify(CONFIG_NAND_LINUX_OFFSET) " 2000" \
	";image_blks " __stringify(CONFIG_MAX_NAND_PAGE_SIZE) \
	";nand read ${loadaddr} " __stringify(CONFIG_NAND_LINUX_OFFSET) " ${img_align_size}\0"

#define ENV_WRITE_UBOOT \
    "wr_uboot=uboot_check;if test ${uboot_result} = good; then image_blks " __stringify(CONFIG_MAX_NAND_PAGE_SIZE) \
    " ${filesize};nand erase " __stringify(CONFIG_NAND_UBOOT_OFFSET) " ${img_align_size}" \
    ";nand write ${loadaddr} " __stringify(CONFIG_NAND_UBOOT_OFFSET) " ${img_align_size};reset; fi\0" 

#define ENV_BOOT_WRITE_SEC_IMAGE \
    "boot_wr_sec_img=image_check; if test ${img_result} = good; then image_blks " __stringify(CONFIG_MAX_NAND_PAGE_SIZE) \
    " ${filesize};nand erase " __stringify(CONFIG_NAND_RECOVERY_OFFSET) " ${img_align_size}" \
    ";nand write ${loadaddr} " __stringify(CONFIG_NAND_RECOVERY_OFFSET) " ${img_align_size}; fi\0" 

#define ENV_BOOT_READ_SEC_IMAGE \
    "boot_rd_sec_img=nand read ${loadaddr} " __stringify(CONFIG_NAND_RECOVERY_OFFSET) " 2000" \
	";image_blks " __stringify(CONFIG_MAX_NAND_PAGE_SIZE) \
	";nand read ${loadaddr} " __stringify(CONFIG_NAND_RECOVERY_OFFSET) " ${img_align_size}\0"

#define ENV_DUIMAGE_RECOVERY \
    "duimage_recovery=serious_image_check; if test ${img_result} = bad; then run boot_rd_sec_img" \
    ";serious_image_check; if test ${img_result} = good; then reco_message; run boot_wr_img; fi; fi\0"

#define ENV_DUIMAGE_BACKUP \
   "duimage_backup=nand read ${loadaddr} " __stringify(CONFIG_NAND_RECOVERY_OFFSET) " 2000" \
   ";image_check; if test ${img_result} = bad" \
   ";then run boot_rd_img; serious_image_check; if test ${img_result} = good; then backup_message" \
   ";run boot_wr_sec_img; fi; fi\0"

#elif defined(ON_BOARD_EMMC_COMPONENT)
/*EMMC Configuration*/
/* Parallel Nor Flash */
/* Build error elimination*/
#define CONFIG_SYS_NO_FLASH
/**/


#define FEATURE_MMC_BOOT_MODE

#define CONFIG_ENV_IS_IN_MMC
#define CONFIG_ENV_SIZE                     SZ_4K
#define CONFIG_ENV_OFFSET                   0x600 
#define CONFIG_SYS_MMC_ENV_DEV              0
#define CONFIG_SYS_MAX_FLASH_SECT           512

#define CONFIG_EMMC_UBOOT_BLOCK             0x200 
#define CONFIG_MAX_UBOOT_SIZE               0x200           // 0x200 * 512 = 256KB
#define CONFIG_EMMC_LINUX_BLOCK             0xA00
#define CONFIG_EMMC_RECOVERY_BLOCK          0x10A00

#define ENV_BOOT_WRITE_IMAGE \
    "boot_wr_img=image_check; if test ${img_result} = good; then image_blks " __stringify(CONFIG_SYS_MAX_FLASH_SECT) \
    " ${filesize};mmc device 0;mmc write ${loadaddr} " __stringify(CONFIG_EMMC_LINUX_BLOCK) " ${img_blks}; fi\0"

#define ENV_BOOT_READ_IMAGE \
    "boot_rd_img=mmc device 0;mmc read ${loadaddr} " __stringify(CONFIG_EMMC_LINUX_BLOCK) " 1" \
    ";image_blks " __stringify(CONFIG_SYS_MAX_FLASH_SECT) \
    ";mmc read ${loadaddr} " __stringify(CONFIG_EMMC_LINUX_BLOCK) " ${img_blks}\0"

#define ENV_WRITE_UBOOT \
    "wr_uboot=uboot_check;if test ${uboot_result} = good; then mmc device 0;mmc write ${loadaddr} " __stringify(CONFIG_EMMC_UBOOT_BLOCK) \
    " " __stringify(CONFIG_MAX_UBOOT_SIZE) ";reset; fi\0"

#define ENV_BOOT_WRITE_SEC_IMAGE \
    "boot_wr_sec_img=image_check; if test ${img_result} = good; then image_blks " __stringify(CONFIG_SYS_MAX_FLASH_SECT) \
    " ${filesize};mmc device 0;mmc write ${loadaddr} " __stringify(CONFIG_EMMC_RECOVERY_BLOCK) " ${img_blks}; fi\0"

#define ENV_DUIMAGE_RECOVERY \
    "duimage_recovery=serious_image_check; if test ${img_result} = bad; then run boot_rd_sec_img" \
    ";serious_image_check; if test ${img_result} = good; then reco_message; run boot_wr_img; fi; fi\0"


#define ENV_BOOT_READ_SEC_IMAGE \
    "boot_rd_sec_img=mmc device 0;mmc read ${loadaddr} " __stringify(CONFIG_EMMC_RECOVERY_BLOCK) " 1" \
    ";image_blks " __stringify(CONFIG_SYS_MAX_FLASH_SECT) \
    ";mmc read ${loadaddr} " __stringify(CONFIG_EMMC_RECOVERY_BLOCK) " ${img_blks}\0"

#define ENV_DUIMAGE_BACKUP \
   "duimage_backup=mmc device 0;mmc read ${loadaddr} " __stringify(CONFIG_EMMC_RECOVERY_BLOCK) " 1" \
   ";image_check; if test ${img_result} = bad" \
   ";then run boot_rd_img; serious_image_check; if test ${img_result} = good; then backup_message" \
   ";run boot_wr_sec_img; fi; fi\0"

#elif defined(OFF_BOARD_SD_CARD_COMPONENT)
/*SD card configuration*/
/* Build error elimination*/
#define CONFIG_SYS_NO_FLASH
/**/

#define CONFIG_ENV_IS_IN_SD
#define CONFIG_ENV_SIZE                     SZ_128K
#define CONFIG_ENV_OFFSET                   0xD2800 
#define CONFIG_SYS_MMC_ENV_DEV              1

#define ENV_BOOT_FROM_SD  \
    "boot5=mmc init; fatload mmc 0:1 0x84000000 uimage ; bootm\0"

#else
#define CONFIG_ENV_IS_NOWHERE
#define CONFIG_ENV_SIZE                     SZ_16K
#endif

#define CONFIG_ENV_VARS_UBOOT_CONFIG

#if defined(ON_BOARD_EMMC_COMPONENT) || defined(OFF_BOARD_SD_CARD_COMPONENT) || defined(FW_UPGRADE_BY_SDXC)
/********************** MMC ***********************************/
#define PART_DEBUG
#define CONFIG_CMD_MMC
#define CONFIG_MMC
#define CONFIG_MEDIATEK_MMC

#endif


#define CONFIG_FS_FAT
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
// add ext4, zImage & FDT capabilities
#define CONFIG_FS_EXT4
#define CONFIG_EXT4_WRITE
#define CONFIG_CMD_EXT4
#define CONFIG_CMD_FS_GENERIC
#define CONFIG_CMD_BOOTZ
#define CONFIG_OF_LIBFDT

/********************** GPIO *************************/
//#define CONFIG_MTGPIO

/********************** Watchdog *************************/
#define CONFIG_WATCHDOG_OFF

/********************** Console UART *************************/
/* Uart baudrate */
//#define CONFIG_BAUDRATE                     921600
#define CONFIG_BAUDRATE                     115200
 
/* Console configuration */
#define CONFIG_SYS_CBSIZE		            1024
#define CONFIG_SYS_PBSIZE		            (CONFIG_SYS_CBSIZE +		\
					                            sizeof(CONFIG_SYS_PROMPT) + 16)
//#define CONFIG_CONSOLE_MUX
#define CONFIG_SYS_CONSOLE_IS_IN_ENV

#define ENV_DEVICE_SETTINGS \
	"stdin=serial\0" \
	"stdout=serial\0" \
	"stderr=serial\0"

/********************** Ethernet *************************/
#define CONFIG_ETHADDR                      00:0C:E7:11:22:33
#define CONFIG_IPADDR                       10.10.10.254
#define CONFIG_SERVERIP                     10.10.10.3
#define CONFIG_BOOTFILE                     "iverson_uImage"
#define CONFIG_CMD_NET
#define CONFIG_RT2880_ETH

#define RALINK_REG(x)		(*((volatile u32 *)(x)))

/**********************USB**************************/
#if defined(FW_UPGRADE_BY_USB)

#define CONFIG_CMD_USB
#define CONFIG_USB_STORAGE
#define CONFIG_USB_HOST
#define CONFIG_USB_XHCI
//#define CONFIG_USB_XHCI_DWC3
//#define CONFIG_USB_XHCI_OMAP
#define CONFIG_SYS_USB_XHCI_MAX_ROOT_PORTS 2
#define CONFIG_SYS_CACHELINE_SIZE	32
#define CONFIG_FS_FAT
#define CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
//#define PART_DEBUG

#endif

/*
Bootmenu is fully removed cause useless, remove the garbage bootargs probably set by the binary?
There are a bunch of other variables set which might be better not there.... 
This definition should give ~66MB for the Kernel, 512KB for bootscripts, 512KB for FDT and ~32MB for init Ramdisk.
Keep in mind, that there's (yet) no 'fallback' in case something is to big! The bootlogic tries to boot first with 
a bootscript defined in mmcscriptfile, in case this fails it tries to boot with zImage, uInitrd, and FDT in /boot/-(mmckernfile,
mmcinitrdfile & mmcfdtfile), if this fails too it tries to boot with zImage and FDT only and finally it looks for a uImage
in /boot/uImage. This can only work with 'appended device tree blob' (not used in Armbian) and uImages (also not default), 
bootz with 'appended device tree blob' is not possible due to CONFIG_OF_LIBFDT. All bootoptions besides scriptboot will
use defaultbooargs as bootargs! Currently there's no 'preboot logic', so it will only try to boot from the SD-Card.
*/

#define DEFAULT_LINUX_BOOT_ENV \
	"loadaddr=0x82000000\0" \
	"kernel_addr_r=0x82000000\0" \
	"scriptaddr=0x85F80000\0" \
	"fdtaddr=0x86000000\0" \
	"fdt_addr_r=0x86000000\0" \
	"rdaddr=0x86080000\0" \
	"ramdisk_addr_r=0x86080000\0" \
	"bootm_size=0x10000000\0" \
	"mmckernfile=boot/zImage\0" \
	"mmcinitrdfile= boot/uInitrd\0" \
	"mmcfdtfile=boot/dtb/mt7623n-bananapi-bpi-r2.dtb\0" \
	"mmcscriptfile=boot/boot.scr\0" \
	"mmctype=ext4\0" \
	"mmcnum=1\0" \
	"mmcpart=1\0" \
	"defaultbooargs=earlyprintk initcall_debug console=ttyS0,115200n1 root=/dev/mmcblk0p1 rw rootfstype=ext4 rootwait audit=0\0"

#define CONFIG_EXTRA_ENV_SETTINGS \
	ENV_DEVICE_SETTINGS \
	DEFAULT_LINUX_BOOT_ENV \
	"fileload=${mmctype}load mmc ${mmcnum}:${mmcpart} " \
		"${loadaddr} ${mmcfile}\0" \
	"kernload=setenv loadaddr ${kernel_addr_r};" \
		"setenv mmcfile ${mmckernfile};" \
		"run fileload\0" \
	"initrdload=setenv loadaddr ${rdaddr};" \
		"setenv mmcfile ${mmcinitrdfile};" \
		"run fileload\0" \
	"fdtload=setenv loadaddr ${fdtaddr};" \
		"setenv mmcfile ${mmcfdtfile};" \
		"run fileload\0" \
	"scriptload=setenv loadaddr ${scriptaddr};" \
		"setenv mmcfile ${mmcscriptfile};" \
		"run fileload\0" \
	"scriptboot=echo Running ${mmcscriptfile} from: mmc ${mmcnum}:${mmcpart} using ${mmcscriptfile};" \
		"source ${scriptaddr}\0" \
	"tryscriptboot=run scriptload;" \
		"run scriptboot\0" \
	"trybootinitrdfdt=run kernload; run initrdload; run fdtload;" \
		"setenv bootargs ${defaultbooargs}; echo {bootargs};" \
		"bootz ${kernel_addr_r} ${rdaddr} ${fdtaddr}\0" \
	"trybootfdt=run kernload; run fdtload;" \
		"setenv bootargs ${defaultbooargs}; echo {bootargs};" \
		"bootz ${kernel_addr_r} - ${fdtaddr}\0"
		
/* eMMC booting ist not tested and **must be** considered as not working! */	
#define CONFIG_BOOTCOMMAND \
	"mmc init 1;" \
	"run tryscriptboot;" \
	"run trybootinitrdfdt;" \
	"run trybootfdt;" \
	"echo Default bootoptions failed!"

	
#define CONFIG_BOOTDELAY                    3


/**********************************************************************************************
 *                                       UBoot Command
 **********************************************************************************************/
/* Shell */
#define CONFIG_SYS_MAXARGS		            32
#define CONFIG_SYS_PROMPT		            "mt7623-uboot > "
#define CONFIG_COMMAND_HISTORY

/* Commands */
#include <config_cmd_default.h>

/* Device tree support */
#define CONFIG_OF_BOARD_SETUP
/* ATAGs support for bootm/bootz */
#define CONFIG_SETUP_MEMORY_TAGS
#define CONFIG_CMDLINE_TAG
#define CONFIG_INITRD_TAG
#ifdef CONFIG_MTK_MTD_NAND
#define CONFIG_NAND_TAG
#endif
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_NFS

#define CONFIG_SYS_LONGHELP
#define CONFIG_CMD_PING

/* Enable Hush Parser */
#define CONFIG_SYS_HUSH_PARSER

/* Compression */
/* 
 * Iverson 20150408 :  
 *      We use kernel to decompress kernel.
 */
//#define CONFIG_LZMA                             

#endif
