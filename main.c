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

void optimaltime();

int main()
{
    optimaltime();

    return 0;
}

void optimaltime(){

  
  // Open File
  const char fname[] = "/workspaces/P1-Energy-Monitoring/data.txt";

  FILE *fp = fopen(fname, "r");

  printf("Opened file: %s\n", fname); 
  // Counts lines
  char cr;
  size_t lines = 0;

  while( cr !=EOF ) {
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

    double elpriser[] = {6,5,4,3,2,2,1,5,4,4,5,6,7,4,3,2,1,2,2,3,4,12,12,12,12,12,12};

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

    temphour = hour_of_the_day;

    for (device_clock = 0; device_clock < array_length; device_clock++)
    {
        
    
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