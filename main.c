#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>

#define MAX_LENGTH 30
#define HOURS_IN_DAY 24
#define VAT_CONST 1.25

#define REFRESH_TOKEN "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ0b2tlblR5cGUiOiJDdXN0b21lckFQSV9SZWZyZXNoIiwidG9rZW5pZCI6ImJhMTE2NzQ4LTI4ZGYtNDkwYi04YjI1LTc1Mzk4MWExMGM5YSIsIndlYkFwcCI6WyJDdXN0b21lckFwaSIsIkN1c3RvbWVyQXBwQXBpIl0sInZlcnNpb24iOiIyIiwiaWRlbnRpdHlUb2tlbiI6IlNsN09KdDdYU3VGaDhoL081c3Fza3BCNk1jN0VxQWxqMUloNmFoTGlqbWx6bFdGdkg5cjM3SnJvWHJjMlN5R0VpOXdKejJ2clpzUm1iZHJkcy95MVRTRloxZXRuMXNldURnL09wM1VSQmRONUx4MDdGTGpDSDQyeXlhU3oxaDdKcGw0Uk9sODZ1TEEzaXB5cDRWWGtmOFlOcmdTdU9wOHJCUm1SRnlWZjBuQTVWdnlOZnFzTklSM1g0czFzQWdkOWJyZVkxS3ZoYVpDVWZrTzIzVkorcnViTXlybHp6ekJYQy9zb3dSNndQWWUzekZ1Qmhkclk4L001eDEyTGNmR2hQeFBLd1FyMk1ZRWk0M21KTnMyOUZaL0Y5U0lMMS9udHFQTnhkV1pzdm5FSjJKVTVpTmRnbmhoMG1YSkdKSFp0SjdoUU9kUUZtR3FWZnFQMVNFWjcxbUU5MkxzTG95eklyMFAzeVBEYW5IM0ZSMysrRk5KT3A0ZzZhZTlpWHN4MUljUFo5dFRuQzJZNkgyeHpvNysvQlZYL2dISGF3ZkVaVk5QandPRExiSDdJL20vMU9kZmNHN29FcTRHZitYeHdUcjQ4VWl0N3crTk5LTWxydCtESGRaMHZjU0tSQ2FaTncxVUVnNExMTU9QQlJqR3k4Mk95U3ozVEZTZGhzWURkR2JxMmRlRHhhWjJDTkFSRG5qWE53bVpmak9QMG03ZWk4dXBOV0wraWRjbGkzVVhFTkVaMWVISWIvbE16VGMzSmkyc01DK0xSdk1YR05rR244emxDOHNTa2h6MmMvY0sxbExGK0ZVaTkwaFBKWW8rVkhvRzhsZzJHa3l6OU5naDhaNGx2cXdEMEJhbndmTnMvZlVaVlFjc2Y5VHJWYmRRQ0pIVjk3bENjWExPMFVzdS9jQjdqQm02aTlpZ1E3Tmp3b3RyV21jY0VPbE9ubjVDTG52VjZhTGdjMkZzRjk4WlRidzhVTjJicFk4OWEvM05rZFpCUEJHeWtvU0xCNmtrWTkybk9kajJPdVdaSzVITExhU2ovdHJzMjdYaTNEWVp2Y1RmbG1WWFJrQWJ0QnVDeTd5T1E2YXVtNUFvdmF0VEdrNlQ0NHl3V2wxKzU2ZVF1QzF3aXhRZkRhTzlTSmhtVnoxOWxaOWZnM3BJV3BFakgvczEvc1dzcXdaS001QU9SUkxyQTFDOEZNTnh3OFlRZmpVRFAxWWxFM2VzYWJyTnBDT1dUeUVlNjNra0U0YzlZRi9hQTFIb1ljekVZNzJqNkF1akE2SnRVR0lQcENUMmZSOEQ4eTNhMjF1T3MxMmJPVEtaeGU3SHNQTkZkOTZ5RlNWYXRqRE1rYzZOaTZSY0VEOWp0K1RLSmZuZWtXK1FiZ1ZTZmdaRWQ4NW85ZXIwR09HRklYQXcvTU81Tjh5MGZqRzQ4cjJwamhlcnlEUVR4NkowbUVGSXVMVEU0Qlk5Y1dBbUwyL25RMUhJSUJhUGIyM2R3KzF6NjhrdmkzWEs2M0JiRmJnYzIrN1JxWXpWWFpvK1ZQTzMzR3RpTHBXUDZ2THRvNmFydU1wMG9KWTZDMnc2YnowK3pUTkV4TmZPNk9INFNqcE1XTC9kSElQeWQrZGtDRkJ0dkpYeTN3eHhhamR3U2V2Y1hZWXdURTJDTStyMmR5VFFVUW5uNk5QN0IvNTNtOWRjMGpObW1HdFBQWlQrVi9kdEVuNWVUaGVJcUQwWDJseXJBbHJqc2prVXFzOERGcWJ5RUV4Y1lsWTJGd1RFampHdmdLSUk0N0dpNjlpTWIwMWZ4cDZuQ3hiZnh4V2lHc1k0MnlWWnFmR29RY1ZVV3hkN1U4TWpVNkh0QTh2bTAxUE5yRnVzTVhnd2I0aEw1M2lCWTdsQmlKZGxGM2dRSnVyTmNWVDRVMXp0RlBJWG91WlVNZHpqMkpPbVE5WmRHRmdLbE5YelNsVGM0Q0VBMHlyaDk2eFNqY25wSXZmaE0xV0x2QW13VTN5a29ScU1ia2lBdCtjS0JZdnFHaGhranIvRXplMWRCYjNkblR1NUNPdE1GeFBvMVozcEV1bWg1bTlUL3c0Ni9lRDhMamE1azVTczNrekFBUWZFdmVidnB1L0NPU01FWXRUTXpTeGpINitVSXZhZHlpZUVlTWlOUy80OGdPZTB4K0ZEVHlXVURzMysvU09iUXU5ckhkMkVWSm5UNFNiOTlneVQvUGg2UFRsTDQ3cWxwVEVwYXlxb01HTkV6anhyZWNsZHpRNUNQbXEzSDlaY2VnSllVdTJQTm9FOEhvYSt3MWJpck91R0FMYnVia2lCSVlxQS8ydGFSMllIaTRHSGt2M1VBSGpNQWpnYUdaMVFXb2Z5L3JXQ25HVjE4ZGRzb0w1a1ZYQUxYZ2xnOXZQTWFQN2JmY1gyb0g5bFZkK0hvbUUyak1DanBFNGh3R0paK1pmMHFMTXNUam5aSUZwdU9NL0N1U3BxWTNOcEp3UExOaytCRkROTGFncVVDbnJXYUhqNFpURVN1ekVpL25GZVRDaVd6eHVHMUxweHNaOVA4UThCWCIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL25hbWVpZGVudGlmaWVyIjoiUElEOjkyMDgtMjAwMi0yLTMzODA3NDY3NjM3MSIsImh0dHA6Ly9zY2hlbWFzLnhtbHNvYXAub3JnL3dzLzIwMDUvMDUvaWRlbnRpdHkvY2xhaW1zL2dpdmVubmFtZSI6IkFkb21hcyBDaXBseXMiLCJsb2dpblR5cGUiOiJLZXlDYXJkIiwicGlkIjoiOTIwOC0yMDAyLTItMzM4MDc0Njc2MzcxIiwiYjNmIjoiL0pBYXZoT3lzU0ZjWVBqM0ZIK1RUUHovbmc3WG84NGZHZ3B3MlhLdGZJRT0iLCJ1c2VySWQiOiI1NjgxMzEiLCJleHAiOjE3MDE1MTI2NTgsImlzcyI6IkVuZXJnaW5ldCIsImp0aSI6ImJhMTE2NzQ4LTI4ZGYtNDkwYi04YjI1LTc1Mzk4MWExMGM5YSIsInRva2VuTmFtZSI6IlByb2pla3QiLCJhdWQiOiJFbmVyZ2luZXQifQ.67x-rcNmBqTJOn4U37skMcsX5KfkfbNfu2Kt765Id5c"

typedef struct {
    char name[MAX_LENGTH];
    double standby_watt;
    double running_watt;
    int amounts;

} device;

typedef struct {
    double net_tariff[24];
    double net_tariff_discount[24];
    double net_tariff_transmission;
    double system_tariff;
    double balance_tariff;
    double electricity_tax;
} prices;

typedef struct {
    double total_price[24];
    double total_tax[24];
    double VAT[24];
} total_prices;

// prototypes
void fill_device(FILE *source, device *devices);

void print_devices(device *devices);

void get_api_fees(char answer);

void get_api_spot_prices();

void readPrices_spotPrice(double *SpotPriceDKK);

void readPrices_tariffs(prices *price_data);

void total_price_calc(double *SpotPriceDKK, prices *price_data, total_prices *result);


        int main() {
    FILE source;
    device *devices;
    prices *price_data;
    total_prices *result;
    char answer;
    char answer1;
    double SpotPricesDKK[24];

    devices = malloc(8 * sizeof(device));
    price_data = malloc(sizeof(prices));
    result = malloc(sizeof(total_prices));

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer);

    get_api_fees(answer);

    printf("Do you want spot prices? y/n: ");
    scanf(" %c", &answer1);

    if (answer1 == 'y') {
        get_api_spot_prices();
    }

    readPrices_spotPrice(SpotPricesDKK);
    readPrices_tariffs(price_data);
    printf("LOOK HERE DOG\n");

    total_price_calc(SpotPricesDKK, price_data, result);


    /* fill_device(&source, devices);
    print_devices(devices); */

    free(devices);
    free(price_data);
    free(result);
    return 0;
}

void fill_device(FILE *source, device *devices) {
    source = fopen("../device.txt", "r");

    if (source == NULL) {
        printf("Error the file does not exist");
        exit(1);
    }

    char line[100];

    for (int i = 0; i < 8; i++) {
        fgets(line, sizeof(line), source);

        sscanf(line, "%[A-Za-z_^], %lf, %lf",
               devices[i].name, &devices[i].standby_watt,
               &devices[i].running_watt);

        printf("Insert amounts for %s > ", devices[i].name);
        scanf("%d", &devices[i].amounts);
    }

    fclose(source);
}

void print_devices(device *devices) {
    for (int i = 0; i < 8; i++) {
        printf("%s, %.2lf, %.2lf, %d\n", devices[i].name, devices[i].standby_watt,
               devices[i].running_watt, devices[i].amounts);
    }
}

size_t save_api_result(char *ptr, size_t size, size_t nmemb, void *not_used) {
    FILE *file;
    file = fopen("../accessToken.json", "w"); // w stands for write, it replaces the old data with the new
    fprintf(file, "%s", ptr);
    fclose(file);

    // returns the size of bytes to check if any dataloss has occured
    return size * nmemb;
}

void get_api_fees(char answer) {
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

    if (answer == 'y') {
        access_token_file = fopen("../accessToken.json",
                                  "w"); // w stands for write, it replaces the old data with the new

        headers = curl_slist_append(headers, "Authorization: Bearer " REFRESH_TOKEN);

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/token");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, access_token_file);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
                printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
            }

            fclose(access_token_file);
            curl_easy_cleanup(curl);
        }
    } else {

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
        if (curl) {
            metering_points = fopen("../meter.json", "w");
            curl_easy_setopt(curl, CURLOPT_URL,
                             "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoints?includeAll=false");
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, metering_points);
            res = curl_easy_perform(curl);

            if (res != CURLE_OK) {
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
    if (curl) {
        char *data = "{\"meteringPoints\":{\"meteringPoint\" : [\"571313105200443001\"]}}";

        prices = fopen("../output.json", "w");
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoint/getcharges");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, prices);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }
        fclose(prices);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void get_api_spot_prices() {
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
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://api.energidataservice.dk/dataset/Elspotprices?start=now-P0DT1H&sort=HourDK&columns=HourDK,SpotPriceDKK&filter={%22PriceArea%22:[%22DK1%22]}");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, spot_prices_file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }

        fclose(spot_prices_file);
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
}

void readPrices_spotPrice(double *SpotPriceDKK) {
    FILE *spotPrices_file; //Opens the spotPrices file
    FILE *output; //Opens the file with personal Tariffs
    char buffer[6000];

    // Here we get the access token from the json file
    json_object *parsed_json;
    json_object *records;

    size_t n_prices;
    size_t i;

    spotPrices_file = fopen("../spotPrices.json", "r");
    fread(buffer, 6000, 1, spotPrices_file);
    fclose(spotPrices_file);

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "records", &records);
    putchar('\n');
    n_prices = json_object_array_length(records);
    printf("Found %lu prices", n_prices);

    putchar('\n');

    for (i = 0; i < n_prices; i++) {
        json_object *spotPrices_temp;
        json_object *spotPrice_temp;
        spotPrices_temp = json_object_array_get_idx(records, i);
        json_object_object_get_ex(spotPrices_temp, "SpotPriceDKK", &spotPrice_temp);
        SpotPriceDKK[i] = json_object_get_double(spotPrice_temp)/1000; //Divide by 1000 to get price in DKK
        printf("%lu, %lf\n", i + 1, SpotPriceDKK[i]);
    }
    json_object_put(parsed_json); //Frees json-object from memory
}

void readPrices_tariffs(prices *price_data) {
    FILE *tariffs_file; //Opens the tariffs file
    FILE *output; //Opens the file with personal Tariffs
    char buffer[6000];

    // Here we get the access token from the json file
    json_object *parsed_json;
    json_object *result_array;
    json_object *result_parent;
    json_object *result_child;
    json_object *tariffs_array;
    json_object *tariff_price_parent;
    json_object *tariff_discount_parent;
    json_object *tariff_prices;
    json_object *tariff_discount;
    json_object *net_tariff_transmission_parent;
    json_object *net_tariff_transmission_array;
    json_object *net_tariff_transmission_price;
    json_object *system_tariff_parent;
    json_object *system_tariff_array;
    json_object *system_tariff_price;
    json_object *balance_tariff_parent;
    json_object *balance_tariff_array;
    json_object *balance_tariff_price;
    json_object *electricity_tax_parent;
    json_object *electricity_tax_array;
    json_object *electricity_tax_price;

    size_t n_prices;
    size_t i;

    tariffs_file = fopen("../output.json", "r");
    fread(buffer, 6000, 1, tariffs_file);
    fclose(tariffs_file);

    parsed_json = json_tokener_parse(buffer);
    json_object_object_get_ex(parsed_json, "result", &result_array);
    result_parent = json_object_array_get_idx(result_array, 0);

    json_object_object_get_ex(result_parent, "result", &result_child);
    json_object_object_get_ex(result_child, "tariffs", &tariffs_array);

    tariff_price_parent = json_object_array_get_idx(tariffs_array, 0);
    tariff_discount_parent = json_object_array_get_idx(tariffs_array, 1);
    net_tariff_transmission_parent = json_object_array_get_idx(tariffs_array, 2);
    system_tariff_parent = json_object_array_get_idx(tariffs_array, 3);
    balance_tariff_parent = json_object_array_get_idx(tariffs_array, 4);
    electricity_tax_parent = json_object_array_get_idx(tariffs_array, 5);

    json_object_object_get_ex(tariff_price_parent, "prices", &tariff_prices);
    json_object_object_get_ex(tariff_discount_parent, "prices", &tariff_discount);
    json_object_object_get_ex(net_tariff_transmission_parent, "prices", &net_tariff_transmission_array);
    json_object_object_get_ex(system_tariff_parent, "prices", &system_tariff_array);
    json_object_object_get_ex(balance_tariff_parent, "prices", &balance_tariff_array);
    json_object_object_get_ex(electricity_tax_parent, "prices", &electricity_tax_array);

    n_prices = json_object_array_length(tariff_prices);
    printf("Found %lu prices", n_prices);
    putchar('\n');

    //Here we get the actual prices
    for (i = 0; i < n_prices; i++) {
        json_object *tariffPrices_temp;
        json_object *tariffPrice_temp;
        json_object *tariffDiscounts_temp;
        json_object *tariffDiscount_temp;
        tariffPrices_temp = json_object_array_get_idx(tariff_prices, i);
        json_object_object_get_ex(tariffPrices_temp, "price", &tariffPrice_temp);
        price_data->net_tariff[i] = json_object_get_double(tariffPrice_temp);
        printf("%lu, %lf\n", i + 1, price_data->net_tariff[i]);

        tariffDiscounts_temp = json_object_array_get_idx(tariff_discount, i);
        json_object_object_get_ex(tariffDiscounts_temp, "price", &tariffDiscount_temp);
        price_data->net_tariff_discount[i] = json_object_get_double(tariffDiscount_temp);
        printf("%lu, %lf\n", i + 1, price_data->net_tariff_discount[i]);
    }

    json_object_object_get_ex(json_object_array_get_idx(net_tariff_transmission_array, 0), "price",
                              &net_tariff_transmission_price);
    json_object_object_get_ex(json_object_array_get_idx(system_tariff_array, 0), "price", &system_tariff_price);
    json_object_object_get_ex(json_object_array_get_idx(balance_tariff_array, 0), "price", &balance_tariff_price);
    json_object_object_get_ex(json_object_array_get_idx(electricity_tax_array, 0), "price", &electricity_tax_price);
    price_data->net_tariff_transmission = json_object_get_double(net_tariff_transmission_price);
    price_data->system_tariff = json_object_get_double(system_tariff_price);
    price_data->balance_tariff = json_object_get_double(balance_tariff_price);
    price_data->electricity_tax = json_object_get_double(electricity_tax_price);

    printf("NetTariff %lf\n", price_data->net_tariff_transmission);
    printf("SystemTariff %lf\n", price_data->system_tariff);
    printf("BalanceTariff %lf\n", price_data->balance_tariff);
    printf("Electricity Tax %lf\n", price_data->electricity_tax);

    json_object_put(parsed_json); //Frees json-object from memory
}

void total_price_calc(double *SpotPriceDKK, prices *price_data, total_prices *result)
{

    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        result->total_tax[i] += price_data->net_tariff[i];
        result->total_tax[i] += price_data->net_tariff_discount[i]; //The value is already negative
        result->total_tax[i] += price_data->net_tariff_transmission;
        result->total_tax[i] += price_data->system_tariff;
        result->total_tax[i] += price_data->balance_tariff;
        result->total_tax[i] += price_data->electricity_tax;
        result->VAT[i] += (SpotPriceDKK[i] + result->total_tax[i]) * 0.25;
        result->total_price[i] += (SpotPriceDKK[i] + result->total_tax[i]) * VAT_CONST;
    }

    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        printf("Hour: %d Total Price: %lf Total Tax: %lf VAT: %lf \n",i+14, result->total_price[i], result->total_tax[i], result->VAT[i]);
    }

}
