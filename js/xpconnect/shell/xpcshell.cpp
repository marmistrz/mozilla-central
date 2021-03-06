/* -*- Mode: C++; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 4 -*-
 * vim: set ts=2 sw=4 et tw=80:
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

/* XPConnect JavaScript interactive shell. */

#include <stdio.h>

#include "mozilla/Util.h"
#include "mozilla/WindowsDllBlocklist.h"

#include "nsXULAppAPI.h"
#ifdef XP_MACOSX
#include "xpcshellMacUtils.h"
#endif
#ifdef XP_WIN
#include <windows.h>
#include <shlobj.h>

// we want a wmain entry point
#define XRE_DONT_PROTECT_DLL_LOAD
#define XRE_WANT_ENVIRON
#include "nsWindowsWMain.cpp"
#endif

int
main(int argc, char** argv, char** envp)
{
#ifdef XP_MACOSX
    InitAutoreleasePool();
#endif

#ifdef HAVE_SETBUF
    // unbuffer stdout so that output is in the correct order; note that stderr
    // is unbuffered by default
    setbuf(stdout, 0);
#endif

#ifdef HAS_DLL_BLOCKLIST
    DllBlocklist_Initialize();
#endif

    int result = XRE_XPCShellMain(argc, argv, envp);

#ifdef XP_MACOSX
    FinishAutoreleasePool();
#endif

    return result;
}
