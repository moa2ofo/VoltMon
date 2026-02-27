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

void test_Init_NullCfg_Return2_ErrInvalSet(void)
{
  uint8_t result;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_SetLastRawAdc_u16(777);
  VoltMon_SetLastVoltage_mV_u16(666);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue =
                                            ((void *)0)

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(50);

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2)), (UNITY_INT)(UNITY_UINT8 )((result)), (
 ((void *)0)
 ), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(((0x00000001u) | (0x00000002u) | (0x00000010u))), (UNITY_INT)(UNITY_UINT32)((VoltMon_GetStatusFlg_u32())), (
 ((void *)0)
 ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((lastRawBefore)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(65), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((lastVoltageBefore)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(66), UNITY_DISPLAY_STYLE_UINT16);
}