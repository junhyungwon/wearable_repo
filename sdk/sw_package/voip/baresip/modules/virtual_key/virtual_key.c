/**
 * @file virtual_key/virtual_key.c  communication between main process.
 *
 * Copyright (C) 2020 LF
 */
#include <re.h>
#include <baresip.h>

static int module_init(void)
{
	info("module init virtual_key success\n");
	return 0;
}

static int module_close(void)
{
	return 0;
}

const struct mod_export DECL_EXPORTS(virtual_key) = {
	"virtual_key",
	"application",
	module_init,
	module_close
};
