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
                "<h2>Konfigurační menu Univerzálního modulu pro podporu týmových her");
    Page += F( //novy formular pro tlacitko pro konfiguraci - upload mode
            "<form method='POST' action='datasave'>"); //vytvarim formular pro data
            //po odeslani pomoci "submit" se posle vysledek na /datasave
            // /datasave je dalsi webovka, ktera prijima data
            // je zapnuta v start_server()

    Page += F(
                "<h2>Odpočítávadlo</h2>"
                "Hra je odstartována zmáčknutím středového tlačítka. Tím je spuštěn odpočet času, který byl předán tomuto módu jako parametr. V tuto chvíli všechny LED svítí zelenou barvou a postupně, po uplynutí každé dvanáctiny času, se přepne jedna LED na červenou. Pokud zbývá z nastaveného času poslední minuta, tak se všechny dosud zeleně svítící LED přepnou do žluté barvy a blikají. Po uplynutí nastaveného času všechny LED svítí červeně. Po stisku středového tlačítka je odpočet znovu nastaven a celá funkčnost se opakuje.<br><br>");
    Page += F(
                "Doba odpočtu [minuty]: "
                "<input type='text' placeholder='");
                
    Page += String(s_vect.odpocitavadlo_timeout); //co je v te promenne, tak se mi ukaze jak by default na webu 

    Page += F(
                "' name='timeout_odpocitavadlo'/><br>" //nazev promenne, kterou odesila prohlizec
                "<input type='submit' name='tlacitko_odpocitavadlo' value='Ulož a aktivuj Odpočítávadlo'/><br>"); //vsechno se po stisku submit odesle
    
     Page += F(
                "<h2>Vábnička</h2>"
                "Po prvním stisku se náhodně rozsvítí jedna z možných barev. Pokud je náhodnost zakázána, tak se po každém stisku středového tlačítka rozsvítí Univerzální modul barvou, která v sekvenci následuje. Pokud je náhodnost povolena, tak se Univerzální modul rozsvítí náhodně vygenerovanou barvou, ne však tou, kterou svítí. <br>"
                "Počet barev: "
                "<input type='text' placeholder='");
    Page += String(s_vect.vabnicka_num_of_colors);

    Page += F(
                 "' name='num_of_colors_vabnicka'/><br>" 
                 "Je mezi barvami černá? ano = 1, ne = 0: "
                "<input type='text' placeholder='");
    Page += String(s_vect.vabnicka_is_black);

    Page += F(
                 "' name='is_black_vabnicka'/><br>" 
                 "Je povolená náhodnost? ano = 1, ne = 0: "
                "<input type='text' placeholder='");
    Page += String(s_vect.vabnicka_is_random);

    Page += F(
                "' name='is_random_vabnicka'/><br>" 
                "<input type='submit' name='tlacitko_vabnicka' value='Ulož a aktivuj Vábničku'/><br>"); 
    Page += F(
                "<h2>Klasický semafor</h2>"
                "Na začátku hry se rozsvítí všechny LED zelenou nebo červenou barvou. Následně je nastaven odpočet času, který je náhodný v rozmezí minimálního a maximálního času změny. Po uplynutí tohoto času svítí na polovině LED žlutá barva. Na druhé polovině zůstává původní barva. Tento stav trvá 2 sekund. Následně se všechny LED přepnout do opačné barvy než byla původní. Pokud tedy v prvním stavu svítily všechny LED červeně, tak nyní budou svítit zeleně a naopak. Tato funkce se neustále opakuje.<br><br>"
                "Minimální čas [sekundy]: "
                "<input type='text' placeholder='");
    Page += String(s_vect.semafor_min_timeout);

    Page += F(
                 "' name='min_timeout_semafor'/><br>" 
                 "Maximální čas [sekundy]: "
                "<input type='text' placeholder='");
    Page += String(s_vect.semafor_max_timeout);

    Page += F(
                "' name='max_timeout_semafor'/><br>" 
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

    server.arg("timeout_odpocitavadlo").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer); //string na cislo
    if(temp > 0) {
        s_vect.odpocitavadlo_timeout = temp;
        Serial.println(s_vect.odpocitavadlo_timeout);
    }

    server.arg("num_of_colors_vabnicka").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer); 
    if(temp > 0) {
        s_vect.vabnicka_num_of_colors = temp;
        Serial.println(s_vect.vabnicka_num_of_colors);
    }

    server.arg("is_black_vabnicka").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer);
    if(temp > 0) {
        s_vect.vabnicka_is_black = temp;
        Serial.println(s_vect.vabnicka_is_black);
    }

    server.arg("is_random_vabnicka").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer);
    if(temp > 0) {
        s_vect.vabnicka_is_random = temp;
        Serial.println(s_vect.vabnicka_is_random);
    }

    server.arg("min_timeout_semafor").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer);
    if(temp > 0) {
        s_vect.semafor_min_timeout = temp;
        Serial.println(s_vect.semafor_min_timeout);
    }

    server.arg("max_timeout_semafor").toCharArray(buffer, sizeof(buffer) - 1);
    temp = atoi(buffer);
    if(temp > 0) {
        s_vect.semafor_max_timeout= temp;
        Serial.println(s_vect.semafor_max_timeout);
    }

     if(server.hasArg("tlacitko_odpocitavadlo")){
        Serial.println("tlacitko odpocitavadlo");
        s_vect.game = ODPOCITAVADLO;
    }
    else if(server.hasArg("tlacitko_vabnicka")){
        Serial.println("tlacitko vabnicka");
        s_vect.game = VABNICKA;
    }
    else if(server.hasArg("tlacitko_semafor")){
        Serial.println("tlacitko semafor");
        s_vect.game = SEMAFOR;
        
    }

    upload_permanently_pref();

    //odkazovani na jakou stranku se vratit apod - nutna nastaveni serveru
    server.sendHeader("Location", "/", true); //po nastaveni parametru se vrat na domovskou stranku = handleRoot
    server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    server.sendHeader("Pragma", "no-cache");
    server.sendHeader("Expires", "-1");
    server.send(302, "text/plain", "");    
    //302 = presmerovani 
    server.client().stop(); 
}

void handleStyle() { //vytvori vzhled stranky
    server.send(200, "text/css", style);
    server.client().stop(); 
}