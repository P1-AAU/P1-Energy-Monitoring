# Energi Price Calculator

This program is a C program that was originally built as part of a university project at Aalborg University. The project was completed at the end of the first semester.

The program uses the json-c and curl libraries to obtain energi spotprices and tariffs from two different remote servers. The spotprices are obtained from an API provided by energidataservice.dk, while the tariffs are obtained from an API provided by datahub.

In addition to retrieving and presenting this information to the user, the program also takes energy usage data from devices, such as a washing machine cycle, and uses it to search for the cheapest time to run the device. This allows users to optimize their energy usage and save money on their energy bills.

## Getting Started

To get started with the program, you will need to have cmake installed on your system. The program uses cmake to build the json-c and curl libraries when compiling, so you do not need to install them manually.

Once cmake is installed, you can compile the program using a C compiler, such as GCC.

Once the program has been compiled, you can run it and it will retrieve the spotprices and tariffs from the remote servers, calculate the prices for each hour, and present them to the user. The program also allows the user to input energy usage data and search for the cheapest time to run a device.

## License

This program is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
