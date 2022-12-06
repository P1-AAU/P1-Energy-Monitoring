#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <time.h>

#define MAX_LENGTH 30
#define HOURS_IN_DAY 24
#define VAT_CONST 1.25
#define BUFFER_SIZE 5000
#define METER_SIZE 100

struct MemoryStruct
{
    char *memory;
    size_t size;
};

typedef struct
{
    double net_tariff[24];
    double net_tariff_discount[24];
    double net_tariff_transmission;
    double system_tariff;
    double balance_tariff;
    double electricity_tax;
} prices;

typedef struct
{
    double total_price[24];
    double total_tax[24];
    double VAT[24];
} total_prices;

// prototypes
void get_api_token(char answer, char *access_token_string);
void get_metering_point(char answer, char *access_token_string, char *meter_id);
void get_tarrifs(char answer, char *access_token_string, char *meter_id);
void get_api_spot_prices();
void readPrices_spotPrice(double *SpotPriceDKK);
void readPrices_tariffs(prices *price_data);
void total_price_calc(double *SpotPriceDKK, prices *price_data, total_prices *result);
void print_prices(double *SpotPriceDKK, total_prices *result);

int main()
{
    prices *price_data;
    total_prices *result;
    char answer_refresh;
    char answer_access;
    char answer_spot;
    double SpotPricesDKK[48];

    char access_token_string[BUFFER_SIZE];
    char meter_id[METER_SIZE];
    price_data = malloc(sizeof(prices));
    result = malloc(sizeof(total_prices));

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer_access);

    get_api_token(answer_access, access_token_string);
    get_metering_point(answer_access, access_token_string, meter_id);
    get_tarrifs(answer_access, access_token_string, meter_id);

    printf("Do you want spot prices? y/n: ");
    scanf(" %c", &answer_spot);

    if (answer_spot == 'y')
    {
        get_api_spot_prices();
    }

    readPrices_spotPrice(SpotPricesDKK);
    readPrices_tariffs(price_data);
    total_price_calc(SpotPricesDKK, price_data, result);
    print_prices(SpotPricesDKK, result);

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

    json_object *parsed_refresh_token;
    json_object *refresh_token;
    json_object *parsed_json;
    json_object *access_token;
    json_object *new_json;

    const char *access_token_str;
    char buffer[BUFFER_SIZE];

    struct MemoryStruct chunk;

    FILE *refresh_token_file;
    FILE *access_token_file;

    chunk.memory = malloc(1); /* will be grown as needed by the realloc above */
    chunk.size = 0;           /* no data at this point */

    refresh_token_file = fopen("../refreshToken.json", "r");

    fread(buffer, BUFFER_SIZE, 1, refresh_token_file);

    parsed_refresh_token = json_tokener_parse(buffer);
    refresh_token = json_object_object_get(parsed_refresh_token, "token");

    headers = curl_slist_append(headers, json_object_get_string(refresh_token));

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

    fclose(refresh_token_file);
    json_object_put(parsed_refresh_token);
    json_object_put(new_json);
    json_object_put(parsed_json);
    free(chunk.memory);

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
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://api.energidataservice.dk/dataset/Elspotprices?start=now-P0DT1H&sort=HourDK&columns=HourDK,SpotPriceDKK&filter={%22PriceArea%22:[%22DK1%22]}");
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

void readPrices_spotPrice(double *SpotPriceDKK)
{
    FILE *spotPrices_file; // Opens the spotPrices file
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
    printf("Found %lu SpotPrices", n_prices);

    putchar('\n');

    for (i = 0; i < n_prices; i++)
    {
        json_object *spotPrices_temp;
        json_object *spotPrice_temp;
        spotPrices_temp = json_object_array_get_idx(records, i);
        json_object_object_get_ex(spotPrices_temp, "SpotPriceDKK", &spotPrice_temp);
        SpotPriceDKK[i] = json_object_get_double(spotPrice_temp) / 1000; // Divide by 1000 to get price in DKK
    }
    json_object_put(parsed_json); // Frees json-object from memory
}

void readPrices_tariffs(prices *price_data)
{
    FILE *tariffs_file; // Opens the tariffs file
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

    tariffs_file = fopen("../tarrifs.json", "r");
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
    printf("Found %lu tariff prices", n_prices);
    putchar('\n');

    // Here we get the actual prices
    for (i = 0; i < n_prices; i++)
    {
        json_object *tariffPrices_temp;
        json_object *tariffPrice_temp;
        json_object *tariffDiscounts_temp;
        json_object *tariffDiscount_temp;
        tariffPrices_temp = json_object_array_get_idx(tariff_prices, i);
        json_object_object_get_ex(tariffPrices_temp, "price", &tariffPrice_temp);
        price_data->net_tariff[i] = json_object_get_double(tariffPrice_temp);

        tariffDiscounts_temp = json_object_array_get_idx(tariff_discount, i);
        json_object_object_get_ex(tariffDiscounts_temp, "price", &tariffDiscount_temp);
        price_data->net_tariff_discount[i] = json_object_get_double(tariffDiscount_temp);
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

    json_object_put(parsed_json); // Frees json-object from memory
}

void total_price_calc(double *SpotPriceDKK, prices *price_data, total_prices *result)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int current_hour = timeinfo->tm_hour;

    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        result->total_tax[i] += price_data->net_tariff[i];
        result->total_tax[i] += price_data->net_tariff_discount[i]; // The value is already negative
        result->total_tax[i] += price_data->net_tariff_transmission;
        result->total_tax[i] += price_data->system_tariff;
        result->total_tax[i] += price_data->balance_tariff;
        result->total_tax[i] += price_data->electricity_tax;
    }

    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        result->VAT[i] += (SpotPriceDKK[i] + result->total_tax[i]) * 0.25; // The spot prices are already given in current hour
        result->total_price[i] += (SpotPriceDKK[i] + result->total_tax[current_hour]) * VAT_CONST;

        if (current_hour == 23)
        {
            current_hour = 0;
        }
        else
        {
            current_hour++;
        }
    }
}

void print_prices(double *SpotPriceDKK, total_prices *result)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int current_hour = timeinfo->tm_hour;

    printf("Hour \t Total-Price \t Spot-Price \t Tax-Total \t VAT \n");
    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        printf("%02d:00 %12lf %15lf %15lf %10lf\n", current_hour, result->total_price[i], SpotPriceDKK[i], result->total_tax[i], result->VAT[i]);
        if (current_hour == 23)
        {
            current_hour = 0;
        }
        else
        {
            current_hour++;
        }
    }
}