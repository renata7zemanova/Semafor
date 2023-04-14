#include "handleHttp.h"

void handleRoot() { //uvodni webovka
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");

    String Page;
    Page += F("Ahoj");

    server.send(200, "text/html", Page);
    server.client().stop();
    
}