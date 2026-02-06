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
{ reset_all(); }

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_flag_if_not_initialized(void)
{
  const VoltMon_cfg_s *dummyCfg = (const VoltMon_cfg_s *)0x1;
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(25, dummyCfg);
  uint8_t ret = VoltMon_UpdateAdc_u8(100u);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((1u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  do { if (((0u) != ((status & (0x00000002u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((29))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_and_inval_flags_if_cfg_is_null(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(34,
 ((void *)0)
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(100u);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  do { if (((0u) != ((status & (0x00000002u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((38))); } } while (0);
  do { if (((0u) != ((status & (0x00000010u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((39))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_and_inval_flags_if_rawAdc_greater_than_rawMax(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 500u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(45, &cfg);
  uint16_t rawAdc = 501u;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(48), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  do { if (((0u) != ((status & (0x00000002u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((50))); } } while (0);
  do { if (((0u) != ((status & (0x00000010u)))))
{ } else { UnityFail( ((" Expected Not-Equal")), (UNITY_UINT)((51))); } } while (0);
}

void test_VoltMon_UpdateAdc_u8_updates_raw_and_voltage_and_clears_inval_flag_when_input_valid(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u) | (0x00000010u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 2u, .offset_s16 = 10, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(57, &cfg);
  uint16_t rawAdc = 500u;
  uint16_t expectedVoltage = 0;
  ComputeVoltage_u16_CMockExpectAndReturn(60, rawAdc, &cfg, (uint16_t)(rawAdc * cfg.factor_u16 + cfg.offset_s16));
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(62), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(63), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc * cfg.factor_u16 + cfg.offset_s16)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(64), UNITY_DISPLAY_STYLE_UINT16);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((((0x00000001u) & ~(0x00000010u)))), (UNITY_INT)(UNITY_UINT32)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(66), UNITY_DISPLAY_STYLE_UINT32);
}

void test_VoltMon_UpdateAdc_u8_accepts_rawAdc_equal_to_rawMax(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u) | (0x00000010u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(72, &cfg);
  uint16_t rawAdc = 1000u;
  ComputeVoltage_u16_CMockExpectAndReturn(74, rawAdc, &cfg, rawAdc);
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(76), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(77), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(78), UNITY_DISPLAY_STYLE_UINT16);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((((0x00000001u) & ~(0x00000010u)))), (UNITY_INT)(UNITY_UINT32)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(80), UNITY_DISPLAY_STYLE_UINT32);
}

void test_VoltMon_UpdateAdc_u8_accepts_rawAdc_just_below_rawMax(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u) | (0x00000010u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(86, &cfg);
  uint16_t rawAdc = 999u;
  ComputeVoltage_u16_CMockExpectAndReturn(88, rawAdc, &cfg, rawAdc);
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(90), UNITY_DISPLAY_STYLE_UINT8);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastRawAdc_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(91), UNITY_DISPLAY_STYLE_UINT16);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT16)((rawAdc)), (UNITY_INT)(UNITY_UINT16)((VoltMon_GetLastVoltage_mV_u16())), (
 ((void *)0)
 ), (UNITY_UINT)(92), UNITY_DISPLAY_STYLE_UINT16);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((((0x00000001u) & ~(0x00000010u)))), (UNITY_INT)(UNITY_UINT32)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(94), UNITY_DISPLAY_STYLE_UINT32);
}

void test_VoltMon_UpdateAdc_u8_rejects_rawAdc_just_above_rawMax(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(100, &cfg);
  uint16_t rawAdc = 1001u;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(103), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertBits((UNITY_INT)(((UNITY_UINT)1 << ((0x00000002u)))), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(105));
  UnityAssertBits((UNITY_INT)(((UNITY_UINT)1 << ((0x00000010u)))), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(106));
}

void test_VoltMon_UpdateAdc_u8_rejects_when_config_pointer_is_null_even_if_initialized(void)
{
  VoltMon_SetStatusFlg_u32((0x00000001u));
  VoltMon_CfgGet_pcfg_CMockExpectAndReturn(111,
 ((void *)0)
 );
  uint8_t ret = VoltMon_UpdateAdc_u8(0u);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((2u)), (UNITY_INT)(UNITY_UINT8 )((ret)), (
 ((void *)0)
 ), (UNITY_UINT)(113), UNITY_DISPLAY_STYLE_UINT8);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  UnityAssertBits((UNITY_INT)(((UNITY_UINT)1 << ((0x00000002u)))), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(115));
  UnityAssertBits((UNITY_INT)(((UNITY_UINT)1 << ((0x00000010u)))), (UNITY_INT)((UNITY_UINT)(-1)), (UNITY_INT)((status)), (
 ((void *)0)
 ), (UNITY_UINT)(116));
}