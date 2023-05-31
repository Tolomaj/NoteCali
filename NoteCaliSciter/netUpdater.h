#include <curl/curl.h>
#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

size_t writeFunction(void* ptr, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)ptr, size * nmemb);
    return size * nmemb;
}

int translateToCurlError(int error) {
    return (0 - error) + 4; // otoèení hodnot zpet do ladna
}

int checkForUpdate() { //return 0 version unactual / 1 vesrion actual / -hodnoty curl err(cant say actuality)
    // CurlCodes jsou v + v rozmezí 0 - 100
    CURL* curl = curl_easy_init();
    std::string response_string = "";
    if (curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://notecali.zironet.cz/version");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);

        CURLcode response = curl_easy_perform(curl);
        if (response != CURLE_OK) {      //error vith geting update
            debugLOG(string("CURL:VERISON_CHECK curl_easy_operation() failed :") + string(curl_easy_strerror(response)));
            curl_easy_cleanup(curl);
            return ((0 - response) - 4); //otoèení czurl erroru do záporna
        }
        debugLOG("CURL_OK");
        debugLOG(response_string);
    } else {
        curl_easy_cleanup(curl);
        return -4;
        // error cant load curl
    }
    curl_easy_cleanup(curl);


    if (response_string.substr(0, 14) == "NoKa_Version:\n") { //check version file integrity
        try {
            double actVer = stof(response_string.substr(14, response_string.length())); // get actial version
            if ((float)actVer <= (float)VERSION) {
                return 1;
            }
            else {
                return -1;
            };
        }
        catch (exception& err) {
            return -3;
        }
    }
    else {
        return -2;
    }



}