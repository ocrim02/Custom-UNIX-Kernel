- FEHLER: ohne , &h) gibt &p 0x0 aus -> do not get it so far xD
- %p zeigt bei keinem argument "0x" -> counts as invalid use of function kprintf(), done
- fehler manche nehmen das letzte argument wieder wenn sie kein neues bekommen -> no they read the next register entry (can be anything), done
	
# sollwert für fehler cases ?
	
- "" geht nicht -> " will be removed by compiler when used within a string
- " geht auch nicht -> " will be removed by compiler when used within a string
- bei \\n kommt \n als ausgabe -> \ is interpreted as start of a command "\\" is one char + '\0'
- 32 bit unsigned int geht über 429.496.729 -> 32 bit unsigned is 0 to 4.294.967.295
- 8 Feld breit mit 0x oder ohne 0x bei %p ?

## % ohne Argumente: -> not solvable so far, Done
- Kein Fehler oder Abbruch
- bei %c kommt ein Zeilen Umbruch
- bei %, %s kommt nichts in die variable
- Bei %x,i,u,p steht irgendwas 

## Variablen aber unzureichend viele Argumente: kein Fehler oder Abbruch -> not solvable so far, Done
- Bei %c unbekanntes Zeichen in der variable
- Bei %s nichts 
- Bei %x,u,i,p bekannte Zeichen in der variable

## Bei falschen Argumententypen: -> Fehler vom Programmierer. Done
- Kein Abbruch oder Fehler 
- Bei s steht nichts beim Rest steht was 

- Teste Symbole: siehe notes!!!!!!

## 0 cases: -> nothing should happen (NULL is not a character), done
- %c mit 0 eingesetzt soll NULL Sein (unklar was NULL für Auswirkungen in der Ausgabe hat) 

## Große unsigned mit Werte: -> cannot reproduce this error with kprintf("%u", (unsigned int) 429496729)
- Aus 429496729 wird 333 obwohl es noch im 32Bit Bereich sein sollte 
> habe in deinen tests gesehen, dass du 4294967629 nutzt und nicht 429496729, wie oben angegeben
> 4294967629 is größer 32 bit

## Aufruf von %8 und %08 mit zu großer Variable (len > 8): -> should be done
- Bis auf bei x wird die Variable (len > 8 ausgeschrieben)

## Fehler bei %8p mit 60x… bei einem Aufruf 
- %08u nimmt seine Werte nicht an -> do not understand what you mean
- %08i sind mit Minuszeichen 9 Blöcke in Benutzung  -> should be done

- Fehler nur bei %06 aber nicht bei %7 oder so
