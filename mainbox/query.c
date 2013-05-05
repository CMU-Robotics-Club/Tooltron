#include "query.h"
#include "event.h"
#include "util.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>

/* Outputs the response to /add_card_event to debug.html */
//#define DEBUG_EVENT_RESPONSE

const char *server;
char *tooltron_password;

int query_init(const char *server_name) {
  CURLcode error_code;
  int len;

  server = server_name;

  error_code = curl_global_init(CURL_GLOBAL_SSL);
  if (error_code) {
    log_print("ERROR: curl_global_init: %s\n", curl_easy_strerror(error_code));
    return error_code;
  }

  tooltron_password = read_file("tooltron_password");
  if (!tooltron_password)
    return 1;
  len = strlen(tooltron_password);
  while (len > 0 && tooltron_password[len-1] == '\n')
    tooltron_password[--len] = '\0';

  return 0;
}

void query_cleanup() {
  curl_global_cleanup();
  if (tooltron_password)
    free(tooltron_password);
}

static size_t write_bool(void *buffer, size_t size, size_t nmemb, void *userp) {
  int *resultp = userp;
  char *str = buffer;

  if (size*nmemb > 0 && str[0] == '1')
    *resultp = 1;
  else
    *resultp = 0;

  return nmemb;
}

static size_t write_ignore(void *buffer, size_t size, size_t nmemb,
    void *userp) {
  return nmemb;
}

/*
 * query_user_permission
 *
 * Makes an HTTP request to the CRM server to see if user_id has access to
 * tool_id. Returns 1 if the server replies with '1' or 0 otherwise.
 */
int query_user_permission(int tool_id, unsigned int user_id) {
  CURL* handle;
  CURLcode error_code;
  char url[1024];
  int result = 0;
  long response = 0;

  handle = curl_easy_init();
  if (handle == NULL)
    return 0;

  sprintf(url, "http://%s/crm/roboauth/%08x/%d/", server, user_id, tool_id);
  error_code = curl_easy_setopt(handle, CURLOPT_URL, url);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_bool);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, &result);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  error_code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  if (error_code) goto error;
  if (response >= 400)
    log_print("ERROR: response %ld from %s", response, url);
  else if (response > 200)
    log_print("WARNING: response %ld from %s", response, url);

  curl_easy_cleanup(handle);
  return result;

error:
  log_print("ERROR: curl: %s", curl_easy_strerror(error_code));
  log_print("ERROR:       when authenticating user %08x on tool %d",
      user_id, tool_id);
  curl_easy_cleanup(handle);
  return 0;
}

/*
 * query_add_event
 *
 * Makes an HTTPS POST request to add an event to the CRM server, including
 * user, tool, start time, and stop time. Reads the password from password.txt.
 * Returns 0 if successful, or 1 if there was an error and the caller should
 * try the same event again later.
 *
 * Times are represented as strftime's "%F %T", which is like "YYYY-MM-DD
 * HH:MM:SS" with 24-hour time
 */
int query_add_event(struct event_t *event) {
  CURL* handle;
  CURLcode error_code;
  struct curl_httppost *formpost = NULL, *lastptr = NULL;
  char buf[1024];
  struct tm *timeinfo;
  long response = 0;

  return 0;

#ifdef DEBUG_EVENT_RESPONSE
  FILE *fdebug;
  fdebug = fopen("debug.html", "w");
#endif

  handle = curl_easy_init();
  if (handle == NULL)
    return 1;

  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "username",
      CURLFORM_COPYCONTENTS, "tooltron",
      CURLFORM_END);

  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "password",
      CURLFORM_COPYCONTENTS, tooltron_password,
      CURLFORM_END);

  timeinfo = localtime(&event->tstart);
  strftime(buf, sizeof(buf), "%F %T", timeinfo);
  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "tstart",
      CURLFORM_COPYCONTENTS, buf,
      CURLFORM_END);

  timeinfo = localtime(&event->tend);
  strftime(buf, sizeof(buf), "%F %T", timeinfo);
  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "tend",
      CURLFORM_COPYCONTENTS, buf,
      CURLFORM_END);

  sprintf(buf, "%08x", event->user);
  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "user_id",
      CURLFORM_COPYCONTENTS, buf,
      CURLFORM_END);

  sprintf(buf, "%d", event->tool_id);
  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "machine_id",
      CURLFORM_COPYCONTENTS, buf,
      CURLFORM_END);

  curl_formadd(&formpost, &lastptr,
      CURLFORM_COPYNAME, "succ",
      CURLFORM_COPYCONTENTS, event->succ? "1" : "0",
      CURLFORM_END);

  sprintf(buf, "https://%s/crm/add_card_event/", server);
  //sprintf(buf, "http://%s/crm/add_card_event/", server);
  error_code = curl_easy_setopt(handle, CURLOPT_URL, buf);
  if (error_code) goto error;

  /* TODO disabling host and peer verification should theoretically be removed
   * eventually */
  error_code = curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L);
  if (error_code) goto error;

#ifdef DEBUG_EVENT_RESPONSE
  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, fdebug);
#else
  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_ignore);
#endif
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_HTTPPOST, formpost);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  error_code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  if (error_code) goto error;
  if (response >= 400)
    log_print("ERROR: response %ld from %s", response, buf);
  else if (response > 200)
    log_print("WARNING: response %ld from %s", response, buf);

  curl_easy_cleanup(handle);
  curl_formfree(formpost);
#ifdef DEBUG_EVENT_RESPONSE
  fclose(fdebug);
#endif
  return response >= 300;

error:
  log_print("ERROR: curl: %s", curl_easy_strerror(error_code));
  curl_easy_cleanup(handle);
  curl_formfree(formpost);
#ifdef DEBUG_EVENT_RESPONSE
  fclose(fdebug);
#endif
  return 1;
}
