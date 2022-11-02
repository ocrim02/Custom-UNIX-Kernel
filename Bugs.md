- FEHLER: ohne , &h) gibt &p 0x0 aus 
- %p zeigt bei keinem argument "0x"
- fehler manche nehmen das letzte argument wieder wenn sie kein neues bekommen
	
# sollwert für fehler cases ?
	
- "" geht nicht 
- " geht auch nicht
- bei \\n kommt \n als ausgabe
- 32 bit unsigned int geht über 429496729
- 8 Feld breit mit 0x oder ohne 0x bei %p ?

- % ohne Argumente:
- Kein Fehler oder Abbruch
- bei %c kommt ein Zeilen Umbruch
- bei %, %s kommt nichts in die variable
- Bei %x,i,u,p steht irgendwas 

- Variablen aber unzureichend viele Argumente: kein Fehler oder Abbruch 
- Bei %c unbekanntes Zeichen in der variable
- Bei %s nichts 
- Bei %x,u,i,p bekannte Zeichen in der variable

## Bei falschen Argumententypen:
- Kein Abbruch oder Fehler 
- Bei s steht nichts beim Rest steht was 

- Teste Symbole: siehe notes!!!!!!

## 0 cases: 
- %c mit 0 eingesetzt soll NULL Sein (unklar was NULL für Auswirkungen in der Ausgabe hat) 

## Große unsigned mit Werte: 
- Aus 429496729 wird 333 obwohl es noch im 32Bit Bereich sein sollte 

- Aufruf von %8 und %08 mit zu großer Variable (len > 8):
- Bis auf bei x wird die Variable (len > 8 ausgeschrieben)

- Fehler bei %8p mit 60x… bei einem Aufruf 
- %08u nimmt seine Werte nicht an 
- %08i sind mit Minuszeichen 9 Blöcke in Benutzung

- Fehler nur bei %06 aber nicht bei %7 oder so
