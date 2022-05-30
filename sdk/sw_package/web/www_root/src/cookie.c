#include <stdio.h>
#include <json-c/json.h>

#include "qdecoder.h"
 
int main(int argc, char **argv)
{
	{
		qentry_t *req = qcgireq_parse(NULL, 0);

		char *mode, *name, *value;

		mode  = req->getstr(req, "mode", false);
		name  = req->getstr(req, "cname", false);
		value = req->getstr(req, "cvalue", false);

		if (mode == NULL)
		{ // View Cookie
			qcgires_setcontenttype(req, "text/plain");
			printf("Total %d entries\n", req->size(req));
			req->print(req, stdout, true);
		}
		else if (!strcmp(mode, "set"))
		{ // Set Cookie
			if (name == NULL || value == NULL)
			{
				qcgires_error(req, "Query not found");
			}
			if (strlen(name) == 0)
			{
				qcgires_error(req, "Empty cookie name can not be stored.");
			}

			qcgires_setcookie(req, name, value, 0, NULL, NULL, false);
			qcgires_setcontenttype(req, "text/html");
			printf("Cookie('%s'='%s') entry is stored.<br>\n", name, value);
			printf("Click <a href='jsonc.cgi'>here</a> to view your cookies\n");
		}
		else if (!strcmp(mode, "remove"))
		{ // Remove Cookie
			if (name == NULL)
			{
				qcgires_error(req, "Query not found");
			}
			if (!strcmp(name, ""))
			{
				qcgires_error(req, "Empty cookie name can not be removed.");
			}

			qcgires_removecookie(req, name, NULL, NULL, false);
			qcgires_setcontenttype(req, "text/html");
			printf("Cookie('%s') entry is removed.<br>\n", name);
			printf("Click <a href='cookie.cgi'>here</a> to view your cookies\n");
		}
		else
		{
			qcgires_error(req, "Unknown mode.");
		}

		req->free(req);
	}

	return(0);

}
