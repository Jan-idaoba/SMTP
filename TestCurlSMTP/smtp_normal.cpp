#define CURL_STATICLIB
#include <stdio.h>
#include <string.h>
#include "libcurl/include/curl/curl.h"

#pragma comment(lib, "libcurl/lib/libcurl_debug.lib")

#define FROM_ADDR    "<idaoba.zeng@gmail.com>"
#define TO_ADDR      "<idaoba@outlook.com>"
#define CC_ADDR      "<idaoba.zeng@gmail.com>"

#define FROM_MAIL "Sender Person " FROM_ADDR
#define TO_MAIL   "A Receiver " TO_ADDR
#define CC_MAIL   "John CC Smith " CC_ADDR

static const char* payload_text =
"Date: Mon, 29 Nov 2011 21:54:29 +1100\r\n"
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

int main_normal(void)
{
    CURL* curl;
    CURLcode res = CURLE_OK;
    struct curl_slist* recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    curl = curl_easy_init();
    if (curl) {
        /* This is the URL for your mailserver */
        res = curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.gmail.com:587");
        res = curl_easy_setopt(curl, CURLOPT_USERNAME, "idaoba.zeng@gmail.com");
        res = curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456qaz..@");

        res = curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_ADDR);
        recipients = curl_slist_append(recipients, TO_ADDR);
        recipients = curl_slist_append(recipients, CC_ADDR);

        res = curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        res = curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        /* Send the message */
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));

        system("pause");
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }

    return (int)res;
}
