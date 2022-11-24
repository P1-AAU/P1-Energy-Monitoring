#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>

#define MAX_LENGTH 30

#define REFRESH_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9SZWZyZXNoIiwidG9rZW5pZCI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBwQXBpIl0sInZlcnNpb24iOiIyIiwiaWRlbnRpdHlUb2tlbiI6IitkM05PeUUwMFE5YXowZ0sxd3VWVlRiTFJBaWF2eTh0eG1mSFhmWjYzN3d5L3RGVGpOTzFHTWtiNFB5dXZDNUVIQU4rWDgzbHh6TTdCK2R2ajlLVEYrVjA3eGc1U1NXcy9NdnVXQlRncjdwa2dKWjZZbFZyeU5aTjV4UTI2TFZsb0s2a2ppUUtWMlZmcTIzK1ZKaGQyYmJCdUo3b04wMmkzdzkwbVJ3TlhQcjRyc3d5U3RQZEo0TTVyajdoYmZRNDNvMngxY3hLeHRrSDNYWUs4TmxJSzR2MlYyekMzV2xwQ0xCMktKRzl1VDdCK1RKQzErODB1UzVjV1RlV0NrU3JzNTZMem1PVERaZnhKNzhGdVZwM2xvYW91bkFjWVNhcWswb2hPV05yeWhtOTRWSlcybDNFOWJ0RzBZckNVTkk3czl1azQ0cXIwUSt2QTFQcEdzRzVjaThPOS9YSGFTcVpzaWY5OXVFMHR4ZXVpZk8yb1ZBa0FmcHJXVUVkSVRDWHduMUtvTUxwMHVoQXU4TnN5Y3hWOGJHYnhxQmJIaS9NektNczJ2c2pUTW1xL2tPeG9VTkhKaUM1RklIUFhFc2VjckZMWWVPOUFVSnlRU01hZVIwTmtCRnVSKzdEaTE5SXdHME10YVVEdUU0TmE0aVR6Z3pSaVloUU51MFhyby91cGFua0lEV0VsUEhnWFRtWWZ1VHJsM0FFcHZIYmptQitVTkNXdi9CS2gzTTRrQ25nd1AvZlpNQmpubUVGdUwvU1d0ODM5V0NEVllZNlByWE9zZGk1bUs2TXRjZnlSUmtQU25oaTk1S3creVgrWWd1RjVManFMQ05tVHNqR2trQ2lrUFpQUVV3WXRGQkhFNjZCWHNFMDg0K0VWdXBMUnc5YUNiN0F2YVFkL3dZbnRFZzhocytXVmVHOGlZK1pzSFpaVFZvdU5OeEZYZjgrUHFrYnlKQjVXbGpwR1NFL3I1cDYrdzdWZkxObmNJMUx4N0VMYWpsb1pyL1gvYi83R3BPWWYvZlZTR2xXUW1CUlVRbmYrWUpnRG5RK0pvaFZJNzhDTk0xOVQxSFhJNnhvWlBlVTczTDBGMk1zeGFGYXMxRTNWVEtYZ1NwMERzbE80bUJZMHdiK0ZNeEFwSFFrUjhCVm1IQ1MrSnFzZ0YxNktJVmFZNDNva1NBZUZpaTdicnZNeGVlL1V4aCtlbTNzQzhabmozS2lRRzBFSTBsY3FJK0t0RkdkRXV1SDlOcEUwMzBuQXZUMTUyWDVYSGxIUEJoNlZEMFdvVmc3d2VyZzIySmt1QmdOOVF1MkVBVkJNeGE4aTE1WkFDc2l1UEtOY3U5ekU0UzlibDRuVmYzc3BWZHdlYmhpQVZweUJKdWxjMXJzR1lIMTB3ZTU1QWFJemN1MnVoU0kvcjJFbWV0NDJqZEk0MTAvVkxjbWNPK2dlNzFZdXk1R2lIUUF5Z0dWVkFRUHpEdFcyQnNLYThJSWhPVGdNTWRWNjl0MVZHbWZjR252TWNSMGdUb1JESnVra1B0b25iaUhTMWk5N0JmRU1zcTdlSGIzeklGQmg5QmwzMDZiL1NsWE41cE51Uk1sZ3duRDNaa2VBUzBJc1VwQkxqdUJJdkQ0Vm9sSEUzdlh0MVVSSnF0REtOV2pKWlhDQXNsSUtzTnJCZkFQSFZac2hTcGVPL0Qxa2c1R1RYY1R3QWlHNUhnNjNYeUR2S1Q2ajZ1ZzM5VGEwMy9MSVRpSEVrdHlxUkpoZmM1RmR3aHZhK1N3cDdzME0ya242L0MvWXdRa3Vub1grN1dPcnhTeTB5MFdZRHlGelNuZTlDTnpkbEZkMU5EVFB3Z1FNQ2NkUlIyZ1h1dk50OXBtZDljSlFCOXBhVUQyYy9sT25helZoNU5oNmZJZWdxYy9DKzFudGl1Um1EUGk4eTZpaEcxcjU1UDRVN1ZadXFnSUZ1RnNObjNieExyblc1cFVVM0ZPSnc2ZDY1U0FiVVpNdTRsZVBoMXVaMnFkVDV4MXhVNUVjTDJVRzFLRGtPZzRSOGFXNkxSc1hrNzZ0VVNLTzlrYk1Nc2ZLeklSNHRpd2s4azI4bU51YlQ3UTB4ZFd5bjZLOGZXY3d0Q1RRMzkyWnhGZ3dsVWdCUTg1SlRJMjdSd0drSHhDYTdZUGFkbHB2bXp6M2VBNVZGRzF3WERSQ2xEYWRtb0ZQWU5YaHNxNVlVcVFCMzU2RTFyUFB3c2JWRGQrQlhCV24vTy9ONnVPa1JNa0J1b2srb0hqenBWZml1U0s3d0YwWWN3WVF4OGxva3p2Zm1TNGZ6d0UvdDBXY3k3czh5Y2ZUakJCcTdPVjQzaWo5VWg4KzV1empyaG9CbVl5K2pxckhJZ2FGYlU5aFNpUjRBK2dWclpWMEluVncwL1MvR1FqOGk3R3d5K1hNQlFtSGNOMTZtWUJzUzRvTGcwUEVWdDFTUmNlWDVpTG5qSG9IemlkOGJ1WWtta1dvRllWbnN0RjREZEtPNjdWdk14T1dLZGRmYjMrNERqanRPVUF1M1BhVEY5ejdTTm1TWUZzSERENiIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWVpZGVudGlmaWVyIjoiUElEOjkyMDgtMjAwMi0yLTg4NzkyNTE5MDcwMSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL2dpdmVubmFtZSI6IkFuZHJlYXMgSmFjayBDaHJpc3RpYW5zZW4iLCJsb2dpblR5cGUiOiJLZXlDYXJkIiwicGlkIjoiOTIwOC0yMDAyLTItODg3OTI1MTkwNzAxIiwiYjNmIjoiS2JXaWZZdjJrNDJTcytka3F1bktaNXhsVkFQQjNPa0NtVHNzdzFXVkNlYz0iLCJ1c2VySWQiOiIyNDEwODMiLCJleHAiOjE3MDAxMzkwNzAsImlzcyI6IkVuZXJnaW5ldCIsImp0aSI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsInRva2VuTmFtZSI6InAxIiwiYXVkIjoiRW5lcmdpbmV0In0.5PR8JuD5ZCMUmrYwrnpaXzZvzLZlOXh0OfXtZ7wuK-o"

typedef struct
{
    char name[MAX_LENGTH];
    double standby_watt;
    double running_watt;
    int amounts;

} device;

// prototypes
void fill_device(FILE *source, device *devices);
void print_devices(device *devices);
void get_api(char answer);

int main()
{
    FILE source;
    device *devices;
    char answer;

    devices = malloc(8 * sizeof(device));

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer);

    get_api(answer);

    /* fill_device(&source, devices);
    print_devices(devices); */

    free(devices);
    return 0;
}

void fill_device(FILE *source, device *devices)
{
    source = fopen("../device.txt", "r");

    if (source == NULL)
    {
        printf("Error the file does not exist");
        exit(1);
    }

    char line[100];

    for (int i = 0; i < 8; i++)
    {
        fgets(line, sizeof(line), source);

        sscanf(line, "%[A-Za-z_^], %lf, %lf",
               devices[i].name, &devices[i].standby_watt,
               &devices[i].running_watt);

        printf("Insert amounts for %s > ", devices[i].name);
        scanf("%d", &devices[i].amounts);
    }

    fclose(source);
}

void print_devices(device *devices)
{
    for (int i = 0; i < 8; i++)
    {
        printf("%s, %.2lf, %.2lf, %d\n", devices[i].name, devices[i].standby_watt,
               devices[i].running_watt, devices[i].amounts);
    }
}

size_t save_api_result(char *ptr, size_t size, size_t nmemb, void *not_used)
{
    FILE *file;
    file = fopen("accessToken.json", "w"); // w stands for write, it replaces the old data with the new
    fprintf(file, "%s", ptr);
    fclose(file);

    // returns the size of bytes to check if any dataloss has occured
    return size * nmemb;
}

void get_api(char answer)
{
    CURL *curl;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);

    struct curl_slist *headers = NULL;

    FILE *access_token_file;
    FILE *metering_points;
    FILE *prices;

    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *access_token;

    if (answer == 'y')
    {
        access_token_file = fopen("accessToken.json", "w"); // w stands for write, it replaces the old data with the new

        headers = curl_slist_append(headers, "Authorization: Bearer " REFRESH_TOKEN);

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/token");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, access_token_file);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
            }

            fclose(access_token_file);
            curl_easy_cleanup(curl);
        }
    }
    else
    {
        access_token_file = fopen("accessToken.json", "r");
        fread(buffer, 5000, 1, access_token_file);
        fclose(access_token_file);

        parsed_json = json_object_new_string(buffer);
        json_object_object_get_ex(parsed_json, "result", &access_token);

        printf("%s", json_object_to_json_string_ext(access_token, JSON_C_TO_STRING_PRETTY));

        headers = curl_slist_append(headers, "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9EYXRhQWNjZXNzIiwidG9rZW5pZCI6IjdhMjIxYTFjLWYwMTYtNDc0NC1hYzE5LTk0M2Q5ZjFhM2E2ZCIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBpIiwiQ3VzdG9tZXJBcHBBcGkiXSwidmVyc2lvbiI6IjIiLCJpZGVudGl0eVRva2VuIjoiK2QzTk95RTAwUTlhejBnSzF3dVZWVGJMUkFpYXZ5OHR4bWZIWGZaNjM3d3kvdEZUak5PMUdNa2I0UHl1dkM1RUhBTitYODNseHpNN0IrZHZqOUtURitWMDd4ZzVTU1dzL012dVdCVGdyN3BrZ0paNllsVnJ5TlpONXhRMjZMVmxvSzZramlRS1YyVmZxMjMrVkpoZDJiYkJ1SjdvTjAyaTN3OTBtUndOWFByNHJzd3lTdFBkSjRNNXJqN2hiZlE0M28yeDFjeEt4dGtIM1hZSzhObElLNHYyVjJ6QzNXbHBDTEIyS0pHOXVUN0IrVEpDMSs4MHVTNWNXVGVXQ2tTcnM1Nkx6bU9URFpmeEo3OEZ1VnAzbG9hb3VuQWNZU2FxazBvaE9XTnJ5aG05NFZKVzJsM0U5YnRHMFlyQ1VOSTdzOXVrNDRxcjBRK3ZBMVBwR3NHNWNpOE85L1hIYVNxWnNpZjk5dUUwdHhldWlmTzJvVkFrQWZwcldVRWRJVENYd24xS29NTHAwdWhBdThOc3ljeFY4YkdieHFCYkhpL016S01zMnZzalRNbXEva094b1VOSEppQzVGSUhQWEVzZWNyRkxZZU85QVVKeVFTTWFlUjBOa0JGdVIrN0RpMTlJd0cwTXRhVUR1RTROYTRpVHpnelJpWWhRTnUwWHJvL3VwYW5rSURXRWxQSGdYVG1ZZnVUcmwzQUVwdkhiam1CK1VOQ1d2L0JLaDNNNGtDbmd3UC9mWk1Cam5tRUZ1TC9TV3Q4MzlXQ0RWWVk2UHJYT3NkaTVtSzZNdGNmeVJSa1BTbmhpOTVLdyt5WCtZZ3VGNUxqcUxDTm1Uc2pHa2tDaWtQWlBRVXdZdEZCSEU2NkJYc0UwODQrRVZ1cExSdzlhQ2I3QXZhUWQvd1ludEVnOGhzK1dWZUc4aVkrWnNIWlpUVm91Tk54RlhmOCtQcWtieUpCNVdsanBHU0UvcjVwNit3N1ZmTE5uY0kxTHg3RUxhamxvWnIvWC9iLzdHcE9ZZi9mVlNHbFdRbUJSVVFuZitZSmdEblErSm9oVkk3OENOTTE5VDFIWEk2eG9aUGVVNzNMMEYyTXN4YUZhczFFM1ZUS1hnU3AwRHNsTzRtQlkwd2IrRk14QXBIUWtSOEJWbUhDUytKcXNnRjE2S0lWYVk0M29rU0FlRmlpN2Jydk14ZWUvVXhoK2VtM3NDOFpuajNLaVFHMEVJMGxjcUkrS3RGR2RFdXVIOU5wRTAzMG5BdlQxNTJYNVhIbEhQQmg2VkQwV29WZzd3ZXJnMjJKa3VCZ045UXUyRUFWQk14YThpMTVaQUNzaXVQS05jdTl6RTRTOWJsNG5WZjNzcFZkd2ViaGlBVnB5Qkp1bGMxcnNHWUgxMHdlNTVBYUl6Y3UydWhTSS9yMkVtZXQ0MmpkSTQxMC9WTGNtY08rZ2U3MVl1eTVHaUhRQXlnR1ZWQVFQekR0VzJCc0thOElJaE9UZ01NZFY2OXQxVkdtZmNHbnZNY1IwZ1RvUkRKdWtrUHRvbmJpSFMxaTk3QmZFTXNxN2VIYjN6SUZCaDlCbDMwNmIvU2xYTjVwTnVSTWxnd25EM1prZUFTMElzVXBCTGp1Qkl2RDRWb2xIRTN2WHQxVVJKcXRES05XakpaWENBc2xJS3NOckJmQVBIVlpzaFNwZU8vRDFrZzVHVFhjVHdBaUc1SGc2M1h5RHZLVDZqNnVnMzlUYTAzL0xJVGlIRWt0eXFSSmhmYzVGZHdodmErU3dwN3MwTTJrbjYvQy9Zd1FrdW5vWCs3V09yeFN5MHkwV1lEeUZ6U25lOUNOemRsRmQxTkRUUHdnUU1DY2RSUjJnWHV2TnQ5cG1kOWNKUUI5cGFVRDJjL2xPbmF6Vmg1Tmg2ZkllZ3FjL0MrMW50aXVSbURQaTh5NmloRzFyNTVQNFU3Vlp1cWdJRnVGc05uM2J4THJuVzVwVVUzRk9KdzZkNjVTQWJVWk11NGxlUGgxdVoycWRUNXgxeFU1RWNMMlVHMUtEa09nNFI4YVc2TFJzWGs3NnRVU0tPOWtiTU1zZkt6SVI0dGl3azhrMjhtTnViVDdRMHhkV3luNks4Zldjd3RDVFEzOTJaeEZnd2xVZ0JRODVKVEkyN1J3R2tIeENhN1lQYWRscHZtenozZUE1VkZHMXdYRFJDbERhZG1vRlBZTlhoc3E1WVVxUUIzNTZFMXJQUHdzYlZEZCtCWEJXbi9PL042dU9rUk1rQnVvaytvSGp6cFZmaXVTSzd3RjBZY3dZUXg4bG9renZmbVM0Znp3RS90MFdjeTdzOHljZlRqQkJxN09WNDNpajlVaDgrNXV6anJob0JtWXkranFySElnYUZiVTloU2lSNEErZ1ZyWlYwSW5WdzAvUy9HUWo4aTdHd3krWE1CUW1IY04xNm1ZQnNTNG9MZzBQRVZ0MVNSY2VYNWlMbmpIb0h6aWQ4YnVZa21rV29GWVZuc3RGNERkS082N1Z2TXhPV0tkZGZiMys0RGpqdE9VQXUzUGFURjl6N1NObVNZRnNIREQ2IiwiaHR0cDovL3NjaGVtYXMueG1sc29hcC5vcmcvd3MvMjAwNS8wNS9pZGVudGl0eS9jbGFpbXMvbmFtZWlkZW50aWZpZXIiOiJQSUQ6OTIwOC0yMDAyLTItODg3OTI1MTkwNzAxIiwiaHR0cDovL3NjaGVtYXMueG1sc29hcC5vcmcvd3MvMjAwNS8wNS9pZGVudGl0eS9jbGFpbXMvZ2l2ZW5uYW1lIjoiQW5kcmVhcyBKYWNrIENocmlzdGlhbnNlbiIsImxvZ2luVHlwZSI6IktleUNhcmQiLCJwaWQiOiI5MjA4LTIwMDItMi04ODc5MjUxOTA3MDEiLCJiM2YiOiJLYldpZll2Mms0MlNzK2RrcXVuS1o1eGxWQVBCM09rQ21Uc3N3MVdWQ2VjPSIsInVzZXJJZCI6IjI0MTA4MyIsImV4cCI6MTY2OTM2NzEzNSwiaXNzIjoiRW5lcmdpbmV0IiwianRpIjoiN2EyMjFhMWMtZjAxNi00NzQ0LWFjMTktOTQzZDlmMWEzYTZkIiwidG9rZW5OYW1lIjoicDEiLCJhdWQiOiJFbmVyZ2luZXQifQ.VzQLHfJPvdzrMpdcb2CixXHYYrKBI4qvttfjShQnJOE");

        curl = curl_easy_init();
        if (curl)
        {
            metering_points = fopen("meter.json", "w");
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoints?includeAll=false");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, metering_points);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK)
            {
                printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
            }
            fclose(metering_points);
            curl_easy_cleanup(curl);
        }
    }

    headers = curl_slist_append(headers, "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9EYXRhQWNjZXNzIiwidG9rZW5pZCI6IjdhMjIxYTFjLWYwMTYtNDc0NC1hYzE5LTk0M2Q5ZjFhM2E2ZCIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBpIiwiQ3VzdG9tZXJBcHBBcGkiXSwidmVyc2lvbiI6IjIiLCJpZGVudGl0eVRva2VuIjoiK2QzTk95RTAwUTlhejBnSzF3dVZWVGJMUkFpYXZ5OHR4bWZIWGZaNjM3d3kvdEZUak5PMUdNa2I0UHl1dkM1RUhBTitYODNseHpNN0IrZHZqOUtURitWMDd4ZzVTU1dzL012dVdCVGdyN3BrZ0paNllsVnJ5TlpONXhRMjZMVmxvSzZramlRS1YyVmZxMjMrVkpoZDJiYkJ1SjdvTjAyaTN3OTBtUndOWFByNHJzd3lTdFBkSjRNNXJqN2hiZlE0M28yeDFjeEt4dGtIM1hZSzhObElLNHYyVjJ6QzNXbHBDTEIyS0pHOXVUN0IrVEpDMSs4MHVTNWNXVGVXQ2tTcnM1Nkx6bU9URFpmeEo3OEZ1VnAzbG9hb3VuQWNZU2FxazBvaE9XTnJ5aG05NFZKVzJsM0U5YnRHMFlyQ1VOSTdzOXVrNDRxcjBRK3ZBMVBwR3NHNWNpOE85L1hIYVNxWnNpZjk5dUUwdHhldWlmTzJvVkFrQWZwcldVRWRJVENYd24xS29NTHAwdWhBdThOc3ljeFY4YkdieHFCYkhpL016S01zMnZzalRNbXEva094b1VOSEppQzVGSUhQWEVzZWNyRkxZZU85QVVKeVFTTWFlUjBOa0JGdVIrN0RpMTlJd0cwTXRhVUR1RTROYTRpVHpnelJpWWhRTnUwWHJvL3VwYW5rSURXRWxQSGdYVG1ZZnVUcmwzQUVwdkhiam1CK1VOQ1d2L0JLaDNNNGtDbmd3UC9mWk1Cam5tRUZ1TC9TV3Q4MzlXQ0RWWVk2UHJYT3NkaTVtSzZNdGNmeVJSa1BTbmhpOTVLdyt5WCtZZ3VGNUxqcUxDTm1Uc2pHa2tDaWtQWlBRVXdZdEZCSEU2NkJYc0UwODQrRVZ1cExSdzlhQ2I3QXZhUWQvd1ludEVnOGhzK1dWZUc4aVkrWnNIWlpUVm91Tk54RlhmOCtQcWtieUpCNVdsanBHU0UvcjVwNit3N1ZmTE5uY0kxTHg3RUxhamxvWnIvWC9iLzdHcE9ZZi9mVlNHbFdRbUJSVVFuZitZSmdEblErSm9oVkk3OENOTTE5VDFIWEk2eG9aUGVVNzNMMEYyTXN4YUZhczFFM1ZUS1hnU3AwRHNsTzRtQlkwd2IrRk14QXBIUWtSOEJWbUhDUytKcXNnRjE2S0lWYVk0M29rU0FlRmlpN2Jydk14ZWUvVXhoK2VtM3NDOFpuajNLaVFHMEVJMGxjcUkrS3RGR2RFdXVIOU5wRTAzMG5BdlQxNTJYNVhIbEhQQmg2VkQwV29WZzd3ZXJnMjJKa3VCZ045UXUyRUFWQk14YThpMTVaQUNzaXVQS05jdTl6RTRTOWJsNG5WZjNzcFZkd2ViaGlBVnB5Qkp1bGMxcnNHWUgxMHdlNTVBYUl6Y3UydWhTSS9yMkVtZXQ0MmpkSTQxMC9WTGNtY08rZ2U3MVl1eTVHaUhRQXlnR1ZWQVFQekR0VzJCc0thOElJaE9UZ01NZFY2OXQxVkdtZmNHbnZNY1IwZ1RvUkRKdWtrUHRvbmJpSFMxaTk3QmZFTXNxN2VIYjN6SUZCaDlCbDMwNmIvU2xYTjVwTnVSTWxnd25EM1prZUFTMElzVXBCTGp1Qkl2RDRWb2xIRTN2WHQxVVJKcXRES05XakpaWENBc2xJS3NOckJmQVBIVlpzaFNwZU8vRDFrZzVHVFhjVHdBaUc1SGc2M1h5RHZLVDZqNnVnMzlUYTAzL0xJVGlIRWt0eXFSSmhmYzVGZHdodmErU3dwN3MwTTJrbjYvQy9Zd1FrdW5vWCs3V09yeFN5MHkwV1lEeUZ6U25lOUNOemRsRmQxTkRUUHdnUU1DY2RSUjJnWHV2TnQ5cG1kOWNKUUI5cGFVRDJjL2xPbmF6Vmg1Tmg2ZkllZ3FjL0MrMW50aXVSbURQaTh5NmloRzFyNTVQNFU3Vlp1cWdJRnVGc05uM2J4THJuVzVwVVUzRk9KdzZkNjVTQWJVWk11NGxlUGgxdVoycWRUNXgxeFU1RWNMMlVHMUtEa09nNFI4YVc2TFJzWGs3NnRVU0tPOWtiTU1zZkt6SVI0dGl3azhrMjhtTnViVDdRMHhkV3luNks4Zldjd3RDVFEzOTJaeEZnd2xVZ0JRODVKVEkyN1J3R2tIeENhN1lQYWRscHZtenozZUE1VkZHMXdYRFJDbERhZG1vRlBZTlhoc3E1WVVxUUIzNTZFMXJQUHdzYlZEZCtCWEJXbi9PL042dU9rUk1rQnVvaytvSGp6cFZmaXVTSzd3RjBZY3dZUXg4bG9renZmbVM0Znp3RS90MFdjeTdzOHljZlRqQkJxN09WNDNpajlVaDgrNXV6anJob0JtWXkranFySElnYUZiVTloU2lSNEErZ1ZyWlYwSW5WdzAvUy9HUWo4aTdHd3krWE1CUW1IY04xNm1ZQnNTNG9MZzBQRVZ0MVNSY2VYNWlMbmpIb0h6aWQ4YnVZa21rV29GWVZuc3RGNERkS082N1Z2TXhPV0tkZGZiMys0RGpqdE9VQXUzUGFURjl6N1NObVNZRnNIREQ2IiwiaHR0cDovL3NjaGVtYXMueG1sc29hcC5vcmcvd3MvMjAwNS8wNS9pZGVudGl0eS9jbGFpbXMvbmFtZWlkZW50aWZpZXIiOiJQSUQ6OTIwOC0yMDAyLTItODg3OTI1MTkwNzAxIiwiaHR0cDovL3NjaGVtYXMueG1sc29hcC5vcmcvd3MvMjAwNS8wNS9pZGVudGl0eS9jbGFpbXMvZ2l2ZW5uYW1lIjoiQW5kcmVhcyBKYWNrIENocmlzdGlhbnNlbiIsImxvZ2luVHlwZSI6IktleUNhcmQiLCJwaWQiOiI5MjA4LTIwMDItMi04ODc5MjUxOTA3MDEiLCJiM2YiOiJLYldpZll2Mms0MlNzK2RrcXVuS1o1eGxWQVBCM09rQ21Uc3N3MVdWQ2VjPSIsInVzZXJJZCI6IjI0MTA4MyIsImV4cCI6MTY2OTM2NzEzNSwiaXNzIjoiRW5lcmdpbmV0IiwianRpIjoiN2EyMjFhMWMtZjAxNi00NzQ0LWFjMTktOTQzZDlmMWEzYTZkIiwidG9rZW5OYW1lIjoicDEiLCJhdWQiOiJFbmVyZ2luZXQifQ.VzQLHfJPvdzrMpdcb2CixXHYYrKBI4qvttfjShQnJOE");
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl = curl_easy_init();
    if (curl)
    {
        char *data = "{\"meteringPoints\":{\"meteringPoint\" : [\"571313105202878672\"]}}";

        prices = fopen("output.json", "w");
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoint/getcharges");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, prices);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }
        fclose(prices);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}