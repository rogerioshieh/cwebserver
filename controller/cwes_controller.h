#ifndef __C_WES_CONTROLLER_H__
#define __C_WES_CONTROLLER_H__

#include "../modules/unix/x86_64-linux-gnu/unix_x86_64_linux_file.h"
#include "../protocol/http/cwes_http.h"

#define __MSG__ "What do you want?"

#define __STATIC_ROOT__ "./statics"

char* controller_control_everything(const char* req_msg)
{
    /*
     *  todo @controller_control_everything check and make decision for all http request
     */
    if (http_req_validate(req_msg) != 1) return __MSG__;

    HTTP_REQ req = http_req_parse_msg(req_msg);
    char* path = (char*)malloc(__SIZE_LARGE__);

    /* For static file */
    sprintf(path, "%s%s", __STATIC_ROOT__, req.uri);
    if (unix_x86_64_linux_get_stat_type(path) == 1)
        return http_res_compose(http_res_build_up(__HTTP_OK__, file_read_text(path)));

    return http_res_compose(
            http_res_build_up(
                    __HTTP_NOT_FOUND__,
                    "not found"
                    ));
}


#endif // __C_WES_CONTROLLER_H__