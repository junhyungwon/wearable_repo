#include <stdio.h>
#include <json-c/json.h>
 
int main(int argc, char **argv)
{
	/*
	 * { "header": { "dataSize": 1000, "dataType": "text", "macAddress": "00:00:00:00:00" }, 
	 *    "data": { "data1": 1, "data2": "TEST2", "comment": "This is a test!!", 
	 *            "nameInfo": { "name": "홍길동", "regiid": "1406163000001" }, 
	 *                    "phoneNumber": [ "010-0000-0000", "010-1111-1111", "010-2222-2222" ] } }
	 *                    */
	char *buff = "{ \"header\": { \"dataSize\": 1000, \"dataType\": \"text\", \"macAddress\": \"00:00:00:00:00\" }, \"data\": { \"data1\": 1, \"data2\": \"TEST2\", \"comment\": \"This is a test!!\", \"nameInfo\": { \"name\": \"홍길동\", \"regiid\": \"1406163000001\" }, \"phoneNumber\": [ \"010-0000-0000\", \"010-1111-1111\", \"010-2222-2222\" ] } }";
	json_object *myobj, *headerobj, *dataobj;
	json_object *dobj, *dval;
	int i;

	/* JSON type의 데이터를 읽는다. */
	myobj = json_tokener_parse(buff);

	headerobj = json_object_object_get(myobj, "header");
	dataobj   = json_object_object_get(myobj, "data");

	/* header 영역 파싱 */
	dval = json_object_object_get(headerobj, "dataType");
	printf("dataType : %s\n", json_object_get_string(dval));
	dval = json_object_object_get(headerobj, "macAddress");
	printf("macAddress : %s\n", json_object_get_string(dval));

	/* data 영역 파싱 */
	dval = json_object_object_get(dataobj, "data1");
	printf("data1 : %d\n", json_object_get_int(dval));

	/* data.nameInfo 영역 파싱( object안에 object ) */
	dobj = json_object_object_get(dataobj, "nameInfo");
	dval = json_object_object_get(dobj, "name");
	printf("nameInfo.name : %s\n", json_object_get_string(dval));
	dval = json_object_object_get(dobj, "regiid");
	printf("nameInfo.regiid : %s\n", json_object_get_string(dval));

	/* data.phoneNumber 파싱 ( array type ) */
	dobj = json_object_object_get(dataobj, "phoneNumber");
	printf("nameInfo.phoneNumber is\n");
	for (i = 0; i < json_object_array_length(dobj); i++)
	{
		dval = json_object_array_get_idx(dobj, i);
		printf("   %s\n", json_object_get_string(dval));
	}

	// print jsonc
	{
		json_object *myobj, *headerobj, *dataobj, *datainobj, *arrayobj;

		myobj = json_object_new_object();

		headerobj = json_object_new_object();
		dataobj   = json_object_new_object();
		datainobj = json_object_new_object();

		/* header { ..., } 영역 */
		json_object_object_add(headerobj, "dataSize", json_object_new_int(1000));
		json_object_object_add(headerobj, "dataType", json_object_new_string("text"));
		json_object_object_add(headerobj, "macAddress", json_object_new_string("00:00:00:00:00"));
		json_object_object_add(myobj, "header", headerobj);

		/* data { ..., nameInfo { ..., } phoneNumber [ ..., ] } 영역 */
		json_object_object_add(dataobj, "data1", json_object_new_int(1));
		json_object_object_add(dataobj, "data2", json_object_new_string("TEST2"));
		json_object_object_add(dataobj, "comment", json_object_new_string("This is a test!!"));

		json_object_object_add(datainobj, "name", json_object_new_string("홍길동"));
		json_object_object_add(datainobj, "regiid", json_object_new_string("1406163000001"));
		json_object_object_add(dataobj, "nameInfo", datainobj);

		// array type 
		arrayobj = json_object_new_array();
		json_object_array_add(arrayobj, json_object_new_string("010-0000-0000"));
		json_object_array_add(arrayobj, json_object_new_string("010-1111-1111"));
		json_object_array_add(arrayobj, json_object_new_string("010-2222-2222"));
		json_object_object_add(dataobj, "phoneNumber", arrayobj);

		json_object_object_add(myobj, "data", dataobj);

		printf("myobj.to_string()=%s\n", json_object_to_json_string(myobj));

		json_object_put(headerobj);
		json_object_put(dataobj);
		json_object_put(myobj);
	}

	return(0);

}
