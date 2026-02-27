// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/VoltMon.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_UpdateAdc_u8.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_cfg.h"
#include "utExecutionAndResults/utUnderTest/src/VoltMon_priv.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnValue;

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg(int call_count)
{
  return callback_VoltMon_CfgGet_pcfg_ReturnValue;
}

static uint16_t callback_ComputeVoltage_u16_ReturnValue;

static uint16_t callback_ComputeVoltage_u16(uint16_t rawAdc_u16, const VoltMon_cfg_s *cfg_pcs, int call_count)
{
  return callback_ComputeVoltage_u16_ReturnValue;
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

  callback_VoltMon_CfgGet_pcfg_ReturnValue =
                                            ((void *)0)

  callback_ComputeVoltage_u16_ReturnValue = 0;
}

void tearDown(void)
{
}

void test_Init_ValidCfg_RawMid_Return0_Updated(void)
{
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 100;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32((0x00000001u) | (0x00000010u));
  VoltMon_SetLastRawAdc_u16(111);
  VoltMon_SetLastVoltage_mV_u16(222);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 20480;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(2048);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0)), (UNITY_INT)(UNITY_UINT8 )((result)), (
 ((void *)0)
 ), (UNITY_UINT)(69), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((2048)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(70), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((20480)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(71), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(((0x00000001u))), (UNITY_INT)(UNITY_UINT32)((VoltMon_GetStatusFlg_u32())), (
 ((void *)0)
 ), (UNITY_UINT)(72), UNITY_DISPLAY_STYLE_UINT32);
}