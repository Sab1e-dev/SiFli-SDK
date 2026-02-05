#include <stdio.h>
#include <rtconfig.h>
#include "sd_drv.h"

static uint32_t sdemmc_cache[128];
static uint8_t  wire_mode = 1;    //0 for 1-wire mode, 1 for 4-wire mode

int sdio_emmc_init(void)
{
    int i;
    uint8_t  rsp_idx;
    uint32_t rsp_arg[4];
    uint8_t  cmd_result;
    uint32_t cmd_arg;
    uint16_t rca;
    uint32_t cid[4];

    //initialize sdmmc host
    sd1_init();
#if defined(CFG_BOOTROM) || defined(FPGA)
    hwp_sdmmc1->CLKCR = 119 << SD_CLKCR_DIV_Pos; //48M/120=400k
#else
    hwp_sdmmc1->CLKCR = 359 << SD_CLKCR_DIV_Pos; //144M/360=400k
#endif /* CFG_BOOTROM || FPGA */
    hwp_sdmmc1->CLKCR |= SD_CLKCR_VOID_FIFO_ERROR;
    hwp_sdmmc1->IER = 0; //mask sdmmc interrupt
    hwp_sdmmc1->TOR = 0x00080000; // 1.3s timeout for 400K

    // add a delay after clock set, at least 74 SD clock
    // need wait more than 200ms for 400khz
    HAL_Delay_us(500);
    rca = 0x0;


    //initialize sd card
    cmd_result = sd1_send_cmd(0, 0); //CMD0

    //set sd_req and wait for sd_busy before access sd in normal mode
    hwp_sdmmc1->CASR = SD_CASR_SD_REQ;
    while ((hwp_sdmmc1->CASR & SD_CASR_SD_BUSY) == 0);

    hwp_sdmmc1->CDR |= SD_CDR_CMD_OD;   // SET TO Open Drain mode

    //start card identification
    // CMD1
    do
    {
        cmd_arg = 0x40000080;
        cmd_result = sd1_send_cmd(1, cmd_arg); //CMD1

        sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);

        HAL_Delay_us(20);
    }
    while (!(rsp_arg[0] & 0x80000000));

    //CMD2
    cmd_arg = 0x0;
    cmd_result = sd1_send_cmd(2, cmd_arg); //CMD2
    if (cmd_result == SD_TIMEOUT)
    {
        return 1;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 2;
    }
    sd1_get_rsp(&rsp_idx, &cid[3], &cid[2], &cid[1], &cid[0]);

    //CMD3, SET_RELATIVE_ADDR
    rca = 1;
    cmd_arg = 0x10000;
    cmd_result = sd1_send_cmd(3, cmd_arg); //CMD3
    if (cmd_result == SD_TIMEOUT)
    {
        return 3;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 4;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);
    if (rsp_idx != 0x3)
    {
        return 5;
    }

    // card identification done, switch mode
    hwp_sdmmc1->CDR &= ~SD_CDR_CMD_OD;  // recover to push pull mode

    cmd_arg = rca << 16;
    //cmd_arg = 0x10000;
    cmd_result = sd1_send_cmd(9, cmd_arg); //CMD9
    if (cmd_result == SD_TIMEOUT)
    {
        return 6;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 7;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);

#if defined(CFG_BOOTROM) || defined(FPGA)
    hwp_sdmmc1->CLKCR = 1 << SD_CLKCR_DIV_Pos; //48M/2=24M
#else
    hwp_sdmmc1->CLKCR = 23 << SD_CLKCR_DIV_Pos; //144M/24=6M
#endif /* CFG_BOOTROM || FPGA */
    hwp_sdmmc1->CLKCR |= SD_CLKCR_VOID_FIFO_ERROR;
    hwp_sdmmc1->TOR = 0x02000000; // 1.4s timeout for 24M
    hwp_sdmmc1->CDR = (0 << SD_CDR_ITIMING_Pos);

    //start card transfer
    //CMD7 (SELECT_CARD)
    cmd_arg = (uint32_t)rca << 16;
    cmd_result = sd1_send_cmd(7, cmd_arg);
    if (cmd_result == SD_TIMEOUT)
    {
        return 8;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 9;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);
    if (rsp_idx != 7)
    {
        return 10;
    }

    //CMD8 EXT_CSD
    hwp_sdmmc1->SR = 0xffffffff; //clear sdmmc interrupts
    sd1_read(0, 1); //1 wire mode,1 blocks
    cmd_arg = 0;
    cmd_result = sd1_send_cmd(8, cmd_arg);
    if (cmd_result == SD_TIMEOUT)
    {
        return 11;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 12;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);
    if (rsp_idx != 8)
    {
        return 13;
    }

    //wait for read data
    hwp_sdmmc1->IER = SD_IER_DATA_DONE_MASK;
    cmd_result = sd1_wait_read();  //wait sdmmc interrupt
    if (hwp_sdmmc1->SR & SD_SR_DATA_TIMEOUT)
    {
        return 13;
    }
    if (hwp_sdmmc1->SR & SD_SR_DATA_CRC)
    {
        return 14;
    }

    for (i = 0; i < 512 / 4; i++)
    {
        sdemmc_cache[i] = hwp_sdmmc1->FIFO;
    }

    // get cap
    uint32_t capacity = sdemmc_cache[212 / 4];
    capacity = capacity >> 10;
    capacity = capacity * 512;

    // CMD6 switch to 4 line
    cmd_arg = (3 << 24)          /* MMC_SWITCH_MODE_WRITE_BYTE, set target value */
              | (183 << 16)      /* EXT_CSD bitfield index */
              | (1 << 8)         /* Card is in 4 bit mode  */
              | (1 << 0);        /* EXT_CSD_CMD_SET_NORMAL */
    cmd_result = sd1_send_cmd(6, cmd_arg);
    if (cmd_result == SD_TIMEOUT)
    {
        return 15;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 16;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);
    if (rsp_idx != 6)
    {
        return 17;
    }

    sd1_read(wire_mode, 1); //4 wire mode,1 blocks

    //CMD17 (READ_SINGLE_BLOCK)
    hwp_sdmmc1->SR = 0xffffffff; //clear sdmmc interrupts
    cmd_arg = 0; //start data address
    cmd_result = sd1_send_cmd(17, cmd_arg);
    if (cmd_result == SD_TIMEOUT)
    {
        return 18;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 19;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg[0], &rsp_arg[1], &rsp_arg[2], &rsp_arg[3]);
    if (rsp_idx != 17)
    {
        return 20;
    }

    //wait read data
    hwp_sdmmc1->IER = SD_IER_DATA_DONE_MASK;
    cmd_result = sd1_wait_read();  //wait sdmmc interrupt
    if (hwp_sdmmc1->SR & SD_SR_DATA_TIMEOUT)
    {
        return 21; // read time Out
    }
    if (hwp_sdmmc1->SR & SD_SR_DATA_CRC)
    {
        return 22; // Data error
    }
    // read data
    for (i = 0; i < 512 / 4; i++)
    {
        sdemmc_cache[i] = hwp_sdmmc1->FIFO;
    }

    return 0;

}

int emmc_read_data(uint32_t addr, uint8_t *data, uint32_t len)
{
    uint8_t  rsp_idx;
    int i;
    uint32_t cmd_result;
    uint32_t cmd_arg;
    uint32_t *buf = (uint32_t *)data;
    uint32_t rsp_arg1, rsp_arg2, rsp_arg3, rsp_arg4;


    //start data read before read command
    sd1_read(wire_mode, 1); //4 wire mode

    //CMD17 (READ_SINGLE_BLOCK)
    hwp_sdmmc1->SR = 0xffffffff; //clear sdmmc interrupts
    cmd_arg =  addr >> 9; //start data address
    cmd_result = sd1_send_cmd(17, cmd_arg);
    if (cmd_result == SD_TIMEOUT)
    {
        return 0;
    }
    else if (cmd_result == SD_CRCERR)
    {
        return 0;
    }
    sd1_get_rsp(&rsp_idx, &rsp_arg1, &rsp_arg2, &rsp_arg3, &rsp_arg4);
    if (rsp_idx != 17)
    {
        return 0;
    }

    //wait for dma read data
    hwp_sdmmc1->SR = 0xffffffff; //clear sdmmc interrupts
    hwp_sdmmc1->IER = SD_IER_DATA_DONE_MASK;
    cmd_result = sd1_wait_read();  //wait sdmmc interrupt
    if (hwp_sdmmc1->SR & SD_SR_DATA_TIMEOUT)
    {
        return 0;
    }
    if (hwp_sdmmc1->SR & SD_SR_DATA_CRC)
    {
        return 0;
    }

    for (i = 0; i < len / 4; i++)
    {
        *buf = hwp_sdmmc1->FIFO;
        buf++;
    }

    return len;
}


