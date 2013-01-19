#include "query.h"
#include <stdio.h>
#include <curl/curl.h>

int query_init() {
  CURLcode error_code;

  error_code = curl_global_init(CURL_GLOBAL_NOTHING);
  if (error_code)
    fprintf(stderr, "curl_global_init: %s\n", curl_easy_strerror(error_code));

  return error_code;
}

void query_cleanup() {
  curl_global_cleanup();
}

static size_t write_data(void *buffer, size_t size, size_t nmemb, void *userp) {
  int *resultp = userp;

  return fwrite(buffer, size, nmemb, stdout);
}

int query_user_permission(int tool_id, int user_id) {
  CURL* handle;
  CURLcode error_code;
  char url[1024];
  int result = 0;

  handle = curl_easy_init();
  if (handle == NULL)
    return 0;

  sprintf(url, "http://roboclub.org/tooltron/%08x/%d/", user_id, tool_id);
  /* TODO temporary */ sprintf(url, "http://www.google.com");
  error_code = curl_easy_setopt(handle, CURLOPT_URL, url);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  curl_easy_cleanup(handle);
  return result;

error:
  fprintf(stderr, "curl: %s\n", curl_easy_strerror(error_code));
  fprintf(stderr, "     when authenticating user %08x on tool %d\n",
      user_id, tool_id);
  curl_easy_cleanup(handle);
  return 0;
}
