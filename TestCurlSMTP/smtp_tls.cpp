//// TestCurlSMTP.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//
///***************************************************************************
// *                                  _   _ ____  _
// *  Project                     ___| | | |  _ \| |
// *                             / __| | | | |_) | |
// *                            | (__| |_| |  _ <| |___
// *                             \___|\___/|_| \_\_____|
// *
// * Copyright (C) 1998 - 2021, Daniel Stenberg, <daniel@haxx.se>, et al.
// *
// * This software is licensed as described in the file COPYING, which
// * you should have received as part of this distribution. The terms
// * are also available at https://curl.se/docs/copyright.html.
// *
// * You may opt to use, copy, modify, merge, publish, distribute and/or sell
// * copies of the Software, and permit persons to whom the Software is
// * furnished to do so, under the terms of the COPYING file.
// *
// * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
// * KIND, either express or implied.
// *
// ***************************************************************************/
//
// /* <DESC>
//  * Send e-mail with SMTP
//  * </DESC>
//  */
//
//#include <stdio.h>
//#include <string.h>
//#include "libcurl/include/curl/curl.h"
//
//#pragma comment(lib, "libcurl/lib/libcurl_debug.lib")
//  /***************************************************************************
//   *                                  _   _ ____  _
//   *  Project                     ___| | | |  _ \| |
//   *                             / __| | | | |_) | |
//   *                            | (__| |_| |  _ <| |___
//   *                             \___|\___/|_| \_\_____|
//   *
//   * Copyright (C) 1998 - 2021, Daniel Stenberg, <daniel@haxx.se>, et al.
//   *
//   * This software is licensed as described in the file COPYING, which
//   * you should have received as part of this distribution. The terms
//   * are also available at https://curl.se/docs/copyright.html.
//   *
//   * You may opt to use, copy, modify, merge, publish, distribute and/or sell
//   * copies of the Software, and permit persons to whom the Software is
//   * furnished to do so, under the terms of the COPYING file.
//   *
//   * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
//   * KIND, either express or implied.
//   *
//   ***************************************************************************/
//
//   /* <DESC>
//    * SMTP example using TLS
//    * </DESC>
//    */
//
//    /* This is a simple example showing how to send mail using libcurl's SMTP
//     * capabilities. It builds on the smtp-mail.c example to add authentication
//     * and, more importantly, transport security to protect the authentication
//     * details from being snooped.
//     *
//     * Note that this example requires libcurl 7.20.0 or above.
//     */
//
//#define FROM_MAIL     "<laihao@dev.appeon.net>"
//#define TO_MAIL       "<laihao@dev.appeon.net>"
//#define CC_MAIL       "<info@example.com>"
//
//static const char* payload_text =
//"Date: Mon, 29 Nov 2010 21:54:29 +1100\r\n"
//"To: " TO_MAIL "\r\n"
//"From: " FROM_MAIL "\r\n"
//"Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
//"dev.appeon.net>\r\n"
//"Subject: SMTP example message\r\n"
//"\r\n" /* empty line to divide headers from body, see RFC5322 */
//"The body of the message starts here.\r\n"
//"\r\n"
//"It could be a lot of lines, could be MIME encoded, whatever.\r\n"
//"Check RFC5322.\r\n";
//
//struct upload_status {
//    size_t bytes_read;
//};
//
//static size_t payload_source(char* ptr, size_t size, size_t nmemb, void* userp)
//{
//    struct upload_status* upload_ctx = (struct upload_status*)userp;
//    const char* data;
//    size_t room = size * nmemb;
//
//    if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
//        return 0;
//    }
//
//    data = &payload_text[upload_ctx->bytes_read];
//
//    if (data) {
//        size_t len = strlen(data);
//        if (room < len)
//            len = room;
//        memcpy(ptr, data, len);
//        upload_ctx->bytes_read += len;
//
//        return len;
//    }
//
//    return 0;
//}
//
//int main_tls(void)
//{
//    CURL* curl;
//    CURLcode res = CURLE_OK;
//    struct curl_slist* recipients = NULL;
//    struct upload_status upload_ctx = { 0 };
//
//    curl = curl_easy_init();
//    if (curl) {
//        /* Set username and password */
//        curl_easy_setopt(curl, CURLOPT_USERNAME, "laihao");
//        curl_easy_setopt(curl, CURLOPT_PASSWORD, "");
//
//        /* This is the URL for your mailserver. Note the use of port 587 here,
//         * instead of the normal SMTP port (25). Port 587 is commonly used for
//         * secure mail submission (see RFC4403), but you should use whatever
//         * matches your server configuration. */
//        curl_easy_setopt(curl, CURLOPT_URL, "smtp://mail.dev.appeon.net:587");
//
//        /* In this example, we will start with a plain text connection, and upgrade
//         * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
//         * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
//         * will continue anyway - see the security discussion in the libcurl
//         * tutorial for more details. */
//        curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
//
//        /* If your server does not have a valid certificate, then you can disable
//         * part of the Transport Layer Security protection by setting the
//         * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
//         *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//         *   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//         * That is, in general, a bad idea. It is still better than sending your
//         * authentication details in plain text though.  Instead, you should get
//         * the issuer certificate (or the host certificate if the certificate is
//         * self-signed) and add it to the set of certificates that are known to
//         * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
//         * for more information. */
//        //curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\appeon\\source\\repos\\TestCurlSMTP\\TestCurlSMTP\\cacert.pem");
//        //curl_easy_setopt(curl, CURLOPT_CAPATH, "C:\\Users\\appeon\\source\\repos\\TestCurlSMTP\\TestCurlSMTP");
//
//		//curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
//		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
//
//
//		//curl_easy_setopt(curl, CURLOPT_SSLCERT, "C:\\Users\\appeon\\source\\repos\\TestCurlSMTP\\TestCurlSMTP\\mail.dev.appeon.net.pfx");
//		//curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "P12");
//		////curl_easy_setopt(curl, CURLOPT_SSLKEY, "key.pem");
//		//curl_easy_setopt(curl, CURLOPT_KEYPASSWD, "appeon123");
//  //      curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\appeon\\source\\repos\\TestCurlSMTP\\TestCurlSMTP\\mail.dev.appeon.net.pem");
//
//        /* Note that this option is not strictly required, omitting it will result
//         * in libcurl sending the MAIL FROM command with empty sender data. All
//         * autoresponses should have an empty reverse-path, and should be directed
//         * to the address in the reverse-path which triggered them. Otherwise,
//         * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
//         * details.
//         */
//        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);
//
//        /* Add two recipients, in this particular case they correspond to the
//         * To: and Cc: addressees in the header, but they could be any kind of
//         * recipient. */
//        recipients = curl_slist_append(recipients, TO_MAIL);
//        //recipients = curl_slist_append(recipients, CC_MAIL);
//        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
//
//        /* We are using a callback function to specify the payload (the headers and
//         * body of the message). You could just use the CURLOPT_READDATA option to
//         * specify a FILE pointer to read from. */
//        curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
//        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
//        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
//
//        /* Since the traffic will be encrypted, it is very useful to turn on debug
//         * information within libcurl to see what is happening during the transfer.
//         */
//        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
//
//        /* Send the message */
//        res = curl_easy_perform(curl);
//
//        /* Check for errors */
//        if (res != CURLE_OK)
//            fprintf(stderr, "curl_easy_perform() failed: %s\n",
//                curl_easy_strerror(res));
//
//        /* Free the list of recipients */
//        curl_slist_free_all(recipients);
//
//        /* Always cleanup */
//        curl_easy_cleanup(curl);
//    }
//
//    return (int)res;
//}
//
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
