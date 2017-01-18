#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <microhttpd.h>

#include "iopcdefine.h"
#include "iopcops_misc.h"
#include "main.h"

static struct MHD_Daemon *service = NULL;

uint8_t* get_name_http()
{
    return "HTTP_TASK";
}

uint8_t* get_help_http()
{
    return "this is WWW server";
}

static ssize_t callback (void *cls, uint64_t pos, char *buf, size_t max)
{
    return MHD_CONTENT_READER_END_OF_STREAM;
}

static int httpd_handler(void *cls, struct MHD_Connection *con, const char *url, const char *method, const char *version, const char *upload_data, size_t *upload_data_size, void **ptr)
{
    static int aptr;
    struct MHD_Response *response;
    int ret;

    if (0 != strcmp (method, "GET")) {
        return MHD_NO;
    }

    if (&aptr != *ptr) {
        *ptr = &aptr;
        return MHD_YES;
    }

    *ptr = NULL;

    response = MHD_create_response_from_callback (MHD_SIZE_UNKNOWN, 1024, &callback, NULL, NULL);
    ret = MHD_queue_response (con, MHD_HTTP_OK, response);

    MHD_destroy_response (response);

    return ret;
}

void* task_handler_http(void* ptr)
{
    printf("Starting HTTP daemon [%s:%d]\n", HTTPD_IP, HTTPD_PORT);
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(HTTPD_PORT);
    inet_pton(AF_INET, HTTPD_IP, &(address.sin_addr.s_addr));
    service = MHD_start_daemon(HTTPD_FLAGS, 
		    htons(HTTPD_PORT),
		    NULL, NULL, 
		    &httpd_handler, NULL, 
		    MHD_OPTION_SOCK_ADDR, (struct sockaddr*) &address,
		    MHD_OPTION_END);
    while(1) {
        printf("start http task\n");
	sleep(10);
    }
}


struct task_desc_t task_descs[] = {
    {"get_name_http", "get_help_http", "task_handler_http"},
    {"", "", ""},
};
