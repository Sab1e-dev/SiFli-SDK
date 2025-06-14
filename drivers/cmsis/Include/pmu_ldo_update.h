#ifndef __PMU_LDO_UPDATE__
#define __PMU_LDO_UPDATE__

extern uint32_t pmu_ldo_inc(int inc);
extern void pmu_ldo_recover(uint32_t org);

#endif  //__PMU_LDO_UPDATE__