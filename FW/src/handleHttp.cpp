#include "handleHttp.h"

void handleRoot() { //uvodni webovka
    Serial.println("Handle root");
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");

    String Page;
    Page += F( //nastaveni cestiny
                "<!DOCTYPE html><html lang='cz'><head>"
                "<meta name='theme-color' content='#404040' />"
                "<meta charset='UTF-8'>"
                "<link rel='stylesheet' type='text/css' href='style.css'>"
                "<meta name='viewport' content='width=device-width'>"
                "<title>Semafor manager</title></head><body>"
                "<h1>Nastavení semaforu #");
    Page += F(
            "<form method='POST' action='datasave'>"); //vytvarim formular pro data
            //po odeslani pomoci "submit" se posle vysledek na /datasave
            // /datasave je dalsi webovka, ktera prijima data
            // je zapnuta v start_server()

    Page += F("Ahoj");
    Page += 
        String(promenna_web) + F("promenna web");

    Page += F(
                "<h2>Tower Defence (3)</h2>"
                "2 módy - stavba a rozebírání zdi. Přepínání mezi nimi dlouhým stiskem tlačítka. Při stavbě zdi mačkání tlačítka postupně rozsvěcí LEDky. Při rozebírání dlouhý stisk tlačítka postupně zhasíná LEDky. Po zhasnutí všech se semafor rozbliká.<br>"
                "Doba krátkého stisku tlačítka [sekundy]:<br>"
                "<input type='text' placeholder='");
                
    Page += String(promenna_web2); //co je v te promenne, tak se mi ukaze jak by default na webu 
    Page += F(
                "' name='promenna_web2'/><br>" //nazev promenne, kterou odesila prohlizec
                "<input type='submit' name='tlacitka_1' value='Ulož a aktivuj Tower Defence'/><br>"); //vsechno se po stisku submit odesle
    // End form and page
    Page += F("</form></body></html>");

    server.send(200, "text/html", Page);
    server.client().stop();
    
}


void handleDataSave() {
    Serial.println("handlaDataSave");
    char buffer[10];
    int32_t temp;

    // server.arg("delayMin").toCharArray(buffer, sizeof(buffer) - 1); //ulozeni hodnoty promenne ze serveru, kterou jsem nastavila
    // temp = atoi(buffer); //ze stringu na cislo 
    // if(temp > 0) {
    //     stateVector.monopolyDelayMin = temp;
    // }

    // server.arg("delayMax").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.monopolyDelayMax = temp;
    // }

    // server.arg("pressShort").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.tdPressShort = temp;
    // }

    server.arg("promenna_web2").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer); //string na cislo
    if(temp > 0) {
        promenna_web2 = temp;
        Serial.println(promenna_web2);
    }

    // server.arg("holdToGet_press").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.holdToGetTimeout = temp;
    // }

    // server.arg("minutkaTimeMinAll").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.minutkaTimeSecAll = temp*60;
    // }

    // server.arg("mikrovlnkaTimeMinAll").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.mikrovlnkaTimeSecAll = temp*60;
    // }

    // server.arg("brightness").toCharArray(buffer, sizeof(buffer) - 1);
    // temp = atoi(buffer);
    // if(temp > 0) {
    //     stateVector.ledBrightness[0] = temp;
    // }
    
     if(server.hasArg("tlacitka_1")) //podle zmacknuteho tlacitka mohu detekovat, kterou hru chci hrat a pak z ni vzit dane promenne
         Serial.println("tlacitko 1");
    // else if(server.hasArg("vabicka"))
    //     stateVector.gameMode = 1;
    // else if(server.hasArg("vlajky"))
    //     stateVector.gameMode = 2;
    // else if(server.hasArg("towerDefence"))
    //     stateVector.gameMode = 3;
    // else if(server.hasArg("holdToGet"))
    //     stateVector.gameMode = 4;
    // else if(server.hasArg("minutka"))
    //     stateVector.gameMode = 5;
    // else if(server.hasArg("mikrovlnka"))
    //     stateVector.gameMode = 6;

    // stateVector_eeprom.write();

//odkazovani na jakou stranku se vratit apod - nutna nastaveni serveru
    server.sendHeader("Location", "/", true); //po nastaveni parametru se vrat na domovskou stranku = handleRoot
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(302, "text/plain", "");    // Empty content inhibits Content-length header so we have to close the socket ourselves.
    //302 = presmerovani 
    server.client().stop(); // Stop is needed because we sent no content length
}