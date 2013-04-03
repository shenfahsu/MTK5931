/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */


/*! \file
    \brief  Declaration of library functions

    Any definitions in this file will be shared among GLUE Layer and internal Driver Stack.
*/

/*******************************************************************************
* Copyright (c) 2009 MediaTek Inc.
*
* All rights reserved. Copying, compilation, modification, distribution
* or any other use whatsoever of this material is strictly prohibited
* except in accordance with a Software License Agreement with
* MediaTek Inc.
********************************************************************************
*/

/*******************************************************************************
* LEGAL DISCLAIMER
*
* BY OPENING THIS FILE, BUYER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND
* AGREES THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK
* SOFTWARE") RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE
* PROVIDED TO BUYER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY
* DISCLAIMS ANY AND ALL WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE OR NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE
* ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
* WHICH MAY BE USED BY, INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK
* SOFTWARE, AND BUYER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY
* WARRANTY CLAIM RELATING THERETO. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE
* FOR ANY MEDIATEK SOFTWARE RELEASES MADE TO BUYER'S SPECIFICATION OR TO
* CONFORM TO A PARTICULAR STANDARD OR OPEN FORUM.
*
* BUYER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND CUMULATIVE
* LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL
* BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT
* ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY
* BUYER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
*
* THE TRANSACTION CONTEMPLATED HEREUNDER SHALL BE CONSTRUED IN ACCORDANCE
* WITH THE LAWS OF THE STATE OF CALIFORNIA, USA, EXCLUDING ITS CONFLICT
* OF LAWS PRINCIPLES.  ANY DISPUTES, CONTROVERSIES OR CLAIMS ARISING
* THEREOF AND RELATED THERETO SHALL BE SETTLED BY ARBITRATION IN SAN
* FRANCISCO, CA, UNDER THE RULES OF THE INTERNATIONAL CHAMBER OF COMMERCE
* (ICC).
********************************************************************************
*/


/*******************************************************************************
*                         C O M P I L E R   F L A G S
********************************************************************************
*/
#define CFG_CHECK_WMT_RESULT (1)
/* BT Port 2 Feature. this command does not need after coex command is down.
confirmed by LC,
*/
#define CFG_WMT_BT_PORT2 (0)

#define CFG_SET_OPT_REG (0)
#define CFG_WMT_I2S_DBGUART_SUPPORT (0)
#define CFG_SET_OPT_REG_SWLA (0)
#define CFG_SET_OPT_REG_MCUCLK (0)
#define CFG_SET_OPT_REG_MCUIRQ (0)

/* For patch file download, using division or not */
#define CFG_PATCH_DWN_DIVISION (0)
/* disable this to avoid inserting unwanted compiler built-in division symbols,
 * such as  __aeabi_uidiv in ARM EABI.
 */

/*******************************************************************************
*                                 M A C R O S
********************************************************************************
*/

/*******************************************************************************
*                    E X T E R N A L   R E F E R E N C E S
********************************************************************************
*/
#include "osal_typedef.h"
#include "wmt_ic.h"
#include "wmt_core.h"
#include "wmt_lib.h"
#include "stp_core.h"

/*******************************************************************************
*                              C O N S T A N T S
********************************************************************************
*/

#ifdef DFT_TAG
#undef DFT_TAG
#endif
#define DFT_TAG "[WMT-IC]"
#define AML_Ver "V1.2.20121229_ICS_release"

#define DEFAULT_PATCH_FRAG_SIZE (1000)
#define MT6620E2_PATCH_FRAG_SIZE (900)
#define WMT_PATCH_FRAG_1ST (0x1)
#define WMT_PATCH_FRAG_MID (0x2)
#define WMT_PATCH_FRAG_LAST (0x3)

/*******************************************************************************
*                             D A T A   T Y P E S
********************************************************************************
*/

/*******************************************************************************
*                           P R I V A T E   D A T A
********************************************************************************
*/

UCHAR gDefPatchName[OSAL_NAME_MAX + 1];
UCHAR gFullPatchName[OSAL_NAME_MAX + 1];
static const WMT_IC_INFO_S *gp_mt6620_info = NULL;

static UCHAR WMT_WAKEUP_DIS_GATE_CMD[] = {0x1, 0x3, 0x01, 0x00, 0x04};
static UCHAR WMT_WAKEUP_DIS_GATE_EVT[] = {0x2, 0x3, 0x02, 0x0, 0x0, 0x04};
#if 0
static UCHAR WMT_WAKEUP_EN_GATE_CMD[] = {0x1, 0x3, 0x01, 0x00, 0x05};
static UCHAR WMT_WAKEUP_EN_GATE_EVT[] = {0x2, 0x3, 0x02, 0x0, 0x0, 0x05};
#endif
static UCHAR WMT_QUERY_BAUD_CMD[] = {0x01, 0x04, 0x01, 0x00, 0x02};
static UCHAR WMT_QUERY_BAUD_EVT_115200[] = {0x02, 0x04, 0x06, 0x00, 0x00, 0x02, 0x00, 0xC2, 0x01, 0x00};
static UCHAR WMT_QUERY_BAUD_EVT_X[] = {0x02, 0x04, 0x06, 0x00, 0x00, 0x02, 0xAA, 0xAA, 0xAA, 0xBB};
static UCHAR WMT_QUERY_STP_CMD[] = {0x01, 0x04, 0x01, 0x00, 0x04};
static UCHAR WMT_QUERY_STP_EVT_DEFAULT[] = {0x02, 0x04, 0x06, 0x00, 0x00, 0x04, 0x11, 0x00, 0x00, 0x00};
static UCHAR WMT_QUERY_STP_EVT_UART[] = {0x02, 0x04, 0x06, 0x00, 0x00, 0x04, 0xDF, 0x0E, 0x68, 0x01};
static UCHAR WMT_SET_BAUD_CMD_X[] = {0x01, 0x04, 0x05, 0x00, 0x01, 0xAA, 0xAA, 0xAA, 0xBB};
static UCHAR WMT_SET_BAUD_EVT[] = {0x02, 0x04, 0x02, 0x00, 0x00, 0x01};
static UCHAR WMT_SET_WAKEUP_WAKE_CMD_RAW[] = {0xFF};
static UCHAR WMT_SET_WAKEUP_WAKE_EVT[] = {0x02, 0x03, 0x02, 0x00, 0x00, 0x03};
static UCHAR WMT_PATCH_CMD[] = {0x01, 0x01, 0x00, 0x00, 0x00};
static UCHAR WMT_PATCH_EVT[] = {0x02, 0x01, 0x01, 0x00, 0x00};
static UCHAR WMT_RESET_CMD[] = {0x01, 0x07, 0x01, 0x00, 0x04};
static UCHAR WMT_RESET_EVT[] = {0x02, 0x07, 0x01, 0x00, 0x00};
#if CFG_WMT_BT_PORT2
static UCHAR WMT_BTP2_CMD[] = {0x01, 0x10, 0x03, 0x00, 0x01, 0x03, 0x01};
static UCHAR WMT_BTP2_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
#endif
/*coex cmd/evt++*/
static UCHAR WMT_COEX_SETTING_CONFIG_CMD[] = {0x01, 0x10, 0x04, 0x00, 0x01, 0xAA, 0xBB, 0xCC};
static UCHAR WMT_COEX_SETTING_CONFIG_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
static UCHAR WMT_BT_COEX_SETTING_CONFIG_CMD[] = {0x01, 0x10, 0x0B,
                                                         0x00, 0x02,
                                                         0x00, 0x00, 0x00, 0x00,
                                                         0xBB,0xCC,0xDD,0xEE,0xFF,0xAA};
static UCHAR WMT_BT_COEX_SETTING_CONFIG_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
static UCHAR WMT_WIFI_COEX_SETTING_CONFIG_CMD[] = {0x01, 0x10, 0x0C,
                                                           0x00, 0x03,
                                                           0x00, 0x00,0x00,0x00,0x00,
                                                           0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0xAA};
static UCHAR WMT_WIFI_COEX_SETTING_CONFIG_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
static UCHAR WMT_PTA_COEX_SETTING_CONFIG_CMD[] = {0x01, 0x10, 0x0A,
                                                          0x00, 0x04,
                                                          0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xEE, 0xFF, 0xFF, 0xFE};
static UCHAR WMT_PTA_COEX_SETTING_CONFIG_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
static UCHAR WMT_MISC_COEX_SETTING_CONFIG_CMD[] = {0x01, 0x10, 0x09,
                                                           0x00, 0x05,
                                                           0xAA, 0xAA, 0xAA, 0xAA,
                                                           0xBB, 0xBB, 0xBB, 0xBB};
static UCHAR WMT_MISC_COEX_SETTING_CONFIG_EVT[] = {0x02, 0x10, 0x01, 0x00, 0x00};
/*coex cmd/evt--*/
static UCHAR WMT_SET_STP_CMD[] = {0x01, 0x04, 0x05, 0x00, 0x03, 0xDF, 0x0E, 0x68, 0x01};
static UCHAR WMT_SET_STP_EVT[] = {0x02, 0x04, 0x02, 0x00, 0x00, 0x03};
static UCHAR WMT_STRAP_CONF_CMD_FM_COMM[] = {0x01, 0x05, 0x02, 0x00, 0x02, 0x02};
static UCHAR WMT_STRAP_CONF_EVT[] = {0x02, 0x05, 0x02, 0x00, 0x00, 0x02};
#if 0
static UCHAR WMT_SET_OSC32K_BYPASS_CMD[]= {0x01, 0x0A, 0x01, 0x00, 0x05};
static UCHAR WMT_SET_OSC32K_BYPASS_EVT[]= {0x02, 0x0A, 0x01, 0x00, 0x00};
#endif

/* enable all interrupt */
static UCHAR WMT_SET_ALLINT_REG_CMD[] = {0x01, 0x08, 0x10, 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /*type: reg */
    ,0x00 /*rev*/
    ,0x01 /*1 registers*/
    ,0x00, 0x03, 0x05, 0x80/*addr:0x80050300*/
    ,0x00, 0xC4, 0x00, 0x00/*value:0x0000C400*/
    ,0x00, 0xC4, 0x00, 0x00/*mask:0x0000C400*/
};

static UCHAR WMT_SET_ALLINT_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /*S: 0*/
    ,0x00 /*type: reg */
    ,0x00 /*rev*/
    ,0x01 /*1 registers*/
};

#if CFG_SET_OPT_REG_SWLA /* enable swla: eesk(7) eecs(8) oscen(19) sck0(24) scs0(25)  */
static UCHAR WMT_SET_SWLA_REG_CMD[] = {0x01, 0x08, 0x1C, 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /*type: reg */
    ,0x00 /*rev*/
    ,0x02 /*2 registers*/
    ,0x10, 0x01, 0x05, 0x80/*addr:0x80050110*/
    ,0x10, 0x10, 0x01, 0x00/*value:0x00011010*/
    ,0xF0, 0xF0, 0x0F, 0x00/*mask:0x000FF0F0*/
    ,0x40, 0x01, 0x05, 0x80/*addr:0x80050140*/
    ,0x00, 0x10, 0x01, 0x00/*value:0x00011000*/
    ,0x00, 0xF0, 0x0F, 0x00/*mask:0x000FF000*/
};
static UCHAR WMT_SET_SWLA_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /*S: 0*/
    ,0x00 /*type: reg */
    ,0x00 /*rev*/
    ,0x02 /*2 registers*/
};
#endif

#if CFG_SET_OPT_REG_MCUCLK /* enable mcu clk: antsel_4, eedi */
static UCHAR WMT_SET_MCUCLK_REG_CMD[] = {0x01, 0x08, (4 + 12*4), 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /* type: reg */
    ,0x00 /* rev */
    ,0x04 /* 4 registers */
    ,0x00, 0x04, 0x00, 0x80 /* addr:0x8000 0400 */
    ,0x00, 0x14, 0x00, 0x00 /* value:0x0000 1400(osc, hclk), 0x0000 1501(PLL, en) */
    ,0xFF, 0xFF, 0x00, 0x00 /* mask:0x0000 FFFF */
    ,0x80, 0x01, 0x05, 0x80 /* addr:0x8005 0180 */
    ,0x12, 0x13, 0x00, 0x00 /* value:0x0000 1312(osc, hclk), 0x0000 1a19(PLL, en) */
    ,0xFF, 0xFF, 0x00, 0x00 /* mask:0x0000 FFFF */
    ,0x00, 0x01, 0x05, 0x80 /* addr:0x8005 0100 */
    ,0x00, 0x00, 0x02, 0x00 /* value:0x0002 0000 */
    ,0x00, 0x00, 0x0F, 0x00 /* mask:0x000F 0000 */
    ,0x10, 0x01, 0x05, 0x80 /* addr:0x8005 0110 */
    ,0x02, 0x00, 0x00, 0x00 /* value:0x0000 0002 */
    ,0x0F, 0x00, 0x00, 0x00 /* mask:0x0000 000F */
};

static UCHAR WMT_SET_MCUCLK_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /* S: 0 */
    ,0x00 /* type: reg */
    ,0x00 /* rev */
    ,0x04 /* 4 registers */
};
#endif

#if CFG_WMT_I2S_DBGUART_SUPPORT /* register write for debug uart */
static UCHAR WMT_SET_DBGUART_REG_CMD[] = {0x01, 0x08, 0x1C, 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /*type: reg */
    ,0x00 /*rev*/
    ,0x02 /*2 registers*/
    ,0x30, 0x01, 0x05, 0x80/*addr:0x80050130*/
    ,0x00, 0x00, 0x00, 0x00/*value:0x00000000*/
    ,0xF0, 0x0F, 0x00, 0x00/*mask:0x00000FF0*/
    ,0x40, 0x01, 0x05, 0x80/*addr:0x80050140*/
    ,0x00, 0x01, 0x00, 0x00/*value:0x00000100*/
    ,0x00, 0x01, 0x00, 0x00/*mask:0x00000100*/
};
static UCHAR WMT_SET_DBGUART_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /*S: 0*/
    ,0x00 /*type: reg */
    ,0x00 /*rev*/
    ,0x02 /*2 registers*/
};
#endif

#if CFG_SET_OPT_REG_MCUIRQ /* enable mcu irq: antsel_4, wlan_act */
#if 1 /* Ray */
static UCHAR WMT_SET_MCUIRQ_REG_CMD[] = {0x01, 0x08, (4 + 12*4), 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /* type: reg */
    ,0x00 /* rev */
    ,0x04 /* 4 registers */
    ,0x00, 0x04, 0x00, 0x80 /* addr:0x8000_0400 */
    ,0x03, 0x14, 0x00, 0x00 /* value:0x0000_1403 check confg debug flag 3 low word */
    ,0xFF, 0xFF, 0x00, 0x00 /* mask:0x0000_FFFF */
    /* cirq_int_n */
    ,0x10, 0x01, 0x05, 0x80 /* addr:0x8005_0110 */
    ,0x02, 0x00, 0x00, 0x00 /* value:0x0000_0002 set EEDI as cirq_int_n debug flag (monitor flag2) */
    ,0x07, 0x00, 0x00, 0x00 /* mask:0x0000_0007 */
    ,0x00, 0x01, 0x05, 0x80 /* addr:0x8005_0100 */
    ,0x00, 0x00, 0x02, 0x00 /* value:0x0002_0000 (ANTSEL4=>monitor flag 0, ahb_x2_gt_ck debug flag) */
    ,0x00, 0x00, 0x07, 0x00 /* mask:0x0007_0000 */
    /* 1.    ARM irq_b, monitor flag 0 */
    ,0x80, 0x01, 0x05, 0x80 /* addr:0x8005_0180 */
    ,0x1F, 0x1E, 0x00, 0x00 /* value:0x0000_1E1F check mcusys debug flag */
    ,0x7F, 0x7F, 0x00, 0x00 /* mask:0x0000_7F7F */
};

static UCHAR WMT_SET_MCUIRQ_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /* S: 0 */
    ,0x00 /* type: reg */
    ,0x00 /* rev */
    ,0x04 /* 5 registers */
};
#elif 0 /* KC */
static UCHAR WMT_SET_MCUIRQ_REG_CMD[] = {0x01, 0x08, (4 + 12*5), 0x00/*length*/
    ,0x01 /* op: w */
    ,0x01 /* type: reg */
    ,0x00 /* rev */
    ,0x05 /* 5 registers */
    ,0x00, 0x04, 0x00, 0x80 /* addr:0x8000_0400 */
    ,0x00, 0x02, 0x00, 0x00 /* value:0x0000_0200 [15:8]=0x2 arm irq_b, 0xA irq_bus[5] bt_timcon_irq_b */
    ,0x00, 0xFF, 0x00, 0x00 /* mask:0x0000_FF00 */
    /* 1.    ARM irq_b, monitor flag 0 */
    ,0x80, 0x01, 0x05, 0x80 /* addr:0x8005_0180 */
    ,0x18, 0x00, 0x00, 0x00 /* value:0x0000_0018 [6:0]=001_1000 (monitor flag 0 select, MCUSYS, SEL:8) */
    ,0x7F, 0x00, 0x00, 0x00 /* mask:0x0000_007F */
    ,0x00, 0x01, 0x05, 0x80 /* addr:0x8005_0100 */
    ,0x00, 0x00, 0x02, 0x00 /* value:0x0002_0000 (ANTSEL4=>monitor flag 0) */
    ,0x00, 0x00, 0x07, 0x00 /* mask:0x0007_0000 */
    /* 2.    irq_bus[5] bt_timcon_irq_b monitor flag 15 */
    ,0xB0, 0x01, 0x05, 0x80 /* addr:0x8005_01B0 */
    ,0x00, 0x00, 0x00, 0x16 /* value:0x1600_0000 [30:24]=001_0110 (monitor flag 15 select, MCUSYS, SEL:6) */
    ,0x00, 0x00, 0x00, 0x7F /* mask:0x7F00_0000 */
    ,0x30, 0x01, 0x05, 0x80 /* addr:0x8005_0130 */
    ,0x00, 0x20, 0x00, 0x00 /* value:0x0000_2000 (WLAN_ACT=>monitor flag 15) */
    ,0x00, 0x70, 0x00, 0x00 /* mask:0x0000_7000 */
};

static UCHAR WMT_SET_MCUIRQ_REG_EVT[] = {0x02, 0x08, 0x04, 0x00/*length*/
    ,0x00 /* S: 0 */
    ,0x00 /* type: reg */
    ,0x00 /* rev */
    ,0x05 /* 5 registers */
};
#endif
#endif

/* stp sdio init scripts */
static struct init_script init_table_1_1[] =
{
    /* table_1_1 is only applied to common SDIO interface */
    INIT_CMD(WMT_SET_ALLINT_REG_CMD, WMT_SET_ALLINT_REG_EVT, "enable all interrupt"),
    /* only applied to MT6620 E1/E2? */
    INIT_CMD(WMT_WAKEUP_DIS_GATE_CMD, WMT_WAKEUP_DIS_GATE_EVT, "disable gating"),
};


static struct init_script init_table_1_2[] =
{
    INIT_CMD(WMT_QUERY_BAUD_CMD, WMT_QUERY_BAUD_EVT_115200, "query baud 115200"),
    INIT_CMD(WMT_QUERY_STP_CMD, WMT_QUERY_STP_EVT_DEFAULT, "query stp default"),
    INIT_CMD(WMT_SET_BAUD_CMD_X, WMT_SET_BAUD_EVT, "set baud rate"),
};

static struct init_script init_table_2[] =
{
    INIT_CMD(WMT_QUERY_BAUD_CMD, WMT_QUERY_BAUD_EVT_X, "query baud X"),
};

static struct init_script init_table_3[] =
{
    INIT_CMD(WMT_RESET_CMD, WMT_RESET_EVT, "wmt reset"),
#if CFG_WMT_BT_PORT2
    INIT_CMD(WMT_BTP2_CMD, WMT_BTP2_EVT, "set bt port2"),
#endif
};

#if 0
static struct init_script init_table_3_1[] =
{
    INIT_CMD(WMT_WAKEUP_EN_GATE_CMD, WMT_WAKEUP_EN_GATE_EVT, "ensable gating"),
};
#endif

static struct init_script init_table_4[] =
{
    INIT_CMD(WMT_SET_STP_CMD, WMT_SET_STP_EVT, "set stp"),
};

static struct init_script init_table_5[] =
{
    INIT_CMD(WMT_QUERY_STP_CMD, WMT_QUERY_STP_EVT_UART, "query stp uart"),
    INIT_CMD(WMT_QUERY_BAUD_CMD, WMT_QUERY_BAUD_EVT_X, "query baud X"),
};

static struct init_script init_table_5_1[] = {
     INIT_CMD(WMT_STRAP_CONF_CMD_FM_COMM, WMT_STRAP_CONF_EVT, "configure FM comm"),
};

#if 0
static struct init_script init_table_6[] =
{
    INIT_CMD(WMT_SET_OSC32K_BYPASS_CMD, WMT_SET_OSC32K_BYPASS_EVT, "set OSC32k by pass mode."),
};
#endif

#if defined(CFG_SET_OPT_REG) && CFG_SET_OPT_REG
static struct init_script set_registers[] =
{
    //INIT_CMD(WMT_SET_GPS_REG_CMD, WMT_SET_GPS_REG_EVT, "set wmt registers"),
    //INIT_CMD(WMT_SET_SDIODRV_REG_CMD, WMT_SET_SDIODRV_REG_EVT, "set SDIO driving registers")
    #if CFG_WMT_I2S_DBGUART_SUPPORT
    INIT_CMD(WMT_SET_DBGUART_REG_CMD, WMT_SET_DBGUART_REG_EVT, "set debug uart registers"),
    #endif
    #if CFG_SET_OPT_REG_SWLA
    INIT_CMD(WMT_SET_SWLA_REG_CMD, WMT_SET_SWLA_REG_EVT, "set swla registers"),
    #endif
    #if CFG_SET_OPT_REG_MCUCLK
    INIT_CMD(WMT_SET_MCUCLK_REG_CMD, WMT_SET_MCUCLK_REG_EVT, "set mcuclk dbg registers"),
    #endif
    #if CFG_SET_OPT_REG_MCUIRQ
    INIT_CMD(WMT_SET_MCUIRQ_REG_CMD, WMT_SET_MCUIRQ_REG_EVT, "set mcu irq dbg registers"),
    #endif
};
#endif

static struct init_script coex_table[] = {
     INIT_CMD(WMT_COEX_SETTING_CONFIG_CMD, WMT_COEX_SETTING_CONFIG_EVT, "coex_wmt"),
     INIT_CMD(WMT_BT_COEX_SETTING_CONFIG_CMD, WMT_BT_COEX_SETTING_CONFIG_EVT, "coex_bt"),
     INIT_CMD(WMT_WIFI_COEX_SETTING_CONFIG_CMD, WMT_WIFI_COEX_SETTING_CONFIG_EVT, "coex_wifi"),
     INIT_CMD(WMT_PTA_COEX_SETTING_CONFIG_CMD, WMT_PTA_COEX_SETTING_CONFIG_EVT, "coex_ext_pta"),
     INIT_CMD(WMT_MISC_COEX_SETTING_CONFIG_CMD, WMT_MISC_COEX_SETTING_CONFIG_EVT, "coex_misc"),
};

/* MT6620 Chip Version and Info Table */
static const WMT_IC_INFO_S mt6620_info_table[] = {
    {
        .u4HwVer            = 0x8A00,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E1,
        .cPatchNameExt     = WMT_IC_PATCH_NO_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E1,
        .bWorkWithoutPatch = MTK_WCN_BOOL_FALSE,
        .bPsmSupport       = MTK_WCN_BOOL_FALSE,
    },
    {
        .u4HwVer            = 0x8A01,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E2,
        .cPatchNameExt     = WMT_IC_PATCH_NO_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E2,
        .bWorkWithoutPatch = MTK_WCN_BOOL_FALSE,
        .bPsmSupport       = MTK_WCN_BOOL_FALSE,
    },
    {
        .u4HwVer            = 0x8A10,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E3,
        .cPatchNameExt     = WMT_IC_PATCH_E3_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E3,
        .bWorkWithoutPatch = MTK_WCN_BOOL_FALSE,
        .bPsmSupport       = MTK_WCN_BOOL_TRUE,
    },
    {
        .u4HwVer            = 0x8A11,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E4,
        .cPatchNameExt     = WMT_IC_PATCH_E3_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E4,
        .bWorkWithoutPatch = MTK_WCN_BOOL_FALSE,
        .bPsmSupport       = MTK_WCN_BOOL_TRUE,
    },
    {
        .u4HwVer            = 0x8A30,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E6,
        .cPatchNameExt     = WMT_IC_PATCH_E6_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E6,
        .bWorkWithoutPatch = MTK_WCN_BOOL_TRUE /*MTK_WCN_BOOL_FALSE*/,
        .bPsmSupport       = MTK_WCN_BOOL_TRUE,
    },
    {
        .u4HwVer            = 0x8B30,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E6,
        .cPatchNameExt     = WMT_IC_PATCH_E6_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E6,
        .bWorkWithoutPatch = MTK_WCN_BOOL_TRUE /*MTK_WCN_BOOL_FALSE*/,
        .bPsmSupport       = MTK_WCN_BOOL_TRUE,
    },
    {
        .u4HwVer            = 0x8B31,
        .cChipName         = WMT_IC_NAME_MT6620,
        .cChipVersion      = WMT_IC_VER_E7,
        .cPatchNameExt     = WMT_IC_PATCH_E6_EXT,
        .eWmtHwVer        = WMTHWVER_MT6620_E7,
        .bWorkWithoutPatch = MTK_WCN_BOOL_TRUE /*MTK_WCN_BOOL_FALSE*/,
        .bPsmSupport       = MTK_WCN_BOOL_TRUE,
    },
};

/*******************************************************************************
*                  F U N C T I O N   D E C L A R A T I O N S
********************************************************************************
*/

static INT32
mt6620_sw_init (
    P_WMT_HIF_CONF pWmtHifConf
    );

static INT32
mt6620_sw_deinit (
    P_WMT_HIF_CONF pWmtHifConf
    );

static INT32
mt6620_pin_ctrl (
    WMT_IC_PIN_ID id,
    WMT_IC_PIN_STATE state,
    UINT32 flag
    );

static INT32
mt6620_aif_ctrl (
    WMT_IC_PIN_STATE state,
    UINT32 flag
    );

static INT32
mt6620_ver_check (VOID);

static const WMT_IC_INFO_S*
mt6620_find_wmt_ic_info (
    const UINT32 hw_ver
    );

static INT32
wmt_stp_init_coex (VOID);

static INT32
mt6620_update_patch_name (VOID);

#if CFG_PATCH_DWN_DIVISION
static INT32
mt6620_patch_dwn_div(PUINT8 pBuf, UINT32 patchSize, UINT32 patchSizePerFrag);
#else
static INT32
mt6620_patch_dwn_nodiv(PUINT8 pBuf, UINT32 patchSize, UINT32 patchSizePerFrag);
#endif


static INT32
mt6620_patch_dwn (VOID);


/*******************************************************************************
*                            P U B L I C   D A T A
********************************************************************************
*/

/* MT6620 Operation Function Table */
const WMT_IC_OPS wmt_ic_ops_mt6620 = {
    .icId = 0x6620,
    .sw_init = mt6620_sw_init,
    .sw_deinit = mt6620_sw_deinit,
    .ic_pin_ctrl = mt6620_pin_ctrl,
    .ic_ver_check = mt6620_ver_check,
};

/*******************************************************************************
*                              F U N C T I O N S
********************************************************************************
*/

static INT32
mt6620_sw_init (
    P_WMT_HIF_CONF pWmtHifConf
    )
{
    INT32 iRet = -1;
    UINT32 u4Res = 0;
    UCHAR evtBuf[256];
    UINT32 ctrlPa1;
    UINT32 ctrlPa2;
    UINT32 hw_ver;

    WMT_DBG_FUNC(" start\n");

    osal_assert(NULL != gp_mt6620_info);
    if ( (NULL == gp_mt6620_info)
        || (NULL == pWmtHifConf)
        ) {
        WMT_ERR_FUNC("null pointers: gp_mt6620_info(0x%p), pWmtHifConf(0x%p)\n",
            gp_mt6620_info, pWmtHifConf);
        return -1;
    }

    hw_ver = gp_mt6620_info->u4HwVer;

    //4 <3.1> start init for sdio
    if (WMT_HIF_SDIO == pWmtHifConf->hifType) {
        /* 1. enable all INT32 */
        /* 2. disable mcu gate (only MT6620E1/E2) */
        iRet = wmt_core_init_script(init_table_1_1, osal_array_size(init_table_1_1));
        if (iRet) {
            WMT_ERR_FUNC("init_table_1_1 fail:%d\n", iRet);
            osal_assert(0);
            return -1;
        }
    }

    //4 <3.2> start init for uart
    if (WMT_HIF_UART == pWmtHifConf->hifType) {
        /* init variable fields for script execution */
        osal_memcpy(&WMT_SET_BAUD_CMD_X[5], &pWmtHifConf->au4HifConf[0], osal_sizeof(UINT32));
        osal_memcpy(&WMT_QUERY_BAUD_EVT_X[6], &pWmtHifConf->au4HifConf[0], osal_sizeof(UINT32));
        if (WMT_UART_MTK_SW_FC == pWmtHifConf->uartFcCtrl)
        {
            WMT_INFO_FUNC("enable MTK SW Flow Control\n");
            WMT_SET_BAUD_CMD_X[8] = (UCHAR)0x80;//* MTK SW flow control */
            WMT_QUERY_BAUD_EVT_X[9] = (UCHAR)0x80; //* MTK SW flow control */
        }
        else if (WMT_UART_LUX_SW_FC == pWmtHifConf->uartFcCtrl)
        {
            WMT_INFO_FUNC("enable Linux SW Flow Control\n");
            WMT_SET_BAUD_CMD_X[8] = (UCHAR)0x80;//* Linux SW flow control */
            WMT_QUERY_BAUD_EVT_X[9] = (UCHAR)0x80; //* Linux SW flow control */
        }
        else if (WMT_UART_HW_FC == pWmtHifConf->uartFcCtrl)
        {
            WMT_INFO_FUNC("enable HW Flow Control\n");
            WMT_SET_BAUD_CMD_X[8] = (UCHAR)0xC0;//* HW flow control */
            WMT_QUERY_BAUD_EVT_X[9] = (UCHAR)0xC0; //* HW flow control */
        }
        else {
            /* WMT_UART_NO_FC and all other cases!!! */
            WMT_INFO_FUNC("no Flow Control (uartFcCtrl:%d)\n", pWmtHifConf->uartFcCtrl);
            WMT_SET_BAUD_CMD_X[8] = (UCHAR)0x00;//* no flow control */
            WMT_QUERY_BAUD_EVT_X[9] = (UCHAR)0x00; //* no flow control */
        }

        /* 3. Query chip baud rate (TEST-ONLY) */
        /* 4. Query chip STP options (TEST-ONLY) */
        /* 5. Change chip baud rate: t_baud */
        iRet = wmt_core_init_script(init_table_1_2, osal_array_size(init_table_1_2));
        if (iRet) {
            WMT_ERR_FUNC("init_table_1_2 fail(%d)\n", iRet);
            osal_assert(0);
            return -2;
        }
        WMT_DBG_FUNC("init_table_1_2 set chip baud rate(%d) ok\n", pWmtHifConf->au4HifConf[0]);

        /* 6. Set host baudrate and flow control*/
        ctrlPa1 = pWmtHifConf->au4HifConf[0]; ctrlPa2 = 0;
        iRet = wmt_core_ctrl(WMT_CTRL_HOST_BAUDRATE_SET, &ctrlPa1, &ctrlPa2);
        if (iRet) {
            WMT_ERR_FUNC("change host baudrate(%d) fail(%d)\n", pWmtHifConf->au4HifConf[0], iRet);
            return -3;
        }
        WMT_INFO_FUNC("change host baudrate(%d) ok\n", pWmtHifConf->au4HifConf[0]);
        /* 7. Wake up chip and check event */
//          iRet = (*kal_stp_tx_raw)(&WMT_SET_WAKEUP_WAKE_CMD_RAW[0], 1, &u4Res);
        iRet = wmt_core_tx((PUINT8)&WMT_SET_WAKEUP_WAKE_CMD_RAW[0], 1, &u4Res, MTK_WCN_BOOL_TRUE);
        if (iRet || (u4Res != 1)) {
            WMT_ERR_FUNC("write raw iRet(%d) written(%d)\n", iRet, u4Res);
            return -4;
        }

        osal_memset(evtBuf, 0, osal_sizeof(evtBuf));
        iRet = wmt_core_rx(evtBuf, osal_sizeof(WMT_SET_WAKEUP_WAKE_EVT), &u4Res);
#ifdef CFG_DUMP_EVT
        WMT_DBG_FUNC("WAKEUP_WAKE_EVT read len %d [%02x,%02x,%02x,%02x,%02x,%02x]\n",
             (INT32)u4Res, evtBuf[0], evtBuf[1], evtBuf[2], evtBuf[3], evtBuf[4], evtBuf[5]);
#endif
        if (iRet || (u4Res != osal_sizeof(WMT_SET_WAKEUP_WAKE_EVT))) {
            WMT_ERR_FUNC("read WAKEUP_WAKE_EVT fail(%d)\n", iRet);
            return -5;
        }
        //WMT_DBG_FUNC("read WMT_SET_WAKEUP_WAKE_EVT ok");

#if CFG_CHECK_WMT_RESULT
        if (osal_memcmp(evtBuf, WMT_SET_WAKEUP_WAKE_EVT, osal_sizeof(WMT_SET_WAKEUP_WAKE_EVT)) != 0) {
            WMT_ERR_FUNC("write WMT_SET_WAKEUP_WAKE_CMD_RAW status fail\n");
            return -6;
        }
#endif

        /* 8. Query baud rate (TEST-ONLY) */
        iRet = wmt_core_init_script(init_table_2, osal_array_size(init_table_2));
        if (iRet) {
            WMT_ERR_FUNC("init_table_2 fail(%d)\n", iRet);
            return -7;
        }
    }
    if (WMT_HIF_UART == pWmtHifConf->hifType) {
        /* 9. Set chip STP options */
        iRet = wmt_core_init_script(init_table_4, osal_array_size(init_table_4));
        if (iRet) {
            WMT_ERR_FUNC("init_table_4 fail(%d)\n", iRet);
            return -8;
        }

        /* 10. Enable host STP-UART mode */
        ctrlPa1 = WMT_STP_CONF_MODE; ctrlPa2 = MTKSTP_UART_FULL_MODE;
        iRet = wmt_core_ctrl(WMT_CTRL_STP_CONF, &ctrlPa1, &ctrlPa2);
        ctrlPa1 = WMT_STP_CONF_EN; ctrlPa2 = 1;
        iRet += wmt_core_ctrl(WMT_CTRL_STP_CONF, &ctrlPa1, &ctrlPa2);
        if (iRet) {
            WMT_ERR_FUNC("enable host STP-UART-FULL mode fail(%d)\n", iRet);
            return -9;
        }
        WMT_INFO_FUNC("enable host STP-UART-FULL mode\n");
        /*11. wait for 10ms, enough for chip do mechanism switch.(at least 2ms is needed)*/
        osal_msleep(10);
        /* 12. Query chip STP options (TEST-ONLY) */
        /* 13. Query baud rate (stp, TEST-ONLY) */
        iRet = wmt_core_init_script(init_table_5, osal_array_size(init_table_5));
        if (iRet) {
            WMT_ERR_FUNC("init_table_5 fail(%d)\n", iRet);
            return -10;
        }
    }

    /* 14. download patch */
    iRet = mt6620_patch_dwn();

    WMT_INFO_FUNC("Not to check the patch validity\n");
#if 0
    if (iRet) {
        WMT_ERR_FUNC("patch dwn fail (%d)\n", iRet);
        return -8;
    }
    else {
        WMT_INFO_FUNC("patch dwn ok\n");
    }
#endif
    /* 15. WMT Reset command */
    iRet = wmt_core_init_script(init_table_3, osal_array_size(init_table_3));
    if (iRet) {
        WMT_ERR_FUNC("init_table_3 fail(%d)\n", iRet);
        return -11;
    }
    iRet = wmt_stp_init_coex();
    if (iRet) {
        WMT_ERR_FUNC("init_coex fail(%d)\n", iRet);
        return -12;
    }
    else {
        WMT_INFO_FUNC("init_coex ok\n");
    }

#if 0
    /*10-2 enable 32K By Pass Mode*/
    //if hwVer = E3/E4, please enable 32K by pass mode.
    //does not support mt6620E1/E2, always enable 32k bypass mode
    //if ((hwVer == 0x8a10 || hwVer == 0x8a11))
    {
        WMT_INFO_FUNC("init_table_6 OSC32K");
        iRet = wmt_core_init_script(init_table_6, osal_array_size(init_table_6));
        if (iRet == 0) {
            WMT_DBG_FUNC("init_table_6 OSC32K, successful\n");
        }
        else {
            WMT_WARN_FUNC("init table 6 OSC32K fail, continue init...\n");
            /*return -11;*/
        }
    }
#endif

    
    /* 15. Set FM strap */
    WMT_STRAP_CONF_CMD_FM_COMM[5] = (UCHAR)pWmtHifConf->au4StrapConf[0];
    WMT_STRAP_CONF_EVT[5] = (UCHAR)pWmtHifConf->au4StrapConf[0];
    iRet = wmt_core_init_script(init_table_5_1, osal_array_size(init_table_5_1));
    if (iRet) {
        WMT_ERR_FUNC("init_table_5_1 fm mode(%d) fail(%d)\n",
            pWmtHifConf->au4StrapConf[0],
            iRet);
        return -15;
    }
    WMT_INFO_FUNC("set fm mode (%d) ok\n", pWmtHifConf->au4StrapConf[0]);

#if CFG_SET_OPT_REG /*set registers*/
        iRet = wmt_core_init_script(set_registers, osal_array_size(set_registers));
        if (iRet) {
            WMT_ERR_FUNC("set_registers fail(%d)", iRet);
            return -16;
        }
#endif

#if 0
    /* 16. trace32 dump when fw assert*/
    {
        INT32 val = 0x00000001;
        WMT_INFO_FUNC("enable assert dump");
        wmt_reg_rw_raw(1, 0x0100092c, &val, 0xFFFFFFFF);
    }
#endif

#if CFG_WMT_PS_SUPPORT
    osal_assert(NULL != gp_mt6620_info);
    if (NULL != gp_mt6620_info) {
       if (MTK_WCN_BOOL_FALSE != gp_mt6620_info->bPsmSupport) {
           wmt_lib_ps_enable();
       }
       else {
           wmt_lib_ps_disable();
       }
    }
#endif

    return 0;
}

static INT32
mt6620_sw_deinit (
    P_WMT_HIF_CONF pWmtHifConf
    )
{
    WMT_DBG_FUNC(" start\n");

#if CFG_WMT_PS_SUPPORT
    osal_assert(NULL != gp_mt6620_info);
    if ( (NULL != gp_mt6620_info)
        && (MTK_WCN_BOOL_FALSE != gp_mt6620_info->bPsmSupport) ) {
           wmt_lib_ps_disable();
       }
#endif

    gp_mt6620_info = NULL;

    return 0;
}

static INT32
mt6620_aif_ctrl (
    WMT_IC_PIN_STATE state,
    UINT32 flag
    )
{
    INT32   ret = -1;
    UINT32  val;

    if ( (flag & WMT_LIB_AIF_FLAG_MASK) == WMT_LIB_AIF_FLAG_SHARE ) {
        WMT_INFO_FUNC("PCM & I2S PIN SHARE\n");
        switch (state) {
        case WMT_IC_AIF_0:
            /* BT_PCM_OFF & FM line in/out */
            val = 0x00000770;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000000;
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);
            break;

        case WMT_IC_AIF_1:
            /* BT_PCM_ON & FM line in/out */
            val = 0x00000700;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000000;
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);
            break;

        case WMT_IC_AIF_2:
            /* BT_PCM_OFF & FM I2S */
            val = 0x00000710;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000800; /* 800:3-wire, 000: 4-wire */
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);
            break;
        default:
            WMT_ERR_FUNC("unsupported state (%d) \n", state);
            ret = -1;
            break;
        }
    }
    else {
         /*PCM & I2S separate*/
        WMT_INFO_FUNC("PCM & I2S PIN SEPARATE\n");
        switch (state) {
        case WMT_IC_AIF_0:
            /* BT_PCM_OFF & FM line in/out */
            val = 0x00000770;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000000;
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);
            break;

        case WMT_IC_AIF_1:
            /* BT_PCM_ON & FM line in/out */
            val = 0x00000700;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000000;
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);
            break;

        case WMT_IC_AIF_2:
            /* BT_PCM_OFF & FM I2S */
            val = 0x00000070;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000800; /* 800:3-wire, 000: 4-wire */
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);

            break;
        case WMT_IC_AIF_3:
            val = 0x00000000;
            ret = wmt_core_reg_rw_raw(1, 0x80050140, &val, 0x00000FF0);
            val = 0x00000800; /* 800:3-wire, 000: 4-wire */
            ret += wmt_core_reg_rw_raw(1, 0x80050150, &val, 0x00000800);

        default:
            WMT_ERR_FUNC("unsupported state (%d) \n", state);
            ret = -1;
            break;
        }
    }

    if (!ret) {
        WMT_INFO_FUNC("new state(%d) ok\n", state);
    }
    else {
        WMT_WARN_FUNC("new state(%d) fail(%d)\n", state, ret);
    }

    return ret;
}

static INT32
mt6620_pin_ctrl (
    WMT_IC_PIN_ID id,
    WMT_IC_PIN_STATE state,
    UINT32 flag
    )
{
    INT32 ret;

    WMT_DBG_FUNC("ic pin id:%d, state:%d, flag:0x%x\n", id , state, flag);

    ret = -1;
    switch (id) {
    case WMT_IC_PIN_AUDIO:
        ret = mt6620_aif_ctrl(state, flag);
        break;

    case WMT_IC_PIN_EEDI:
        WMT_WARN_FUNC("TBD!!");
        ret = 0;
        break;

    case WMT_IC_PIN_EEDO:
        WMT_WARN_FUNC("TBD!!");
        ret = 0;
        break;
    case WMT_IC_PIN_GSYNC:
        ret = 0;
        WMT_WARN_FUNC("TBD!!");
        break;
    default:
        break;
    }
    WMT_INFO_FUNC("ret = (%d)\n" , ret);

    return ret;
}


static INT32
mt6620_ver_check (VOID)
{
    UINT32 hw_ver;
    UINT32 fw_ver;
    INT32 iret;
    const WMT_IC_INFO_S *p_info;
    UINT32 ctrlPa1;
    UINT32 ctrlPa2;

    /* 1. identify chip versions: HVR(HW_VER) and FVR(FW_VER) */
    WMT_LOUD_FUNC("MT6620: before read hw_ver (hw version)\n");
    iret = wmt_core_reg_rw_raw(0, GEN_HVR, &hw_ver, GEN_VER_MASK);
    if (iret) {
        WMT_ERR_FUNC("MT6620: read hw_ver fail:%d\n", iret);
        return -2;
    }
    WMT_INFO_FUNC("MT6620: read hw_ver (hw version) (0x%x)\n", hw_ver);

    WMT_LOUD_FUNC("MT6620: before fw_ver (rom version) \n");
    iret = wmt_core_reg_rw_raw(0, GEN_FVR, &fw_ver, GEN_VER_MASK);
    if (iret) {
        WMT_ERR_FUNC("MT6620: read fw_ver fail:%d\n", iret);
        return -2;
    }
    WMT_INFO_FUNC("MT6620: read fw_ver (rom version) (0x%x)\n", fw_ver);

    p_info = mt6620_find_wmt_ic_info(hw_ver);
    if (NULL == p_info) {
        WMT_ERR_FUNC("MT6620: hw_ver(0x%x) find wmt ic info fail\n");
        return -3;
    }

    WMT_INFO_FUNC("MT6620: wmt ic info: %s.%s (0x%x, WMTHWVER:%d, patch_ext:%s)\n",
        p_info->cChipName, p_info->cChipVersion,
        p_info->u4HwVer, p_info->eWmtHwVer,
        p_info->cPatchNameExt);

    /* hw id & version */
    ctrlPa1 = (0x00006620UL << 16) | (hw_ver & 0x0000FFFF);
    /* translated hw version & fw rom version */
    ctrlPa2 = ((UINT32)(p_info->eWmtHwVer) << 16) | (fw_ver & 0x0000FFFF);

    iret = wmt_core_ctrl(WMT_CTRL_HWIDVER_SET, &ctrlPa1, &ctrlPa2);
    if (iret) {
        WMT_WARN_FUNC("MT6620: WMT_CTRL_HWIDVER_SET fail(%d)\n", iret);
    }

    gp_mt6620_info = p_info;
    return 0;
}

static const WMT_IC_INFO_S*
mt6620_find_wmt_ic_info (
    const UINT32 hw_ver
    )
{
    /* match chipversion with u4HwVer item in mt6620_info_table */
    const UINT32 size = osal_array_size(mt6620_info_table);
    INT32 index;

    /* George: reverse the search order to favor newer version products */
    // TODO:[FixMe][GeorgeKuo] Remove full match once API wmt_lib_get_hwver() is changed correctly in the future!!
    // Leave full match here is a workaround for GPS to distinguish E3/E4 ICs.
    index = size - 1;
    /* full match */
    while ( (0 <= index)
        && (hw_ver != mt6620_info_table[index].u4HwVer) /* full match */
        ) {
        --index;
    }
    if (0 <= index) {
        WMT_INFO_FUNC("found ic info(0x%x) by full match! index:%d\n", hw_ver, index);
        return &mt6620_info_table[index];
    }

    WMT_WARN_FUNC("find no ic info for (0x%x) by full match!try major num match!\n", hw_ver);

    /* George: The ONLY CORRECT method to find supported hw table. Match MAJOR
     * NUM only can help us support future minor hw ECO, or fab switch, etc.
     * FULL matching eliminate such flexibility and software package have to be
     * updated EACH TIME even when minor hw ECO or fab switch!!!
     */
    /* George: reverse the search order to favor newer version products */
    index = size - 1;
    /* major num match */
    while ( (0 <= index)
        && (MAJORNUM(hw_ver) != MAJORNUM(mt6620_info_table[index].u4HwVer))
        ) {
        --index;
    }
    if (0 <= index) {
        WMT_INFO_FUNC("MT6620: found ic info for hw_ver(0x%x) by major num! index:%d\n", hw_ver, index);
        return &mt6620_info_table[index];
    }

    WMT_ERR_FUNC("MT6620: find no ic info for hw_ver(0x%x) by full match nor major num match!\n", hw_ver);
    return NULL;
}


static INT32
wmt_stp_init_coex (VOID)
{
    INT32 iRet;
    UINT32 addr;
    WMT_GEN_CONF *pWmtGenConf;

    #define COEX_WMT  0
    #define COEX_BT   1
    #define COEX_WIFI 2
    #define COEX_PTA  3
    #define COEX_MISC 4

    /*Get wmt config*/
    iRet = wmt_core_ctrl(WMT_CTRL_GET_WMT_CONF, &addr, 0);
    if (iRet) {
        WMT_ERR_FUNC("ctrl GET_WMT_CONF fail(%d)\n", iRet);
        return -2;
    }
    WMT_INFO_FUNC("ctrl GET_WMT_CONF ok(0x%08lx)\n", addr);

    pWmtGenConf = (P_WMT_GEN_CONF)addr;

    /*Check if WMT.cfg exists*/
    if (pWmtGenConf->cfgExist == 0) {
        WMT_INFO_FUNC("cfgExist == 0, skip config chip\n");
        /*if WMT.cfg not existed, still return success and adopt the default value*/
        return 0;
    }

    /*Dump the coex-related info*/
    WMT_DBG_FUNC("coex_wmt:0x%x 0x%x 0x%x\n",
        pWmtGenConf->coex_wmt_ant_mode,
        pWmtGenConf->coex_wmt_wifi_time_ctl,
        pWmtGenConf->coex_wmt_ext_pta_dev_on
        );
    WMT_DBG_FUNC("coex_bt:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        pWmtGenConf->coex_bt_rssi_upper_limit,
        pWmtGenConf->coex_bt_rssi_mid_limit,
        pWmtGenConf->coex_bt_rssi_lower_limit,
        pWmtGenConf->coex_bt_pwr_high,
        pWmtGenConf->coex_bt_pwr_mid,
        pWmtGenConf->coex_bt_pwr_low
        );
    WMT_DBG_FUNC("coex_wifi:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        pWmtGenConf->coex_wifi_rssi_upper_limit,
        pWmtGenConf->coex_wifi_rssi_mid_limit,
        pWmtGenConf->coex_wifi_rssi_lower_limit,
        pWmtGenConf->coex_wifi_pwr_high,
        pWmtGenConf->coex_wifi_pwr_mid,
        pWmtGenConf->coex_wifi_pwr_low
        );
    WMT_DBG_FUNC("coex_ext_pta:0x%x 0x%x 0x%x 0x%x 0x%x 0x%x 0x%x\n",
        pWmtGenConf->coex_ext_pta_hi_tx_tag,
        pWmtGenConf->coex_ext_pta_hi_rx_tag,
        pWmtGenConf->coex_ext_pta_lo_tx_tag,
        pWmtGenConf->coex_ext_pta_lo_rx_tag,
        pWmtGenConf->coex_ext_pta_sample_t1,
        pWmtGenConf->coex_ext_pta_sample_t2,
        pWmtGenConf->coex_ext_pta_wifi_bt_con_trx
        );
    WMT_DBG_FUNC("coex_misc:0x%x 0x%x 0x%x\n",
        pWmtGenConf->coex_misc_ext_pta_on,
        pWmtGenConf->coex_misc_ext_feature_set
        );

    /*command adjustion due to WMT.cfg*/
    coex_table[COEX_WMT].cmd[5]= pWmtGenConf->coex_wmt_ant_mode;
    coex_table[COEX_WMT].cmd[6]= pWmtGenConf->coex_wmt_wifi_time_ctl;
    coex_table[COEX_WMT].cmd[7]= pWmtGenConf->coex_wmt_ext_pta_dev_on;
    if (gWmtDbgLvl >= WMT_LOG_DBG) {
        wmt_core_dump_data(&coex_table[COEX_WMT].cmd[0],
            coex_table[COEX_WMT].str,
            coex_table[COEX_WMT].cmdSz);
    }

    coex_table[COEX_BT].cmd[9]=   pWmtGenConf->coex_bt_rssi_upper_limit;
    coex_table[COEX_BT].cmd[10]=  pWmtGenConf->coex_bt_rssi_mid_limit;
    coex_table[COEX_BT].cmd[11]=  pWmtGenConf->coex_bt_rssi_lower_limit;
    coex_table[COEX_BT].cmd[12]=  pWmtGenConf->coex_bt_pwr_high;
    coex_table[COEX_BT].cmd[13]=  pWmtGenConf->coex_bt_pwr_mid;
    coex_table[COEX_BT].cmd[14]=  pWmtGenConf->coex_bt_pwr_low;
    if (gWmtDbgLvl >= WMT_LOG_DBG) {
        wmt_core_dump_data(&coex_table[COEX_BT].cmd[0],
            coex_table[COEX_BT].str,
            coex_table[COEX_BT].cmdSz);
    }
    coex_table[COEX_WIFI].cmd[10]=   pWmtGenConf->coex_wifi_rssi_upper_limit;
    coex_table[COEX_WIFI].cmd[11]=  pWmtGenConf->coex_wifi_rssi_mid_limit;
    coex_table[COEX_WIFI].cmd[12]=  pWmtGenConf->coex_wifi_rssi_lower_limit;
    coex_table[COEX_WIFI].cmd[13]=  pWmtGenConf->coex_wifi_pwr_high;
    coex_table[COEX_WIFI].cmd[14]=  pWmtGenConf->coex_wifi_pwr_mid;
    coex_table[COEX_WIFI].cmd[15]=  pWmtGenConf->coex_wifi_pwr_low;
    if (gWmtDbgLvl >= WMT_LOG_DBG) {
        wmt_core_dump_data(&coex_table[COEX_WIFI].cmd[0],
            coex_table[COEX_WIFI].str,
            coex_table[COEX_WIFI].cmdSz);
    }
    coex_table[COEX_PTA].cmd[5]=   pWmtGenConf->coex_ext_pta_hi_tx_tag;
    coex_table[COEX_PTA].cmd[6]=   pWmtGenConf->coex_ext_pta_hi_rx_tag;
    coex_table[COEX_PTA].cmd[7]=   pWmtGenConf->coex_ext_pta_lo_tx_tag;
    coex_table[COEX_PTA].cmd[8]=   pWmtGenConf->coex_ext_pta_lo_rx_tag;
    coex_table[COEX_PTA].cmd[9]=   ((pWmtGenConf->coex_ext_pta_sample_t1 & 0xff00) >> 8);
    coex_table[COEX_PTA].cmd[10]=  ((pWmtGenConf->coex_ext_pta_sample_t1 & 0x00ff) >> 0);
    coex_table[COEX_PTA].cmd[11]=  ((pWmtGenConf->coex_ext_pta_sample_t2 & 0xff00) >> 8);
    coex_table[COEX_PTA].cmd[12]=  ((pWmtGenConf->coex_ext_pta_sample_t2 & 0x00ff) >> 0);
    coex_table[COEX_PTA].cmd[13]=  pWmtGenConf->coex_ext_pta_wifi_bt_con_trx;
    if (gWmtDbgLvl >= WMT_LOG_DBG) {
        wmt_core_dump_data(&coex_table[COEX_PTA].cmd[0],
            coex_table[COEX_PTA].str,
            coex_table[COEX_PTA].cmdSz);
    }

    osal_memcpy(&coex_table[COEX_MISC].cmd[5], &pWmtGenConf->coex_misc_ext_pta_on, sizeof(pWmtGenConf->coex_misc_ext_pta_on));
    osal_memcpy(&coex_table[COEX_MISC].cmd[9], &pWmtGenConf->coex_misc_ext_feature_set, sizeof(pWmtGenConf->coex_misc_ext_feature_set));
    if (gWmtDbgLvl >= WMT_LOG_DBG) {
        wmt_core_dump_data(& coex_table[COEX_MISC].cmd[0],
            coex_table[COEX_MISC].str,
            coex_table[COEX_MISC].cmdSz);
    }
    iRet = wmt_core_init_script(coex_table, sizeof(coex_table)/sizeof(coex_table[0]));

    return iRet;
}

#if CFG_PATCH_DWN_DIVISION
static INT32
mt6620_patch_dwn_div(PUINT8 pBuf, UINT32 patchSize, UINT32 patchSizePerFrag)
{
    INT32 iRet;
    UINT32 fragSeq;
    UINT32 fragNum;
    UINT16 fragSize;
    UINT32 offset;
    UINT16 cmdLen;
    UINT32 u4Res;
    UINT8 evtBuf[8];

    iRet = 0;

    /* |<-WMT_CMD: 5 Bytes->|<-patch body: X Bytes (X=patchSize)----->|
     * pbuf points to the start of WMT_CMD.
     */
    fragNum = patchSize / patchSizePerFrag;
    fragNum += ((fragNum * patchSizePerFrag) == patchSize) ? 0 : 1;
    WMT_DBG_FUNC("patch size(%d) fragNum(%d)\n", patchSize, fragNum);

    /* send all fragments */
    offset = sizeof(WMT_PATCH_CMD);
    fragSeq = 0;
    while (fragSeq < fragNum) {
        WMT_DBG_FUNC("patch size(%d) fragNum(%d)\n", patchSize, fragNum);
        if (fragSeq == (fragNum-1)) {
            /* last fragment */
            fragSize = patchSize - fragSeq * patchSizePerFrag;
            WMT_PATCH_CMD[4] = WMT_PATCH_FRAG_LAST;
        }
        else {
            fragSize = patchSizePerFrag;
            WMT_PATCH_CMD[4] = (fragSeq == 0) ? WMT_PATCH_FRAG_1ST: WMT_PATCH_FRAG_MID;
        }
        /* update length field in CMD:flag+frag*/
        cmdLen = 1 + fragSize;
        osal_memcpy(&WMT_PATCH_CMD[2], &cmdLen, 2);
        /* copy patch CMD to buf (OVERWRITE LAST 5 BYTES IN PREV FRAG!!!) */
        osal_memcpy(pBuf + offset - sizeof(WMT_PATCH_CMD), WMT_PATCH_CMD, sizeof(WMT_PATCH_CMD));

        iRet = wmt_core_tx(pBuf + offset - sizeof(WMT_PATCH_CMD), fragSize + sizeof(WMT_PATCH_CMD), &u4Res, MTK_WCN_BOOL_FALSE);
        if (iRet || (u4Res != fragSize + sizeof(WMT_PATCH_CMD))) {
            WMT_ERR_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) fail(%d)\n", fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res, iRet );
            iRet = -4;
            break;
        }
        WMT_DBG_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) ok\n",
            fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res);

        osal_memset(evtBuf, 0, sizeof(evtBuf));
        iRet = wmt_core_rx(evtBuf, sizeof(WMT_PATCH_EVT), &u4Res);
        if (iRet || (u4Res != sizeof(WMT_PATCH_EVT))) {
            WMT_ERR_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) fail(%d)\n", sizeof(WMT_PATCH_EVT), u4Res, iRet);
            iRet = -5;
            break;
        }
    #if CFG_CHECK_WMT_RESULT
        if (osal_memcmp(evtBuf, WMT_PATCH_EVT, sizeof(WMT_PATCH_EVT)) != 0) {
            WMT_ERR_FUNC("wmt_core: compare WMT_PATCH_EVT result error rx(%d):[%02X,%02X,%02X,%02X,%02X] exp(%d):[%02X,%02X,%02X,%02X,%02X]\n",
                u4Res, evtBuf[0], evtBuf[1], evtBuf[2], evtBuf[3], evtBuf[4], sizeof(WMT_PATCH_EVT), WMT_PATCH_EVT[0], WMT_PATCH_EVT[1], WMT_PATCH_EVT[2], WMT_PATCH_EVT[3], WMT_PATCH_EVT[4]);
            iRet = -6;
            break;
        }
    #endif
        WMT_DBG_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) ok\n",
            sizeof(WMT_PATCH_EVT), u4Res);

        //offset += patchSizePerFrag;
        /* The above line wrong for last block but will not affect result for
         * the while loop will stop after last block is sent successfully.
         */
        offset += fragSize;
        ++fragSeq;
    }

    WMT_INFO_FUNC("wmt_core: patch dwn:%d frag(%d, %d) %s\n",
        iRet, fragSeq, fragSize, (!iRet && (fragSeq == fragNum)) ? "ok": "fail");

    if (fragSeq != fragNum) {
        iRet = -7;
    }

    return iRet;
}

#else /* no div */

static INT32
mt6620_patch_dwn_nodiv(PUINT8 pBuf, UINT32 patchSize, UINT32 patchSizePerFrag)
{
    INT32 iRet;
    UINT32 fragSeq;
    UINT16 fragSize;
    UINT32 offset;
    UINT16 cmdLen;
    UINT32 u4Res;
    UINT8 evtBuf[8];

    iRet = 0;

    /* |<-WMT_CMD: 5 Bytes->|<-patch body: X Bytes (X=patchSize)----->|
     * pbuf points to the start of WMT_CMD.
     * Total = sizeof(WMT_PATCH_CMD) + patchSize.
     */
    WMT_DBG_FUNC("patch size(%d)\n", patchSize);

    /* send all fragments */
    /* |<-WMT_CMD: 5 Bytes->|<-patch body: X Bytes (X=patchSize)----->|
     *  |[0]|[1]|[2]|[3]|[4]|[5]offset starts from here.
     */
    offset = sizeof(WMT_PATCH_CMD);
    fragSeq = 0;
    while ( offset < (sizeof(WMT_PATCH_CMD) + patchSize) ) {
        if ( (offset + patchSizePerFrag) >= (sizeof(WMT_PATCH_CMD) + patchSize) ) {
            /* If current offset value + fragSize exceeds total length, it's now
             * at the last fragment.
             */
            fragSize = patchSize - fragSeq * patchSizePerFrag;
            WMT_PATCH_CMD[4] = WMT_PATCH_FRAG_LAST;
        }
        else {
            fragSize = patchSizePerFrag;
            WMT_PATCH_CMD[4] = (fragSeq == 0) ? WMT_PATCH_FRAG_1ST: WMT_PATCH_FRAG_MID;
        }
        WMT_DBG_FUNC("fragSeq(%d) fragSize(%d)\n", fragSeq, fragSize);

        /* update length field in CMD:flag+frag*/
        cmdLen = 1 + fragSize;
        osal_memcpy(&WMT_PATCH_CMD[2], &cmdLen, 2);
        /* copy patch CMD to buf (overwrite last 5-byte in prev frag) */
        osal_memcpy(pBuf + offset - sizeof(WMT_PATCH_CMD), WMT_PATCH_CMD, sizeof(WMT_PATCH_CMD));

        iRet = wmt_core_tx(pBuf + offset - sizeof(WMT_PATCH_CMD), fragSize + sizeof(WMT_PATCH_CMD), &u4Res, MTK_WCN_BOOL_FALSE);
        if (iRet || (u4Res != fragSize + sizeof(WMT_PATCH_CMD))) {
            WMT_ERR_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) fail(%d)\n", fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res, iRet );
            iRet = -4;
            break;
        }
        WMT_DBG_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) ok\n",
            fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res);

        osal_memset(evtBuf, 0, sizeof(evtBuf));
        iRet = wmt_core_rx(evtBuf, sizeof(WMT_PATCH_EVT), &u4Res);
        if (iRet || (u4Res != sizeof(WMT_PATCH_EVT))) {
            WMT_ERR_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) fail(%d)\n", sizeof(WMT_PATCH_EVT), u4Res, iRet);
            iRet = -5;
            break;
        }
    #if CFG_CHECK_WMT_RESULT
        if (osal_memcmp(evtBuf, WMT_PATCH_EVT, sizeof(WMT_PATCH_EVT)) != 0) {
            WMT_ERR_FUNC("wmt_core: compare WMT_PATCH_EVT result error rx(%d):[%02X,%02X,%02X,%02X,%02X] exp(%d):[%02X,%02X,%02X,%02X,%02X]\n",
                u4Res, evtBuf[0], evtBuf[1], evtBuf[2], evtBuf[3], evtBuf[4], sizeof(WMT_PATCH_EVT), WMT_PATCH_EVT[0], WMT_PATCH_EVT[1], WMT_PATCH_EVT[2], WMT_PATCH_EVT[3], WMT_PATCH_EVT[4]);
            iRet = -6;
            break;
        }
    #endif
        WMT_DBG_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) ok\n",
            sizeof(WMT_PATCH_EVT), u4Res);

        offset += fragSize;
        ++fragSeq;
    }

    if (offset != (sizeof(WMT_PATCH_CMD) + patchSize)) {
        iRet = -7;
    }

    WMT_INFO_FUNC("wmt_core: patch dwn:%d frag(%d, %d) %s\n",
        iRet, fragSeq, fragSize, (iRet) ? "fail" : "ok");

    return iRet;
}

#endif /* end of CFG_PATCH_DWN_DIVISION */

static INT32
mt6620_patch_dwn (VOID)
{
    INT32 iRet;
    P_WMT_PATCH patchHdr;
    PUINT8 pbuf;
    UINT32 patchSize;
#if 0 /* moved to patch_dwn_(no)div() functions */
    UINT32 fragSeq;
    UINT32 fragNum;
    UINT32 offset;
    UINT16 cmdLen;
    UINT16 fragSize;
    UINT32 u4Res;
    UINT8 evtBuf[8];
#endif
    PCHAR cDataTime;
    /*PCHAR cPlat = NULL;*/
    UINT16 u2HwVer;
    UINT16 u2SwVer;
    UINT32 u4PatchVer;
    UINT32 patchSizePerFrag;
    WMT_CTRL_DATA ctrlData;

    /*1.check hardware information */
    if (NULL == gp_mt6620_info) {
        WMT_ERR_FUNC("null gp_mt6620_info!\n");
        return -1;
    }

    ctrlData.ctrlId = WMT_CTRL_GET_PATCH_NAME;
    ctrlData.au4CtrlData[0] = (UINT32)&gDefPatchName;
    iRet = wmt_ctrl(&ctrlData);

    if (mt6620_update_patch_name()) {
        WMT_ERR_FUNC("invalid patch name, ommit patch download process.\n");
        return -1;
    }

    ctrlData.ctrlId = WMT_CTRL_GET_PATCH;
    ctrlData.au4CtrlData[0] = (UINT32)&gDefPatchName;
    ctrlData.au4CtrlData[1] = (UINT32)&gFullPatchName;
    ctrlData.au4CtrlData[2] = (UINT32)&pbuf;
    ctrlData.au4CtrlData[3] = (UINT32)&patchSize;
    iRet = wmt_ctrl(&ctrlData);
    if (iRet) {
        WMT_ERR_FUNC("wmt_core: WMT_CTRL_GET_PATCH fail:%d \n", iRet);
        iRet = -2;
        goto done;
    }

    /* |<-BCNT_PATCH_BUF_HEADROOM(8) bytes dummy allocated->|<-patch file->| */
    pbuf += BCNT_PATCH_BUF_HEADROOM;
    /* patch file with header:
     * |<-patch header: 28 Bytes->|<-patch body: X Bytes ----->|
     */
    patchHdr = (P_WMT_PATCH)pbuf;
    // check patch file information

    cDataTime = patchHdr->ucDateTime;
    u2HwVer = patchHdr->u2HwVer;
    u2SwVer = patchHdr->u2SwVer;
    u4PatchVer = patchHdr->u4PatchVer;
    /*cPlat = &patchHdr->ucPLat[0];*/

    cDataTime[15] = '\0';
    WMT_INFO_FUNC("===========================================\n");
    WMT_INFO_FUNC("[Combo Patch] Built Time = %s\n", cDataTime);
    WMT_INFO_FUNC("[Combo Patch] Hw Ver = 0x%x\n", ((u2HwVer & 0x00ff) << 8) | ((u2HwVer & 0xff00) >> 8));
    WMT_INFO_FUNC("[Combo Patch] Sw Ver = 0x%x\n", ((u2SwVer & 0x00ff) << 8) | ((u2SwVer & 0xff00) >> 8));
    WMT_INFO_FUNC("[Combo Patch] Ph Ver = 0x%04x\n",  ((u4PatchVer & 0xff000000) >> 24) | ((u4PatchVer & 0x00ff0000) >> 16));
    WMT_INFO_FUNC("[Combo Patch] Platform = %c%c%c%c\n", patchHdr->ucPLat[0],
        patchHdr->ucPLat[1], patchHdr->ucPLat[2], patchHdr->ucPLat[3]);
    WMT_INFO_FUNC("===========================================\n");
    WMT_INFO_FUNC("AML release Ver = %s\n",AML_Ver);
    WMT_INFO_FUNC("===========================================\n");
    // TODO: [FixMe][George]shall we check sw_ver against fw_ver read from chip?!
    if (MTK_WCN_BOOL_FALSE ==
        wmt_core_patch_check(((patchHdr->u2SwVer & 0x00ff) << 8) | ((patchHdr->u2SwVer & 0xff00) >> 8), gp_mt6620_info->u4HwVer)
        ) {
        iRet = -3;
        goto done;
    }
    /* remove patch header:
         * |<-patch body: X Bytes (X=patchSize)--->|
         */
    patchSize -= sizeof(WMT_PATCH);
    pbuf += sizeof(WMT_PATCH);
    patchSizePerFrag = (MAJORNUM(gp_mt6620_info->u4HwVer) != 0) ?
        DEFAULT_PATCH_FRAG_SIZE : MT6620E2_PATCH_FRAG_SIZE;

  /* reserve 1st patch cmd space before patch body
     *        |<-WMT_CMD: 5 Bytes->|<-patch body: X Bytes (X=patchSize)----->|
     */
    pbuf -= sizeof(WMT_PATCH_CMD);

#if CFG_PATCH_DWN_DIVISION
    #if 1 /* move logics to new function mt6620_patch_dwn_div() */
    iRet = mt6620_patch_dwn_div(pbuf, patchSize, patchSizePerFrag);
    #else /* reserve old-styled for reference */

    fragNum = patchSize / patchSizePerFrag;
    fragNum += ((fragNum * patchSizePerFrag) == patchSize) ? 0 : 1;

    WMT_DBG_FUNC("patch size(%d) fragNum(%d)\n", patchSize, fragNum);


    /* send all fragments */
    offset = sizeof(WMT_PATCH_CMD);
    fragSeq = 0;
    while (fragSeq < fragNum) {
        WMT_DBG_FUNC("patch size(%d) fragNum(%d)\n", patchSize, fragNum);
        if (fragSeq == (fragNum-1)) {
            /* last fragment */
            fragSize = patchSize - fragSeq * patchSizePerFrag;
            WMT_PATCH_CMD[4] = WMT_PATCH_FRAG_LAST;
        }
        else {
            fragSize = patchSizePerFrag;
            WMT_PATCH_CMD[4] = (fragSeq == 0) ? WMT_PATCH_FRAG_1ST: WMT_PATCH_FRAG_MID;
        }
        /* update length field in CMD:flag+frag*/
        cmdLen = 1 + fragSize;
        osal_memcpy(&WMT_PATCH_CMD[2], &cmdLen, 2);
        /* copy patch CMD to buf (overwrite last 5-byte in prev frag) */
        osal_memcpy(pbuf + offset - sizeof(WMT_PATCH_CMD), WMT_PATCH_CMD, sizeof(WMT_PATCH_CMD));

        iRet = wmt_core_tx(pbuf + offset - sizeof(WMT_PATCH_CMD), fragSize + sizeof(WMT_PATCH_CMD), &u4Res, MTK_WCN_BOOL_FALSE);
        if (iRet || (u4Res != fragSize + sizeof(WMT_PATCH_CMD))) {
            WMT_ERR_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) fail(%d)\n", fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res, iRet );
            iRet = -4;
            break;
        }
        WMT_DBG_FUNC("wmt_core: write fragSeq(%d) size(%d, %d) ok\n",
            fragSeq, fragSize + sizeof(WMT_PATCH_CMD), u4Res);

        osal_memset(evtBuf, 0, sizeof(evtBuf));
        iRet = wmt_core_rx(evtBuf, sizeof(WMT_PATCH_EVT), &u4Res);
        if (iRet || (u4Res != sizeof(WMT_PATCH_EVT))) {
            WMT_ERR_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) fail(%d)\n", sizeof(WMT_PATCH_EVT), u4Res, iRet);
            iRet = -5;
            break;
        }
    #if CFG_CHECK_WMT_RESULT
        if (osal_memcmp(evtBuf, WMT_PATCH_EVT, sizeof(WMT_PATCH_EVT)) != 0) {
            WMT_ERR_FUNC("wmt_core: compare WMT_PATCH_EVT result error rx(%d):[%02X,%02X,%02X,%02X,%02X] exp(%d):[%02X,%02X,%02X,%02X,%02X]\n",
                u4Res, evtBuf[0], evtBuf[1], evtBuf[2], evtBuf[3], evtBuf[4], sizeof(WMT_PATCH_EVT), WMT_PATCH_EVT[0], WMT_PATCH_EVT[1], WMT_PATCH_EVT[2], WMT_PATCH_EVT[3], WMT_PATCH_EVT[4]);
            iRet = -6;
            break;
        }
    #endif
        WMT_DBG_FUNC("wmt_core: read WMT_PATCH_EVT length(%d, %d) ok\n",
            sizeof(WMT_PATCH_EVT), u4Res);

        offset += patchSizePerFrag;
        ++fragSeq;
    }

    WMT_INFO_FUNC("wmt_core: patch dwn:%d frag(%d, %d) %s\n",
        iRet, fragSeq, fragSize, (!iRet && (fragSeq == fragNum)) ? "ok": "fail");

    if (fragSeq != fragNum) {
        iRet = -7;
    }
    #endif
#else
    /* no division version */
    iRet = mt6620_patch_dwn_nodiv(pbuf, patchSize, patchSizePerFrag);

#endif /* end of CFG_PATCH_DWN_DIVISION */

done:
    /* WMT_CTRL_FREE_PATCH always return 0 */
    wmt_core_ctrl(WMT_CTRL_FREE_PATCH, NULL, NULL);
    if ( (iRet == -2) || (iRet == -3) ) {
        /*no patch found or patch version does not match with hw version, we check if patch is mandatory or not, if yes, return iRet, if not return 0*/
        if (MTK_WCN_BOOL_FALSE != gp_mt6620_info->bWorkWithoutPatch) {
            iRet = 0;
        }
    }

    return iRet;
}

static INT32
mt6620_update_patch_name (VOID)
{
    INT32 len;
    UCHAR cTmpPatchName[OSAL_NAME_MAX + 1];

    /*init.get hardware version */
    // TODO:[FixMe][GeorgeKuo]: check using memcpy or strncpy???
    /*osal_memcpy (gFullPatchName, gDefPatchName, osal_strlen(gDefPatchName));*/
    osal_strncpy(gFullPatchName, gDefPatchName, osal_sizeof(gFullPatchName));

    /*1.check hardware information */
    if (NULL == gp_mt6620_info) {
        WMT_ERR_FUNC("null gp_mt6620_info!\n");
        osal_memset(gFullPatchName, 0, osal_sizeof(gFullPatchName));
        return -1;
    }

    osal_memset(&cTmpPatchName[0], 0, sizeof(cTmpPatchName));
    /*2.make possible firmware patch name with original name and hardware version*/
    if ( (osal_strlen(gDefPatchName) > osal_strlen(WMT_IC_PATCH_TAIL))
        && ((osal_strlen(gDefPatchName) + osal_strlen(WMT_IC_PATCH_DUMMY_EXT) <= OSAL_NAME_MAX))
        ) {
        len = osal_strlen(gDefPatchName) - osal_strlen(WMT_IC_PATCH_TAIL);
        osal_memcpy (cTmpPatchName, gDefPatchName, len > OSAL_NAME_MAX ? OSAL_NAME_MAX : len);
        osal_memcpy (cTmpPatchName + osal_strlen(cTmpPatchName), gp_mt6620_info->cPatchNameExt, osal_strlen(gp_mt6620_info->cPatchNameExt));
        osal_memcpy (cTmpPatchName + osal_strlen(cTmpPatchName), WMT_IC_PATCH_TAIL, osal_strlen(WMT_IC_PATCH_TAIL));
        cTmpPatchName[osal_strlen(cTmpPatchName)] = '\0';
    }
    else {
        WMT_ERR_FUNC("invalid default firmware patch name (%s)\n", gDefPatchName);
        osal_memset(gFullPatchName, 0, osal_sizeof(gFullPatchName));
        return -2;
    }

    /*patch with versioned name exist , update cFullPatchName with full named patch*/
    osal_memcpy (gFullPatchName, cTmpPatchName, osal_strlen(cTmpPatchName));
    *(gFullPatchName + osal_strlen(cTmpPatchName)) = '\0';
    WMT_INFO_FUNC("full firmware patch name: %s\n", cTmpPatchName);

    return 0;
}

