#include "VoltMon.h"
#include "VoltMon_UpdateAdc_u8.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"
#include "unity.h"
#include <stdbool.h>
#include <stdint.h>

static void reset_all(void) {
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0u);
  VoltMon_SetLastVoltage_mV_u16(0u);
  VoltMon_SetUvActive_b(false);
  VoltMon_SetOvActive_b(false);
}

void setUp(void) { reset_all(); }

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_flag_if_not_initialized(void) {
  const VoltMon_cfg_s *dummyCfg = (const VoltMon_cfg_s *)0x1;
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_CfgGet_pcfg_ExpectAndReturn(dummyCfg);
  uint8_t ret = VoltMon_UpdateAdc_u8(100u);
  TEST_ASSERT_EQUAL_UINT8(1u, ret);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_NOT_EQUAL(0u, (status & VOLTMON_STATUS_ERR_U32));
}

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_and_inval_flags_if_cfg_is_null(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_CfgGet_pcfg_ExpectAndReturn(NULL);
  uint8_t ret = VoltMon_UpdateAdc_u8(100u);
  TEST_ASSERT_EQUAL_UINT8(2u, ret);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_NOT_EQUAL(0u, (status & VOLTMON_STATUS_ERR_U32));
  TEST_ASSERT_NOT_EQUAL(0u, (status & VOLTMON_STATUS_INVAL_U32));
}

void test_VoltMon_UpdateAdc_u8_returns_error_and_sets_err_and_inval_flags_if_rawAdc_greater_than_rawMax(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_cfg_s cfg = {.rawMax_u16 = 500u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
  uint16_t rawAdc = 501u;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  TEST_ASSERT_EQUAL_UINT8(2u, ret);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_NOT_EQUAL(0u, (status & VOLTMON_STATUS_ERR_U32));
  TEST_ASSERT_NOT_EQUAL(0u, (status & VOLTMON_STATUS_INVAL_U32));
}

void test_VoltMon_UpdateAdc_u8_updates_raw_and_voltage_and_clears_inval_flag_when_input_valid(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 2u, .offset_s16 = 10, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
  uint16_t rawAdc = 500u;
  uint16_t expectedVoltage = 0;
  ComputeVoltage_u16_ExpectAndReturn(rawAdc, &cfg, (uint16_t)(rawAdc * cfg.factor_u16 + cfg.offset_s16));
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  TEST_ASSERT_EQUAL_UINT8(0u, ret);
  TEST_ASSERT_EQUAL_UINT16(rawAdc, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(rawAdc * cfg.factor_u16 + cfg.offset_s16, VoltMon_GetLastVoltage_mV_u16());
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_EQUAL_UINT32((VOLTMON_STATUS_INIT_U32 & ~VOLTMON_STATUS_INVAL_U32), status);
}

void test_VoltMon_UpdateAdc_u8_accepts_rawAdc_equal_to_rawMax(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
  uint16_t rawAdc = 1000u;
  ComputeVoltage_u16_ExpectAndReturn(rawAdc, &cfg, rawAdc);
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  TEST_ASSERT_EQUAL_UINT8(0u, ret);
  TEST_ASSERT_EQUAL_UINT16(rawAdc, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(rawAdc, VoltMon_GetLastVoltage_mV_u16());
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_EQUAL_UINT32((VOLTMON_STATUS_INIT_U32 & ~VOLTMON_STATUS_INVAL_U32), status);
}

void test_VoltMon_UpdateAdc_u8_accepts_rawAdc_just_below_rawMax(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
  uint16_t rawAdc = 999u;
  ComputeVoltage_u16_ExpectAndReturn(rawAdc, &cfg, rawAdc);
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  TEST_ASSERT_EQUAL_UINT8(0u, ret);
  TEST_ASSERT_EQUAL_UINT16(rawAdc, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(rawAdc, VoltMon_GetLastVoltage_mV_u16());
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_EQUAL_UINT32((VOLTMON_STATUS_INIT_U32 & ~VOLTMON_STATUS_INVAL_U32), status);
}

void test_VoltMon_UpdateAdc_u8_rejects_rawAdc_just_above_rawMax(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_cfg_s cfg = {.rawMax_u16 = 1000u, .factor_u16 = 1u, .offset_s16 = 0, .uvTh_mV_u16 = 0, .ovTh_mV_u16 = 0, .hyst_mV_u16 = 0};
  VoltMon_CfgGet_pcfg_ExpectAndReturn(&cfg);
  uint16_t rawAdc = 1001u;
  uint8_t ret = VoltMon_UpdateAdc_u8(rawAdc);
  TEST_ASSERT_EQUAL_UINT8(2u, ret);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_BIT_HIGH(VOLTMON_STATUS_ERR_U32, status);
  TEST_ASSERT_BIT_HIGH(VOLTMON_STATUS_INVAL_U32, status);
}

void test_VoltMon_UpdateAdc_u8_rejects_when_config_pointer_is_null_even_if_initialized(void) {
  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_CfgGet_pcfg_ExpectAndReturn(NULL);
  uint8_t ret = VoltMon_UpdateAdc_u8(0u);
  TEST_ASSERT_EQUAL_UINT8(2u, ret);
  uint32_t status = VoltMon_GetStatusFlg_u32();
  TEST_ASSERT_BIT_HIGH(VOLTMON_STATUS_ERR_U32, status);
  TEST_ASSERT_BIT_HIGH(VOLTMON_STATUS_INVAL_U32, status);
}