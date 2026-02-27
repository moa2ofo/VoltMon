#include "VoltMon_Init_u8.h"  /* Function under test + Get/Set APIs */
#include "mock_VoltMon_cfg.h" /* Mock: VoltMon_CfgGet_pcfg() */
#include "unity.h"

/* ============================================================================
 * CALLBACKS
 * ========================================================================== */

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnNull(int call_count) {
  return NULL;
}

static VoltMon_cfg_s valid_cfg;

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnValid(int call_count) {
  valid_cfg.rawMax_u16 = 4095u;
  valid_cfg.factor_u16 = 10u;
  valid_cfg.offset_s16 = 0;
  valid_cfg.uvTh_mV_u16 = 9000u;
  valid_cfg.ovTh_mV_u16 = 16000u;
  valid_cfg.hyst_mV_u16 = 500u;
  return &valid_cfg;
}

/* ============================================================================
 * SETUP AND TEARDOWN
 * ========================================================================== */

void setUp(void) {
  VoltMon_SetStatusFlg_u32(0u);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0u);
  VoltMon_SetLastVoltage_mV_u16(0u);
  VoltMon_SetUvActive_b(false);
  VoltMon_SetOvActive_b(false);
}

void tearDown(void) {
}

/* ============================================================================
 * TEST FUNCTIONS
 * ========================================================================== */

void test_init_valid_cfg_returns_zero(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  uint8_t result = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(0u, result);
}

void test_init_valid_cfg_clears_status_flag(void) {
  VoltMon_SetStatusFlg_u32(0xFFFFFFFFu);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT32(0u, VoltMon_GetStatusFlg_u32());
}

void test_init_valid_cfg_sets_mode_idle(void) {
  VoltMon_SetMode_e(VoltMon_modeRun_e);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL(VoltMon_modeIdle_e, VoltMon_GetMode_e());
}

void test_init_valid_cfg_clears_last_raw_adc(void) {
  VoltMon_SetLastRawAdc_u16(1234u);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastRawAdc_u16());
}

void test_init_valid_cfg_clears_last_voltage(void) {
  VoltMon_SetLastVoltage_mV_u16(5678u);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastVoltage_mV_u16());
}

void test_init_valid_cfg_clears_uv_active(void) {
  VoltMon_SetUvActive_b(true);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
}

void test_init_valid_cfg_clears_ov_active(void) {
  VoltMon_SetOvActive_b(true);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_init_missing_cfg_returns_one(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnNull);

  uint8_t result = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(1u, result);
}

void test_init_invalid_cfg_returns_one(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnNull);

  uint8_t result = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(1u, result);
}

void test_init_twice_returns_zero_both_times(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  uint8_t result1 = VoltMon_Init_u8();
  uint8_t result2 = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(0u, result1);
  TEST_ASSERT_EQUAL_UINT8(0u, result2);
  TEST_ASSERT_EQUAL(VoltMon_modeIdle_e, VoltMon_GetMode_e());
  TEST_ASSERT_EQUAL_UINT32(0u, VoltMon_GetStatusFlg_u32());
}

void test_init_clears_preexisting_status_flag(void) {
  VoltMon_SetStatusFlg_u32(0xDEADBEEFu);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT32(0u, VoltMon_GetStatusFlg_u32());
}

void test_init_resets_preexisting_mode(void) {
  VoltMon_SetMode_e(VoltMon_modeDiag_e);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL(VoltMon_modeIdle_e, VoltMon_GetMode_e());
}

void test_init_clears_preexisting_raw_adc(void) {
  VoltMon_SetLastRawAdc_u16(9999u);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastRawAdc_u16());
}

void test_init_clears_preexisting_voltage(void) {
  VoltMon_SetLastVoltage_mV_u16(12345u);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT16(0u, VoltMon_GetLastVoltage_mV_u16());
}

void test_init_clears_preexisting_uv_active(void) {
  VoltMon_SetUvActive_b(true);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_FALSE(VoltMon_GetUvActive_b());
}

void test_init_clears_preexisting_ov_active(void) {
  VoltMon_SetOvActive_b(true);

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  VoltMon_Init_u8();

  TEST_ASSERT_FALSE(VoltMon_GetOvActive_b());
}

void test_init_success_boundary_exactly_zero(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnValid);

  uint8_t result = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(0u, result);
}

void test_init_fail_boundary_exactly_one(void) {
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg_ReturnNull);

  uint8_t result = VoltMon_Init_u8();

  TEST_ASSERT_EQUAL_UINT8(1u, result);
}