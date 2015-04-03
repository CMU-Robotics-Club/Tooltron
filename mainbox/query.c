#include "query.h"
#include "event.h"
#include "util.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <curl/curl.h>
#include <json.h>


/* Outputs the response to /add_card_event to debug.html */
//#define DEBUG_EVENT_RESPONSE

/* Size of buffer written to by write_buffer(). We are reading json
 * for /api/machines/ so this needs to be rather large.
 */
// TODO: malloc this based upon the number of machine entries
#define WRITE_BUFFER_SIZE 5000


static const char *server;
static char buffer[WRITE_BUFFER_SIZE+1]; // +1 for null byte
static int buffer_idx;

static char* public_key;
static char* private_key;

int query_init(const char *server_name) {
  CURLcode error_code;

  server = server_name;

  error_code = curl_global_init(CURL_GLOBAL_SSL);
  if (error_code) {
    log_print("ERROR: curl_global_init: %s\n", curl_easy_strerror(error_code));
    return error_code;
  }

  public_key = read_file("tooltron_public_key");
  if(!public_key) {
    log_print("No file tooltron_public_key");
    return -1;
  }

  private_key = read_file("tooltron_private_key");
  if(!public_key) {
    log_print("No file tooltron_private_key");
    return -1;
  }

  return 0;
}

void query_cleanup() {
  //TODO: query_tools_cleanup or free 'tools' from query_tools here

  if(public_key)
    free(public_key);

  if(private_key)
    free(private_key);

  curl_global_cleanup();
}

static size_t write_buffer(void *buf_in, size_t size, size_t nmemb, void *userp) {
  size_t to_read;

  to_read = nmemb;

  if (buffer_idx + to_read*size > WRITE_BUFFER_SIZE)
    to_read = (WRITE_BUFFER_SIZE - buffer_idx)/size;

  if (to_read > 0) {
    memcpy(buffer+buffer_idx, buf_in, to_read*size);
    buffer[buffer_idx + to_read*size] = '\0';
    buffer_idx += to_read*size;
  }

  return to_read;
}

static size_t write_ignore(void *buffer, size_t size, size_t nmemb,
    void *userp) {
  return nmemb;
}

int query_tools(struct tool_t*** tools) {
  log_print("Requesting machines from server");
  
  CURL* handle;
  CURLcode error_code;
  char url[1024];
  long response = 0;

  handle = curl_easy_init();
  if (handle == NULL)
    return 0;

  // only retrieve machines that have Tooltron boxes on them
  sprintf(url, "%s/api/machines/?toolbox_id__isnull=False", server);

  error_code = curl_easy_setopt(handle, CURLOPT_URL, url);
  if (error_code) goto error;

  buffer_idx = 0;
  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_buffer);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  error_code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  if (error_code) goto error;
  if (response >= 400)
    log_print("ERROR: response %ld from %s", response, url);
  else if (response > 200)
    log_print("WARNING: response %ld from %s", response, url);

  struct json_object *json_tools = json_tokener_parse((char*)buffer);

  int num_tools = json_object_array_length(json_tools);

  log_print("Read %d tools from server", num_tools);

  (*tools) = (struct tool_t**)malloc(num_tools * sizeof(struct tool_t*));

  if(!tools) {
    log_print("Out of memory mallocing tool array");
    return -1;
  }

  int i;
  for(i = 0; i < num_tools; i++) {
    json_object* json_tool = json_object_array_get_idx(json_tools, i);

    json_object* json_tool_id = json_object_object_get(json_tool, "id");
    json_object* json_tool_toolbox_id = json_object_object_get(json_tool, "toolbox_id");
    json_object* json_tool_name = json_object_object_get(json_tool, "type");

    const char* tool_id = json_object_get_string(json_tool_id);
    const char* tool_name = json_object_get_string(json_tool_name);
    int tool_toolbox_id = json_object_get_int(json_tool_toolbox_id);

    log_print("Tool ID: %s, Tool Name: %s, Toolbox ID: %d", tool_id, tool_name, tool_toolbox_id);

    struct tool_t* tool = malloc(sizeof(struct tool_t));

    if(!tool) {
      log_print("Out of memory mallocing tool");
      return -1;
    }

    tool->id = malloc(strlen(tool_id) + 1);

    if(!tool->id) {
      log_print("Out of memory mallocing tool id");
      return -1;
    }

    strcpy(tool->id, tool_id);


    tool->name = malloc(strlen(tool_name) + 1);

    if(!tool->name) {
      log_print("Out of memory mallocing tool name");
      return -1;
    }

    strcpy(tool->name, tool_name);

  
    tool->address = tool_toolbox_id;
    tool->connected = 1;
    tool->state = TS_INIT;
    tool->user = 0;
    tool->event = NULL;
    tool->powered = false;

    (*tools)[i] = tool;
  }

  curl_easy_cleanup(handle);
  return num_tools;

error:
  log_print("ERROR: curl: %s", curl_easy_strerror(error_code));
  log_print("ERROR:       when requesting machines from server");
  curl_easy_cleanup(handle);
  return -1;

}

/*
 * query_user_permission
 *
 * Checks whether user_id has permission for tool_id.
 * Makes an HTTP request to the CRM server to see if user_id
 * has access to the tool. Returns 0 if there was a problem. 
 */
int query_user_permission(int tool_id, unsigned int user_id) {
  log_print("Requesting permissions for %08x from server", user_id);
  
  CURL* handle;
  CURLcode error_code;
  char url[1024];
  int result = 0;
  long response = 0;

  handle = curl_easy_init();
  if (handle == NULL)
    return 0;

  sprintf(url, "%s/crm/roboauth/%08x/%d/", server, user_id, tool_id);
  error_code = curl_easy_setopt(handle, CURLOPT_URL, url);
  if (error_code) goto error;

  buffer_idx = 0;
  error_code = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, write_buffer);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
  if (error_code) goto error;

  error_code = curl_easy_perform(handle);
  if (error_code) goto error;

  error_code = curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &response);
  if (error_code) goto error;
  if (response >= 400)
    log_print("ERROR: response %ld from %s", response, url);
  else if (response > 200)
    log_print("WARNING: response %ld from %s", response, url);

  result = atoi(buffer);

  curl_easy_cleanup(handle);
  return result;

error:
  log_print("ERROR: curl: %s", curl_easy_strerror(error_code));
  log_print("ERROR:       when authenticating user %08x", user_id);
  curl_easy_cleanup(handle);
  return -1;
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

#ifdef DEBUG_EVENT_RESPONSE
  FILE *fdebug;
  fdebug = fopen("debug.html", "w");
#endif

  handle = curl_easy_init();
  if (handle == NULL)
    return 1;

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

  sprintf(buf, "%s/crm/add_card_event/", server);
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
  // return error if it's not a 200-level response
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

int query_tool_set_powered(const char* tool_id, bool powered) {
  CURL* handle;
  CURLcode error_code;
  char buf[1024];
  long response = 0;
  struct curl_slist *headers = NULL;

#ifdef DEBUG_EVENT_RESPONSE
  FILE *fdebug;
  fdebug = fopen("debug.html", "w");
#endif

  handle = curl_easy_init();
  if (handle == NULL)
    return 1;

  headers = curl_slist_append(headers, "Accept: application/json");
  headers = curl_slist_append(headers, "Content-Type: application/json");

  sprintf(buf, "PUBLIC_KEY: %s", public_key);
  headers = curl_slist_append(headers, buf);

  sprintf(buf, "PRIVATE_KEY: %s", private_key);
  headers = curl_slist_append(headers, buf);

  // For new ModWSGI version.  Delete previous
  // authentication headers when Roboclub8 fully migrated
  sprintf(buf, "PUBLIC-KEY: %s", public_key);
  headers = curl_slist_append(headers, buf);

  sprintf(buf, "PRIVATE-KEY: %s", private_key);
  headers = curl_slist_append(headers, buf);

  json_object* json = json_object_new_object();
  json_object_object_add(json, "powered", json_object_new_boolean(powered));

  sprintf(buf, "%s/api/machines/%s/", server, tool_id);
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

  error_code = curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "PUT");
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_HTTPHEADER, headers);
  if (error_code) goto error;

  error_code = curl_easy_setopt(handle, CURLOPT_POSTFIELDS, json_object_to_json_string(json));
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
  curl_slist_free_all(headers);
#ifdef DEBUG_EVENT_RESPONSE
  fclose(fdebug);
#endif

  // Free json object
  json_object_put(json);

  // return error if it's not a 200-level response
  return response >= 300;

error:
  log_print("ERROR: curl: %s", curl_easy_strerror(error_code));
  curl_easy_cleanup(handle);
  curl_slist_free_all(headers);
#ifdef DEBUG_EVENT_RESPONSE
  fclose(fdebug);
#endif

  // Free json object
  json_object_put(json);

  return 1;
}

