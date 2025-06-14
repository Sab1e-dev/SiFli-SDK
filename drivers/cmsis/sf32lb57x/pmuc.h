#ifndef __PMUC_H
#define __PMUC_H

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t WER;
    __IO uint32_t WSR;
    __IO uint32_t WCR;
    __IO uint32_t VRTC_CR;
    __IO uint32_t LRC32_CR;
    __IO uint32_t LRC10_CR;
    __IO uint32_t LXT_CR;
    __IO uint32_t BG_CR;
    __IO uint32_t VBAT_LDO;
    __IO uint32_t BUCK_CR1;
    __IO uint32_t BUCK_CR2;
    __IO uint32_t BUCK2_CR;
    __IO uint32_t HPSYS_LDO;
    __IO uint32_t LPSYS_LDO;
    __IO uint32_t PMU_TR;
    __IO uint32_t PMU_RSVD;
    __IO uint32_t HXT_CR1;
    __IO uint32_t HXT_CR2;
    __IO uint32_t HXT_CR3;
    __IO uint32_t HRC_CR;
    __IO uint32_t CAU_BGR;
    __IO uint32_t CAU_TR;
    __IO uint32_t CAU_RSVD;
    __IO uint32_t WKUP_CNT;
    __IO uint32_t PWRKEY_CNT;
    __IO uint32_t HPSYS_VOUT;
    __IO uint32_t LPSYS_VOUT;
    __IO uint32_t BUCK_VOUT;
    __IO uint32_t BUCK2_VOUT;
} PMUC_TypeDef;


/******************** Bit definition for PMUC_CR register *********************/
#define PMUC_CR_HIBER_EN_Pos            (0U)
#define PMUC_CR_HIBER_EN_Msk            (0x1UL << PMUC_CR_HIBER_EN_Pos)
#define PMUC_CR_HIBER_EN                PMUC_CR_HIBER_EN_Msk
#define PMUC_CR_REBOOT_Pos              (1U)
#define PMUC_CR_REBOOT_Msk              (0x1UL << PMUC_CR_REBOOT_Pos)
#define PMUC_CR_REBOOT                  PMUC_CR_REBOOT_Msk
#define PMUC_CR_LPSYSRST_Pos            (2U)
#define PMUC_CR_LPSYSRST_Msk            (0x1UL << PMUC_CR_LPSYSRST_Pos)
#define PMUC_CR_LPSYSRST                PMUC_CR_LPSYSRST_Msk
#define PMUC_CR_PA_RET_Pos              (4U)
#define PMUC_CR_PA_RET_Msk              (0x1UL << PMUC_CR_PA_RET_Pos)
#define PMUC_CR_PA_RET                  PMUC_CR_PA_RET_Msk
#define PMUC_CR_SA_RET_Pos              (5U)
#define PMUC_CR_SA_RET_Msk              (0x1UL << PMUC_CR_SA_RET_Pos)
#define PMUC_CR_SA_RET                  PMUC_CR_SA_RET_Msk
#define PMUC_CR_SB_RET_Pos              (6U)
#define PMUC_CR_SB_RET_Msk              (0x1UL << PMUC_CR_SB_RET_Pos)
#define PMUC_CR_SB_RET                  PMUC_CR_SB_RET_Msk
#define PMUC_CR_PIN0_MODE_Pos           (8U)
#define PMUC_CR_PIN0_MODE_Msk           (0x3UL << PMUC_CR_PIN0_MODE_Pos)
#define PMUC_CR_PIN0_MODE               PMUC_CR_PIN0_MODE_Msk
#define PMUC_CR_PIN1_MODE_Pos           (10U)
#define PMUC_CR_PIN1_MODE_Msk           (0x3UL << PMUC_CR_PIN1_MODE_Pos)
#define PMUC_CR_PIN1_MODE               PMUC_CR_PIN1_MODE_Msk
#define PMUC_CR_PIN0_SEL_Pos            (12U)
#define PMUC_CR_PIN0_SEL_Msk            (0xFUL << PMUC_CR_PIN0_SEL_Pos)
#define PMUC_CR_PIN0_SEL                PMUC_CR_PIN0_SEL_Msk
#define PMUC_CR_PIN1_SEL_Pos            (16U)
#define PMUC_CR_PIN1_SEL_Msk            (0xFUL << PMUC_CR_PIN1_SEL_Pos)
#define PMUC_CR_PIN1_SEL                PMUC_CR_PIN1_SEL_Msk
#define PMUC_CR_RST_CNT_Pos             (20U)
#define PMUC_CR_RST_CNT_Msk             (0xFUL << PMUC_CR_RST_CNT_Pos)
#define PMUC_CR_RST_CNT                 PMUC_CR_RST_CNT_Msk

/******************** Bit definition for PMUC_WER register ********************/
#define PMUC_WER_PIN0_Pos               (0U)
#define PMUC_WER_PIN0_Msk               (0x1UL << PMUC_WER_PIN0_Pos)
#define PMUC_WER_PIN0                   PMUC_WER_PIN0_Msk
#define PMUC_WER_PIN1_Pos               (1U)
#define PMUC_WER_PIN1_Msk               (0x1UL << PMUC_WER_PIN1_Pos)
#define PMUC_WER_PIN1                   PMUC_WER_PIN1_Msk
#define PMUC_WER_RTC_Pos                (2U)
#define PMUC_WER_RTC_Msk                (0x1UL << PMUC_WER_RTC_Pos)
#define PMUC_WER_RTC                    PMUC_WER_RTC_Msk
#define PMUC_WER_IWDT_Pos               (3U)
#define PMUC_WER_IWDT_Msk               (0x1UL << PMUC_WER_IWDT_Pos)
#define PMUC_WER_IWDT                   PMUC_WER_IWDT_Msk

/******************** Bit definition for PMUC_WSR register ********************/
#define PMUC_WSR_PIN0_Pos               (0U)
#define PMUC_WSR_PIN0_Msk               (0x1UL << PMUC_WSR_PIN0_Pos)
#define PMUC_WSR_PIN0                   PMUC_WSR_PIN0_Msk
#define PMUC_WSR_PIN1_Pos               (1U)
#define PMUC_WSR_PIN1_Msk               (0x1UL << PMUC_WSR_PIN1_Pos)
#define PMUC_WSR_PIN1                   PMUC_WSR_PIN1_Msk
#define PMUC_WSR_RTC_Pos                (2U)
#define PMUC_WSR_RTC_Msk                (0x1UL << PMUC_WSR_RTC_Pos)
#define PMUC_WSR_RTC                    PMUC_WSR_RTC_Msk
#define PMUC_WSR_IWDT_Pos               (3U)
#define PMUC_WSR_IWDT_Msk               (0x1UL << PMUC_WSR_IWDT_Pos)
#define PMUC_WSR_IWDT                   PMUC_WSR_IWDT_Msk
#define PMUC_WSR_PWRKEY_Pos             (4U)
#define PMUC_WSR_PWRKEY_Msk             (0x1UL << PMUC_WSR_PWRKEY_Pos)
#define PMUC_WSR_PWRKEY                 PMUC_WSR_PWRKEY_Msk
#define PMUC_WSR_LOWBAT_Pos             (5U)
#define PMUC_WSR_LOWBAT_Msk             (0x1UL << PMUC_WSR_LOWBAT_Pos)
#define PMUC_WSR_LOWBAT                 PMUC_WSR_LOWBAT_Msk
#define PMUC_WSR_IWDTRST_Pos            (6U)
#define PMUC_WSR_IWDTRST_Msk            (0x1UL << PMUC_WSR_IWDTRST_Pos)
#define PMUC_WSR_IWDTRST                PMUC_WSR_IWDTRST_Msk
#define PMUC_WSR_SYSRSTREQ_Pos          (7U)
#define PMUC_WSR_SYSRSTREQ_Msk          (0x1UL << PMUC_WSR_SYSRSTREQ_Pos)
#define PMUC_WSR_SYSRSTREQ              PMUC_WSR_SYSRSTREQ_Msk

/******************** Bit definition for PMUC_WCR register ********************/
#define PMUC_WCR_PIN0_Pos               (0U)
#define PMUC_WCR_PIN0_Msk               (0x1UL << PMUC_WCR_PIN0_Pos)
#define PMUC_WCR_PIN0                   PMUC_WCR_PIN0_Msk
#define PMUC_WCR_PIN1_Pos               (1U)
#define PMUC_WCR_PIN1_Msk               (0x1UL << PMUC_WCR_PIN1_Pos)
#define PMUC_WCR_PIN1                   PMUC_WCR_PIN1_Msk
#define PMUC_WCR_PWRKEY_Pos             (4U)
#define PMUC_WCR_PWRKEY_Msk             (0x1UL << PMUC_WCR_PWRKEY_Pos)
#define PMUC_WCR_PWRKEY                 PMUC_WCR_PWRKEY_Msk
#define PMUC_WCR_LOWBAT_Pos             (5U)
#define PMUC_WCR_LOWBAT_Msk             (0x1UL << PMUC_WCR_LOWBAT_Pos)
#define PMUC_WCR_LOWBAT                 PMUC_WCR_LOWBAT_Msk
#define PMUC_WCR_IWDTRST_Pos            (6U)
#define PMUC_WCR_IWDTRST_Msk            (0x1UL << PMUC_WCR_IWDTRST_Pos)
#define PMUC_WCR_IWDTRST                PMUC_WCR_IWDTRST_Msk
#define PMUC_WCR_SYSRSTREQ_Pos          (7U)
#define PMUC_WCR_SYSRSTREQ_Msk          (0x1UL << PMUC_WCR_SYSRSTREQ_Pos)
#define PMUC_WCR_SYSRSTREQ              PMUC_WCR_SYSRSTREQ_Msk

/****************** Bit definition for PMUC_VRTC_CR register ******************/
#define PMUC_VRTC_CR_VRTC_VBIT_Pos      (0U)
#define PMUC_VRTC_CR_VRTC_VBIT_Msk      (0xFUL << PMUC_VRTC_CR_VRTC_VBIT_Pos)
#define PMUC_VRTC_CR_VRTC_VBIT          PMUC_VRTC_CR_VRTC_VBIT_Msk
#define PMUC_VRTC_CR_VRTC_TRIM_Pos      (4U)
#define PMUC_VRTC_CR_VRTC_TRIM_Msk      (0xFUL << PMUC_VRTC_CR_VRTC_TRIM_Pos)
#define PMUC_VRTC_CR_VRTC_TRIM          PMUC_VRTC_CR_VRTC_TRIM_Msk
#define PMUC_VRTC_CR_BOR_EN_Pos         (8U)
#define PMUC_VRTC_CR_BOR_EN_Msk         (0x1UL << PMUC_VRTC_CR_BOR_EN_Pos)
#define PMUC_VRTC_CR_BOR_EN             PMUC_VRTC_CR_BOR_EN_Msk
#define PMUC_VRTC_CR_BOR_VT_TRIM_Pos    (9U)
#define PMUC_VRTC_CR_BOR_VT_TRIM_Msk    (0xFUL << PMUC_VRTC_CR_BOR_VT_TRIM_Pos)
#define PMUC_VRTC_CR_BOR_VT_TRIM        PMUC_VRTC_CR_BOR_VT_TRIM_Msk

/***************** Bit definition for PMUC_LRC32_CR register ******************/
#define PMUC_LRC32_CR_EN_Pos            (0U)
#define PMUC_LRC32_CR_EN_Msk            (0x1UL << PMUC_LRC32_CR_EN_Pos)
#define PMUC_LRC32_CR_EN                PMUC_LRC32_CR_EN_Msk
#define PMUC_LRC32_CR_CMPBM1_Pos        (1U)
#define PMUC_LRC32_CR_CMPBM1_Msk        (0x3UL << PMUC_LRC32_CR_CMPBM1_Pos)
#define PMUC_LRC32_CR_CMPBM1            PMUC_LRC32_CR_CMPBM1_Msk
#define PMUC_LRC32_CR_CMPBM2_Pos        (3U)
#define PMUC_LRC32_CR_CMPBM2_Msk        (0x1UL << PMUC_LRC32_CR_CMPBM2_Pos)
#define PMUC_LRC32_CR_CMPBM2            PMUC_LRC32_CR_CMPBM2_Msk
#define PMUC_LRC32_CR_CHGCRT_Pos        (4U)
#define PMUC_LRC32_CR_CHGCRT_Msk        (0x3UL << PMUC_LRC32_CR_CHGCRT_Pos)
#define PMUC_LRC32_CR_CHGCRT            PMUC_LRC32_CR_CHGCRT_Msk
#define PMUC_LRC32_CR_RSEL_Pos          (6U)
#define PMUC_LRC32_CR_RSEL_Msk          (0xFUL << PMUC_LRC32_CR_RSEL_Pos)
#define PMUC_LRC32_CR_RSEL              PMUC_LRC32_CR_RSEL_Msk
#define PMUC_LRC32_CR_RDY_Pos           (31U)
#define PMUC_LRC32_CR_RDY_Msk           (0x1UL << PMUC_LRC32_CR_RDY_Pos)
#define PMUC_LRC32_CR_RDY               PMUC_LRC32_CR_RDY_Msk

/***************** Bit definition for PMUC_LRC10_CR register ******************/
#define PMUC_LRC10_CR_EN_Pos            (0U)
#define PMUC_LRC10_CR_EN_Msk            (0x1UL << PMUC_LRC10_CR_EN_Pos)
#define PMUC_LRC10_CR_EN                PMUC_LRC10_CR_EN_Msk
#define PMUC_LRC10_CR_CMPBM1_Pos        (1U)
#define PMUC_LRC10_CR_CMPBM1_Msk        (0x3UL << PMUC_LRC10_CR_CMPBM1_Pos)
#define PMUC_LRC10_CR_CMPBM1            PMUC_LRC10_CR_CMPBM1_Msk
#define PMUC_LRC10_CR_CMPBM2_Pos        (3U)
#define PMUC_LRC10_CR_CMPBM2_Msk        (0x1UL << PMUC_LRC10_CR_CMPBM2_Pos)
#define PMUC_LRC10_CR_CMPBM2            PMUC_LRC10_CR_CMPBM2_Msk
#define PMUC_LRC10_CR_CHGCRT_Pos        (4U)
#define PMUC_LRC10_CR_CHGCRT_Msk        (0x3UL << PMUC_LRC10_CR_CHGCRT_Pos)
#define PMUC_LRC10_CR_CHGCRT            PMUC_LRC10_CR_CHGCRT_Msk
#define PMUC_LRC10_CR_CHGCAP_Pos        (6U)
#define PMUC_LRC10_CR_CHGCAP_Msk        (0x3UL << PMUC_LRC10_CR_CHGCAP_Pos)
#define PMUC_LRC10_CR_CHGCAP            PMUC_LRC10_CR_CHGCAP_Msk
#define PMUC_LRC10_CR_REFRES_Pos        (8U)
#define PMUC_LRC10_CR_REFRES_Msk        (0x1UL << PMUC_LRC10_CR_REFRES_Pos)
#define PMUC_LRC10_CR_REFRES            PMUC_LRC10_CR_REFRES_Msk
#define PMUC_LRC10_CR_LDO_VBIT_Pos      (9U)
#define PMUC_LRC10_CR_LDO_VBIT_Msk      (0xFUL << PMUC_LRC10_CR_LDO_VBIT_Pos)
#define PMUC_LRC10_CR_LDO_VBIT          PMUC_LRC10_CR_LDO_VBIT_Msk
#define PMUC_LRC10_CR_LDO_TRIM_Pos      (13U)
#define PMUC_LRC10_CR_LDO_TRIM_Msk      (0xFUL << PMUC_LRC10_CR_LDO_TRIM_Pos)
#define PMUC_LRC10_CR_LDO_TRIM          PMUC_LRC10_CR_LDO_TRIM_Msk
#define PMUC_LRC10_CR_RDY_Pos           (31U)
#define PMUC_LRC10_CR_RDY_Msk           (0x1UL << PMUC_LRC10_CR_RDY_Pos)
#define PMUC_LRC10_CR_RDY               PMUC_LRC10_CR_RDY_Msk

/****************** Bit definition for PMUC_LXT_CR register *******************/
#define PMUC_LXT_CR_EN_Pos              (0U)
#define PMUC_LXT_CR_EN_Msk              (0x1UL << PMUC_LXT_CR_EN_Pos)
#define PMUC_LXT_CR_EN                  PMUC_LXT_CR_EN_Msk
#define PMUC_LXT_CR_RSN_Pos             (1U)
#define PMUC_LXT_CR_RSN_Msk             (0x1UL << PMUC_LXT_CR_RSN_Pos)
#define PMUC_LXT_CR_RSN                 PMUC_LXT_CR_RSN_Msk
#define PMUC_LXT_CR_BM_Pos              (2U)
#define PMUC_LXT_CR_BM_Msk              (0xFUL << PMUC_LXT_CR_BM_Pos)
#define PMUC_LXT_CR_BM                  PMUC_LXT_CR_BM_Msk
#define PMUC_LXT_CR_AMP_BM_Pos          (6U)
#define PMUC_LXT_CR_AMP_BM_Msk          (0x3UL << PMUC_LXT_CR_AMP_BM_Pos)
#define PMUC_LXT_CR_AMP_BM              PMUC_LXT_CR_AMP_BM_Msk
#define PMUC_LXT_CR_AMPCTRL_ENB_Pos     (8U)
#define PMUC_LXT_CR_AMPCTRL_ENB_Msk     (0x1UL << PMUC_LXT_CR_AMPCTRL_ENB_Pos)
#define PMUC_LXT_CR_AMPCTRL_ENB         PMUC_LXT_CR_AMPCTRL_ENB_Msk
#define PMUC_LXT_CR_BMSEL_Pos           (9U)
#define PMUC_LXT_CR_BMSEL_Msk           (0x1UL << PMUC_LXT_CR_BMSEL_Pos)
#define PMUC_LXT_CR_BMSEL               PMUC_LXT_CR_BMSEL_Msk
#define PMUC_LXT_CR_BMSTART_Pos         (10U)
#define PMUC_LXT_CR_BMSTART_Msk         (0xFUL << PMUC_LXT_CR_BMSTART_Pos)
#define PMUC_LXT_CR_BMSTART             PMUC_LXT_CR_BMSTART_Msk
#define PMUC_LXT_CR_CAP_SEL_Pos         (14U)
#define PMUC_LXT_CR_CAP_SEL_Msk         (0x1UL << PMUC_LXT_CR_CAP_SEL_Pos)
#define PMUC_LXT_CR_CAP_SEL             PMUC_LXT_CR_CAP_SEL_Msk
#define PMUC_LXT_CR_EXT_EN_Pos          (15U)
#define PMUC_LXT_CR_EXT_EN_Msk          (0x1UL << PMUC_LXT_CR_EXT_EN_Pos)
#define PMUC_LXT_CR_EXT_EN              PMUC_LXT_CR_EXT_EN_Msk
#define PMUC_LXT_CR_RDY_Pos             (31U)
#define PMUC_LXT_CR_RDY_Msk             (0x1UL << PMUC_LXT_CR_RDY_Pos)
#define PMUC_LXT_CR_RDY                 PMUC_LXT_CR_RDY_Msk

/******************* Bit definition for PMUC_BG_CR register *******************/
#define PMUC_BG_CR_LDO_BG_STEP_Pos      (0U)
#define PMUC_BG_CR_LDO_BG_STEP_Msk      (0x3UL << PMUC_BG_CR_LDO_BG_STEP_Pos)
#define PMUC_BG_CR_LDO_BG_STEP          PMUC_BG_CR_LDO_BG_STEP_Msk
#define PMUC_BG_CR_LDO_BG_TRIM_Pos      (2U)
#define PMUC_BG_CR_LDO_BG_TRIM_Msk      (0x7UL << PMUC_BG_CR_LDO_BG_TRIM_Pos)
#define PMUC_BG_CR_LDO_BG_TRIM          PMUC_BG_CR_LDO_BG_TRIM_Msk
#define PMUC_BG_CR_LDO_BG_POLAR_Pos     (5U)
#define PMUC_BG_CR_LDO_BG_POLAR_Msk     (0x1UL << PMUC_BG_CR_LDO_BG_POLAR_Pos)
#define PMUC_BG_CR_LDO_BG_POLAR         PMUC_BG_CR_LDO_BG_POLAR_Msk
#define PMUC_BG_CR_BUCK_BG_STEP_Pos     (8U)
#define PMUC_BG_CR_BUCK_BG_STEP_Msk     (0x3UL << PMUC_BG_CR_BUCK_BG_STEP_Pos)
#define PMUC_BG_CR_BUCK_BG_STEP         PMUC_BG_CR_BUCK_BG_STEP_Msk
#define PMUC_BG_CR_BUCK_BG_TRIM_Pos     (10U)
#define PMUC_BG_CR_BUCK_BG_TRIM_Msk     (0x7UL << PMUC_BG_CR_BUCK_BG_TRIM_Pos)
#define PMUC_BG_CR_BUCK_BG_TRIM         PMUC_BG_CR_BUCK_BG_TRIM_Msk
#define PMUC_BG_CR_BUCK_BG_POLAR_Pos    (13U)
#define PMUC_BG_CR_BUCK_BG_POLAR_Msk    (0x1UL << PMUC_BG_CR_BUCK_BG_POLAR_Pos)
#define PMUC_BG_CR_BUCK_BG_POLAR        PMUC_BG_CR_BUCK_BG_POLAR_Msk

/***************** Bit definition for PMUC_VBAT_LDO register ******************/
#define PMUC_VBAT_LDO_SET_VOUT_Pos      (0U)
#define PMUC_VBAT_LDO_SET_VOUT_Msk      (0xFUL << PMUC_VBAT_LDO_SET_VOUT_Pos)
#define PMUC_VBAT_LDO_SET_VOUT          PMUC_VBAT_LDO_SET_VOUT_Msk
#define PMUC_VBAT_LDO_SET_VTHN_Pos      (4U)
#define PMUC_VBAT_LDO_SET_VTHN_Msk      (0x7UL << PMUC_VBAT_LDO_SET_VTHN_Pos)
#define PMUC_VBAT_LDO_SET_VTHN          PMUC_VBAT_LDO_SET_VTHN_Msk
#define PMUC_VBAT_LDO_SET_VTHP_Pos      (7U)
#define PMUC_VBAT_LDO_SET_VTHP_Msk      (0x7UL << PMUC_VBAT_LDO_SET_VTHP_Pos)
#define PMUC_VBAT_LDO_SET_VTHP          PMUC_VBAT_LDO_SET_VTHP_Msk
#define PMUC_VBAT_LDO_MODE_DET_EN_Pos   (10U)
#define PMUC_VBAT_LDO_MODE_DET_EN_Msk   (0x1UL << PMUC_VBAT_LDO_MODE_DET_EN_Pos)
#define PMUC_VBAT_LDO_MODE_DET_EN       PMUC_VBAT_LDO_MODE_DET_EN_Msk

/***************** Bit definition for PMUC_BUCK_CR1 register ******************/
#define PMUC_BUCK_CR1_EN_Pos            (0U)
#define PMUC_BUCK_CR1_EN_Msk            (0x1UL << PMUC_BUCK_CR1_EN_Pos)
#define PMUC_BUCK_CR1_EN                PMUC_BUCK_CR1_EN_Msk
#define PMUC_BUCK_CR1_CTRL_Pos          (1U)
#define PMUC_BUCK_CR1_CTRL_Msk          (0x1UL << PMUC_BUCK_CR1_CTRL_Pos)
#define PMUC_BUCK_CR1_CTRL              PMUC_BUCK_CR1_CTRL_Msk
#define PMUC_BUCK_CR1_BYPASS_PG_Pos     (2U)
#define PMUC_BUCK_CR1_BYPASS_PG_Msk     (0x1UL << PMUC_BUCK_CR1_BYPASS_PG_Pos)
#define PMUC_BUCK_CR1_BYPASS_PG         PMUC_BUCK_CR1_BYPASS_PG_Msk
#define PMUC_BUCK_CR1_BYPASS_OCP_Pos    (3U)
#define PMUC_BUCK_CR1_BYPASS_OCP_Msk    (0x1UL << PMUC_BUCK_CR1_BYPASS_OCP_Pos)
#define PMUC_BUCK_CR1_BYPASS_OCP        PMUC_BUCK_CR1_BYPASS_OCP_Msk
#define PMUC_BUCK_CR1_BYPASS_UVLO_Pos   (4U)
#define PMUC_BUCK_CR1_BYPASS_UVLO_Msk   (0x1UL << PMUC_BUCK_CR1_BYPASS_UVLO_Pos)
#define PMUC_BUCK_CR1_BYPASS_UVLO       PMUC_BUCK_CR1_BYPASS_UVLO_Msk
#define PMUC_BUCK_CR1_COMP_BM_AHI_Pos   (5U)
#define PMUC_BUCK_CR1_COMP_BM_AHI_Msk   (0x1UL << PMUC_BUCK_CR1_COMP_BM_AHI_Pos)
#define PMUC_BUCK_CR1_COMP_BM_AHI       PMUC_BUCK_CR1_COMP_BM_AHI_Msk
#define PMUC_BUCK_CR1_COMP_IQ_TUNE_Pos  (6U)
#define PMUC_BUCK_CR1_COMP_IQ_TUNE_Msk  (0x3UL << PMUC_BUCK_CR1_COMP_IQ_TUNE_Pos)
#define PMUC_BUCK_CR1_COMP_IQ_TUNE      PMUC_BUCK_CR1_COMP_IQ_TUNE_Msk
#define PMUC_BUCK_CR1_COMP_IDYN_TUNE_Pos  (8U)
#define PMUC_BUCK_CR1_COMP_IDYN_TUNE_Msk  (0x3UL << PMUC_BUCK_CR1_COMP_IDYN_TUNE_Pos)
#define PMUC_BUCK_CR1_COMP_IDYN_TUNE    PMUC_BUCK_CR1_COMP_IDYN_TUNE_Msk
#define PMUC_BUCK_CR1_IOCP_TUNE_Pos     (10U)
#define PMUC_BUCK_CR1_IOCP_TUNE_Msk     (0x7UL << PMUC_BUCK_CR1_IOCP_TUNE_Pos)
#define PMUC_BUCK_CR1_IOCP_TUNE         PMUC_BUCK_CR1_IOCP_TUNE_Msk
#define PMUC_BUCK_CR1_OCP_BM_Pos        (13U)
#define PMUC_BUCK_CR1_OCP_BM_Msk        (0x7UL << PMUC_BUCK_CR1_OCP_BM_Pos)
#define PMUC_BUCK_CR1_OCP_BM            PMUC_BUCK_CR1_OCP_BM_Msk
#define PMUC_BUCK_CR1_OCP_AON_Pos       (16U)
#define PMUC_BUCK_CR1_OCP_AON_Msk       (0x1UL << PMUC_BUCK_CR1_OCP_AON_Pos)
#define PMUC_BUCK_CR1_OCP_AON           PMUC_BUCK_CR1_OCP_AON_Msk
#define PMUC_BUCK_CR1_ZCD_AON_Pos       (17U)
#define PMUC_BUCK_CR1_ZCD_AON_Msk       (0x1UL << PMUC_BUCK_CR1_ZCD_AON_Pos)
#define PMUC_BUCK_CR1_ZCD_AON           PMUC_BUCK_CR1_ZCD_AON_Msk
#define PMUC_BUCK_CR1_COT_CTUNE_Pos     (18U)
#define PMUC_BUCK_CR1_COT_CTUNE_Msk     (0x7UL << PMUC_BUCK_CR1_COT_CTUNE_Pos)
#define PMUC_BUCK_CR1_COT_CTUNE         PMUC_BUCK_CR1_COT_CTUNE_Msk
#define PMUC_BUCK_CR1_MOT_CTUNE_Pos     (21U)
#define PMUC_BUCK_CR1_MOT_CTUNE_Msk     (0x7UL << PMUC_BUCK_CR1_MOT_CTUNE_Pos)
#define PMUC_BUCK_CR1_MOT_CTUNE         PMUC_BUCK_CR1_MOT_CTUNE_Msk
#define PMUC_BUCK_CR1_SEL_IOCP_HI_Pos   (24U)
#define PMUC_BUCK_CR1_SEL_IOCP_HI_Msk   (0x1UL << PMUC_BUCK_CR1_SEL_IOCP_HI_Pos)
#define PMUC_BUCK_CR1_SEL_IOCP_HI       PMUC_BUCK_CR1_SEL_IOCP_HI_Msk
#define PMUC_BUCK_CR1_SEL_LX22_Pos      (25U)
#define PMUC_BUCK_CR1_SEL_LX22_Msk      (0x1UL << PMUC_BUCK_CR1_SEL_LX22_Pos)
#define PMUC_BUCK_CR1_SEL_LX22          PMUC_BUCK_CR1_SEL_LX22_Msk
#define PMUC_BUCK_CR1_PSWDRV_BM_Pos     (26U)
#define PMUC_BUCK_CR1_PSWDRV_BM_Msk     (0x7UL << PMUC_BUCK_CR1_PSWDRV_BM_Pos)
#define PMUC_BUCK_CR1_PSWDRV_BM         PMUC_BUCK_CR1_PSWDRV_BM_Msk
#define PMUC_BUCK_CR1_UVLO_X_BIAS_Pos   (29U)
#define PMUC_BUCK_CR1_UVLO_X_BIAS_Msk   (0x1UL << PMUC_BUCK_CR1_UVLO_X_BIAS_Pos)
#define PMUC_BUCK_CR1_UVLO_X_BIAS       PMUC_BUCK_CR1_UVLO_X_BIAS_Msk
#define PMUC_BUCK_CR1_SS_DONE_Pos       (31U)
#define PMUC_BUCK_CR1_SS_DONE_Msk       (0x1UL << PMUC_BUCK_CR1_SS_DONE_Pos)
#define PMUC_BUCK_CR1_SS_DONE           PMUC_BUCK_CR1_SS_DONE_Msk

/***************** Bit definition for PMUC_BUCK_CR2 register ******************/
#define PMUC_BUCK_CR2_H2M_EN_Pos        (0U)
#define PMUC_BUCK_CR2_H2M_EN_Msk        (0x1UL << PMUC_BUCK_CR2_H2M_EN_Pos)
#define PMUC_BUCK_CR2_H2M_EN            PMUC_BUCK_CR2_H2M_EN_Msk
#define PMUC_BUCK_CR2_H2L_EN_Pos        (1U)
#define PMUC_BUCK_CR2_H2L_EN_Msk        (0x1UL << PMUC_BUCK_CR2_H2L_EN_Pos)
#define PMUC_BUCK_CR2_H2L_EN            PMUC_BUCK_CR2_H2L_EN_Msk
#define PMUC_BUCK_CR2_M2L_EN_Pos        (2U)
#define PMUC_BUCK_CR2_M2L_EN_Msk        (0x1UL << PMUC_BUCK_CR2_M2L_EN_Pos)
#define PMUC_BUCK_CR2_M2L_EN            PMUC_BUCK_CR2_M2L_EN_Msk
#define PMUC_BUCK_CR2_L2M_EN_Pos        (3U)
#define PMUC_BUCK_CR2_L2M_EN_Msk        (0x1UL << PMUC_BUCK_CR2_L2M_EN_Pos)
#define PMUC_BUCK_CR2_L2M_EN            PMUC_BUCK_CR2_L2M_EN_Msk
#define PMUC_BUCK_CR2_M2H_CNT_Pos       (4U)
#define PMUC_BUCK_CR2_M2H_CNT_Msk       (0xFUL << PMUC_BUCK_CR2_M2H_CNT_Pos)
#define PMUC_BUCK_CR2_M2H_CNT           PMUC_BUCK_CR2_M2H_CNT_Msk
#define PMUC_BUCK_CR2_L2H_CNT_Pos       (8U)
#define PMUC_BUCK_CR2_L2H_CNT_Msk       (0xFUL << PMUC_BUCK_CR2_L2H_CNT_Pos)
#define PMUC_BUCK_CR2_L2H_CNT           PMUC_BUCK_CR2_L2H_CNT_Msk
#define PMUC_BUCK_CR2_L2M_CNT_Pos       (12U)
#define PMUC_BUCK_CR2_L2M_CNT_Msk       (0xFUL << PMUC_BUCK_CR2_L2M_CNT_Pos)
#define PMUC_BUCK_CR2_L2M_CNT           PMUC_BUCK_CR2_L2M_CNT_Msk
#define PMUC_BUCK_CR2_SET_VOUT_M_Pos    (16U)
#define PMUC_BUCK_CR2_SET_VOUT_M_Msk    (0x1FUL << PMUC_BUCK_CR2_SET_VOUT_M_Pos)
#define PMUC_BUCK_CR2_SET_VOUT_M        PMUC_BUCK_CR2_SET_VOUT_M_Msk
#define PMUC_BUCK_CR2_SET_VOUT_L_Pos    (21U)
#define PMUC_BUCK_CR2_SET_VOUT_L_Msk    (0x1FUL << PMUC_BUCK_CR2_SET_VOUT_L_Pos)
#define PMUC_BUCK_CR2_SET_VOUT_L        PMUC_BUCK_CR2_SET_VOUT_L_Msk
#define PMUC_BUCK_CR2_TDIS_Pos          (26U)
#define PMUC_BUCK_CR2_TDIS_Msk          (0xFUL << PMUC_BUCK_CR2_TDIS_Pos)
#define PMUC_BUCK_CR2_TDIS              PMUC_BUCK_CR2_TDIS_Msk

/***************** Bit definition for PMUC_BUCK2_CR register ******************/
#define PMUC_BUCK2_CR_EN_Pos            (0U)
#define PMUC_BUCK2_CR_EN_Msk            (0x1UL << PMUC_BUCK2_CR_EN_Pos)
#define PMUC_BUCK2_CR_EN                PMUC_BUCK2_CR_EN_Msk
#define PMUC_BUCK2_CR_CTRL_Pos          (1U)
#define PMUC_BUCK2_CR_CTRL_Msk          (0x1UL << PMUC_BUCK2_CR_CTRL_Pos)
#define PMUC_BUCK2_CR_CTRL              PMUC_BUCK2_CR_CTRL_Msk
#define PMUC_BUCK2_CR_BYPASS_PG_Pos     (2U)
#define PMUC_BUCK2_CR_BYPASS_PG_Msk     (0x1UL << PMUC_BUCK2_CR_BYPASS_PG_Pos)
#define PMUC_BUCK2_CR_BYPASS_PG         PMUC_BUCK2_CR_BYPASS_PG_Msk
#define PMUC_BUCK2_CR_BYPASS_OCP_Pos    (3U)
#define PMUC_BUCK2_CR_BYPASS_OCP_Msk    (0x1UL << PMUC_BUCK2_CR_BYPASS_OCP_Pos)
#define PMUC_BUCK2_CR_BYPASS_OCP        PMUC_BUCK2_CR_BYPASS_OCP_Msk
#define PMUC_BUCK2_CR_BYPASS_UVLO_Pos   (4U)
#define PMUC_BUCK2_CR_BYPASS_UVLO_Msk   (0x1UL << PMUC_BUCK2_CR_BYPASS_UVLO_Pos)
#define PMUC_BUCK2_CR_BYPASS_UVLO       PMUC_BUCK2_CR_BYPASS_UVLO_Msk
#define PMUC_BUCK2_CR_COMP_BM_AHI_Pos   (5U)
#define PMUC_BUCK2_CR_COMP_BM_AHI_Msk   (0x1UL << PMUC_BUCK2_CR_COMP_BM_AHI_Pos)
#define PMUC_BUCK2_CR_COMP_BM_AHI       PMUC_BUCK2_CR_COMP_BM_AHI_Msk
#define PMUC_BUCK2_CR_COMP_IQ_TUNE_Pos  (6U)
#define PMUC_BUCK2_CR_COMP_IQ_TUNE_Msk  (0x3UL << PMUC_BUCK2_CR_COMP_IQ_TUNE_Pos)
#define PMUC_BUCK2_CR_COMP_IQ_TUNE      PMUC_BUCK2_CR_COMP_IQ_TUNE_Msk
#define PMUC_BUCK2_CR_COMP_IDYN_TUNE_Pos  (8U)
#define PMUC_BUCK2_CR_COMP_IDYN_TUNE_Msk  (0x3UL << PMUC_BUCK2_CR_COMP_IDYN_TUNE_Pos)
#define PMUC_BUCK2_CR_COMP_IDYN_TUNE    PMUC_BUCK2_CR_COMP_IDYN_TUNE_Msk
#define PMUC_BUCK2_CR_IOCP_TUNE_Pos     (10U)
#define PMUC_BUCK2_CR_IOCP_TUNE_Msk     (0x7UL << PMUC_BUCK2_CR_IOCP_TUNE_Pos)
#define PMUC_BUCK2_CR_IOCP_TUNE         PMUC_BUCK2_CR_IOCP_TUNE_Msk
#define PMUC_BUCK2_CR_OCP_BM_Pos        (13U)
#define PMUC_BUCK2_CR_OCP_BM_Msk        (0x7UL << PMUC_BUCK2_CR_OCP_BM_Pos)
#define PMUC_BUCK2_CR_OCP_BM            PMUC_BUCK2_CR_OCP_BM_Msk
#define PMUC_BUCK2_CR_OCP_AON_Pos       (16U)
#define PMUC_BUCK2_CR_OCP_AON_Msk       (0x1UL << PMUC_BUCK2_CR_OCP_AON_Pos)
#define PMUC_BUCK2_CR_OCP_AON           PMUC_BUCK2_CR_OCP_AON_Msk
#define PMUC_BUCK2_CR_ZCD_AON_Pos       (17U)
#define PMUC_BUCK2_CR_ZCD_AON_Msk       (0x1UL << PMUC_BUCK2_CR_ZCD_AON_Pos)
#define PMUC_BUCK2_CR_ZCD_AON           PMUC_BUCK2_CR_ZCD_AON_Msk
#define PMUC_BUCK2_CR_COT_CTUNE_Pos     (18U)
#define PMUC_BUCK2_CR_COT_CTUNE_Msk     (0x7UL << PMUC_BUCK2_CR_COT_CTUNE_Pos)
#define PMUC_BUCK2_CR_COT_CTUNE         PMUC_BUCK2_CR_COT_CTUNE_Msk
#define PMUC_BUCK2_CR_MOT_CTUNE_Pos     (21U)
#define PMUC_BUCK2_CR_MOT_CTUNE_Msk     (0x7UL << PMUC_BUCK2_CR_MOT_CTUNE_Pos)
#define PMUC_BUCK2_CR_MOT_CTUNE         PMUC_BUCK2_CR_MOT_CTUNE_Msk
#define PMUC_BUCK2_CR_SEL_IOCP_HI_Pos   (24U)
#define PMUC_BUCK2_CR_SEL_IOCP_HI_Msk   (0x1UL << PMUC_BUCK2_CR_SEL_IOCP_HI_Pos)
#define PMUC_BUCK2_CR_SEL_IOCP_HI       PMUC_BUCK2_CR_SEL_IOCP_HI_Msk
#define PMUC_BUCK2_CR_SEL_LX22_Pos      (25U)
#define PMUC_BUCK2_CR_SEL_LX22_Msk      (0x1UL << PMUC_BUCK2_CR_SEL_LX22_Pos)
#define PMUC_BUCK2_CR_SEL_LX22          PMUC_BUCK2_CR_SEL_LX22_Msk
#define PMUC_BUCK2_CR_PSWDRV_BM_Pos     (26U)
#define PMUC_BUCK2_CR_PSWDRV_BM_Msk     (0x7UL << PMUC_BUCK2_CR_PSWDRV_BM_Pos)
#define PMUC_BUCK2_CR_PSWDRV_BM         PMUC_BUCK2_CR_PSWDRV_BM_Msk
#define PMUC_BUCK2_CR_UVLO_X_BIAS_Pos   (29U)
#define PMUC_BUCK2_CR_UVLO_X_BIAS_Msk   (0x1UL << PMUC_BUCK2_CR_UVLO_X_BIAS_Pos)
#define PMUC_BUCK2_CR_UVLO_X_BIAS       PMUC_BUCK2_CR_UVLO_X_BIAS_Msk
#define PMUC_BUCK2_CR_SS_DONE_Pos       (31U)
#define PMUC_BUCK2_CR_SS_DONE_Msk       (0x1UL << PMUC_BUCK2_CR_SS_DONE_Pos)
#define PMUC_BUCK2_CR_SS_DONE           PMUC_BUCK2_CR_SS_DONE_Msk

/***************** Bit definition for PMUC_HPSYS_LDO register *****************/
#define PMUC_HPSYS_LDO_EN_Pos           (0U)
#define PMUC_HPSYS_LDO_EN_Msk           (0x1UL << PMUC_HPSYS_LDO_EN_Pos)
#define PMUC_HPSYS_LDO_EN               PMUC_HPSYS_LDO_EN_Msk
#define PMUC_HPSYS_LDO_PD_OUT_Pos       (1U)
#define PMUC_HPSYS_LDO_PD_OUT_Msk       (0x1UL << PMUC_HPSYS_LDO_PD_OUT_Pos)
#define PMUC_HPSYS_LDO_PD_OUT           PMUC_HPSYS_LDO_PD_OUT_Msk
#define PMUC_HPSYS_LDO_LS_CONFIG_Pos    (2U)
#define PMUC_HPSYS_LDO_LS_CONFIG_Msk    (0x1UL << PMUC_HPSYS_LDO_LS_CONFIG_Pos)
#define PMUC_HPSYS_LDO_LS_CONFIG        PMUC_HPSYS_LDO_LS_CONFIG_Msk
#define PMUC_HPSYS_LDO_SET_ISTART_Pos   (4U)
#define PMUC_HPSYS_LDO_SET_ISTART_Msk   (0x7UL << PMUC_HPSYS_LDO_SET_ISTART_Pos)
#define PMUC_HPSYS_LDO_SET_ISTART       PMUC_HPSYS_LDO_SET_ISTART_Msk
#define PMUC_HPSYS_LDO_ISTART_OFF_Pos   (7U)
#define PMUC_HPSYS_LDO_ISTART_OFF_Msk   (0x1UL << PMUC_HPSYS_LDO_ISTART_OFF_Pos)
#define PMUC_HPSYS_LDO_ISTART_OFF       PMUC_HPSYS_LDO_ISTART_OFF_Msk
#define PMUC_HPSYS_LDO_VREF_Pos         (8U)
#define PMUC_HPSYS_LDO_VREF_Msk         (0xFUL << PMUC_HPSYS_LDO_VREF_Pos)
#define PMUC_HPSYS_LDO_VREF             PMUC_HPSYS_LDO_VREF_Msk
#define PMUC_HPSYS_LDO_VREF2_Pos        (12U)
#define PMUC_HPSYS_LDO_VREF2_Msk        (0xFUL << PMUC_HPSYS_LDO_VREF2_Pos)
#define PMUC_HPSYS_LDO_VREF2            PMUC_HPSYS_LDO_VREF2_Msk
#define PMUC_HPSYS_LDO_DLY_Pos          (16U)
#define PMUC_HPSYS_LDO_DLY_Msk          (0xFUL << PMUC_HPSYS_LDO_DLY_Pos)
#define PMUC_HPSYS_LDO_DLY              PMUC_HPSYS_LDO_DLY_Msk
#define PMUC_HPSYS_LDO_RDY_Pos          (31U)
#define PMUC_HPSYS_LDO_RDY_Msk          (0x1UL << PMUC_HPSYS_LDO_RDY_Pos)
#define PMUC_HPSYS_LDO_RDY              PMUC_HPSYS_LDO_RDY_Msk

/***************** Bit definition for PMUC_LPSYS_LDO register *****************/
#define PMUC_LPSYS_LDO_EN_Pos           (0U)
#define PMUC_LPSYS_LDO_EN_Msk           (0x1UL << PMUC_LPSYS_LDO_EN_Pos)
#define PMUC_LPSYS_LDO_EN               PMUC_LPSYS_LDO_EN_Msk
#define PMUC_LPSYS_LDO_PD_OUT_Pos       (1U)
#define PMUC_LPSYS_LDO_PD_OUT_Msk       (0x1UL << PMUC_LPSYS_LDO_PD_OUT_Pos)
#define PMUC_LPSYS_LDO_PD_OUT           PMUC_LPSYS_LDO_PD_OUT_Msk
#define PMUC_LPSYS_LDO_LS_CONFIG_Pos    (2U)
#define PMUC_LPSYS_LDO_LS_CONFIG_Msk    (0x1UL << PMUC_LPSYS_LDO_LS_CONFIG_Pos)
#define PMUC_LPSYS_LDO_LS_CONFIG        PMUC_LPSYS_LDO_LS_CONFIG_Msk
#define PMUC_LPSYS_LDO_SET_ISTART_Pos   (4U)
#define PMUC_LPSYS_LDO_SET_ISTART_Msk   (0x7UL << PMUC_LPSYS_LDO_SET_ISTART_Pos)
#define PMUC_LPSYS_LDO_SET_ISTART       PMUC_LPSYS_LDO_SET_ISTART_Msk
#define PMUC_LPSYS_LDO_ISTART_OFF_Pos   (7U)
#define PMUC_LPSYS_LDO_ISTART_OFF_Msk   (0x1UL << PMUC_LPSYS_LDO_ISTART_OFF_Pos)
#define PMUC_LPSYS_LDO_ISTART_OFF       PMUC_LPSYS_LDO_ISTART_OFF_Msk
#define PMUC_LPSYS_LDO_VREF_Pos         (8U)
#define PMUC_LPSYS_LDO_VREF_Msk         (0xFUL << PMUC_LPSYS_LDO_VREF_Pos)
#define PMUC_LPSYS_LDO_VREF             PMUC_LPSYS_LDO_VREF_Msk
#define PMUC_LPSYS_LDO_VREF2_Pos        (12U)
#define PMUC_LPSYS_LDO_VREF2_Msk        (0xFUL << PMUC_LPSYS_LDO_VREF2_Pos)
#define PMUC_LPSYS_LDO_VREF2            PMUC_LPSYS_LDO_VREF2_Msk
#define PMUC_LPSYS_LDO_DLY_Pos          (16U)
#define PMUC_LPSYS_LDO_DLY_Msk          (0xFUL << PMUC_LPSYS_LDO_DLY_Pos)
#define PMUC_LPSYS_LDO_DLY              PMUC_LPSYS_LDO_DLY_Msk
#define PMUC_LPSYS_LDO_PSWDLY_Pos       (20U)
#define PMUC_LPSYS_LDO_PSWDLY_Msk       (0xFUL << PMUC_LPSYS_LDO_PSWDLY_Pos)
#define PMUC_LPSYS_LDO_PSWDLY           PMUC_LPSYS_LDO_PSWDLY_Msk
#define PMUC_LPSYS_LDO_RDY_Pos          (31U)
#define PMUC_LPSYS_LDO_RDY_Msk          (0x1UL << PMUC_LPSYS_LDO_RDY_Pos)
#define PMUC_LPSYS_LDO_RDY              PMUC_LPSYS_LDO_RDY_Msk

/****************** Bit definition for PMUC_PMU_TR register *******************/
#define PMUC_PMU_TR_PMU_DC_TR_Pos       (0U)
#define PMUC_PMU_TR_PMU_DC_TR_Msk       (0x7UL << PMUC_PMU_TR_PMU_DC_TR_Pos)
#define PMUC_PMU_TR_PMU_DC_TR           PMUC_PMU_TR_PMU_DC_TR_Msk
#define PMUC_PMU_TR_PMU_DC_BR_Pos       (3U)
#define PMUC_PMU_TR_PMU_DC_BR_Msk       (0x7UL << PMUC_PMU_TR_PMU_DC_BR_Pos)
#define PMUC_PMU_TR_PMU_DC_BR           PMUC_PMU_TR_PMU_DC_BR_Msk
#define PMUC_PMU_TR_PMU_DC_MR_Pos       (6U)
#define PMUC_PMU_TR_PMU_DC_MR_Msk       (0x7UL << PMUC_PMU_TR_PMU_DC_MR_Pos)
#define PMUC_PMU_TR_PMU_DC_MR           PMUC_PMU_TR_PMU_DC_MR_Msk

/***************** Bit definition for PMUC_PMU_RSVD register ******************/
#define PMUC_PMU_RSVD_RESERVE0_Pos      (0U)
#define PMUC_PMU_RSVD_RESERVE0_Msk      (0xFFUL << PMUC_PMU_RSVD_RESERVE0_Pos)
#define PMUC_PMU_RSVD_RESERVE0          PMUC_PMU_RSVD_RESERVE0_Msk
#define PMUC_PMU_RSVD_RESERVE1_Pos      (8U)
#define PMUC_PMU_RSVD_RESERVE1_Msk      (0xFFUL << PMUC_PMU_RSVD_RESERVE1_Pos)
#define PMUC_PMU_RSVD_RESERVE1          PMUC_PMU_RSVD_RESERVE1_Msk
#define PMUC_PMU_RSVD_RESERVE2_Pos      (16U)
#define PMUC_PMU_RSVD_RESERVE2_Msk      (0xFFUL << PMUC_PMU_RSVD_RESERVE2_Pos)
#define PMUC_PMU_RSVD_RESERVE2          PMUC_PMU_RSVD_RESERVE2_Msk
#define PMUC_PMU_RSVD_RESERVE3_Pos      (24U)
#define PMUC_PMU_RSVD_RESERVE3_Msk      (0xFFUL << PMUC_PMU_RSVD_RESERVE3_Pos)
#define PMUC_PMU_RSVD_RESERVE3          PMUC_PMU_RSVD_RESERVE3_Msk

/****************** Bit definition for PMUC_HXT_CR1 register ******************/
#define PMUC_HXT_CR1_EN_Pos             (0U)
#define PMUC_HXT_CR1_EN_Msk             (0x1UL << PMUC_HXT_CR1_EN_Pos)
#define PMUC_HXT_CR1_EN                 PMUC_HXT_CR1_EN_Msk
#define PMUC_HXT_CR1_BUF_EN_Pos         (1U)
#define PMUC_HXT_CR1_BUF_EN_Msk         (0x1UL << PMUC_HXT_CR1_BUF_EN_Pos)
#define PMUC_HXT_CR1_BUF_EN             PMUC_HXT_CR1_BUF_EN_Msk
#define PMUC_HXT_CR1_BUF_DIG_EN_Pos     (2U)
#define PMUC_HXT_CR1_BUF_DIG_EN_Msk     (0x1UL << PMUC_HXT_CR1_BUF_DIG_EN_Pos)
#define PMUC_HXT_CR1_BUF_DIG_EN         PMUC_HXT_CR1_BUF_DIG_EN_Msk
#define PMUC_HXT_CR1_BUF_DIG_STR_Pos    (3U)
#define PMUC_HXT_CR1_BUF_DIG_STR_Msk    (0x3UL << PMUC_HXT_CR1_BUF_DIG_STR_Pos)
#define PMUC_HXT_CR1_BUF_DIG_STR        PMUC_HXT_CR1_BUF_DIG_STR_Msk
#define PMUC_HXT_CR1_BUF_DLL_EN_Pos     (5U)
#define PMUC_HXT_CR1_BUF_DLL_EN_Msk     (0x1UL << PMUC_HXT_CR1_BUF_DLL_EN_Pos)
#define PMUC_HXT_CR1_BUF_DLL_EN         PMUC_HXT_CR1_BUF_DLL_EN_Msk
#define PMUC_HXT_CR1_BUF_DLL_STR_Pos    (6U)
#define PMUC_HXT_CR1_BUF_DLL_STR_Msk    (0x3UL << PMUC_HXT_CR1_BUF_DLL_STR_Pos)
#define PMUC_HXT_CR1_BUF_DLL_STR        PMUC_HXT_CR1_BUF_DLL_STR_Msk
#define PMUC_HXT_CR1_BUF_AUD_EN_Pos     (8U)
#define PMUC_HXT_CR1_BUF_AUD_EN_Msk     (0x1UL << PMUC_HXT_CR1_BUF_AUD_EN_Pos)
#define PMUC_HXT_CR1_BUF_AUD_EN         PMUC_HXT_CR1_BUF_AUD_EN_Msk
#define PMUC_HXT_CR1_BUF_AUD_STR_Pos    (9U)
#define PMUC_HXT_CR1_BUF_AUD_STR_Msk    (0x3UL << PMUC_HXT_CR1_BUF_AUD_STR_Pos)
#define PMUC_HXT_CR1_BUF_AUD_STR        PMUC_HXT_CR1_BUF_AUD_STR_Msk
#define PMUC_HXT_CR1_BUF_RF_STR_Pos     (11U)
#define PMUC_HXT_CR1_BUF_RF_STR_Msk     (0x3UL << PMUC_HXT_CR1_BUF_RF_STR_Pos)
#define PMUC_HXT_CR1_BUF_RF_STR         PMUC_HXT_CR1_BUF_RF_STR_Msk
#define PMUC_HXT_CR1_LDO_VREF_Pos       (13U)
#define PMUC_HXT_CR1_LDO_VREF_Msk       (0xFUL << PMUC_HXT_CR1_LDO_VREF_Pos)
#define PMUC_HXT_CR1_LDO_VREF           PMUC_HXT_CR1_LDO_VREF_Msk
#define PMUC_HXT_CR1_LDO_FLT_RSEL_Pos   (17U)
#define PMUC_HXT_CR1_LDO_FLT_RSEL_Msk   (0x3UL << PMUC_HXT_CR1_LDO_FLT_RSEL_Pos)
#define PMUC_HXT_CR1_LDO_FLT_RSEL       PMUC_HXT_CR1_LDO_FLT_RSEL_Msk
#define PMUC_HXT_CR1_GM_EN_Pos          (19U)
#define PMUC_HXT_CR1_GM_EN_Msk          (0x1UL << PMUC_HXT_CR1_GM_EN_Pos)
#define PMUC_HXT_CR1_GM_EN              PMUC_HXT_CR1_GM_EN_Msk
#define PMUC_HXT_CR1_CBANK_SEL_Pos      (20U)
#define PMUC_HXT_CR1_CBANK_SEL_Msk      (0x3FFUL << PMUC_HXT_CR1_CBANK_SEL_Pos)
#define PMUC_HXT_CR1_CBANK_SEL          PMUC_HXT_CR1_CBANK_SEL_Msk

/****************** Bit definition for PMUC_HXT_CR2 register ******************/
#define PMUC_HXT_CR2_AGC_EN_Pos         (0U)
#define PMUC_HXT_CR2_AGC_EN_Msk         (0x1UL << PMUC_HXT_CR2_AGC_EN_Pos)
#define PMUC_HXT_CR2_AGC_EN             PMUC_HXT_CR2_AGC_EN_Msk
#define PMUC_HXT_CR2_AGC_ISTART_SEL_Pos  (1U)
#define PMUC_HXT_CR2_AGC_ISTART_SEL_Msk  (0x1UL << PMUC_HXT_CR2_AGC_ISTART_SEL_Pos)
#define PMUC_HXT_CR2_AGC_ISTART_SEL     PMUC_HXT_CR2_AGC_ISTART_SEL_Msk
#define PMUC_HXT_CR2_AGC_VTH_Pos        (2U)
#define PMUC_HXT_CR2_AGC_VTH_Msk        (0xFUL << PMUC_HXT_CR2_AGC_VTH_Pos)
#define PMUC_HXT_CR2_AGC_VTH            PMUC_HXT_CR2_AGC_VTH_Msk
#define PMUC_HXT_CR2_AGC_VINDC_Pos      (6U)
#define PMUC_HXT_CR2_AGC_VINDC_Msk      (0x3UL << PMUC_HXT_CR2_AGC_VINDC_Pos)
#define PMUC_HXT_CR2_AGC_VINDC          PMUC_HXT_CR2_AGC_VINDC_Msk
#define PMUC_HXT_CR2_ACBUF_SEL_Pos      (8U)
#define PMUC_HXT_CR2_ACBUF_SEL_Msk      (0x3UL << PMUC_HXT_CR2_ACBUF_SEL_Pos)
#define PMUC_HXT_CR2_ACBUF_SEL          PMUC_HXT_CR2_ACBUF_SEL_Msk
#define PMUC_HXT_CR2_ACBUF_RSEL_Pos     (10U)
#define PMUC_HXT_CR2_ACBUF_RSEL_Msk     (0x1UL << PMUC_HXT_CR2_ACBUF_RSEL_Pos)
#define PMUC_HXT_CR2_ACBUF_RSEL         PMUC_HXT_CR2_ACBUF_RSEL_Msk
#define PMUC_HXT_CR2_BUF_SEL2_Pos       (11U)
#define PMUC_HXT_CR2_BUF_SEL2_Msk       (0x3UL << PMUC_HXT_CR2_BUF_SEL2_Pos)
#define PMUC_HXT_CR2_BUF_SEL2           PMUC_HXT_CR2_BUF_SEL2_Msk
#define PMUC_HXT_CR2_BUF_SEL3_Pos       (13U)
#define PMUC_HXT_CR2_BUF_SEL3_Msk       (0x3UL << PMUC_HXT_CR2_BUF_SEL3_Pos)
#define PMUC_HXT_CR2_BUF_SEL3           PMUC_HXT_CR2_BUF_SEL3_Msk
#define PMUC_HXT_CR2_IDAC_EN_Pos        (15U)
#define PMUC_HXT_CR2_IDAC_EN_Msk        (0x1UL << PMUC_HXT_CR2_IDAC_EN_Pos)
#define PMUC_HXT_CR2_IDAC_EN            PMUC_HXT_CR2_IDAC_EN_Msk
#define PMUC_HXT_CR2_IDAC_Pos           (16U)
#define PMUC_HXT_CR2_IDAC_Msk           (0x3FFUL << PMUC_HXT_CR2_IDAC_Pos)
#define PMUC_HXT_CR2_IDAC               PMUC_HXT_CR2_IDAC_Msk
#define PMUC_HXT_CR2_SDADC_CLKIN_EN_Pos  (26U)
#define PMUC_HXT_CR2_SDADC_CLKIN_EN_Msk  (0x1UL << PMUC_HXT_CR2_SDADC_CLKIN_EN_Pos)
#define PMUC_HXT_CR2_SDADC_CLKIN_EN     PMUC_HXT_CR2_SDADC_CLKIN_EN_Msk
#define PMUC_HXT_CR2_SDADC_CLKDIV1_SEL_Pos  (27U)
#define PMUC_HXT_CR2_SDADC_CLKDIV1_SEL_Msk  (0x3UL << PMUC_HXT_CR2_SDADC_CLKDIV1_SEL_Pos)
#define PMUC_HXT_CR2_SDADC_CLKDIV1_SEL  PMUC_HXT_CR2_SDADC_CLKDIV1_SEL_Msk
#define PMUC_HXT_CR2_SDADC_CLKDIV2_SEL_Pos  (29U)
#define PMUC_HXT_CR2_SDADC_CLKDIV2_SEL_Msk  (0x3UL << PMUC_HXT_CR2_SDADC_CLKDIV2_SEL_Pos)
#define PMUC_HXT_CR2_SDADC_CLKDIV2_SEL  PMUC_HXT_CR2_SDADC_CLKDIV2_SEL_Msk
#define PMUC_HXT_CR2_SLEEP_EN_Pos       (31U)
#define PMUC_HXT_CR2_SLEEP_EN_Msk       (0x1UL << PMUC_HXT_CR2_SLEEP_EN_Pos)
#define PMUC_HXT_CR2_SLEEP_EN           PMUC_HXT_CR2_SLEEP_EN_Msk

/****************** Bit definition for PMUC_HXT_CR3 register ******************/
#define PMUC_HXT_CR3_BUF_DAC_STR_Pos    (0U)
#define PMUC_HXT_CR3_BUF_DAC_STR_Msk    (0x3UL << PMUC_HXT_CR3_BUF_DAC_STR_Pos)
#define PMUC_HXT_CR3_BUF_DAC_STR        PMUC_HXT_CR3_BUF_DAC_STR_Msk
#define PMUC_HXT_CR3_BUF_OSLO_STR_Pos   (2U)
#define PMUC_HXT_CR3_BUF_OSLO_STR_Msk   (0x3UL << PMUC_HXT_CR3_BUF_OSLO_STR_Pos)
#define PMUC_HXT_CR3_BUF_OSLO_STR       PMUC_HXT_CR3_BUF_OSLO_STR_Msk
#define PMUC_HXT_CR3_DLY_Pos            (4U)
#define PMUC_HXT_CR3_DLY_Msk            (0x3FUL << PMUC_HXT_CR3_DLY_Pos)
#define PMUC_HXT_CR3_DLY                PMUC_HXT_CR3_DLY_Msk

/****************** Bit definition for PMUC_HRC_CR register *******************/
#define PMUC_HRC_CR_EN_Pos              (0U)
#define PMUC_HRC_CR_EN_Msk              (0x1UL << PMUC_HRC_CR_EN_Pos)
#define PMUC_HRC_CR_EN                  PMUC_HRC_CR_EN_Msk
#define PMUC_HRC_CR_LDO_VREF_Pos        (1U)
#define PMUC_HRC_CR_LDO_VREF_Msk        (0xFUL << PMUC_HRC_CR_LDO_VREF_Pos)
#define PMUC_HRC_CR_LDO_VREF            PMUC_HRC_CR_LDO_VREF_Msk
#define PMUC_HRC_CR_FREQ_TRIM_Pos       (5U)
#define PMUC_HRC_CR_FREQ_TRIM_Msk       (0x7FFUL << PMUC_HRC_CR_FREQ_TRIM_Pos)
#define PMUC_HRC_CR_FREQ_TRIM           PMUC_HRC_CR_FREQ_TRIM_Msk
#define PMUC_HRC_CR_TEMP_TRIM_Pos       (16U)
#define PMUC_HRC_CR_TEMP_TRIM_Msk       (0x7UL << PMUC_HRC_CR_TEMP_TRIM_Pos)
#define PMUC_HRC_CR_TEMP_TRIM           PMUC_HRC_CR_TEMP_TRIM_Msk
#define PMUC_HRC_CR_IDWN_SEL_Pos        (19U)
#define PMUC_HRC_CR_IDWN_SEL_Msk        (0x3UL << PMUC_HRC_CR_IDWN_SEL_Pos)
#define PMUC_HRC_CR_IDWN_SEL            PMUC_HRC_CR_IDWN_SEL_Msk
#define PMUC_HRC_CR_CLK96M_EN_Pos       (21U)
#define PMUC_HRC_CR_CLK96M_EN_Msk       (0x1UL << PMUC_HRC_CR_CLK96M_EN_Pos)
#define PMUC_HRC_CR_CLK96M_EN           PMUC_HRC_CR_CLK96M_EN_Msk
#define PMUC_HRC_CR_CLKHP_EN_Pos        (22U)
#define PMUC_HRC_CR_CLKHP_EN_Msk        (0x1UL << PMUC_HRC_CR_CLKHP_EN_Pos)
#define PMUC_HRC_CR_CLKHP_EN            PMUC_HRC_CR_CLKHP_EN_Msk
#define PMUC_HRC_CR_CLKHP_SEL_Pos       (23U)
#define PMUC_HRC_CR_CLKHP_SEL_Msk       (0x3UL << PMUC_HRC_CR_CLKHP_SEL_Pos)
#define PMUC_HRC_CR_CLKHP_SEL           PMUC_HRC_CR_CLKHP_SEL_Msk
#define PMUC_HRC_CR_CLKHP_STR_Pos       (25U)
#define PMUC_HRC_CR_CLKHP_STR_Msk       (0x3UL << PMUC_HRC_CR_CLKHP_STR_Pos)
#define PMUC_HRC_CR_CLKHP_STR           PMUC_HRC_CR_CLKHP_STR_Msk
#define PMUC_HRC_CR_CLKLP_EN_Pos        (27U)
#define PMUC_HRC_CR_CLKLP_EN_Msk        (0x1UL << PMUC_HRC_CR_CLKLP_EN_Pos)
#define PMUC_HRC_CR_CLKLP_EN            PMUC_HRC_CR_CLKLP_EN_Msk
#define PMUC_HRC_CR_CLKLP_SEL_Pos       (28U)
#define PMUC_HRC_CR_CLKLP_SEL_Msk       (0x3UL << PMUC_HRC_CR_CLKLP_SEL_Pos)
#define PMUC_HRC_CR_CLKLP_SEL           PMUC_HRC_CR_CLKLP_SEL_Msk
#define PMUC_HRC_CR_CLKLP_STR_Pos       (30U)
#define PMUC_HRC_CR_CLKLP_STR_Msk       (0x3UL << PMUC_HRC_CR_CLKLP_STR_Pos)
#define PMUC_HRC_CR_CLKLP_STR           PMUC_HRC_CR_CLKLP_STR_Msk

/****************** Bit definition for PMUC_CAU_BGR register ******************/
#define PMUC_CAU_BGR_HPBG_VDDPSW_EN_Pos  (0U)
#define PMUC_CAU_BGR_HPBG_VDDPSW_EN_Msk  (0x1UL << PMUC_CAU_BGR_HPBG_VDDPSW_EN_Pos)
#define PMUC_CAU_BGR_HPBG_VDDPSW_EN     PMUC_CAU_BGR_HPBG_VDDPSW_EN_Msk
#define PMUC_CAU_BGR_HPBG_EN_Pos        (1U)
#define PMUC_CAU_BGR_HPBG_EN_Msk        (0x1UL << PMUC_CAU_BGR_HPBG_EN_Pos)
#define PMUC_CAU_BGR_HPBG_EN            PMUC_CAU_BGR_HPBG_EN_Msk
#define PMUC_CAU_BGR_LPBG_EN_Pos        (2U)
#define PMUC_CAU_BGR_LPBG_EN_Msk        (0x1UL << PMUC_CAU_BGR_LPBG_EN_Pos)
#define PMUC_CAU_BGR_LPBG_EN            PMUC_CAU_BGR_LPBG_EN_Msk
#define PMUC_CAU_BGR_LPBG_VREF06_Pos    (3U)
#define PMUC_CAU_BGR_LPBG_VREF06_Msk    (0xFUL << PMUC_CAU_BGR_LPBG_VREF06_Pos)
#define PMUC_CAU_BGR_LPBG_VREF06        PMUC_CAU_BGR_LPBG_VREF06_Msk
#define PMUC_CAU_BGR_LPBG_VREF12_Pos    (7U)
#define PMUC_CAU_BGR_LPBG_VREF12_Msk    (0xFUL << PMUC_CAU_BGR_LPBG_VREF12_Pos)
#define PMUC_CAU_BGR_LPBG_VREF12        PMUC_CAU_BGR_LPBG_VREF12_Msk
#define PMUC_CAU_BGR_LPBG_DLY_Pos       (11U)
#define PMUC_CAU_BGR_LPBG_DLY_Msk       (0x1UL << PMUC_CAU_BGR_LPBG_DLY_Pos)
#define PMUC_CAU_BGR_LPBG_DLY           PMUC_CAU_BGR_LPBG_DLY_Msk

/****************** Bit definition for PMUC_CAU_TR register *******************/
#define PMUC_CAU_TR_CAU_DC_TR_Pos       (0U)
#define PMUC_CAU_TR_CAU_DC_TR_Msk       (0x7UL << PMUC_CAU_TR_CAU_DC_TR_Pos)
#define PMUC_CAU_TR_CAU_DC_TR           PMUC_CAU_TR_CAU_DC_TR_Msk
#define PMUC_CAU_TR_CAU_DC_BR_Pos       (3U)
#define PMUC_CAU_TR_CAU_DC_BR_Msk       (0x7UL << PMUC_CAU_TR_CAU_DC_BR_Pos)
#define PMUC_CAU_TR_CAU_DC_BR           PMUC_CAU_TR_CAU_DC_BR_Msk
#define PMUC_CAU_TR_CAU_DC_MR_Pos       (6U)
#define PMUC_CAU_TR_CAU_DC_MR_Msk       (0x7UL << PMUC_CAU_TR_CAU_DC_MR_Pos)
#define PMUC_CAU_TR_CAU_DC_MR           PMUC_CAU_TR_CAU_DC_MR_Msk

/***************** Bit definition for PMUC_CAU_RSVD register ******************/
#define PMUC_CAU_RSVD_RESERVE0_Pos      (0U)
#define PMUC_CAU_RSVD_RESERVE0_Msk      (0xFFUL << PMUC_CAU_RSVD_RESERVE0_Pos)
#define PMUC_CAU_RSVD_RESERVE0          PMUC_CAU_RSVD_RESERVE0_Msk
#define PMUC_CAU_RSVD_RESERVE1_Pos      (8U)
#define PMUC_CAU_RSVD_RESERVE1_Msk      (0xFFUL << PMUC_CAU_RSVD_RESERVE1_Pos)
#define PMUC_CAU_RSVD_RESERVE1          PMUC_CAU_RSVD_RESERVE1_Msk

/***************** Bit definition for PMUC_WKUP_CNT register ******************/
#define PMUC_WKUP_CNT_PIN0_CNT_Pos      (0U)
#define PMUC_WKUP_CNT_PIN0_CNT_Msk      (0xFFFFUL << PMUC_WKUP_CNT_PIN0_CNT_Pos)
#define PMUC_WKUP_CNT_PIN0_CNT          PMUC_WKUP_CNT_PIN0_CNT_Msk
#define PMUC_WKUP_CNT_PIN1_CNT_Pos      (16U)
#define PMUC_WKUP_CNT_PIN1_CNT_Msk      (0xFFFFUL << PMUC_WKUP_CNT_PIN1_CNT_Pos)
#define PMUC_WKUP_CNT_PIN1_CNT          PMUC_WKUP_CNT_PIN1_CNT_Msk

/**************** Bit definition for PMUC_PWRKEY_CNT register *****************/
#define PMUC_PWRKEY_CNT_RST_CNT_Pos     (4U)
#define PMUC_PWRKEY_CNT_RST_CNT_Msk     (0xFFFFUL << PMUC_PWRKEY_CNT_RST_CNT_Pos)
#define PMUC_PWRKEY_CNT_RST_CNT         PMUC_PWRKEY_CNT_RST_CNT_Msk

/**************** Bit definition for PMUC_HPSYS_VOUT register *****************/
#define PMUC_HPSYS_VOUT_VOUT_Pos        (0U)
#define PMUC_HPSYS_VOUT_VOUT_Msk        (0xFUL << PMUC_HPSYS_VOUT_VOUT_Pos)
#define PMUC_HPSYS_VOUT_VOUT            PMUC_HPSYS_VOUT_VOUT_Msk

/**************** Bit definition for PMUC_LPSYS_VOUT register *****************/
#define PMUC_LPSYS_VOUT_VOUT_Pos        (0U)
#define PMUC_LPSYS_VOUT_VOUT_Msk        (0xFUL << PMUC_LPSYS_VOUT_VOUT_Pos)
#define PMUC_LPSYS_VOUT_VOUT            PMUC_LPSYS_VOUT_VOUT_Msk

/***************** Bit definition for PMUC_BUCK_VOUT register *****************/
#define PMUC_BUCK_VOUT_VOUT_Pos         (0U)
#define PMUC_BUCK_VOUT_VOUT_Msk         (0x1FUL << PMUC_BUCK_VOUT_VOUT_Pos)
#define PMUC_BUCK_VOUT_VOUT             PMUC_BUCK_VOUT_VOUT_Msk

/**************** Bit definition for PMUC_BUCK2_VOUT register *****************/
#define PMUC_BUCK2_VOUT_VOUT_Pos        (0U)
#define PMUC_BUCK2_VOUT_VOUT_Msk        (0x1FUL << PMUC_BUCK2_VOUT_VOUT_Pos)
#define PMUC_BUCK2_VOUT_VOUT            PMUC_BUCK2_VOUT_VOUT_Msk

#endif
