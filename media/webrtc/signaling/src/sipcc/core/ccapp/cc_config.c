/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "cc_config.h"
#include "CCProvider.h"
#include "phone_debug.h"
#include "cpr_types.h"
#include "dialplan.h"
#include "capability_set.h"
#include "configmgr.h"
#include "dialplanint.h"
#include "ccapp_task.h"
#include "cc_device_manager.h"
#include "config_api.h"

extern int config_parser_main( char *config, int complete_config);
/**
 * Set the maximum line available for registration
 * @param lines the maximum line could be configured.
 * @return
 */
int CC_Config_SetAvailableLines(cc_lineid_t lines) {
    ccappTaskPostMsg(CCAPP_UPDATELINES, &lines, sizeof(unsigned short), CCAPP_CCPROVIER);
    return CC_SUCCESS;
}

/**
 * The following section defines the configuration methods.
 */
/**
 * Defines the CFGID parameter setting methods.
 * see cfgid.h for all possible configuration parameters
 * @todo
 */
void CC_Config_setIntValue(int cfgid, int value) {
    config_set_value(cfgid, &value, sizeof(int));
    return;
}

void CC_Config_setBooleanValue(int cfgid, cc_boolean value) {
    int temp = (int) value;

    //  For Now, convert all numeric parameters to Integer.  This simplifies
    //  the required SIP and GSM code changes.  We will go to the right size
    //  when we implement the "full" JNI.

    //    config_set_value(cfg_id, &bool_value, sizeof(boolean));
    config_set_value(cfgid, &temp, sizeof(int));
    return;
}

void CC_Config_setStringValue(int cfgid, const char* value) {
    config_set_string(cfgid, (char *) value);
    return;
}

void CC_Config_setByteValue(int cfgid, unsigned char value) {
    int temp = (int) value;

//  For Now, convert all numeric parameters to Integer.  This simplifies
//  the required SIP and GSM code changes.  We will go to the right size
//  when we implement the "full" JNI.

//    config_set_value(cfg_id, &byte_value, sizeof(unsigned char));
    config_set_value(cfgid, &temp, sizeof(int));
    return;
}

void CC_Config_setArrayValue(int cfgid, char *byte_array, int length) {
    unsigned char *byte_ptr;
    int i;

    byte_ptr = cpr_malloc(length);
    if (byte_ptr == NULL) {
        TNP_DEBUG(DEB_F_PREFIX"setPropertyCacheByteArray():malloc failed.", DEB_F_PREFIX_ARGS(JNI, "nSetPropertyCacheByteArray"));
        return;
    }

    for (i = 0; i < length; i++) {
        byte_ptr[i] = (unsigned char) byte_array[i];
    }
    config_set_value(cfgid, byte_ptr, length);
    cpr_free(byte_ptr);

    return;
}

/**
 * Set the dialplan file
 * @param dial_plan_string the dial plan content string
 * @param length the length of dial plan string, the maximum size will be 0x2000.
 * @return string dial plan version stamp
 */
char* CC_Config_setDialPlan(const char *dial_plan_string, int length) {
    const char fname[] = "CC_Config_setDialPlan";
    int ret;

    /**
     * If the string is null, empty or oversized, we will reset the dial
     * plan by setting the length to 0.
     */
    if (dial_plan_string == NULL || length == 0 || length >= DIALPLAN_MAX_SIZE) {
        TNP_DEBUG(DEB_F_PREFIX"Setting NULL dialplan string (length [%d] is 0, or length is larger than maximum [%d])",
                DEB_F_PREFIX_ARGS(JNI, fname), length, DIALPLAN_MAX_SIZE);

        dp_init_template (NULL, 0);
        return (NULL);
     }

    ret = dp_init_template(dial_plan_string, length);
    TNP_DEBUG(DEB_F_PREFIX"Parsed dial_plan_string.  Version=[%s], Length=[%d]", DEB_F_PREFIX_ARGS(JNI, fname), g_dp_version_stamp, length);
    if (ret != 0)
    {
        return (NULL);
    }

    return (g_dp_version_stamp);
}

/**
 * Set the feature control plan
 * @param fcp plan string
 * @param length the length of fcp string
 * @return string feature version stamp
 */

char* CC_Config_setFcp(const char *fcp_plan_string, int len) {
    const   char fname[] = "CC_Config_setFcp";
    int ret               = 0;

    /**
     * If the string is null, return null (version)
     */

    TNP_DEBUG(DEB_F_PREFIX"FCP Parsing FCP doc", DEB_F_PREFIX_ARGS(JNI, fname));
    if (fcp_plan_string == NULL)
    {
        TNP_DEBUG(DEB_F_PREFIX"Null FCP xml document",
                DEB_F_PREFIX_ARGS(JNI, fname));

        fcp_init_template (NULL);
        return (NULL);
    }

    ret = fcp_init_template (fcp_plan_string);
    TNP_DEBUG(DEB_F_PREFIX"Parsed FCP xml.  Version=[%s]", DEB_F_PREFIX_ARGS(JNI, fname), g_fp_version_stamp);
    if (ret != 0)
    {
        return (NULL);
    }

    return (g_fp_version_stamp);
}
