// TestCurlSMTP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

#pragma comment(lib, "libcurl/lib/libcurl_debug.lib")
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
    * SMTP example using TLS
    * </DESC>
    */

    /* This is a simple example showing how to send mail using libcurl's SMTP
     * capabilities. It builds on the smtp-mail.c example to add authentication
     * and, more importantly, transport security to protect the authentication
     * details from being snooped.
     *
     * Note that this example requires libcurl 7.20.0 or above.
     */

//#define FROM_MAIL     "<idaoba.zeng@gmail.com>"
//#define TO_MAIL       "<15914356786@163.com>"
//#define CC_MAIL       "<zengzhaofa@dev.appeon.net>"
//#define FROM_MAIL     "<15914356786@163.com>"
//#define TO_MAIL       "<15914356786@163.com>"
//#define CC_MAIL       "<zengzhaofa@dev.appeon.net>"
#define FROM_MAIL     "<idaoba@outlook.com>"
#define TO_MAIL       "<idaoba.zeng@gmail.com>"
#define CC_MAIL       "<zengzhaofa@dev.appeon.net>"

static const char* payload_text =
"Date: Mon, 29 Nov 2222 21:54:29 +1100\r\n"
"To: " TO_MAIL "\r\n"
"From: " FROM_MAIL "\r\n"
"Message-ID: <dcd7cb36-11db-487a-9f3a-e652a9458efd@"
"dev.appeon.net>\r\n"
"Subject: TEST TLS SMTP example message\r\n"
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

int main(void)
{
    CURL* curl;
    CURLcode res = CURLE_OK;
    struct curl_slist* recipients = NULL;
    struct upload_status upload_ctx = { 0 };

    curl = curl_easy_init();
    if (curl) {
        /* Set username and password */
        res = curl_easy_setopt(curl, CURLOPT_USERNAME, "idaoba");
        res = curl_easy_setopt(curl, CURLOPT_PASSWORD, "123456qaz");

        res = curl_easy_setopt(curl, CURLOPT_URL, "smtp://smtp.mail.outlook.com:587"); //smtp://smtp.gmail.com:587

        res = curl_easy_setopt(curl, CURLOPT_USE_SSL, (long)CURLUSESSL_ALL);
  /*      curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 3000L);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3000L);*/

		//curl_easy_setopt(curl, CURLOPT_SSLCERT, "cacert.pem");
		//curl_easy_setopt(curl, CURLOPT_CAPATH, "C:\\Users\\appeon\\Desktop\\TestCurlSMTP\\TestCurlSMTP");

        // 将这里设置为FALSE才能认证通过
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        res = curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);


		//curl_easy_setopt(curl, CURLOPT_SSLCERT, "C:\\Users\\appeon\\Desktop\\TestCurlSMTP\\TestCurlSMTP\\mail.dev.appeon.net.pfx");
		//curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
		//curl_easy_setopt(curl, CURLOPT_SSLKEY, "key.pem");
		//curl_easy_setopt(curl, CURLOPT_KEYPASSWD, "appeon123");
        //curl_easy_setopt(curl, CURLOPT_CAINFO, "C:\\Users\\appeon\\Desktop\\TestCurlSMTP\\TestCurlSMTP\\mail.dev.appeon.net.pem");
        res = curl_easy_setopt(curl, CURLOPT_MAIL_FROM, FROM_MAIL);
        recipients = curl_slist_append(recipients, TO_MAIL);
        //recipients = curl_slist_append(recipients, CC_MAIL);
        res = curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        res = curl_easy_setopt(curl, CURLOPT_READFUNCTION, payload_source);
        res = curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
        res = curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

        res = curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        /* Send the message */
        res = curl_easy_perform(curl);

        /* Check for errors */
        if (res != CURLE_OK)
            fprintf(stderr, "curl_easy_perform() failed: %s\n",
                curl_easy_strerror(res));
        system("pause");
        /* Free the list of recipients */
        curl_slist_free_all(recipients);

        /* Always cleanup */
        curl_easy_cleanup(curl);

       
    }

    return (int)res;
}

// 建立连接对话流程
//*Trying 172.19.0.92:587...
//* Connected to mail.dev.appeon.net(172.19.0.92) port 587 (#0)
//< 220 mail.dev.appeon.net Microsoft ESMTP MAIL Service ready at Fri, 21 Jan 2022 13 : 49 : 15 + 0800
//> EHLO T133
//< 250 - mail.dev.appeon.net Hello[172.16.5.166]
//	< 250 - SIZE 37748736
//	< 250 - PIPELINING
//	< 250 - DSN
//	< 250 - ENHANCEDSTATUSCODES
//	< 250 - STARTTLS
//	< 250 - X - ANONYMOUSTLS
//	< 250 - AUTH GSSAPI NTLM LOGIN
//	< 250 - X - EXPS GSSAPI NTLM
//	< 250 - 8BITMIME
//	< 250 - BINARYMIME
//	< 250 - CHUNKING
//	< 250 - SMTPUTF8
//	< 250 XRDST
//	> STARTTLS
//	< 220 2.0.0 SMTP server ready
//	* SSL connection using TLSv1.2 / ECDHE - RSA - AES256 - GCM - SHA384
//	* Server certificate :
//*subject : CN = mail.dev.appeon.net
//* start date : Jul 17 01 : 49 : 37 2020 GMT
//* expire date : Jul 17 01 : 49 : 37 2025 GMT
//* issuer : CN = mail.dev.appeon.net
//* SSL certificate verify result : self - signed certificate(18), continuing anyway.
//	> EHLO T133
//	< 250 - mail.dev.appeon.net Hello[172.16.5.166]
//	< 250 - SIZE 37748736
//	< 250 - PIPELINING
//	< 250 - DSN
//	< 250 - ENHANCEDSTATUSCODES
//	< 250 - AUTH GSSAPI NTLM LOGIN
//	< 250 - X - EXPS GSSAPI NTLM
//	< 250 - 8BITMIME
//	< 250 - BINARYMIME
//	< 250 - CHUNKING
//	< 250 - SMTPUTF8
//	< 250 XRDST
//	> AUTH GSSAPI
//	< 334 GSSAPI supported
//	> YIIGNgYJKoZIhvcSAQICAQBuggYlMIIGIaADAgEFoQMCAQ6iBwMFAAAAAACjggSxYYIErTCCBKmgAwIBBaEQGw5ERVYuQVBQRU9OLk5FVKImMCSgAwIBAqEdMBsbBHNtdHAbE21haWwuZGV2LmFwcGVvbi5uZXSjggRmMIIEYqADAgESoQMCAROiggRUBIIEUMpe1D7iLzcXjF4XwROUWhT6ln8vmlFcTyXirvUbZrdOhrzXOQLRa / EQScRqnd0yRUkrMKsMYthex / aQQ6190oOjke3aOEdMPB8pk2DMQxhN7C6QcY2OCg2bTG0t2eA + sGS2aHne8UpGqkAVAYKhsPH8l0Gurlx1YT6thTOtzh3P09133UTkFQYwGMq / URRzRVuxDjcbkYJL9BD5pteG1HTF0UES6sZKspLlBnqhzalls + z5n7IEBdA4c9VQp0eliDAQLaM004wsR3Ym3KSV0 / 9Mza8k1FTjSwk6MajvCLKXQ75x4ZBdEUMEj7xRwQsCAXqR8h4OZcmaHZ6c1VqfPbdCEHReUCUp6V70CSPRqcPRy1IgdruxLzz1GEOXNvm7HNuNN7tjyjq59fLDhUAnyaNiuuXK3p7b4iKrlfEj9vLzvjotqJIMBbObcYmHib57P71c93 + WiE5qmY6fV1lc6rsURK84KSPaoCIwziVfoOgY17DRXR7TcrL + 6X9l6zC1tL7kFl / i32B5acwemp5lBE5e + e6FSZxkCLSyAdK3t45rPNhEGVFi1sYalF3MsGVO9VPU3boo2iYMI3he0 / X9CGFyR2hml03wfPcbuwoEsp7kFAoMfJ5euC / SxraCtbG5j1lQLZiUaK0eCnKVMTzED7H75kYDxVUrGx2ej1tNg6GSMCM3yt1WVbNkRYPKNUV9 / 4wGPQ4XecPYwDnBK + lYg + wkQRTgCB6Cb0ZmX8HSnKfA310oLas3 + GzoYZ5X43zFgcRBuUeW00BMHKPy + yZKK7tIc4Hzi1JYutnZmuPKVW4qKZtiTMhYlKO7ZIoIOdCy5T5qbpx + cP4K51twWEe14f + NQ7AysCQa0M + FwnaijFC9XepUtrVcEyD7ZTE7pxBFUrr2Ge4NbQOKPDzEVUqk09n8Zm5t6ieV3RB2TcgcbEnC0pt3NmCc++t2nrxNFHd6n0bzyFH5m0 + zKSjzxA6N4bzctRvJPLG3VSxshXQAnYPbFdw7JIMEjBnCKeEnQPPiYapFqBxLfYSDIhPcIKdUmSDMGrY0dDywxWsPV7RK3gBBNM427KwAnb5ITpc2U5TwD9KAf0ZJvENQJTSnaGw0L8eGk4nIF16lnj0t6xHmKT24Ebp27zaAq39IXNjxCUKF9H1nfiRyA6kMR0S3jzSB7872ZZtz4vDNwPuatQULtyAJlgoRhg4cbQmVZLEYquSYmb0zGe0 / z2XzS2zT93Q3ielG + NTK3++IkjCYRY0R6eL5lceyA8fto5NI7 / cRBQqC4HhR9Y4U1c45vpFK83s9FyNchICe9xgxJklr3kpenT7duirAuhvx42JgGDUmO8zLn7fW / ZMi86BTXzfQrDP1rxxWuD / OvHd7pqLlxT4oJJUPdXW2K / fiQ8VNjObS / d9FPA / J0 + RKnV6pD + Vc / GprsD7SZw6NZkm + kuVgLOblVl75Va / lUKWe6rHa41xbyF8 / 1KSCAVUwggFRoAMCARKiggFIBIIBRKKc9oy7QA7CU6xR79fiD6Fl8hEJ3f9 + bTmlsCQqBp3yaGMJF7v73 / siWMt4sNq97rzdsvQAT32eIfhi4Hp8DAlwseyVsXfLRSwTZS4mD6UNX78dG1Gp7E3PQsGjYLpme7fIL1fwA + d1 / rYAIU9lYI37qmCuAn8Cn + 1RtNJeVCkQxhEFhIbuL6lqkaY9fcOwmU4bzrxuePeNP3NQCj / OfJfW72qOedFubJf29LIVJMuIIFjKCdscuwSX3B + 5OuiE68f7JzlTpS3fvq1ImDBeBMuRtMdyRkSBuyA7 / geRik47uvvY3FD + pAh0QBMCTrXqEqRMw0TumX5mITgUlyGZbKI8dgpyYpvMACusG16PznAUTEZy0QB0SuoWsr5FTa89ybAbW4Vs6BDkm6VgPldLs1RDGxdulYpgRn6TeQWC6JoQGjW2kw ==
//	< 334 BQQB / wAMAAwAAAAABMAc19VuI9HWk2JMOKWMfgEAu4A =
//	> BQQA / wAMAAwAAAAABMAc13y5rSjX7O1lXA / XXAEAAAA =
//	< 235 2.7.0 Authentication successful
//> MAIL FROM : <zengzhaofa@dev.appeon.net>
//< 250 2.1.0 Sender OK
//> RCPT TO : <zengzhaofa@dev.appeon.net>
//< 250 2.1.5 Recipient OK
//> DATA
//< 354 Start mail input; end with <CRLF>.<CRLF>
//	< 250 2.6.0 < dcd7cb36 - 11db - 487a - 9f3a - e652a9458efd@dev.appeon.net > [InternalId = 56959856279580, Hostname = mail.dev.appeon.net] 1457 bytes in 0.105, 13.430 KB / sec Queued mail for delivery
//	* Connection #0 to host mail.dev.appeon.net left intact
//
//	C : \Users\appeon\Desktop\TestCurlSMTP\Debug\TestCurlSMTP.exe(process 20284) exited with code 0.
//	To automatically close the console when debugging stops, enable Tools->Options->Debugging->Automatically close the console when debugging stops.
//	Press any key to close this window . . .

