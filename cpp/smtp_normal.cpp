/***************************************************************************
 *                                  _   _ ____  _
 *  Project                     ___| | | |  _ \| |
 *                             / __| | | | |_) | |
 *                            | (__| |_| |  _ <| |___
 *                             \___|\___/|_| \_\_____|
 *
 * Copyright (C) 1998 - 2021, Daniel Stenberg, <daniel@haxx.se>, et al.
 *
 * This software is licensed as described in the file COPYING, which
 * you should have received as part of this distribution. The terms
 * are also available at https://curl.se/docs/copyright.html.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the COPYING file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 ***************************************************************************/

 /* <DESC>
  * Send e-mail with SMTP
  * </DESC>
  */

#include <stdio.h>
#include <string.h>
#include "libcurl/include/curl/curl.h"

#define CURL_STATICLIB

#pragma comment(lib, "libcurl/lib/libcurl_debug.lib")


  /*
   * For an SMTP example using the multi interface please see smtp-multi.c.
   */

   /* The libcurl options want plain addresses, the viewable headers in the mail
    * can very well get a full name as well.
    */
#define FROM_ADDR    "<idaoba.zeng@gmail.com>"
#define TO_ADDR      "<idaoba.zeng@gmail.com>"
#define CC_ADDR      "<zengzhaofa@dev.appeon.net>"
//#define FROM_MAIL     "<15914356786@163.com>"
//#define TO_MAIL       "<15914356786@163.com>"
//#define CC_MAIL       "<zengzhaofa@dev.appeon.net>"
//#define FROM_ADDR    "<15914356786@163.com>"
//#define TO_ADDR      "<15914356786@163.com>"
//#define CC_ADDR      "<zengzhaofa@dev.appeon.net>"

#define FROM_MAIL "Sender Person " FROM_ADDR
#define TO_MAIL   "A Receiver " TO_ADDR
#define CC_MAIL   "John CC Smith " CC_ADDR

static const char* payload_text =
"Date: Mon, 29 Nov 2011 21:02:29 +1100\r\n"
"To: " TO_MAIL "\r\n"
"From: " FROM_MAIL "\r\n"
"Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
"dev.appeon.net>\r\n"
"Subject: SMTP example message\r\n"
"\r\n" /* empty line to divide headers from body, see RFC5322 */
"The body of the message starts here.\r\n"
"\r\n"
"It could be a lot of lines, could be MIME encoded, whatever.\r\n"
"Check RFC5322.\r\n";

struct upload_status {
    size_t bytes_read;
};

static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp)
{
    struct upload_status* upload_ctx = (struct upload_status*)userp;
    const char* data;
    size_t room = size * nmemb;

    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
        return 0;
    }

    data = &payload_text[upload_ctx->bytes_read];

    if (data) {
        size_t len = strlen(data);
        if (room < len)
            len = room;
        memcpy(ptr, data, len);
        upload_ctx->bytes_read += len;

        return len;
    }

    return 0;
}


size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp)
{
    FILE* fp;
    fopen_s(&fp,"C:\\Users\\appeon\\Desktop\\TestCurlSMTP\\TestCurlSMTP\\test.txt","w+");

    fwrite(buffer, strlen((char*)buffer) + 1, 1, fp);
    fclose(fp);

    return size;
}

int main_nal(void)
{
    CURL* curl;
    CURLcode res = CURLE_OK;
    struct curl_slist* recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver */
        res = curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
		res = curl_easy_setopt(curl, CURLOPT_USERNAME, "idaoba.zeng");
		res = curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456qaz..@");
        res = curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);
        recipients = curl_slist_append(recipients, TO_ADDR);
        //recipients = curl_slist_append(recipients, CC_ADDR);
        res = curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
        res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        res = curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        //res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        /* Free the list of recipients */
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
    while (1)
    {
        system("pause");
    }
   
    return (int)res;
}
