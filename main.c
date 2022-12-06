#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>

#define MAX_LENGTH 30
#define BUFFER_SIZE 5000
#define METER_SIZE 100

#define REFRESH_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9SZWZyZXNoIiwidG9rZW5pZCI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBwQXBpIl0sInZlcnNpb24iOiIyIiwiaWRlbnRpdHlUb2tlbiI6IitkM05PeUUwMFE5YXowZ0sxd3VWVlRiTFJBaWF2eTh0eG1mSFhmWjYzN3d5L3RGVGpOTzFHTWtiNFB5dXZDNUVIQU4rWDgzbHh6TTdCK2R2ajlLVEYrVjA3eGc1U1NXcy9NdnVXQlRncjdwa2dKWjZZbFZyeU5aTjV4UTI2TFZsb0s2a2ppUUtWMlZmcTIzK1ZKaGQyYmJCdUo3b04wMmkzdzkwbVJ3TlhQcjRyc3d5U3RQZEo0TTVyajdoYmZRNDNvMngxY3hLeHRrSDNYWUs4TmxJSzR2MlYyekMzV2xwQ0xCMktKRzl1VDdCK1RKQzErODB1UzVjV1RlV0NrU3JzNTZMem1PVERaZnhKNzhGdVZwM2xvYW91bkFjWVNhcWswb2hPV05yeWhtOTRWSlcybDNFOWJ0RzBZckNVTkk3czl1azQ0cXIwUSt2QTFQcEdzRzVjaThPOS9YSGFTcVpzaWY5OXVFMHR4ZXVpZk8yb1ZBa0FmcHJXVUVkSVRDWHduMUtvTUxwMHVoQXU4TnN5Y3hWOGJHYnhxQmJIaS9NektNczJ2c2pUTW1xL2tPeG9VTkhKaUM1RklIUFhFc2VjckZMWWVPOUFVSnlRU01hZVIwTmtCRnVSKzdEaTE5SXdHME10YVVEdUU0TmE0aVR6Z3pSaVloUU51MFhyby91cGFua0lEV0VsUEhnWFRtWWZ1VHJsM0FFcHZIYmptQitVTkNXdi9CS2gzTTRrQ25nd1AvZlpNQmpubUVGdUwvU1d0ODM5V0NEVllZNlByWE9zZGk1bUs2TXRjZnlSUmtQU25oaTk1S3creVgrWWd1RjVManFMQ05tVHNqR2trQ2lrUFpQUVV3WXRGQkhFNjZCWHNFMDg0K0VWdXBMUnc5YUNiN0F2YVFkL3dZbnRFZzhocytXVmVHOGlZK1pzSFpaVFZvdU5OeEZYZjgrUHFrYnlKQjVXbGpwR1NFL3I1cDYrdzdWZkxObmNJMUx4N0VMYWpsb1pyL1gvYi83R3BPWWYvZlZTR2xXUW1CUlVRbmYrWUpnRG5RK0pvaFZJNzhDTk0xOVQxSFhJNnhvWlBlVTczTDBGMk1zeGFGYXMxRTNWVEtYZ1NwMERzbE80bUJZMHdiK0ZNeEFwSFFrUjhCVm1IQ1MrSnFzZ0YxNktJVmFZNDNva1NBZUZpaTdicnZNeGVlL1V4aCtlbTNzQzhabmozS2lRRzBFSTBsY3FJK0t0RkdkRXV1SDlOcEUwMzBuQXZUMTUyWDVYSGxIUEJoNlZEMFdvVmc3d2VyZzIySmt1QmdOOVF1MkVBVkJNeGE4aTE1WkFDc2l1UEtOY3U5ekU0UzlibDRuVmYzc3BWZHdlYmhpQVZweUJKdWxjMXJzR1lIMTB3ZTU1QWFJemN1MnVoU0kvcjJFbWV0NDJqZEk0MTAvVkxjbWNPK2dlNzFZdXk1R2lIUUF5Z0dWVkFRUHpEdFcyQnNLYThJSWhPVGdNTWRWNjl0MVZHbWZjR252TWNSMGdUb1JESnVra1B0b25iaUhTMWk5N0JmRU1zcTdlSGIzeklGQmg5QmwzMDZiL1NsWE41cE51Uk1sZ3duRDNaa2VBUzBJc1VwQkxqdUJJdkQ0Vm9sSEUzdlh0MVVSSnF0REtOV2pKWlhDQXNsSUtzTnJCZkFQSFZac2hTcGVPL0Qxa2c1R1RYY1R3QWlHNUhnNjNYeUR2S1Q2ajZ1ZzM5VGEwMy9MSVRpSEVrdHlxUkpoZmM1RmR3aHZhK1N3cDdzME0ya242L0MvWXdRa3Vub1grN1dPcnhTeTB5MFdZRHlGelNuZTlDTnpkbEZkMU5EVFB3Z1FNQ2NkUlIyZ1h1dk50OXBtZDljSlFCOXBhVUQyYy9sT25helZoNU5oNmZJZWdxYy9DKzFudGl1Um1EUGk4eTZpaEcxcjU1UDRVN1ZadXFnSUZ1RnNObjNieExyblc1cFVVM0ZPSnc2ZDY1U0FiVVpNdTRsZVBoMXVaMnFkVDV4MXhVNUVjTDJVRzFLRGtPZzRSOGFXNkxSc1hrNzZ0VVNLTzlrYk1Nc2ZLeklSNHRpd2s4azI4bU51YlQ3UTB4ZFd5bjZLOGZXY3d0Q1RRMzkyWnhGZ3dsVWdCUTg1SlRJMjdSd0drSHhDYTdZUGFkbHB2bXp6M2VBNVZGRzF3WERSQ2xEYWRtb0ZQWU5YaHNxNVlVcVFCMzU2RTFyUFB3c2JWRGQrQlhCV24vTy9ONnVPa1JNa0J1b2srb0hqenBWZml1U0s3d0YwWWN3WVF4OGxva3p2Zm1TNGZ6d0UvdDBXY3k3czh5Y2ZUakJCcTdPVjQzaWo5VWg4KzV1empyaG9CbVl5K2pxckhJZ2FGYlU5aFNpUjRBK2dWclpWMEluVncwL1MvR1FqOGk3R3d5K1hNQlFtSGNOMTZtWUJzUzRvTGcwUEVWdDFTUmNlWDVpTG5qSG9IemlkOGJ1WWtta1dvRllWbnN0RjREZEtPNjdWdk14T1dLZGRmYjMrNERqanRPVUF1M1BhVEY5ejdTTm1TWUZzSERENiIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWVpZGVudGlmaWVyIjoiUElEOjkyMDgtMjAwMi0yLTg4NzkyNTE5MDcwMSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL2dpdmVubmFtZSI6IkFuZHJlYXMgSmFjayBDaHJpc3RpYW5zZW4iLCJsb2dpblR5cGUiOiJLZXlDYXJkIiwicGlkIjoiOTIwOC0yMDAyLTItODg3OTI1MTkwNzAxIiwiYjNmIjoiS2JXaWZZdjJrNDJTcytka3F1bktaNXhsVkFQQjNPa0NtVHNzdzFXVkNlYz0iLCJ1c2VySWQiOiIyNDEwODMiLCJleHAiOjE3MDAxMzkwNzAsImlzcyI6IkVuZXJnaW5ldCIsImp0aSI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsInRva2VuTmFtZSI6InAxIiwiYXVkIjoiRW5lcmdpbmV0In0.5PR8JuD5ZCMUmrYwrnpaXzZvzLZlOXh0OfXtZ7wuK-o"

struct MemoryStruct
{
    char *memory;
    size_t size;
};

// prototypes
void get_api_token(char answer, char *access_token_string);
void get_metering_point(char answer, char *access_token_string, char *meter_id);
void get_tarrifs(char answer, char *acces_token_string, char *meter_id);
void get_api_spot_prices();

int main()
{
    FILE source;
    char answer;
    char answer1;

    char access_token_string[BUFFER_SIZE];
    char meter_id[METER_SIZE];

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer);

    get_api_token(answer, access_token_string);
    get_metering_point(answer, access_token_string, meter_id);
    get_tarrifs(answer, access_token_string, meter_id);

    printf("Do you want spot prices? y/n: ");
    scanf(" %c", &answer1);

    if (answer1 == 'y')
    {
        get_api_spot_prices();
    }

    /* fill_device(&source, devices);
    print_devices(devices); */

    return 0;
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void get_api_token(char answer, char *access_token_string)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;

    json_object *parsed_json;
    json_object *access_token;
    json_object *new_json;

    const char *access_token_str;

    char buffer[BUFFER_SIZE];

    struct MemoryStruct chunk;

    FILE *access_token_file;

    chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;           /* no data at this point */

    if (answer == 'y')
    {

        headers = curl_slist_append(headers, "Authorization: Bearer " REFRESH_TOKEN);

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/token");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // this is saved into the WriteMemoryCallback function as a *ptr
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
            }
            else
            {

                parsed_json = json_tokener_parse(chunk.memory);
                json_object_object_get_ex(parsed_json, "result", &access_token);

                access_token_str = json_object_get_string(access_token);

                char buf[json_object_get_string_len(access_token)];

                strcpy(buf, access_token_str);

                char header_string[json_object_get_string_len(access_token) + 23];
                strcpy(header_string, "Authorization: Bearer ");

                strcat(header_string, buf);

                strcpy(access_token_string, header_string);

                new_json = json_object_new_object();
                json_object_object_add(new_json, "result", json_object_new_string(header_string));

                access_token_file = fopen("../accessToken.json", "w");
                fprintf(access_token_file, "%s", json_object_get_string(new_json));
                fclose(access_token_file);
            }

            curl_easy_cleanup(curl);
        }

        json_object_put(new_json);
        json_object_put(parsed_json);
        free(chunk.memory);
    }

    curl_global_cleanup();
}

void get_metering_point(char answer, char *access_token_string, char *meter_id)
{

    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;

    struct MemoryStruct chunk;

    FILE *access_token_file;

    chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;           /* no data at this point */

    if (answer == 'y')
    {
        headers = curl_slist_append(headers, access_token_string);
    }
    else
    {
        FILE *access_token_file;

        char buffer[BUFFER_SIZE];

        json_object *parsed_json;
        json_object *access_token;

        access_token_file = fopen("../accessToken.json", "r");
        fread(buffer, BUFFER_SIZE, 1, access_token_file);
        fclose(access_token_file);

        parsed_json = json_tokener_parse(buffer);
        json_object_object_get_ex(parsed_json, "result", &access_token);

        headers = curl_slist_append(headers, json_object_get_string(access_token));

        json_object_put(parsed_json);
    }

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoints?includeAll=false");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, save_meter_result);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); // this is saved into the WriteMemoryCallback function as a *ptr
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }
        else
        {
            FILE *file;

            json_object *parsed_json;
            json_object *meter_parent;
            json_object *meter_child;
            json_object *meter_point_id;

            parsed_json = json_tokener_parse(chunk.memory);
            json_object_object_get_ex(parsed_json, "result", &meter_parent);
            meter_child = json_object_array_get_idx(meter_parent, 0);
            json_object_object_get_ex(meter_child, "meteringPointId", &meter_point_id);

            json_object *new_meter_parent = json_object_new_object();
            json_object *new_meter_child = json_object_new_object();
            json_object *new_meter_child_array = json_object_new_array();

            json_object_array_add(new_meter_child_array, json_object_get(meter_point_id));
            json_object_object_add(new_meter_child, "meteringPoint", new_meter_child_array);
            json_object_object_add(new_meter_parent, "meteringPoints", new_meter_child);

            file = fopen("../meter.json", "w"); // w stands for write, it replaces the old data with the new
            fprintf(file, "%s", json_object_get_string(new_meter_parent));
            fclose(file);

            strcpy(meter_id, json_object_get_string(new_meter_parent));

            json_object_put(new_meter_parent);
            json_object_put(parsed_json);

            curl_easy_cleanup(curl);
        }
    }

    curl_global_cleanup();
}

void get_tarrifs(char answer, char *access_token_string, char *meter_id)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;

    curl = curl_easy_init();
    if (curl)
    {
        FILE *tarrif_file;
        tarrif_file = fopen("../tarrifs.json", "w");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoint/getcharges");

        json_object *parsed_json;
        json_object *access_token;
        json_object *parsed_meter;

        if (answer == 'y')
        {
            headers = curl_slist_append(headers, access_token_string);
            headers = curl_slist_append(headers, "Content-Type: application/json");

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, meter_id);
        }
        else
        {
            FILE *access_token_file;
            FILE *metering_point_file;

            char buffer[BUFFER_SIZE];
            char meter_buffer[METER_SIZE];

            access_token_file = fopen("../accessToken.json", "r");
            fread(buffer, BUFFER_SIZE, 1, access_token_file);
            fclose(access_token_file);

            parsed_json = json_tokener_parse(buffer);
            json_object_object_get_ex(parsed_json, "result", &access_token);

            headers = curl_slist_append(headers, json_object_get_string(access_token));
            headers = curl_slist_append(headers, "Content-Type: application/json");

            metering_point_file = fopen("../meter.json", "r");
            fread(meter_buffer, METER_SIZE, 1, metering_point_file);
            fclose(metering_point_file);

            parsed_meter = json_tokener_parse(meter_buffer);

            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_get_string(parsed_meter));
        }

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tarrif_file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }
        fclose(tarrif_file);

        if (answer == 'n')
        {
            json_object_put(parsed_json);
            json_object_put(parsed_meter);
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void get_api_spot_prices()
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;

    FILE *spot_prices_file;

    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *access_token;

    spot_prices_file = fopen("../spotPrices.json", "w"); // w stands for write, it replaces the old data with the new

    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.energidataservice.dk/dataset/Elspotprices?offset=11&limit=24&start=now-P1D&sort=HourDK&columns=HourDK,SpotPriceDKK&filter={%22PriceArea%22:[%22DK1%22]}");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, spot_prices_file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }

        fclose(spot_prices_file);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}