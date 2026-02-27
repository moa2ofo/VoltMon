// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static const VoltMon_cfg_s *callback_cfg_ptr =
                                              ((void *)0)

static uint16_t callback_computed_voltage = 0;

static const VoltMon_cfg_s *VoltMon_CfgGet_pcfg_Callback(int call_count)
{
  (void)call_count;
  return callback_cfg_ptr;
}

static uint16_t ComputeVoltage_u16_Callback(uint16_t rawAdc_u16, const VoltMon_cfg_s *cfg_pcs, int call_count)
{
  (void)rawAdc_u16;
  (void)cfg_pcs;
  (void)call_count;
  return callback_computed_voltage;
}

void setUp(void)
{
  VoltMon_SetStatusFlg_u32(0);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);
  VoltMon_SetUvActive_b(
                       0
                            );
  VoltMon_SetOvActive_b(
                       0
                            );

  callback_cfg_ptr =
                    ((void *)0)

  callback_computed_voltage = 0;
}

void tearDown(void)
{
}

void test_null_cfg_and_invalid_raw_returns_2(void)
{
  uint8_t result;

  VoltMon_SetStatusFlg_u32((0x00000001u));
  callback_cfg_ptr =
                    ((void *)0)

  VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);

  result = VoltMon_UpdateAdc_u8(
                               (65535)
                                         );

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2)), (UNITY_INT)(UNITY_UINT8 )((result)), (
 ((void *)0)
 ), (UNITY_UINT)(59), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(((0x00000001u) | (0x00000002u) | (0x00000010u))), (UNITY_INT)(UNITY_UINT32)((VoltMon_GetStatusFlg_u32())), (
 ((void *)0)
 ), (UNITY_UINT)(60), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(61), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((0)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(62), UNITY_DISPLAY_STYLE_UINT16);
}