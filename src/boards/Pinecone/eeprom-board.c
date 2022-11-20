#include "eeprom-board.h"
#include "bflb_flash.h"
#include "bl602_common.h"
#include "littlefs/lfs.h"

struct lfs_config lfsConfig;
lfs_t lfs;
struct lfs_file file_p = {};

const size_t blockSize  = 4096;
const size_t size = 0x100000; // 1MB
const size_t startAddress = 0x100000;

int FlashRead(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size);
int FlashProg(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size);
int FlashErase(const struct lfs_config* c, lfs_block_t block);
int FlashSync(const struct lfs_config* c);

void EepromMcuInit(void) {
  lfsConfig.context = NULL;
  lfsConfig.read = FlashRead;
  lfsConfig.prog = FlashProg;
  lfsConfig.erase = FlashErase;
  lfsConfig.sync = FlashSync;
  lfsConfig.read_size = 16;
  lfsConfig.prog_size = 8;
  lfsConfig.block_size = blockSize;
  lfsConfig.block_count = size / blockSize;
  lfsConfig.block_cycles = 1000u;
  lfsConfig.cache_size = 16;
  lfsConfig.lookahead_size = 16;
  lfsConfig.name_max = 50;
  lfsConfig.attr_max = 50;

  // Mount the FS, format it if it's invalid
  int err = lfs_mount(&lfs, &lfsConfig);
  if (err != LFS_ERR_OK) {
    lfs_format(&lfs, &lfsConfig);
    err = lfs_mount(&lfs, &lfsConfig);
    if (err != LFS_ERR_OK) {
      return;
    }
  }

  int ret = lfs_file_open(&lfs, &file_p, "/loramac.dat", LFS_O_WRONLY | LFS_O_CREAT);
  if(ret < 0) {
    return;
  }
}

int FlashSync(const struct lfs_config* c) {
  return 0;
}

int FlashErase(const struct lfs_config* c, lfs_block_t block) {
  const size_t address = startAddress + (block * blockSize);
  int ret = bflb_flash_erase(address, blockSize);

  if(ret == SUCCESS) return LFS_ERR_OK;
  return LFS_ERR_IO;
}

int FlashProg(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, const void* buffer, lfs_size_t size) {
  const size_t address = startAddress + (block * blockSize) + off;
  int ret = bflb_flash_write(address, (uint8_t*)buffer, size);
  if(ret == SUCCESS) return LFS_ERR_OK;
  return LFS_ERR_IO;
}

int FlashRead(const struct lfs_config* c, lfs_block_t block, lfs_off_t off, void* buffer, lfs_size_t size) {
  //printf("FlashRead()\n");
  const size_t address = startAddress + (block * blockSize) + off;
  int ret = bflb_flash_read(address, (uint8_t*)buffer, size);
  if(ret == SUCCESS) return LFS_ERR_OK;
  return LFS_ERR_IO;
}

LmnStatus_t EepromMcuReadBuffer( uint16_t addr, uint8_t *buffer, uint16_t size ) {
  int ret = lfs_file_seek(&lfs, &file_p, addr, LFS_SEEK_SET);
  if(ret < 0) {
    return LMN_STATUS_ERROR;
  }

  ret = lfs_file_read(&lfs, &file_p, buffer, size);
  if(ret < 0) {
    return LMN_STATUS_ERROR;
  }
  return LMN_STATUS_OK;
}

LmnStatus_t EepromMcuWriteBuffer( uint16_t addr, uint8_t *buffer, uint16_t size ) {
  int ret = lfs_file_seek(&lfs, &file_p, addr, LFS_SEEK_SET);
  if(ret < 0) {
    return LMN_STATUS_ERROR;
  }

  ret = lfs_file_write(&lfs, &file_p, buffer, size);
  if(ret < 0) {
    return LMN_STATUS_ERROR;
  }
  lfs_file_close(&lfs, &file_p);
  lfs_file_open(&lfs, &file_p, "/loramac.dat", LFS_O_WRONLY | LFS_O_CREAT);
  return LMN_STATUS_OK;
}