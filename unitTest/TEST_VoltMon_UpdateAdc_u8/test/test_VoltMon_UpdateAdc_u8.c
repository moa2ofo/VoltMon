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

static const VoltMon_cfg_s* callback_cfg_ptr = NULL;
static uint16_t callback_computed_voltage = 0;

static const VoltMon_cfg_s* VoltMon_CfgGet_pcfg_Callback(int call_count)
{
    (void)call_count;
    return callback_cfg_ptr;
}

static uint16_t ComputeVoltage_u16_Callback(uint16_t rawAdc_u16, const VoltMon_cfg_s* cfg_pcs, int call_count)
{
    (void)rawAdc_u16;
    (void)cfg_pcs;
    (void)call_count;
    return callback_computed_voltage;
}

/* ============================================================================
 * SETUP AND TEARDOWN
 * ========================================================================== */

void setUp(void)
{
    VoltMon_SetStatusFlg_u32(0);
    VoltMon_SetMode_e(VoltMon_modeIdle_e);
    VoltMon_SetLastRawAdc_u16(0);
    VoltMon_SetLastVoltage_mV_u16(0);
    VoltMon_SetUvActive_b(false);
    VoltMon_SetOvActive_b(false);
    
    callback_cfg_ptr = NULL;
    callback_computed_voltage = 0;
}

void tearDown(void)
{
}

/* ============================================================================
 * TEST FUNCTIONS
 * ========================================================================== */

void test_not_init_returns_1_sets_err(void)
{
    uint8_t result;
    
    VoltMon_SetStatusFlg_u32(0);
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(100);
    
    TEST_ASSERT_EQUAL_UINT8(1, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastVoltage_mV_u16());
}

void test_init_null_cfg_returns_2_sets_err_inval(void)
{
    uint8_t result;
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = NULL;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(100);
    
    TEST_ASSERT_EQUAL_UINT8(2, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, 
                             VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastVoltage_mV_u16());
}

void test_valid_raw_zero_lower_boundary(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 50;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(0);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(50, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_valid_raw_one_just_inside_lower(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 60;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(1);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(1, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(60, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_valid_raw_mid_range(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 5050;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(500);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(5050, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_valid_raw_at_rawMax_upper_boundary(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 10050;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(1000);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(1000, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(10050, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_invalid_raw_rawMax_plus_one(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    VoltMon_SetLastRawAdc_u16(500);
    VoltMon_SetLastVoltage_mV_u16(5050);
    callback_cfg_ptr = &cfg;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(1001);
    
    TEST_ASSERT_EQUAL_UINT8(2, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, 
                             VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(5050, VoltMon_GetLastVoltage_mV_u16());
}

void test_invalid_raw_uint16_max(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    VoltMon_SetLastRawAdc_u16(500);
    VoltMon_SetLastVoltage_mV_u16(5050);
    callback_cfg_ptr = &cfg;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(UINT16_MAX);
    
    TEST_ASSERT_EQUAL_UINT8(2, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, 
                             VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(5050, VoltMon_GetLastVoltage_mV_u16());
}

void test_compute_voltage_called_correctly(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    uint16_t test_raw = 750;
    uint16_t expected_voltage = 7550;
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = expected_voltage;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_ExpectAndReturn(test_raw, &cfg, expected_voltage);
    
    result = VoltMon_UpdateAdc_u8(test_raw);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(test_raw, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(expected_voltage, VoltMon_GetLastVoltage_mV_u16());
}

void test_inval_flag_cleared_on_valid_input(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_INVAL_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 5050;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(500);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(5050, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32, VoltMon_GetStatusFlg_u32());
}

void test_err_flag_remains_on_valid_input(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32);
    callback_cfg_ptr = &cfg;
    callback_computed_voltage = 5050;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    ComputeVoltage_u16_Stub(ComputeVoltage_u16_Callback);
    
    result = VoltMon_UpdateAdc_u8(500);
    
    TEST_ASSERT_EQUAL_UINT8(0, result);
    TEST_ASSERT_EQUAL_UINT16(500, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(5050, VoltMon_GetLastVoltage_mV_u16());
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
}

void test_not_init_takes_precedence_over_invalid_raw(void)
{
    uint8_t result;
    VoltMon_cfg_s cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMon_SetStatusFlg_u32(0);
    callback_cfg_ptr = &cfg;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(1001);
    
    TEST_ASSERT_EQUAL_UINT8(1, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_ERR_U32, VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastVoltage_mV_u16());
}

void test_null_cfg_and_invalid_raw_returns_2(void)
{
    uint8_t result;
    
    VoltMon_SetStatusFlg_u32(VOLTMON_STATUS_INIT_U32);
    callback_cfg_ptr = NULL;
    
    VoltMon_CfgGet_pcfg_Stub(VoltMon_CfgGet_pcfg_Callback);
    
    result = VoltMon_UpdateAdc_u8(UINT16_MAX);
    
    TEST_ASSERT_EQUAL_UINT8(2, result);
    TEST_ASSERT_EQUAL_UINT32(VOLTMON_STATUS_INIT_U32 | VOLTMON_STATUS_ERR_U32 | VOLTMON_STATUS_INVAL_U32, 
                             VoltMon_GetStatusFlg_u32());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastRawAdc_u16());
    TEST_ASSERT_EQUAL_UINT16(0, VoltMon_GetLastVoltage_mV_u16());
}