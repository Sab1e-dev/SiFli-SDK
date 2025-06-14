#ifndef __FFT_H
#define __FFT_H

typedef struct
{
    __IO uint32_t CR;
    __IO uint32_t SAR;
    __IO uint32_t DAR;
    __IO uint32_t WAR;
    __IO uint32_t SCLR1;
    __IO uint32_t SCLR2;
    __IO uint32_t BFR1;
    __IO uint32_t BFR2;
    __IO uint32_t IRQ;
} FFT_TypeDef;


/********************* Bit definition for FFT_CR register *********************/
#define FFT_CR_START_Pos                (0U)
#define FFT_CR_START_Msk                (0x1UL << FFT_CR_START_Pos)
#define FFT_CR_START                    FFT_CR_START_Msk
#define FFT_CR_FFT_MODE_Pos             (1U)
#define FFT_CR_FFT_MODE_Msk             (0x1UL << FFT_CR_FFT_MODE_Pos)
#define FFT_CR_FFT_MODE                 FFT_CR_FFT_MODE_Msk
#define FFT_CR_FFT_LENGTH_Pos           (2U)
#define FFT_CR_FFT_LENGTH_Msk           (0x7UL << FFT_CR_FFT_LENGTH_Pos)
#define FFT_CR_FFT_LENGTH               FFT_CR_FFT_LENGTH_Msk
#define FFT_CR_INPUT_BW_SEL_Pos         (5U)
#define FFT_CR_INPUT_BW_SEL_Msk         (0x3UL << FFT_CR_INPUT_BW_SEL_Pos)
#define FFT_CR_INPUT_BW_SEL             FFT_CR_INPUT_BW_SEL_Msk
#define FFT_CR_INPUT_REAL_Pos           (7U)
#define FFT_CR_INPUT_REAL_Msk           (0x1UL << FFT_CR_INPUT_REAL_Pos)
#define FFT_CR_INPUT_REAL               FFT_CR_INPUT_REAL_Msk
#define FFT_CR_OUTPUT_BW_SEL_Pos        (8U)
#define FFT_CR_OUTPUT_BW_SEL_Msk        (0x3UL << FFT_CR_OUTPUT_BW_SEL_Pos)
#define FFT_CR_OUTPUT_BW_SEL            FFT_CR_OUTPUT_BW_SEL_Msk
#define FFT_CR_OUTPUT_ALIGN_Pos         (10U)
#define FFT_CR_OUTPUT_ALIGN_Msk         (0x1UL << FFT_CR_OUTPUT_ALIGN_Pos)
#define FFT_CR_OUTPUT_ALIGN             FFT_CR_OUTPUT_ALIGN_Msk
#define FFT_CR_OUTPUT_REAL_Pos          (11U)
#define FFT_CR_OUTPUT_REAL_Msk          (0x1UL << FFT_CR_OUTPUT_REAL_Pos)
#define FFT_CR_OUTPUT_REAL              FFT_CR_OUTPUT_REAL_Msk
#define FFT_CR_FFT_WIN_EN_Pos           (12U)
#define FFT_CR_FFT_WIN_EN_Msk           (0x1UL << FFT_CR_FFT_WIN_EN_Pos)
#define FFT_CR_FFT_WIN_EN               FFT_CR_FFT_WIN_EN_Msk
#define FFT_CR_FFT_WIN_REAL_Pos         (13U)
#define FFT_CR_FFT_WIN_REAL_Msk         (0x1UL << FFT_CR_FFT_WIN_REAL_Pos)
#define FFT_CR_FFT_WIN_REAL             FFT_CR_FFT_WIN_REAL_Msk
#define FFT_CR_FFT_WIN_BW_SEL_Pos       (14U)
#define FFT_CR_FFT_WIN_BW_SEL_Msk       (0x3UL << FFT_CR_FFT_WIN_BW_SEL_Pos)
#define FFT_CR_FFT_WIN_BW_SEL           FFT_CR_FFT_WIN_BW_SEL_Msk
#define FFT_CR_FFT_BF_EN_Pos            (16U)
#define FFT_CR_FFT_BF_EN_Msk            (0x1UL << FFT_CR_FFT_BF_EN_Pos)
#define FFT_CR_FFT_BF_EN                FFT_CR_FFT_BF_EN_Msk
#define FFT_CR_SCALE_EN_Pos             (17U)
#define FFT_CR_SCALE_EN_Msk             (0x1UL << FFT_CR_SCALE_EN_Pos)
#define FFT_CR_SCALE_EN                 FFT_CR_SCALE_EN_Msk
#define FFT_CR_FORCE_RSTB_Pos           (18U)
#define FFT_CR_FORCE_RSTB_Msk           (0x1UL << FFT_CR_FORCE_RSTB_Pos)
#define FFT_CR_FORCE_RSTB               FFT_CR_FORCE_RSTB_Msk
#define FFT_CR_FSM_HALT_EN_Pos          (19U)
#define FFT_CR_FSM_HALT_EN_Msk          (0x1UL << FFT_CR_FSM_HALT_EN_Pos)
#define FFT_CR_FSM_HALT_EN              FFT_CR_FSM_HALT_EN_Msk
#define FFT_CR_FSM_HALT_STATE_Pos       (20U)
#define FFT_CR_FSM_HALT_STATE_Msk       (0xFUL << FFT_CR_FSM_HALT_STATE_Pos)
#define FFT_CR_FSM_HALT_STATE           FFT_CR_FSM_HALT_STATE_Msk
#define FFT_CR_FSM_GO_Pos               (24U)
#define FFT_CR_FSM_GO_Msk               (0x1UL << FFT_CR_FSM_GO_Pos)
#define FFT_CR_FSM_GO                   FFT_CR_FSM_GO_Msk

/******************** Bit definition for FFT_SAR register *********************/
#define FFT_SAR_INPUT_ADDR_Pos          (0U)
#define FFT_SAR_INPUT_ADDR_Msk          (0xFFFFFFFFUL << FFT_SAR_INPUT_ADDR_Pos)
#define FFT_SAR_INPUT_ADDR              FFT_SAR_INPUT_ADDR_Msk

/******************** Bit definition for FFT_DAR register *********************/
#define FFT_DAR_OUTPUT_ADDR_Pos         (0U)
#define FFT_DAR_OUTPUT_ADDR_Msk         (0xFFFFFFFFUL << FFT_DAR_OUTPUT_ADDR_Pos)
#define FFT_DAR_OUTPUT_ADDR             FFT_DAR_OUTPUT_ADDR_Msk

/******************** Bit definition for FFT_WAR register *********************/
#define FFT_WAR_WINDOW_ADDR_Pos         (0U)
#define FFT_WAR_WINDOW_ADDR_Msk         (0xFFFFFFFFUL << FFT_WAR_WINDOW_ADDR_Pos)
#define FFT_WAR_WINDOW_ADDR             FFT_WAR_WINDOW_ADDR_Msk

/******************* Bit definition for FFT_SCLR1 register ********************/
#define FFT_SCLR1_SCALE_REAL_Pos        (0U)
#define FFT_SCLR1_SCALE_REAL_Msk        (0xFFFFFFUL << FFT_SCLR1_SCALE_REAL_Pos)
#define FFT_SCLR1_SCALE_REAL            FFT_SCLR1_SCALE_REAL_Msk

/******************* Bit definition for FFT_SCLR2 register ********************/
#define FFT_SCLR2_SCALE_IMG_Pos         (0U)
#define FFT_SCLR2_SCALE_IMG_Msk         (0xFFFFFFUL << FFT_SCLR2_SCALE_IMG_Pos)
#define FFT_SCLR2_SCALE_IMG             FFT_SCLR2_SCALE_IMG_Msk

/******************** Bit definition for FFT_BFR1 register ********************/
#define FFT_BFR1_FFT_BF_THD1_Pos        (0U)
#define FFT_BFR1_FFT_BF_THD1_Msk        (0xFFFFFFUL << FFT_BFR1_FFT_BF_THD1_Pos)
#define FFT_BFR1_FFT_BF_THD1            FFT_BFR1_FFT_BF_THD1_Msk

/******************** Bit definition for FFT_BFR2 register ********************/
#define FFT_BFR2_FFT_BF_THD2_Pos        (0U)
#define FFT_BFR2_FFT_BF_THD2_Msk        (0xFFFFFFUL << FFT_BFR2_FFT_BF_THD2_Pos)
#define FFT_BFR2_FFT_BF_THD2            FFT_BFR2_FFT_BF_THD2_Msk
#define FFT_BFR2_FFT_TOTAL_SHFT_Pos     (24U)
#define FFT_BFR2_FFT_TOTAL_SHFT_Msk     (0xFUL << FFT_BFR2_FFT_TOTAL_SHFT_Pos)
#define FFT_BFR2_FFT_TOTAL_SHFT         FFT_BFR2_FFT_TOTAL_SHFT_Msk

/******************** Bit definition for FFT_IRQ register *********************/
#define FFT_IRQ_ISR_Pos                 (0U)
#define FFT_IRQ_ISR_Msk                 (0x3UL << FFT_IRQ_ISR_Pos)
#define FFT_IRQ_ISR                     FFT_IRQ_ISR_Msk
#define FFT_IRQ_IRSR_Pos                (2U)
#define FFT_IRQ_IRSR_Msk                (0x3UL << FFT_IRQ_IRSR_Pos)
#define FFT_IRQ_IRSR                    FFT_IRQ_IRSR_Msk
#define FFT_IRQ_ICR_Pos                 (4U)
#define FFT_IRQ_ICR_Msk                 (0x1UL << FFT_IRQ_ICR_Pos)
#define FFT_IRQ_ICR                     FFT_IRQ_ICR_Msk
#define FFT_IRQ_IMR_Pos                 (5U)
#define FFT_IRQ_IMR_Msk                 (0x3UL << FFT_IRQ_IMR_Pos)
#define FFT_IRQ_IMR                     FFT_IRQ_IMR_Msk
#define FFT_IRQ_BUSY_Pos                (7U)
#define FFT_IRQ_BUSY_Msk                (0x1UL << FFT_IRQ_BUSY_Pos)
#define FFT_IRQ_BUSY                    FFT_IRQ_BUSY_Msk

#endif
