 /* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/*
 * The contents of this file are subject to the Netscape Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is Mozilla Communicator client code, 
 * released March 31, 1998. 
 *
 * The Initial Developer of the Original Code is Netscape Communications 
 * Corporation.  Portions created by Netscape are
 * Copyright (C) 1998 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s): 
 *     Daniel Veditz <dveditz@netscape.com>
 *     Douglas Turner <dougt@netscape.com>
 *     Samir Gehani <sgehani@netscape.com>
 */

#include <string.h>
#include "nscore.h"
#include "nsInstallResources.h"

static nsXPIResourceTableItem XPIResTable[] = 
{
    /*---------------------------------------------------------------------*
     *   Install Actions 
     *---------------------------------------------------------------------*/
    { "InstallFile",        "Installing: %s" },
    { "ReplaceFile",        "Replacing: %s" },
    { "DeleteFile",         "Deleting file: %s" },
    { "DeleteComponent",    "Deleting component: %s" },
    { "Execute",            "Executing: %s" },
    { "ExecuteWithArgs",    "Executing: %s with argument: %s" },
    { "Patch",              "Patching: %s" },
    { "Uninstall",          "Uninstalling: %s" },

    // XXX FileOp*() action strings
    // XXX WinReg and WinProfile action strings

    /*---------------------------------------------------------------------*
     *   Dialog Messages 
     *---------------------------------------------------------------------*/
    { "ShouldWeInstallMsg",  "Attempting to download and install software. Do you feel lucky punk?" },
    { "FinishingInstallMsg", "Finishing install... please wait." },

    /*---------------------------------------------------------------------*
     *    Miscellaneous 
     *---------------------------------------------------------------------*/
    { "ERROR", "ERROR" },

    { NS_XPI_EOT, "" }
};

char* 
nsInstallResources::GetDefaultVal(const char* aResName)
{
    char    *currResName = XPIResTable[0].resName;
    char    *currResVal = nsnull;
    PRInt32 idx, len = 0;

    for (idx = 0; 0 != strcmp(currResName, NS_XPI_EOT); idx++)
    {
        currResName = XPIResTable[idx].resName;
        len = strlen(currResName);

        if (0 == strncmp(currResName, aResName, len))
        {
            currResVal = XPIResTable[idx].defaultString;
            break;
        }
    }

    return currResVal;
}

