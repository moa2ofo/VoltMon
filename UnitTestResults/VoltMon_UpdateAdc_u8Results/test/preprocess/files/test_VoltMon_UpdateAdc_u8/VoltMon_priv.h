// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _VOLTMON_PRIV_H_ // Ceedling-generated include guard
#define _VOLTMON_PRIV_H_

#include "VoltMon_cfg.h"

uint16_t ComputeVoltage_u16(uint16_t rawAdc_u16, const VoltMon_cfg_s * cfg_pcs);
uint8_t CheckThresholds_u8(uint16_t voltage_mV_u16,
                                  const VoltMon_cfg_s * cfg_pcs,

                                 _Bool
                                      * uvActive_pb,

                                 _Bool
                                      * ovActive_pb);
void UpdateStatusFlags_v(
                        _Bool
                             uvActive_b,
                                         _Bool
                                              ovActive_b,
                                                          _Bool
                                                               errSticky_b);

#endif // _VOLTMON_PRIV_H_
