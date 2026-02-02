// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _VOLTMON_CFG_H_ // Ceedling-generated include guard
#define _VOLTMON_CFG_H_

typedef struct {
  uint16_t rawMax_u16;
  uint16_t factor_u16;
  int16_t offset_s16;
  uint16_t uvTh_mV_u16;
  uint16_t ovTh_mV_u16;
  uint16_t hyst_mV_u16;
} VoltMon_cfg_s;
const VoltMon_cfg_s *VoltMon_CfgGet_pcfg(void);

#endif // _VOLTMON_CFG_H_
