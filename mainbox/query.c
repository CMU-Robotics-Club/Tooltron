#include "query.h"
#include <stdio.h>
#include <curl/curl.h>

const char *server;

int query_init(const char *server_name) {
  CURLcode error_code;

  server = server_name;

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
  char *str = buffer;

  if (size*nmemb > 0 && str[0] == '1')
    *resultp = 1;
  else
    *resultp = 0;

  return nmemb;
}

int query_user_permission(int tool_id, int user_id) {
  CURL* handle;
  CURLcode error_code;
  char url[1024];
  int result = 0;
  long response = 0;

  handle = curl_easy_init();
  if (handle == NULL)
    return 0;

  sprintf(url, "http://%s/roboauth/%08x/%d/", server, user_id, tool_id);
  error_code = curl_easy_setopt(handle, CURLOPT_URL, url);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_data);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  error_code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  if (error_code) goto error;
  if (response >= 400)
    fprintf(stderr, "Error %ld from %s\n", response, url);
  else if (response > 200)
    fprintf(stderr, "Warning: response %ld from %s\n", response, url);

  curl_easy_cleanup(handle);
  return result;

error:
  fprintf(stderr, "curl: %s\n", curl_easy_strerror(error_code));
  fprintf(stderr, "      when authenticating user %08x on tool %d\n",
      user_id, tool_id);
  curl_easy_cleanup(handle);
  return 0;
}
