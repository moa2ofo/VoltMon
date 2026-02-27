#include "VoltMon.h"
#include "VoltMon_UpdateAdc_u8.h"
#include "VoltMon_cfg.h"
#include "VoltMon_priv.h"
#include "mock_VoltMon_cfg.h"
#include "mock_VoltMon_priv.h"
#include "unity.h"
#include <stdbool.h>
#include <stdint.h>

/* ============================================================================
 * CALLBACKS
 * ========================================================================== */

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg_ReturnValue;

static const VoltMon_cfg_s *callback_VoltMon_CfgGet_pcfg(int call_count) {
  return callback_VoltMon_CfgGet_pcfg_ReturnValue;
}

static uint16_t callback_ComputeVoltage_u16_ReturnValue;

static uint16_t callback_ComputeVoltage_u16(uint16_t rawAdc_u16, const VoltMon_cfg_s *cfg_pcs, int call_count) {
  return callback_ComputeVoltage_u16_ReturnValue;
}

/* ============================================================================
 * SETUP AND TEARDOWN
 * ========================================================================== */

void setUp(void) {
  VoltMon_SetStatusFlg_u32(0);
  VoltMon_SetMode_e(VoltMon_modeIdle_e);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);
  VoltMon_SetUvActive_b(false);
  VoltMon_SetOvActive_b(false);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = NULL;
  callback_ComputeVoltage_u16_ReturnValue = 0;
}

void tearDown(void) {
}

/* ============================================================================
 * TEST FUNCTIONS
 * ========================================================================== */

void test_NotInit_Return1_ErrSet(void) {
  uint8_t result;
  uint32_t statusBefore;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  VoltMon_SetStatusFlg_u32(0x00000000u);
  VoltMon_SetLastRawAdc_u16(999);
  VoltMon_SetLastVoltage_mV_u16(888);

  statusBefore = VoltMon_GetStatusFlg_u32();
  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(100);

  TEST_ASSERT_EQUAL_UINT8(1, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_NullCfg_Return2_ErrInvalSet(void) {
  uint8_t result;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_SetLastRawAdc_u16(777);
  VoltMon_SetLastVoltage_mV_u16(666);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = NULL;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(50);

  TEST_ASSERT_EQUAL_UINT8(2, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_ValidCfg_Raw0_Return0_Updated(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_SetLastRawAdc_u16(555);
  VoltMon_SetLastVoltage_mV_u16(444);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 0;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(0);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_Init_ValidCfg_Raw1_Return0_Updated(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_SetLastRawAdc_u16(333);
  VoltMon_SetLastVoltage_mV_u16(222);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 10;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(1);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(1, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(10, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_Init_ValidCfg_RawMid_Return0_Updated(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 100;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_SetLastRawAdc_u16(111);
  VoltMon_SetLastVoltage_mV_u16(222);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 20480;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(2048);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(2048, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(20480, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_Init_ValidCfg_RawMax_Return0_Updated(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 50000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 40950;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(4095);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(4095, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(40950, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_Init_ValidCfg_RawMaxPlus1_Return2_ErrInvalSet(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_SetLastRawAdc_u16(1000);
  VoltMon_SetLastVoltage_mV_u16(2000);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(4096);

  TEST_ASSERT_EQUAL_UINT8(2, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_ValidCfg_RawUINT16MAX_Return2_ErrInvalSet(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_SetLastRawAdc_u16(500);
  VoltMon_SetLastVoltage_mV_u16(1500);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(65535);

  TEST_ASSERT_EQUAL_UINT8(2, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_ValidCfg_ComputeVoltageCalled_ResultStored(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 5;
  cfg.offset_s16 = 200;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 12345;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(1234);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(1234, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(12345, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_ValidCfg_InvalFlagSet_Return0_InvalCleared(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 5000;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(500);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(5000, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_Init_ValidCfg_ErrFlagSet_Return0_ErrRemains(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32);
  VoltMon_SetLastRawAdc_u16(0);
  VoltMon_SetLastVoltage_mV_u16(0);

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  callback_ComputeVoltage_u16_ReturnValue = 3000;
  ComputeVoltage_u16_Stub(callback_ComputeVoltage_u16);

  result = VoltMon_UpdateAdc_u8(300);

  TEST_ASSERT_EQUAL_UINT8(0, result);
  TEST_ASSERT_EQUAL_UINT16(300, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(3000, VoltMon_GetLastVoltage_mV_u16());
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
}

void test_NotInit_NullCfg_Return1_ErrSet(void) {
  uint8_t result;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  VoltMon_SetStatusFlg_u32(0x00000000u);
  VoltMon_SetLastRawAdc_u16(123);
  VoltMon_SetLastVoltage_mV_u16(456);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = NULL;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(100);

  TEST_ASSERT_EQUAL_UINT8(1, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_NotInit_RawExceedsMax_Return1_ErrSet(void) {
  uint8_t result;
  VoltMon_cfg_s cfg;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  cfg.rawMax_u16 = 4095;
  cfg.factor_u16 = 10;
  cfg.offset_s16 = 0;
  cfg.uvTh_mV_u16 = 1000;
  cfg.ovTh_mV_u16 = 5000;
  cfg.hyst_mV_u16 = 100;

  VoltMon_SetStatusFlg_u32(0x00000000u);
  VoltMon_SetLastRawAdc_u16(789);
  VoltMon_SetLastVoltage_mV_u16(987);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = &cfg;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(5000);

  TEST_ASSERT_EQUAL_UINT8(1, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}

void test_Init_NullCfg_RawExceedsMax_Return2_ErrInvalSet(void) {
  uint8_t result;
  uint16_t lastRawBefore;
  uint16_t lastVoltageBefore;

  VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
  VoltMon_SetLastRawAdc_u16(321);
  VoltMon_SetLastVoltage_mV_u16(654);

  lastRawBefore = VoltMon_GetLastRawAdc_u16();
  lastVoltageBefore = VoltMon_GetLastVoltage_mV_u16();

  callback_VoltMon_CfgGet_pcfg_ReturnValue = NULL;
  VoltMon_CfgGet_pcfg_Stub(callback_VoltMon_CfgGet_pcfg);

  result = VoltMon_UpdateAdc_u8(10000);

  TEST_ASSERT_EQUAL_UINT8(2, result);
  TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, VoltMon_GetStatusFlg_u32());
  TEST_ASSERT_EQUAL_UINT16(lastRawBefore, VoltMon_GetLastRawAdc_u16());
  TEST_ASSERT_EQUAL_UINT16(lastVoltageBefore, VoltMon_GetLastVoltage_mV_u16());
}