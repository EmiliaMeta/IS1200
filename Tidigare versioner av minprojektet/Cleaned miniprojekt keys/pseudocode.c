//testfunktion
/*
Vad behövs i koden?
- en funktion för start/initiering
- en funktion för alla textmeddelanden i terminalen
- en funktion för battle system
- en funktion för rummen
-
*/

/* PSEUDOKOD

STRUKTUR:
main.c
 ├── io.h
 ├── map.h
 │    ├── objects.h
 │    └── systems.h
 │         ├── textstring.h
 │         ├── io.h
 │         └── random.h
 ├── textstring.h
 └── random.h

 
 start(){
    init_hardware()
    init_player()
    init_map()
    init_items()
 }

 textstring()

 player()

 map(player)

 gameloop()

 catchsys()

 fightsys()

 shopsys()

 creaturesys()

 gameover(){
    if win{
    return 0;

    if lose{
    return 1;
 
 end()

 main(void){

    start(){
    }
    while(1){
        gameloop{
            map(player){
            return location;
            }
            update_game_state(input):
                location = map(player, input)
                if event_in(location) == CATCH:
                    catchsys(location)
                if event_in(location) == FIGHT:
                    fightsys(location)
                if event_in(location) == SHOP:
                    shopsys()

            }
            if(gameover()){
                break;
            }
        }
    }
    end(){
    }
 }


*/


    //fightsys:
/* STEN SAX PÅSE
        Sten slår sax
        Sax slår påse
        Påse slår sten.
    
    Vunnen fight ger 1 coin
    
    Win:
        Händer när motståndaren når 0HP 
        Ger 1 coin
    
    Lose:
        Händer när alla spelarens creature når 0HP.
        Ger game over

    Turnbased, player får förtur. 
        På spelarens turn:
            Välj antingen slå med current creature eller byt till annan creature
        På motståndarens turn:
            Slå

    Vid attack:
        Effektiv typ kombination gör 2 skada 
            ex: sten slår sax för 2 HP
        Övrig typ kombination gör 1 skada
            ex: sax slår sten för 1 HP
    "logiken blir att slåss för att sammla pengar för att kunna fånga creatures"
    - toggle creatures->switchar
    slutbossen: 
    - som vanlig creature fast mer hp, desto mer creatures samlade desto bättre, fler chanser att slås
    miniboss
*/
//catch sys: fångar creatures, om fångat 4 st så får den inte fånga mer
//3 typer av pokemon boll: brons: 25% catcatch rate, silver:50% catch rate, guld: 100% catchrate
//randomisar chatchen brons och silver 
//shopsytem, en switch för att kolla hur mycket pengar man har, print på saldo, hur mycket man har kvar 
//ha med i/o grejer så om tex switch 1 high så ska den fighta med en creature 
