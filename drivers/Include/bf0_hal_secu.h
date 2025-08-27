/*
 * SPDX-FileCopyrightText: 2019-2025 SiFli Technologies(Nanjing) Co., Ltd
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __BF0_HAL_SECU_H
#define __BF0_HAL_SECU_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "bf0_hal_def.h"

/** @addtogroup BF0_HAL_Driver
  * @{
  */

/** @defgroup SECU SECU
  * @brief SECU HAL module driver
  * @{
  */

/* Exported types ------------------------------------------------------------*/
/** @defgroup SECU_Exported_Types SECU Exported Types
  * @{
  */

/** Security supported SECU groups definition  */
typedef enum
{
    SECU_GROUP_INVALID = 0,
    /*SECU1*/

    SECU_GROUP_HPMST,   /*HPSYS master group*/
    SECU_GROUP_HPSLV,   /*HPSYS slave  group*/
    SECU_GROUP_HPMPI1,  /*HPSYS MPI1   group*/
    SECU_GROUP_HPMPI2,  /*HPSYS MPI2   group*/
    SECU_GROUP_HPMPI3,  /*HPSYS MPI3   group*/
    SECU_GROUP_HPRAM,   /*HPSYS SRAM   group*/

    /*SECU2*/
    SECU_GROUP_LPMST,   /*LPSYS master group*/
    SECU_GROUP_LPSLV,   /*LPSYS slave  group*/
    SECU_GROUP_LPRAM,   /*LPSYS SRAM   group*/
} SECU_GROUP_TYPE;


/** Security supported modules definition */
typedef enum
{
    SECU_MOD_INVALID = 0,

    /**SECU1 modules*/
    SECU_MOD_PTC1,    /*!<Master & Slave*/
    SECU_MOD_DMAC1,   /*!<Master & Slave */
    SECU_MOD_USBC,    /*!<Master */
    SECU_MOD_AES,     /*!<Master  & Slave*/
    SECU_MOD_LCDC1,   /*!<Master */
    SECU_MOD_EPIC,    /*!<Master */
    SECU_MOD_EXTDMA,  /*!<Master */
    SECU_MOD_HCPU,    /*!<Master */
    SECU_MOD_EZIP,    /*Master & Slave */
#if (SECU_VERSION == 2)
    SECU_MOD_LCPU,    /*Master */
#endif
    SECU_MOD_JPEG,    /*Master & Slave */
    SECU_MOD_NPU,     /*Master & Slave */
    SECU_MOD_V2DGPU,  /*Master & Slave */
    SECU_MOD_ACPU,    /*Master & Slave */

    SECU_MOD_TRNG,    /*Slave */
    SECU_MOD_EFUSE,   /*Slave */
    SECU_MOD_GPIO1,   /*Slave */
    SECU_MOD_MPI1,    /*Slave */
    SECU_MOD_MPI2,    /*Slave */
    SECU_MOD_MPI3,    /*Slave */
    SECU_MOD_SDMMC1,   /*Slave */
    SECU_MOD_CRC1,    /*Slave */
    SECU_MOD_DCMI,    /*Slave */
    SECU_MOD_RCC1,    /*Slave */
    SECU_MOD_PINMUX1, /*Slave */
    SECU_MOD_ATIM1,   /*Slave */
    SECU_MOD_AUDPRC,  /*Slave */
    SECU_MOD_I2S1,    /*Slave */
    SECU_MOD_SYSCFG1, /*Slave */
    SECU_MOD_MABOX1,  /*Slave */
    SECU_MOD_UART1,   /*Slave */
    SECU_MOD_UART2,   /*Slave */
    SECU_MOD_UART3,   /*Slave */
    SECU_MOD_AUDCODEC, /*Slave */
    SECU_MOD_TSEN,    /*Slave */
    SECU_MOD_GPTIM1,  /*Slave */
    SECU_MOD_GPTIM2,  /*Slave */
    SECU_MOD_BTIM1,   /*Slave */
    SECU_MOD_BTIM2,   /*Slave */
    SECU_MOD_UART4,   /*Slave */
    SECU_MOD_CAN,     /*Slave */
    SECU_MOD_SDMMC2,  /*Slave */
    SECU_MOD_WDT1,    /*Slave */
    SECU_MOD_SPI1,    /*Slave */
    SECU_MOD_SPI2,    /*Slave */
    SECU_MOD_PDM1,    /*Slave */
    SECU_MOD_I2C1,    /*Slave */
    SECU_MOD_I2C2,    /*Slave */
    SECU_MOD_I2C3,    /*Slave */
    SECU_MOD_I2C4,    /*Slave */
    SECU_MOD_GPADC,   /*Slave */
    SECU_MOD_HPSYS_AON,   /*Slave */
    SECU_MOD_LPTIM1,  /*Slave */
    SECU_MOD_LPTIM2,  /*Slave */
    SECU_MOD_PMUC,    /*Slave */
    SECU_MOD_RTC,     /*Slave */

    /*SECU2*/
    SECU_MOD_PTC2,    /*Master & Slave */
    SECU_MOD_DMAC2,   /*Master & Slave */
#if (SECU_VERSION == 1)
    SECU_MOD_LCPU,    /*Master */
#endif

    SECU_MOD_AON,     /*Slave */
    SECU_MOD_GPIO2,   /*Slave */
    SECU_MOD_RFC,     /*Slave */
    SECU_MOD_BT_PHY,  /*Slave */
    SECU_MOD_BT_MAC,  /*Slave */
    SECU_MOD_CRC2,    /*Slave */
    SECU_MOD_RCC2,    /*Slave */
    SECU_MOD_MABOX2,  /*Slave */
    SECU_MOD_PINMUX2, /*Slave */
    SECU_MOD_PATCH,   /*Slave */

    SECU_MOD_UART5,   /*Slave */
    SECU_MOD_BTIM3,   /*Slave */
    SECU_MOD_BTIM4,   /*Slave */
    SECU_MOD_WDT2,    /*Slave */
    SECU_MOD_SYSCFG2, /*Slave */
} SECU_MODULE_TYPE;


/** Security supported memory definition */
typedef enum
{
    SECU_MEM_INVALID = 0,
    /*SECU1*/
    SECU_MEM_MPI1,
    SECU_MEM_MPI2,
    SECU_MEM_MPI3,
    SECU_MEM_HPSYS_RAM0,
    SECU_MEM_HPSYS_RAM1,
    SECU_MEM_HPSYS_RAM2,

    /*SECU2*/
    SECU_MEM_LPSYS_RAM0,
    SECU_MEM_LPSYS_RAM1,

} SECU_MEM_TYPE;

typedef enum
{
    SECU_MEM_R0 = 0,
    SECU_MEM_R1 = 1,
    SECU_MEM_R2 = 2,
    SECU_MEM_RG_NUM = 3,
} SECU_MEM_REGION;

/** SECU_KEY_TYPE
  *
  */
typedef enum
{
    SECU_KEY0     = 0,
    SECU_KEY1     = 1,
    SECU_KEY_NULL = 2,
} SECU_KEY_TYPE;
/** SECU_MPIKEY_TYPE
  *
  */
typedef enum
{
    SECU_MPI2_KEY0 = 0,
    SECU_MPI2_KEY1 = 1,
    SECU_MPI3_KEY0 = 2,
    SECU_MPI3_KEY1 = 3,
} SECU_MPIKEY_TYPE;
/** SECU_ROOTKEY_AES_TYPE disable or allow aes module using rootkey
 *
 */
typedef enum
{
    SECU_ROOTKEY_UNLK = 0,
    SECU_ROOTKEY_LK   = 1,
} SECU_ROOTKEY_AES_TYPE;
/** SECU_HCPU_LOCK_TYPE
  *
  */
typedef enum
{
    SECU_LOCK_TCM     = 0,
    SECU_LOCK_SAU     = 1,
    SECU_LOCK_NSMPU   = 2,
    SECU_LOCK_SMPU    = 3,
    SECU_LOCK_NSVTOR  = 4,
    SECU_LOCK_SVTOR   = 5,
} SECU_HCPU_LOCK_TYPE;
/** Memory attribution rang:[start,end] */
typedef struct
{
    uint32_t start; /*!< start address which must align to #SECU_MEM_MIN_BLOCK, */
    uint32_t end;   /*!< end address align to #SECU_MEM_MIN_BLOCK*/
    uint32_t flag;  /*!< Can be value of @ref SECU_Flag */
} SECU_MemAttr_Type;


/** SECU_MemAttr_Reg
  *
  */
typedef struct
{
    volatile uint32_t *st_reg; /* start address config register addr */
    volatile uint32_t *end_reg; /* end address config register addr*/
    volatile uint32_t *rw_reg; /* read write config register addr*/
    uint32_t rg_of; /* config region offset, r0 r1 r2*/
} SECU_MemAttr_Reg;


/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/** @defgroup SECU_Exported_Macros SECU Exported Macros
  * @{
  */

/** @defgroup SECU_Flag  Security setting flags
  * @{
  */
#define SECU_FLAG_NONE      (0x00UL)           /* !< None security mode and none privilege mode*/
#define SECU_FLAG_PRIV      (0x01UL)           /* !< Privilege mode */
#define SECU_FLAG_SECU      (0x02UL)           /* !< Security  mode */
#define SECU_FLAG_SEC_WR    (0x04UL)           /* !< just Security mode can write*/
/**
  * @}
  */

/** SECU_RW  Security setting flags
  *
  */

#define SECU_PRIV_RD_ATTR  (0x01UL) /* priviledge read attribute */
#define SECU_PRIV_WR_ATTR  (0x02UL) /* priviledge write attribute */
#define SECU_SEC_RD_ATTR   (0x04UL) /* secure read attribute */
#define SECU_SEC_WR_ATTR   (0x08UL) /* secure write attribute */
/**
  * @}
  */

/** @defgroup SECU_Role  Security role
  * @{
  */
#define SECU_ROLE_MASTER    (0x01UL)         /*!< Master role */
#define SECU_ROLE_SLAVE     (0x02UL)         /*!< Slave role */
/**
  * @}
  */


#define SECU_MEM_MIN_BLOCK (1024)           /*!< Minimal security block size*/
#define SECU_MEMRange_ALIGN(v) ((v)&0xFFFFFC00)
#define SECU_MEMRange_ALIGN_UP(v) SECU_MEMRange_ALIGN((v) + SECU_MEM_MIN_BLOCK - 1)
#define SECU_SRAM_MIN_BLOCK (4)
#define SECU_SRAMRange_ALIGN(v) ((v)&0xFFFFFFFC)
#define SECU_SRAMRange_ALIGN_UP(v) SECU_SRAMRange_ALIGN((v) + SECU_SRAM_MIN_BLOCK - 1)


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @defgroup SECU_Exported_Functions SECU Exported Functions
  * @{
  */


/**
  * @brief  Config a module's security.
  * @param  module  - module control by SECU, the value is in type of #SECU_MODULE_TYPE
  * @param  role   - apply security on module while it act as this role @ref SECU_Role
  * @param  flag - module's security attribute, @ref SECU_Flag
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_SetAttr(SECU_MODULE_TYPE module, uint32_t role, uint32_t flag);

/**
  * @brief  Config memory's security.
  * @param  memory_type  - memory control by SECU, the value can be one of #SECU_MEM_TYPE
  * @param  attrs        - memory's security attributes
  * @param  attrs_cnt     - length of attrs
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_SetMemoryAttr(SECU_MEM_TYPE memory_type, const SECU_MemAttr_Type *attrs, uint32_t attrs_cnt);

/**
  * @brief  Config memory's security.
  * @param  memory_type  - memory control by SECU, the value can be one of @ref SECU_MEM_TYPE
  * @param  attrs        - memory's security attributes, attrs.flag select SECU_PRIV_RD_ATTR SECU_PRIV_WR_ATTR SECU_SEC_RD_ATTR SECU_SEC_WR_ATTR
  * @param  attrs_cnt     - length of attrs
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_SetMemoryAttr_RW(SECU_MEM_TYPE memory_type, const SECU_MemAttr_Type *attrs, uint32_t attrs_cnt);

/**
  * @brief  Security groups lock
  * @param  group  - group of SECU, the value can be one of @ref SECU_GROUP_TYPE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_Lock(SECU_GROUP_TYPE group);

/**
  * @brief  Security groups apply
  * @param  group  - group of SECU, the value can be one of @ref SECU_GROUP_TYPE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_Apply(SECU_GROUP_TYPE group);

/**
  * @brief  Security groups lock and apply
  * @param  group  - group of SECU, the value can be one of @ref SECU_GROUP_TYPE
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_ApplyAndLock(SECU_GROUP_TYPE group);

/**
  * @brief  secu mpi real-time decryption key set.
  * @param  key_type - secu key type @ref SECU_KEY_TYPE
  * @param  key - secu key addr.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_KeySet(SECU_KEY_TYPE key_type, uint32_t *key);

/**
  * @brief  secu mpi key select.
  * @param  mpi_key - secu mpi key type @ref SECU_MPIKEY_TYPE.
  * @param  key_type - secu key type @ref SECU_KEY_TYPE.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_MpiKey_Select(SECU_MPIKEY_TYPE mpi_key, SECU_KEY_TYPE key_type);

/**
  * @brief  secu rootkey aes lock.
  * @param  lock - secu rootkey aes lock type @ref SECU_ROOTKEY_AES_TYPE.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_RootKey_AesLock(SECU_ROOTKEY_AES_TYPE lock);

/**
  * @brief  hcpu delay clock number become to no-safe mode after leave nmi handler.
  * @param  delay - delay clock period.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_NMI_ClearDelay(uint32_t delay);

/**
  * @brief  acpu vtor addr set.
  * @param  offset - vtor addr offset.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_ACPU_VTOR(uint32_t offset);

/**
  * @brief  secu hcpu lock.
  * @param  lock_type - hcpu lock type @ref SECU_HCPU_LOCK_TYPE.
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SECU_HCPU_LOCK(SECU_HCPU_LOCK_TYPE lock_type);

/**
  * @brief  triger hcpu into nmi handler enable.
  * @retval HAL status
  */
void HAL_SECU_NMI_TriggerEN(void);

/**
  * @brief  triger hcpu into nmi handler disable.
  * @retval HAL status
  */
void HAL_SECU_NMI_TriggerDIS(void);

/**
  * @brief  acpu clk enable.
  * @retval HAL status
  */
void HAL_SECU_ACPU_CLKEN(void);

/**
  * @brief  acpu clk disable.
  * @retval HAL status
  */
void HAL_SECU_ACPU_CLKDIS(void);

/**
  * @brief  acpu reset.
  * @retval HAL status
  */
void HAL_SECU_ACPU_RSTR(void);

/**
  * @brief  acpu vtor lock.
  * @retval HAL status
  */
void HAL_SECU_ACPU_VTOR_LOCK(void);

/**
  * @brief  acpu launch.
  * @retval HAL status
  */
void HAL_SECU_ACPU_LAUNCH(void);


/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __BF0_HAL_SECU_H */