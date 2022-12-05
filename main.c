#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <math.h>

#define MAX_LENGTH 30
#define MAX_LINES 1000000000
#define MAX_LEN 1000000000

#define REFRESH_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9SZWZyZXNoIiwidG9rZW5pZCI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBwQXBpIl0sInZlcnNpb24iOiIyIiwiaWRlbnRpdHlUb2tlbiI6IitkM05PeUUwMFE5YXowZ0sxd3VWVlRiTFJBaWF2eTh0eG1mSFhmWjYzN3d5L3RGVGpOTzFHTWtiNFB5dXZDNUVIQU4rWDgzbHh6TTdCK2R2ajlLVEYrVjA3eGc1U1NXcy9NdnVXQlRncjdwa2dKWjZZbFZyeU5aTjV4UTI2TFZsb0s2a2ppUUtWMlZmcTIzK1ZKaGQyYmJCdUo3b04wMmkzdzkwbVJ3TlhQcjRyc3d5U3RQZEo0TTVyajdoYmZRNDNvMngxY3hLeHRrSDNYWUs4TmxJSzR2MlYyekMzV2xwQ0xCMktKRzl1VDdCK1RKQzErODB1UzVjV1RlV0NrU3JzNTZMem1PVERaZnhKNzhGdVZwM2xvYW91bkFjWVNhcWswb2hPV05yeWhtOTRWSlcybDNFOWJ0RzBZckNVTkk3czl1azQ0cXIwUSt2QTFQcEdzRzVjaThPOS9YSGFTcVpzaWY5OXVFMHR4ZXVpZk8yb1ZBa0FmcHJXVUVkSVRDWHduMUtvTUxwMHVoQXU4TnN5Y3hWOGJHYnhxQmJIaS9NektNczJ2c2pUTW1xL2tPeG9VTkhKaUM1RklIUFhFc2VjckZMWWVPOUFVSnlRU01hZVIwTmtCRnVSKzdEaTE5SXdHME10YVVEdUU0TmE0aVR6Z3pSaVloUU51MFhyby91cGFua0lEV0VsUEhnWFRtWWZ1VHJsM0FFcHZIYmptQitVTkNXdi9CS2gzTTRrQ25nd1AvZlpNQmpubUVGdUwvU1d0ODM5V0NEVllZNlByWE9zZGk1bUs2TXRjZnlSUmtQU25oaTk1S3creVgrWWd1RjVManFMQ05tVHNqR2trQ2lrUFpQUVV3WXRGQkhFNjZCWHNFMDg0K0VWdXBMUnc5YUNiN0F2YVFkL3dZbnRFZzhocytXVmVHOGlZK1pzSFpaVFZvdU5OeEZYZjgrUHFrYnlKQjVXbGpwR1NFL3I1cDYrdzdWZkxObmNJMUx4N0VMYWpsb1pyL1gvYi83R3BPWWYvZlZTR2xXUW1CUlVRbmYrWUpnRG5RK0pvaFZJNzhDTk0xOVQxSFhJNnhvWlBlVTczTDBGMk1zeGFGYXMxRTNWVEtYZ1NwMERzbE80bUJZMHdiK0ZNeEFwSFFrUjhCVm1IQ1MrSnFzZ0YxNktJVmFZNDNva1NBZUZpaTdicnZNeGVlL1V4aCtlbTNzQzhabmozS2lRRzBFSTBsY3FJK0t0RkdkRXV1SDlOcEUwMzBuQXZUMTUyWDVYSGxIUEJoNlZEMFdvVmc3d2VyZzIySmt1QmdOOVF1MkVBVkJNeGE4aTE1WkFDc2l1UEtOY3U5ekU0UzlibDRuVmYzc3BWZHdlYmhpQVZweUJKdWxjMXJzR1lIMTB3ZTU1QWFJemN1MnVoU0kvcjJFbWV0NDJqZEk0MTAvVkxjbWNPK2dlNzFZdXk1R2lIUUF5Z0dWVkFRUHpEdFcyQnNLYThJSWhPVGdNTWRWNjl0MVZHbWZjR252TWNSMGdUb1JESnVra1B0b25iaUhTMWk5N0JmRU1zcTdlSGIzeklGQmg5QmwzMDZiL1NsWE41cE51Uk1sZ3duRDNaa2VBUzBJc1VwQkxqdUJJdkQ0Vm9sSEUzdlh0MVVSSnF0REtOV2pKWlhDQXNsSUtzTnJCZkFQSFZac2hTcGVPL0Qxa2c1R1RYY1R3QWlHNUhnNjNYeUR2S1Q2ajZ1ZzM5VGEwMy9MSVRpSEVrdHlxUkpoZmM1RmR3aHZhK1N3cDdzME0ya242L0MvWXdRa3Vub1grN1dPcnhTeTB5MFdZRHlGelNuZTlDTnpkbEZkMU5EVFB3Z1FNQ2NkUlIyZ1h1dk50OXBtZDljSlFCOXBhVUQyYy9sT25helZoNU5oNmZJZWdxYy9DKzFudGl1Um1EUGk4eTZpaEcxcjU1UDRVN1ZadXFnSUZ1RnNObjNieExyblc1cFVVM0ZPSnc2ZDY1U0FiVVpNdTRsZVBoMXVaMnFkVDV4MXhVNUVjTDJVRzFLRGtPZzRSOGFXNkxSc1hrNzZ0VVNLTzlrYk1Nc2ZLeklSNHRpd2s4azI4bU51YlQ3UTB4ZFd5bjZLOGZXY3d0Q1RRMzkyWnhGZ3dsVWdCUTg1SlRJMjdSd0drSHhDYTdZUGFkbHB2bXp6M2VBNVZGRzF3WERSQ2xEYWRtb0ZQWU5YaHNxNVlVcVFCMzU2RTFyUFB3c2JWRGQrQlhCV24vTy9ONnVPa1JNa0J1b2srb0hqenBWZml1U0s3d0YwWWN3WVF4OGxva3p2Zm1TNGZ6d0UvdDBXY3k3czh5Y2ZUakJCcTdPVjQzaWo5VWg4KzV1empyaG9CbVl5K2pxckhJZ2FGYlU5aFNpUjRBK2dWclpWMEluVncwL1MvR1FqOGk3R3d5K1hNQlFtSGNOMTZtWUJzUzRvTGcwUEVWdDFTUmNlWDVpTG5qSG9IemlkOGJ1WWtta1dvRllWbnN0RjREZEtPNjdWdk14T1dLZGRmYjMrNERqanRPVUF1M1BhVEY5ejdTTm1TWUZzSERENiIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWVpZGVudGlmaWVyIjoiUElEOjkyMDgtMjAwMi0yLTg4NzkyNTE5MDcwMSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL2dpdmVubmFtZSI6IkFuZHJlYXMgSmFjayBDaHJpc3RpYW5zZW4iLCJsb2dpblR5cGUiOiJLZXlDYXJkIiwicGlkIjoiOTIwOC0yMDAyLTItODg3OTI1MTkwNzAxIiwiYjNmIjoiS2JXaWZZdjJrNDJTcytka3F1bktaNXhsVkFQQjNPa0NtVHNzdzFXVkNlYz0iLCJ1c2VySWQiOiIyNDEwODMiLCJleHAiOjE3MDAxMzkwNzAsImlzcyI6IkVuZXJnaW5ldCIsImp0aSI6IjY0OWUyZjUyLTgwM2MtNDNjNC05MTlmLTg4NTY5OTczZTZjYSIsInRva2VuTmFtZSI6InAxIiwiYXVkIjoiRW5lcmdpbmV0In0.5PR8JuD5ZCMUmrYwrnpaXzZvzLZlOXh0OfXtZ7wuK-o"

// prototypes
void get_api_fees(char answer);
void get_api_spot_prices();
void optimaltime();

int main()
{
    optimaltime();
    /*FILE source;
    char answer;
    char answer1;

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer);

    get_api_fees(answer);

    printf("Do you want spot prices? y/n: ");
    scanf(" %c", &answer1);

    if (answer1 == 'y')
    {
        get_api_spot_prices();
    }*/

    /* fill_device(&source, devices);
    print_devices(devices); */
    
    return 0;
}

size_t save_api_result(char *ptr, size_t size, size_t nmemb, void *not_used)
{
    FILE *file;
    file = fopen("../accessToken.json", "w"); // w stands for write, it replaces the old data with the new
    fprintf(file, "%s", ptr);
    fclose(file);

    // returns the size of bytes to check if any dataloss has occured
    return size * nmemb;
}

void get_api_fees(char answer)
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
        access_token_file = fopen("../accessToken.json", "w"); // w stands for write, it replaces the old data with the new

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

        // Here we get the access token from the json file
        access_token_file = fopen("../accessToken.json", "r");
        fread(buffer, 5000, 1, access_token_file);
        fclose(access_token_file);

        parsed_json = json_tokener_parse(buffer);
        json_object_object_get_ex(parsed_json, "result", &access_token);

        const char *access_token_string = json_object_get_string(access_token);

        char buf[json_object_get_string_len(access_token)];

        strcpy(buf, access_token_string);

        char header_string[json_object_get_string_len(access_token) + 23];
        strcpy(header_string, "Authorization: Bearer ");

        strcat(header_string, buf);

        headers = curl_slist_append(headers, header_string);
    
        curl = curl_easy_init();
        if (curl)
        {
            metering_points = fopen("../meter.json", "w");
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

    // Here we get the access token from the json file
    access_token_file = fopen("../accessToken.json", "r");
    fread(buffer, 5000, 1, access_token_file);
    fclose(access_token_file);

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "result", &access_token);

    const char *access_token_string = json_object_get_string(access_token);

    char buf[json_object_get_string_len(access_token)];

    strcpy(buf, access_token_string);

    char header_string[json_object_get_string_len(access_token) + 23];
    strcpy(header_string, "Authorization: Bearer ");

    strcat(header_string, buf);

    headers = curl_slist_append(headers, header_string);
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl = curl_easy_init();
    if (curl)
    {
        char *data = "{\"meteringPoints\":{\"meteringPoint\" : [\"571313105202878672\"]}}";

        prices = fopen("../output.json", "w");
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

void optimaltime(){

  
  // Open File
  const char fname[] = "/workspaces/P1-Energy-Monitoring/data.txt";

  FILE *fp = fopen(fname, "r");

  printf("Opened file: %s\n", fname); 
  // Counts lines
  char cr;
  size_t lines = 0;

  while( cr != EOF ) {
    if ( cr == '\n' ) {
      lines++;
    }
    cr = getc(fp);
  }
  printf("Number of lines: %ld\n", lines); 
  char tempdata[lines];
  rewind(fp);

  // Read data
  {
    char *data[lines];
    size_t n;

    for (size_t i = 0; i < lines; i++) {
      data[i] = NULL;
      size_t n = 0;
      getline(&data[i], &n, fp);
    }

    for (size_t i = 0; i < lines; i++) {
        tempdata[i] = *data[i];
    }
  }

  // Close File
  fclose(fp);

    double elpriser[] = {6,5,4,3,2,5,3,7,4,4,5,6,7,4,3,2,2,4,4,3,4,5,6,3};

    int  watt, 
    totalcost = 1000000, 
    countingcost, 
    device_stoptime,
    temphour, 
    elpriser_watt_second, 
    device_clock, 
    hour_of_the_day,
    starttime,
    runningtemphour,
    array_length = sizeof(tempdata)/sizeof(tempdata[0]),
    day_clock; 

   for (day_clock = 0; day_clock < 86400; day_clock++)
   {

    int hour_of_the_day = day_clock / 3600;

    for (device_clock = 0; device_clock < array_length; device_clock++)
    {
        
        temphour = hour_of_the_day;
    
        if(((day_clock + device_clock) % 3600) == 0){
            temphour++;
        }
        
        double elpriser_watt_second = elpriser[temphour];

        countingcost = elpriser_watt_second * tempdata[device_clock] + countingcost;
      
        
    }
    if (countingcost < totalcost)
    {
        totalcost = countingcost;
        starttime = day_clock;
    }  
    countingcost = 0;
    temphour = 0;
   }
   printf("Det er billigst klokken %d, og det koster %d\n", starttime, totalcost);
   
}