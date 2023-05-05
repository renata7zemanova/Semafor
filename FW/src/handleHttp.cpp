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
                "<h1>Konfigurační menu Univerzálního modulu pro podporu týmových her");
    Page += F( //novy formular pro tlacitko pro konfiguraci - upload mode
            "<form method='POST' action='datasave'>"); //vytvarim formular pro data
            //po odeslani pomoci "submit" se posle vysledek na /datasave
            // /datasave je dalsi webovka, ktera prijima data
            // je zapnuta v start_server()

    //Page += F("Ahoj");
    //Page += 
    //    String(promenna_web) + F("promenna web");

    Page += F(
                "<h2>Odpočítávadlo</h2>"
                //"2 módy - stavba a rozebírání zdi. Přepínání mezi nimi dlouhým stiskem tlačítka. Při stavbě zdi mačkání tlačítka postupně rozsvěcí LEDky. Při rozebírání dlouhý stisk tlačítka postupně zhasíná LEDky. Po zhasnutí všech se semafor rozbliká.<br>"
                "Doba odpočtu [sekundy]:<br>"
                "<input type='text' placeholder='");
                
    Page += String(s_vect.odpocitavadlo_timeout); //co je v te promenne, tak se mi ukaze jak by default na webu 
    Page += F(
                "' name='timeout_odpocitavadlo'/><br>" //nazev promenne, kterou odesila prohlizec
                "<input type='submit' name='tlacitko_odpocitavadlo' value='Ulož a aktivuj Odpočítávadlo'/><br>"); //vsechno se po stisku submit odesle
    
     Page += F(
                "<h2>Vábnička1</h2>"
                "Počet hrajících týmů:<br>"
                "<input type='text' placeholder='");
    Page += String(s_vect.vabnicka_num_of_teams);
    Page += F(
                "' name='num_of_teams_vabnicka'/><br>" 
                "<input type='submit' name='tlacitko_vabnicka1' value='Ulož a aktivuj Vábničku1'/><br>"); 
    
    Page += F(
                "<h2>Vábnička2</h2>");
    Page += F(
                "<input type='submit' name='tlacitko_vabnicka2' value='Ulož a aktivuj Vábničku2'/><br>"); 
    

    Page += F(
                "<h2>Pán hory</h2>"
                "Počet hrajících týmů:<br>"
                "<input type='text' placeholder='");
    Page += String(s_vect.pan_hory_num_of_teams);
    Page += F(
                "' name='num_of_teams_pan_hory'/><br>" 
                "<input type='submit' name='tlacitko_pan_hory' value='Ulož a aktivuj Pán hory'/><br>"); 
    
    Page += F(
                "<h2>Semafor</h2>");
                //"Počet hrajících týmů:<br>"
                //"<input type='text' placeholder='");
    Page += F(
                //"' name='num_of_teams_pan_hory'/><br>" 
                "<input type='submit' name='tlacitko_semafor' value='Ulož a aktivuj Semafor'/><br>"); 
    

    // End form and page
    Page += F("</form></body></html>");

    server.send(200, "text/html", Page);
    server.client().stop();
    
}


void handleDataSave() {
    Serial.println("handleDataSave");
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

    server.arg("timeout_odpocitavadlo").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer); //string na cislo
    if(temp > 0) {
        s_vect.odpocitavadlo_timeout = temp;
        //tady rovnou ulozit do eprrom = preferences
        Serial.println(s_vect.odpocitavadlo_timeout);
    }

    server.arg("num_of_teams_vabnicka").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer); //string na cislo
    if(temp > 0) {
        s_vect.vabnicka_num_of_teams = temp;
        Serial.println(s_vect.vabnicka_num_of_teams);
    }

    server.arg("num_of_teams_pan_hory").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer);
    if(temp > 0) {
        s_vect.pan_hory_num_of_teams = temp;
        Serial.println(s_vect.pan_hory_num_of_teams);
    }

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
    
     if(server.hasArg("tlacitko_odpocitavadlo")){ //podle zmacknuteho tlacitka mohu detekovat, kterou hru chci hrat a pak z ni vzit dane promenne
        Serial.println("tlacitko odpocitavadlo");
        s_vect.game = ODPOCITAVADLO;
        //game = ODPOCITAVADLO;
    }
    else if(server.hasArg("tlacitko_vabnicka1")){
        Serial.println("tlacitko vabnicka1");
        s_vect.game = VABNICKA1;
    }
    else if(server.hasArg("tlacitko_vabnicka2")){
        Serial.println("tlacitko vabnicka2");
        s_vect.game = VABNICKA2;
    }
    else if(server.hasArg("tlacitko_pan_hory")){
        Serial.println("tlacitko pan hory");
        s_vect.game = PAN_HORY;
    }
    else if(server.hasArg("tlacitko_semafor")){
        Serial.println("tlacitko semafor");
        s_vect.game = SEMAFOR;
    }

    //ukladat do Preferences

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