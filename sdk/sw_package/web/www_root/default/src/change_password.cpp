#include <stdio.h>
#include <string.h>

#include "cgi.h"

int main()
{
	PUT_CONTENT_TYPE_XML;
	PUT_CRLF;

	printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
			<?xml-stylesheet type=\"text/xsl\" href=\"/html/change_password.xsl\"?>\n");

	printf("<new_password_body lang=\"%s\">\n", SUPPORT_LANG);

	printf("<new_password_table>\n");
	printf("<max_user>%d</max_user>\n", MAX_ACCOUNT);

	printf("<model_name>%s</model_name>\n", MODEL_NAME);

	printf("</new_password_table>\n");
	printf("</new_password_body>\n");

	return 0;
}
