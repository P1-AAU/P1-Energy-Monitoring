#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <time.h>

// predefined values
#define MAX_LENGTH 30
#define HOURS_IN_DAY 24
#define VAT_CONST 1.25
#define BUFFER_SIZE 5000
#define METER_SIZE 100
#define MAX_ARRAY_LENGTH 35
#define HOUR_IN_SECONDS 3600
#define WATT_PER_SECONDS_CONVERTER_TO_KWH 3600000
#define MIN_IN_SECONDS 60
#define UPPER_BOUND 10000000000
#define CONVERT_UNIT 1000

// Structs
typedef struct
{
    char *memory;
    size_t size;
} MemoryStruct;

typedef struct
{
    double net_tariff[HOURS_IN_DAY];
    double net_tariff_discount[HOURS_IN_DAY];
    double net_tariff_transmission;
    double system_tariff;
    double balance_tariff;
    double electricity_tax;
} tarrifs;

typedef struct
{
    double total_price[MAX_ARRAY_LENGTH];
    double total_tax[MAX_ARRAY_LENGTH];
    double VAT[MAX_ARRAY_LENGTH];
} total_prices;

// prototypes
void get_api_token();
void get_metering_point();
void get_tarrifs();
void get_spot_prices();
void readPrices_spotPrice(double *SpotPriceDKK, size_t *lengthOfArray);
void readPrices_tariffs(tarrifs *price_data);
void total_price_calc(double *SpotPriceDKK, tarrifs *price_data, total_prices *result, size_t lengthOfArray);
void print_prices(double *SpotPriceDKK, total_prices *result, size_t lengthOfArray);
void optimaltime(total_prices *result, size_t lengthOfArray, char *device);

int main()
{
    char answer_access;
    char answer_spot;
    double SpotPricesDKK[48];
    size_t lengthOfSpotPriceData = 0;

    // Here we initialize curl so that it can be used in our program
    curl_global_init(CURL_GLOBAL_ALL);

    // Here we allocate memory for tarrifs and total prices
    tarrifs *price_data = malloc(sizeof(tarrifs));
    total_prices *result = malloc(sizeof(total_prices));

    // Here we check if the memory has been allocated and if not we stop the program
    if (!price_data)
    {
        /* out of memory! */
        printf("not enough memory to allocate space for the tarrifs\n");
        return 0;
    }

    if (!result)
    {
        /* out of memory! */
        printf("not enough memory to allocate space for the total price array\n");
        return 0;
    }

    printf("Do you want a new access token? y/n: ");
    scanf("%c", &answer_access);

    // If the user says yes to get a new access token, the program will generate
    // a new accesss token and update the metering point id as well.
    if (answer_access == 'y')
    {
        // This function generates a new access token
        get_api_token();
        // This function retrieves the metering point id corresponding to the access token
        get_metering_point();
    }

    // This function grabs the users personal tarrifs
    get_tarrifs();

    printf("Do you want spot prices? y/n: ");
    scanf(" %c", &answer_spot);

    // If the user says yes to get new spot prices the program will
    // retrieve the spot prices from current hour and as far ahead as possible
    if (answer_spot == 'y')
    {
        get_spot_prices();
    }

    // These functions reads the spotprices and tarrifs from the files.
    readPrices_spotPrice(SpotPricesDKK, &lengthOfSpotPriceData);
    readPrices_tariffs(price_data);
    // This function calculates the total prices
    total_price_calc(SpotPricesDKK, price_data, result, lengthOfSpotPriceData);
    // This function prints the finished result in a readable way.
    print_prices(SpotPricesDKK, result, lengthOfSpotPriceData);

    // These functions calculates and prints the optimal time to use the device
    optimaltime(result, lengthOfSpotPriceData, "washing machine");
    optimaltime(result, lengthOfSpotPriceData, "dishwasher");

    // Here we free the allocated memory again.
    free(price_data);
    free(result);

    // Here we release all resources related to curl globally
    curl_global_cleanup();

    // Here we run the python program to get a graph
    system("python3 ../MakePlotPython.py");

    return 0;
}

// This is a callback function used to allocate enough memory for the content returned by the api
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    // Here the size of the returned content gets caluclated
    size_t realsize = size * nmemb;

    // Here we assign the memory struct to value of userp
    // which contains the memory and size of the api output
    MemoryStruct *mem = (MemoryStruct *)userp;

    // Here we use realloc to increase the memory already allocated
    // we increase the mem->memory by taking the prevous memory size and adding
    // the size of the returned content that we calculated earlier.
    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (!ptr)
    {
        /* out of memory! */
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    // Here we update the mem->memory to the newly allocated memory we allocated with realloc above.
    mem->memory = ptr;

    // We then add the new content to the end of our current memory
    // mem->memory[mem->size] represents the end of our memory here we then
    // add the contents to it using memcpy where we also specifiy the size of
    // our new content which we calculated earlier.
    memcpy(&(mem->memory[mem->size]), contents, realsize);

    // Now we add the contents size to the mem_size and
    // update the mem->memory so that it starts on the new size
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    // Lastly we return the contents size so that curl knows that
    // we recived the correct amount of data.
    // otherwise curl would have returned an error.
    return realsize;
}

// This function asks the api for a new access token through
// a GET method using Datahubs api (eloverblik.dk)
// which is usable for the next 24 hours.
void get_api_token()
{
    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;

    json_object *parsed_refresh_token;
    json_object *refresh_token;
    json_object *parsed_json;
    json_object *access_token;
    json_object *new_json;

    const char *access_token_str;

    char buffer[BUFFER_SIZE];

    FILE *refresh_token_file;
    FILE *access_token_file;

    MemoryStruct chunk;

    // Here we start by allocating some space for the curl output
    // This memory will then automaticly increase due to the realloc in WriteMemoryCallback
    chunk.memory = malloc(1);
    chunk.size = 0; /* no data at this point */

    // Here we test if the memory allocation was succesful
    // and aborts if it was not
    if (!chunk.memory)
    {
        /* out of memory! */
        printf("not enough memory to allocate space for the refresh token \n");
        abort();
    }

    // Here we open and reads the whole file into a buffer
    refresh_token_file = fopen("../refreshToken.json", "r");
    fread(buffer, BUFFER_SIZE, 1, refresh_token_file);

    // We then create a json object out of the buffer
    parsed_refresh_token = json_tokener_parse(buffer);
    // Now we search the json object for a key called "token" and assign its value
    refresh_token = json_object_object_get(parsed_refresh_token, "token");

    // Here we parse the value we got above into the header as a string
    // the parsed value is a personal refresh token that a user can get from eloverblik.dk
    // it is parsed in the format: "Authorization: Bearer <token>"
    // without this token the api wouldnt allow us access
    headers = curl_slist_append(headers, json_object_get_string(refresh_token));

    // Here we start the Curl which is what talks to the api
    curl = curl_easy_init();
    if (curl)
    {
        // We then pass infromation to curl, such as the api link, headers and output place.
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/token");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // These 2 options below is showing where curl should output the result
        // in this case it outputs the result to our callback function
        //  WriteMemoryCallback and to the struct array called chunk.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Here we finally perform the call to the api
        res = curl_easy_perform(curl);

        // We then test if the call was succesful and if not it returns an error message
        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }
        else
        {
            parsed_json = json_tokener_parse(chunk.memory);
            access_token = json_object_object_get(parsed_json, "result");
            access_token_str = json_object_get_string(access_token);

            // Here we create a string buffer with the length of the value above
            char buf[json_object_get_string_len(access_token)];

            // We then copy the string we got above into the newly assigned buffer called buf
            strcpy(buf, access_token_str);

            // Here we create a variable and assign its length to the length of the token + 23
            // the + 23 is because we need to make space for "Authorization: Bearer " which is 23 char
            // in length and is required to be in front of the token when we pass it in the curl header.
            char header_string[json_object_get_string_len(access_token) + 23];

            strcpy(header_string, "Authorization: Bearer ");
            strcat(header_string, buf);

            // Here we create a new empty json object
            new_json = json_object_new_object();
            // We then create a key called "result" and add the header string to it as a string
            json_object_object_add(new_json, "result", json_object_new_string(header_string));

            access_token_file = fopen("../accessToken.json", "w");
            fprintf(access_token_file, "%s", json_object_get_string(new_json));
            fclose(access_token_file);
        }
        // here we release all the resources curl has used above
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    // here we close files and free memory
    fclose(refresh_token_file);
    json_object_put(parsed_refresh_token);
    json_object_put(new_json);
    json_object_put(parsed_json);
    free(chunk.memory);
}

// This function retrives the metering point id through
// a GET method using Datahubs api (eloverblik.dk)
// this function uses a lot of the same parts as the prevous function
// these parts will therefore not be commented on again.
void get_metering_point()
{
    CURL *curl;
    CURLcode res;

    struct curl_slist *headers = NULL;

    MemoryStruct chunk;

    FILE *access_token_file;

    chunk.memory = malloc(1);
    chunk.size = 0;

    if (!chunk.memory)
    {
        printf("not enough memory to allocate space for the meter id \n");
        abort();
    }

    char buffer[BUFFER_SIZE];

    json_object *parsed_json;
    json_object *access_token;

    access_token_file = fopen("../accessToken.json", "r");
    fread(buffer, BUFFER_SIZE, 1, access_token_file);
    fclose(access_token_file);

    parsed_json = json_tokener_parse(buffer);
    access_token = json_object_object_get(parsed_json, "result");

    headers = curl_slist_append(headers, json_object_get_string(access_token));

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.eloverblik.dk/customerapi/api/meteringpoints/meteringpoints?includeAll=false");
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
            FILE *file;

            json_object *parsed_json;
            json_object *meter_parent;
            json_object *meter_child;
            json_object *meter_point_id;

            parsed_json = json_tokener_parse(chunk.memory);
            meter_parent = json_object_object_get(parsed_json, "result");

            // We know that meter_parent is an array, therefore we search the array for the first index
            meter_child = json_object_array_get_idx(meter_parent, 0);
            meter_point_id = json_object_object_get(meter_child, "meteringPointId");

            // Here we create two new objects and an array
            json_object *new_meter_parent = json_object_new_object();
            json_object *new_meter_child = json_object_new_object();
            json_object *new_meter_child_array = json_object_new_array();

            // We then add the meter point id to the array we just created
            json_object_array_add(new_meter_child_array, meter_point_id);
            // We then add a new object called "meteringPoint" and the array above as the value
            json_object_object_add(new_meter_child, "meteringPoint", new_meter_child_array);
            // We then add another object called "meteringPoints" and assign the object above as the value
            json_object_object_add(new_meter_parent, "meteringPoints", new_meter_child);
            // We know have a nested json format looking like this
            // { "meteringPoints": { "meteringPoint": [ "metering point id" ] } }
            // this is the required format for the api

            file = fopen("../meter.json", "w"); // w stands for write, it replaces the old data with the new
            fprintf(file, "%s", json_object_get_string(new_meter_parent));
            fclose(file);

            json_object_put(new_meter_parent);
            json_object_put(parsed_json);

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }
    }

    json_object_put(parsed_json);
}

// This function retrieves the users personal tarrifs
// through a POST method using Datahubs api (eloverblik.dk)
// by sending the meter id in json format that we made in the above function
// this function again uses a lot of the same parts as the prevous function
// these parts will therefore not be commented on again.
void get_tarrifs()
{
    CURL *curl;
    CURLcode res;

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

        FILE *access_token_file;
        FILE *metering_point_file;

        char buffer[BUFFER_SIZE];
        char meter_buffer[METER_SIZE];

        access_token_file = fopen("../accessToken.json", "r");
        fread(buffer, BUFFER_SIZE, 1, access_token_file);
        fclose(access_token_file);

        parsed_json = json_tokener_parse(buffer);
        access_token = json_object_object_get(parsed_json, "result");

        headers = curl_slist_append(headers, json_object_get_string(access_token));
        // Here we specifiy the content type for what we want to send to the api
        // in this case we want to send the metering point id in a json format
        headers = curl_slist_append(headers, "Content-Type: application/json");

        metering_point_file = fopen("../meter.json", "r");
        fread(meter_buffer, METER_SIZE, 1, metering_point_file);
        fclose(metering_point_file);

        parsed_meter = json_tokener_parse(meter_buffer);

        // Here we insert the json formatted metering id into the postfield to be used by curl
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_object_get_string(parsed_meter));

        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        // we then set the ouput place to be the tarrif file
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, tarrif_file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }

        fclose(tarrif_file);

        json_object_put(parsed_json);
        json_object_put(parsed_meter);
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }
}

// This function retrives the spotprices through
// a GET method using energidataservices api
void get_spot_prices()
{
    CURL *curl;
    CURLcode res;

    FILE *spot_prices_file;

    char buffer[5000];

    struct json_object *parsed_json;
    struct json_object *access_token;

    spot_prices_file = fopen("../spotPrices.json", "w"); // w stands for write, it replaces the old data with the new

    curl = curl_easy_init();
    if (curl)
    {
        // here we specify the api link and in this link we specify our output
        // such as the data and time from when the spotprices should start and
        // which columns from the dataset we want in our case we want the hour
        // and the price.
        curl_easy_setopt(curl, CURLOPT_URL,
                         "https://api.energidataservice.dk/dataset/Elspotprices?start=now-P0DT1H&sort=HourDK&columns=HourDK,SpotPriceDKK&filter={%22PriceArea%22:[%22DK1%22]}");
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, spot_prices_file);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            printf("curl_easy_perform() returned %s\n", curl_easy_strerror(res));
        }

        fclose(spot_prices_file);
        curl_easy_cleanup(curl);
    }
}

// This function reads the retrieved spot prices from above
// and adds them into an array called SpotPriceDKK
void readPrices_spotPrice(double *SpotPriceDKK, size_t *lengthOfArray)
{
    FILE *spotPrices_file; // Opens the spotPrices file
    char buffer[BUFFER_SIZE];

    // Here we get the access token from the json file
    json_object *parsed_json;
    json_object *records;

    size_t n_prices;
    size_t i;

    spotPrices_file = fopen("../spotPrices.json", "r");
    fread(buffer, BUFFER_SIZE, 1, spotPrices_file);
    fclose(spotPrices_file);

    parsed_json = json_tokener_parse(buffer);
    records = json_object_object_get(parsed_json, "records");
    putchar('\n');

    // here we assign n_prices the length of the array containing the spotprices
    n_prices = json_object_array_length(records);
    *lengthOfArray = n_prices;
    printf("Found %lu SpotPrices", n_prices);

    putchar('\n');

    // This for loop, loops through the records array
    // from the spotprices file which contains the spotprices
    // it then takes the spotprices and puts them into a new array
    // that we can use later to calculate total price
    for (i = 0; i < n_prices; i++)
    {
        json_object *spotPrices_temp;
        json_object *spotPrice_temp;
        spotPrices_temp = json_object_array_get_idx(records, i);
        spotPrice_temp = json_object_object_get(spotPrices_temp, "SpotPriceDKK");

        // Here we use json_object_get_double to convert the json formatted spotprice
        // to a double we then divide by 1000 to get the price in DKK
        SpotPriceDKK[i] = json_object_get_double(spotPrice_temp) / CONVERT_UNIT;
    }

    json_object_put(parsed_json); // Frees json-object from memory
}

// This function reads the retrived tarrifs from above
// and inserts them into a struct array for later usage
void readPrices_tariffs(tarrifs *price_data)
{
    FILE *tariffs_file;
    char buffer[BUFFER_SIZE];

    // The tarrifs file contains a very nested json which
    // which requires a lot of json_objects to handle
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
    fread(buffer, BUFFER_SIZE, 1, tariffs_file);
    fclose(tariffs_file);

    parsed_json = json_tokener_parse(buffer);
    result_array = json_object_object_get(parsed_json, "result");
    result_parent = json_object_array_get_idx(result_array, 0);

    result_child = json_object_object_get(result_parent, "result");
    tariffs_array = json_object_object_get(result_child, "tariffs");

    tariff_price_parent = json_object_array_get_idx(tariffs_array, 0);
    tariff_discount_parent = json_object_array_get_idx(tariffs_array, 1);
    net_tariff_transmission_parent = json_object_array_get_idx(tariffs_array, 2);
    system_tariff_parent = json_object_array_get_idx(tariffs_array, 3);
    balance_tariff_parent = json_object_array_get_idx(tariffs_array, 4);
    electricity_tax_parent = json_object_array_get_idx(tariffs_array, 5);

    tariff_prices = json_object_object_get(tariff_price_parent, "prices");
    tariff_discount = json_object_object_get(tariff_discount_parent, "prices");
    net_tariff_transmission_array = json_object_object_get(net_tariff_transmission_parent, "prices");
    system_tariff_array = json_object_object_get(system_tariff_parent, "prices");
    balance_tariff_array = json_object_object_get(balance_tariff_parent, "prices");
    electricity_tax_array = json_object_object_get(electricity_tax_parent, "prices");

    n_prices = json_object_array_length(tariff_prices);
    putchar('\n');

    // Here we get the actual prices
    for (i = 0; i < n_prices; i++)
    {
        json_object *tariffPrices_temp;
        json_object *tariffPrice_temp;
        json_object *tariffDiscounts_temp;
        json_object *tariffDiscount_temp;

        tariffPrices_temp = json_object_array_get_idx(tariff_prices, i);
        tariffPrice_temp = json_object_object_get(tariffPrices_temp, "price");
        price_data->net_tariff[i] = json_object_get_double(tariffPrice_temp);

        tariffDiscounts_temp = json_object_array_get_idx(tariff_discount, i);
        tariffDiscount_temp = json_object_object_get(tariffDiscounts_temp, "price");
        price_data->net_tariff_discount[i] = json_object_get_double(tariffDiscount_temp);
    }

    net_tariff_transmission_price = json_object_object_get(json_object_array_get_idx(net_tariff_transmission_array, 0), "price");
    system_tariff_price = json_object_object_get(json_object_array_get_idx(system_tariff_array, 0), "price");
    balance_tariff_price = json_object_object_get(json_object_array_get_idx(balance_tariff_array, 0), "price");
    electricity_tax_price = json_object_object_get(json_object_array_get_idx(electricity_tax_array, 0), "price");

    price_data->net_tariff_transmission = json_object_get_double(net_tariff_transmission_price);
    price_data->system_tariff = json_object_get_double(system_tariff_price);
    price_data->balance_tariff = json_object_get_double(balance_tariff_price);
    price_data->electricity_tax = json_object_get_double(electricity_tax_price);

    json_object_put(parsed_json);
}

// This function calculates the total price per hour along with the total tax
void total_price_calc(double *SpotPriceDKK, tarrifs *price_data, total_prices *result, size_t lengthOfArray)
{
    // Here we use a time library to get the current hour
    // firstly we create a empty time object and time struct
    time_t rawtime;
    struct tm *timeinfo;
    // we then gets the current calender time using time()
    time(&rawtime);
    // rawtime is now a bunch of seconds, therefore we use localtime()
    // to get a more human readable time which will be inserted into time struct
    timeinfo = localtime(&rawtime);
    // the time struct seperates the time into hours, minutes, days and so on
    // in our case we only want the hour so we just take the our part
    // by writing timeinfo->tm_hour.
    int current_hour = timeinfo->tm_hour;

    // Here we calculate the total tax per hour
    for (int i = 0; i < HOURS_IN_DAY; i++)
    {
        result->total_tax[i] += price_data->net_tariff[i];
        result->total_tax[i] += price_data->net_tariff_discount[i]; // The value is already negative
        result->total_tax[i] += price_data->net_tariff_transmission;
        result->total_tax[i] += price_data->system_tariff;
        result->total_tax[i] += price_data->balance_tariff;
        result->total_tax[i] += price_data->electricity_tax;
    }

    // Here we calculate the total VAT and price
    for (int i = 0; i < lengthOfArray; i++)
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

// This function is used present the prices in a nicely way
void print_prices(double *SpotPriceDKK, total_prices *result, size_t lengthOfArray)
{
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    int current_hour = timeinfo->tm_hour;

    printf("Hour \t Total-Price \t Spot-Price \t Tax-Total \t VAT \n");
    for (int i = 0; i < lengthOfArray; i++)
    {
        printf("%02d:00 %12lf %15lf %15lf %10lf\n", current_hour, result->total_price[i], SpotPriceDKK[i], result->total_tax[current_hour], result->VAT[i]);
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

void optimaltime(total_prices *result, size_t lengthOfArray, char *device)
{
    // Opens the data file for the given device, which includes the watt usage every second the device is running
    FILE *myFile = NULL;
    if (strcmp(device, "washing machine") == 0)
    {
        myFile = fopen("../washingMachine.txt", "r");
    }
    else if (strcmp(device, "dishwasher") == 0)
    {
        myFile = fopen("../dishWasher.txt", "r");
    }

    // Error check if the data file is not opened
    if (myFile == NULL)
    {
        printf("Error Reading File\n");
        exit(0);
    }

    // Reading the datafile into an array
    int i = 0,
        c = 0,
        array_length = 0;

    // Counts the amount of lines in the data file, where each line equals one second the device is running
    for (c = getc(myFile); c != EOF; c = getc(myFile))
    {
        // Counts array_length +1 when there is a newline
        if (c == '\n')
            array_length = array_length + 1;
    }
    // Sets the position at the beginning of the file again
    rewind(myFile);

    // Mallocing space for the upcoming dataset
    double *tempdata = malloc(array_length * sizeof(double));
    char *buffertemp = malloc(array_length * sizeof(char));
    char *buffer = 0;

    // Reads the data file and inputs it into an array
    for (i = 0; i < array_length; i++)
    {
        // Takes each line of the data file as a string and stores it in buffertemp
        fscanf(myFile, "%s\n", &buffertemp[i]);
        // Converts the string into a double and stores it in buffer, which puts the value in tempdata
        tempdata[i] = strtod(&buffertemp[i], &buffer);
    }

    // Closes the file
    fclose(myFile);

    int watt = 0,
        device_stoptime = 0,
        temphour = 0,
        energyprice_temphour = 0,
        device_clock = 0,
        starttime = 0,
        runningtemphour = 0,
        array_hour = 0,
        j = 0,
        finalhour = 0,
        finalminute = 0,
        finalsecond = 0,
        tomorrow = 0;

    double finalprice = 0.0,
           countingcost = 0.0,
           totalcost = UPPER_BOUND;

    // Finds the local time of the user, when starting the program
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Converts the current time to seconds, for easier upcoming calculations
    int current_seconds = timeinfo->tm_hour * HOUR_IN_SECONDS;

    // Makes a limit for the program, so it runs until we do not have information on energy prices
    int max_seconds = current_seconds + (lengthOfArray * HOUR_IN_SECONDS) - HOUR_IN_SECONDS;

    // The "main" function to find the most optimal time of use

    // The first for-loop is running through each second in the day/next day
    // It starts at the current time
    // It runs until we do not have more information on energy prices minus the amount of time it takes for the device cycle to finish

    for (; current_seconds < max_seconds - array_length; current_seconds++)
    {
        // Temphour is useful if the device cycle runs in multiple hours, because then the energy prices has to change accordingly
        temphour = j;

        // The for-loop for the device runtime
        // device_clock is how far the device is in its cycle
        // The array_length found before is how many seconds the device is running
        for (device_clock = 0; device_clock < array_length; device_clock++)
        {
            // If-statement checking if the cycle runs into the next hour
            // If it does, it counts temphour one up, which is used for the cost calculations
            if (((current_seconds + device_clock) % HOUR_IN_SECONDS) == 0)
            {
                temphour++;
            }

            // Finding the energy price for the hour the cycle is in
            double energyprice_temphour = result->total_price[temphour];

            // Counting the cost for each second the device cycle is running
            // Basically: Energy price for the hour * the watt usage at that time + the counting cost
            countingcost = energyprice_temphour * tempdata[device_clock] + countingcost;
        }

        // Counts the hour up if the current_seconds counts to a new hour
        if (((current_seconds) % HOUR_IN_SECONDS) == 0)
        {
            j++;
        }

        // Checks if the counting cost is "cheaper" than the totalcost
        // Totalcost is the running cost for the cheapest cycle
        if (countingcost < totalcost)
        {
            // Saves the cost and the time to start the cycle
            totalcost = countingcost;
            starttime = current_seconds;

            // Converts time from seconds to hours, minutes, seconds
            finalhour = current_seconds / HOUR_IN_SECONDS;
            finalminute = (current_seconds - (HOUR_IN_SECONDS * finalhour)) / MIN_IN_SECONDS;
            finalsecond = (current_seconds - (HOUR_IN_SECONDS * finalhour) - (finalminute * MIN_IN_SECONDS));

            if (finalhour >= HOURS_IN_DAY)
            {
                finalhour -= HOURS_IN_DAY;
                tomorrow = 1;
            }

            // Right now the price is calculated pr. second using the kW/h price
            // Converts the price to be correct
            finalprice = (double)countingcost / WATT_PER_SECONDS_CONVERTER_TO_KWH;
        }

        // Resets the counting cost
        countingcost = 0;
    }
    // Print the result
    if (tomorrow == 0)
    {
        printf("It is cheapest to start your %s at %d:%d:%d, and it costs %.2lf DKK.\n", device, finalhour, finalminute, finalsecond, finalprice);
    }
    else
    {
        printf("It is cheapest to start your %s at %d:%d:%d tommorrow, and it costs %.2lf DKK.\n", device, finalhour, finalminute, finalsecond, finalprice);
    }

    // Free the malloc
    free(tempdata);
    free(buffertemp);
}