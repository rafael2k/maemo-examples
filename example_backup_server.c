/**
 * This file is part of maemo-examples package
 * 
 * This maemo code example is licensed under a MIT-style license,
 * that can be found in the file called "COPYING" in the same
 * directory as this file.
 * Copyright (c) 2007-2008 Nokia Corporation. All rights reserved.
 */

#include <libosso.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    osso_context_t *osso_context;
    osso_rpc_t retval;
    osso_return_t ret;

    /* Initialize maemo application */
    osso_context = osso_initialize("com.nokia.backup", "0.0.2", TRUE, NULL);

    /* Check that initialization was ok */
    if (osso_context == NULL) {
        return OSSO_ERROR;
    }

    g_print("Sending backup_start...\n");

    ret = osso_rpc_run(osso_context,
		       "com.nokia.backup",
		       "/com/nokia/backup",
		       "com.nokia.backup",
		       "backup_start", &retval, DBUS_TYPE_INVALID);
    osso_rpc_free_val(&retval);
    
    g_print("Waiting 10 seconds to send backup_finish...\n");
    sleep(10);
    g_print("Sending backup_finish...\n");
    
    ret = osso_rpc_run(osso_context,
		       "com.nokia.backup",
		       "/com/nokia/backup",
		       "com.nokia.backup",
		       "backup_finish", &retval, DBUS_TYPE_INVALID);
    osso_rpc_free_val(&retval);

    osso_deinitialize(osso_context);
    
    return 0;
}
