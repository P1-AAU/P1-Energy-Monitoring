#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 30

typedef struct
{
    char name[30];
    double standby_watt;
    double running_watt;
    int amounts;

} device;

// prototypes
void fill_device(FILE *source, device *devices);
void print_devices(device *devices);

int main()
{
    FILE source;
    device *devices;

    devices = malloc(8 * sizeof(device));

    fill_device(&source, devices);
    print_devices(devices);

    free(devices);
    return 0;
}

void fill_device(FILE *source, device *devices)
{
    source = fopen("../device.txt", "r+");

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