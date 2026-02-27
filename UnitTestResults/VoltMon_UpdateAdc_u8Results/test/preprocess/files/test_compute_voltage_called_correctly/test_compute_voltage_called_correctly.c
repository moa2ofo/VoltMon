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

void test_compute_voltage_called_correctly(void)
{
  uint8_t result;
  VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
  uint16_t test_raw = 750;
  uint16_t expected_voltage = 7550;

  VoltMon_SetStatusFlg_u32((0x00000001u));
  callback_cfg_ptr = &cfg;
  callback_computed_voltage = expected_voltage;

  VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
  ComputeVoltage_u16_CMockExpectAndReturn(60, test_raw, &cfg, expected_voltage);

  result = VoltMon_UpdateAdc_u8(test_raw);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0)), (UNITY_INT)(UNITY_UINT8 )((result)), (
 ((void *)0)
 ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((test_raw)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((expected_voltage)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(66), UNITY_DISPLAY_STYLE_UINT16);
}