// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static void reset_all(void)
{
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0u);
  VoltMon_SetLastVoltage_mV_u16(0u);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_SetOvActive_b(
                       0
                            );
}

void setUp(void)
{
  reset_all();
}

void test_VoltMon_UpdateAdc_u8_updates_raw_and_voltage_and_clears_inval_flag_when_input_valid(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u) | (0x00000010u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 2u, .offset_s16 = 10, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(27, &cfg);
  uint16_t rawAdc = 500u;
  uint16_t expectedVoltage = 0;
  ComputeVoltage_u16_CMockExpectAndReturn(30, rawAdc, &cfg, (uint16_t)(rawAdc * cfg.factor_u16 + cfg.offset_s16));
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(32), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(33), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc * cfg.factor_u16 + cfg.offset_s16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(34), UNITY_DISPLAY_STYLE_UINT16);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((((0x00000001u) & ~(0x00000010u)))), (UNITY_INT)(UNITY_UINT32)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_UINT32);
}