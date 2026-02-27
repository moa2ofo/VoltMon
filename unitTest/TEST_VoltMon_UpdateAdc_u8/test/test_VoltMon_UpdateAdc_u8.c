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

static const VoltMoncfgs* callbackcfgptr = NULL;
static uint16t callbackcomputed_voltage = 0;

static const VoltMoncfgs* VoltMonCfgGetpcfgCallback(int callcount)
{
    (void)call_count;
    return callbackcfgptr;
}

static uint16t ComputeVoltageu16Callback(uint16t rawAdcu16, const VoltMoncfgs* cfgpcs, int call_count)
{
    (void)call_count;
    (void)rawAdc_u16;
    (void)cfg_pcs;
    return callbackcomputedvoltage;
}

/* ============================================================================
 * SETUP AND TEARDOWN
 * ========================================================================== */

void setUp(void)
{
    VoltMonSetStatusFlgu32(0);
    VoltMonSetModee(VoltMonmodeIdlee);
    VoltMonSetLastRawAdcu16(0);
    VoltMonSetLastVoltagemV_u16(0);
    VoltMonSetUvActiveb(false);
    VoltMonSetOvActiveb(false);
    
    callbackcfgptr = NULL;
    callbackcomputedvoltage = 0;
}

void tearDown(void)
{
}

/* ============================================================================
 * TEST FUNCTIONS
 * ========================================================================== */

void testnotinitreturns1setserr(void)
{
    uint8_t result;
    
    VoltMonSetStatusFlgu32(0);
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(100);
    
    TESTASSERTEQUAL_UINT8(1, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSERRU32, VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastVoltagemVu16());
}

void testinitnullcfgreturns2setserrinval(void)
{
    uint8_t result;
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = NULL;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(100);
    
    TESTASSERTEQUAL_UINT8(2, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32 | VOLTMONSTATUSINVALU32, 
                             VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastVoltagemVu16());
}

void testvalidrawzerolower_boundary(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 50;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(0);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(50, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testvalidrawonejustinsidelower(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 60;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(1);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(1, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(60, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testvalidrawmidrange(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 2550;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(500);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(500, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(2550, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testvalidrawatrawMaxupperboundary(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 10050;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(1000);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(1000, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(10050, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testinvalidrawrawMaxplus_one(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    VoltMonSetLastRawAdcu16(500);
    VoltMonSetLastVoltagemV_u16(2550);
    callbackcfgptr = &cfg;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(1001);
    
    TESTASSERTEQUAL_UINT8(2, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32 | VOLTMONSTATUSINVALU32, 
                             VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(500, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(2550, VoltMonGetLastVoltagemVu16());
}

void testinvalidrawuint16max(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    VoltMonSetLastRawAdcu16(500);
    VoltMonSetLastVoltagemV_u16(2550);
    callbackcfgptr = &cfg;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(UINT16_MAX);
    
    TESTASSERTEQUAL_UINT8(2, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32 | VOLTMONSTATUSINVALU32, 
                             VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(500, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(2550, VoltMonGetLastVoltagemVu16());
}

void testvalidrawclearsinval_flag(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSINVALU32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 1050;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(100);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(100, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(1050, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testvalidrawerrflag_unchanged(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 1050;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16Stub(ComputeVoltageu16Callback);
    
    result = VoltMonUpdateAdcu8(100);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(100, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(1050, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32, VoltMonGetStatusFlg_u32());
}

void testcomputevoltagecalledcorrectly(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = &cfg;
    callbackcomputedvoltage = 3050;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    ComputeVoltageu16ExpectAndReturn(300, &cfg, 3050);
    
    result = VoltMonUpdateAdcu8(300);
    
    TESTASSERTEQUAL_UINT8(0, result);
    TESTASSERTEQUALUINT16(300, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(3050, VoltMonGetLastVoltagemVu16());
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32, VoltMonGetStatusFlgu32());
}

void testnotinitnullcfg_precedence(void)
{
    uint8_t result;
    
    VoltMonSetStatusFlgu32(0);
    callbackcfgptr = NULL;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(100);
    
    TESTASSERTEQUAL_UINT8(1, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSERRU32, VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastVoltagemVu16());
}

void testnotinitexceedsrawMax_precedence(void)
{
    uint8_t result;
    VoltMoncfgs cfg = {1000, 10, 50, 3000, 5000, 100};
    
    VoltMonSetStatusFlgu32(0);
    callbackcfgptr = &cfg;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(1001);
    
    TESTASSERTEQUAL_UINT8(1, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSERRU32, VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastVoltagemVu16());
}

void testinitnullcfgexceeds_rawMax(void)
{
    uint8_t result;
    
    VoltMonSetStatusFlgu32(VOLTMONSTATUSINIT_U32);
    callbackcfgptr = NULL;
    
    VoltMonCfgGetpcfgStub(VoltMonCfgGetpcfgCallback);
    
    result = VoltMonUpdateAdcu8(2000);
    
    TESTASSERTEQUAL_UINT8(2, result);
    TESTASSERTEQUALUINT32(VOLTMONSTATUSINITU32 | VOLTMONSTATUSERRU32 | VOLTMONSTATUSINVALU32, 
                             VoltMonGetStatusFlgu32());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastRawAdc_u16());
    TESTASSERTEQUALUINT16(0, VoltMonGetLastVoltagemVu16());
}