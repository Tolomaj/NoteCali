<h1 align="center">NoteCali</h1>

# Úvodem

![NoteCaliBTH](https://github.com/Tolomaj/NoteCali/assets/59420562/86f6d0a8-53f7-4f80-8fcf-d44d5821090f)



Lepší aplikace Kalkulačky.\
Protože ta ve windows stojí za prd :)

Nejnovější portable verzi i instaler lze najít na stránkách [NoteCali](https://notecali.zironet.cz/).\
Stejně tak návod jak nastavit spouštení pomocí funkčního tlačítka pc.

### Cíle kalkulačky jsou:
- Lepší alternativa k defaultní kalkulačce.
- Rychlá spustitelnost.
- Více linek které jsou využitelné k počítání výsledků.
- Vysoká nastavitelnost.
- Spoustu zbytečných ale využitelných fičur.

# Použití kalkulačky
V horní liště kalkulačky lze najít v pořadí, Otevření nasavení, Připnutí nad všechny ostatní aplikace, Minimalizování a zavření aplikace.\
Poté klakulačka obsahuje dvě pole, Pole pro zadávání a pole pro výsledky. Do zadávacího pole zadáváme příklady které se vypočítají a zapišou do pole pro výsledky, které nejdou označovat ale kliknutm se uloží do clipboardu.

## Počítání v kalkulačce

### Základní počty a funkce

#### Základní počítání
```
5+5                   | 10
5-5                   |  0
5/2+2                 | 4.5
5/0                   | ⚠️
5*(5+5)+2             | 52
2*(5*(5+5)+2          | 104
```
Základní počty závorky atd.\
Všchny operátory lze najít v nastavení v sekci ***Interface***\
Pokud v nastavení je zaplé ***Parenthesis corection*** budou doplněné chybějící závorky na konec řádku 2*(2+2  ***)*** -> 2*(4) -> 8.

#### Funkce 

```
sin(50)+5 | 5.7660444
abs(-5)   |      5
```
Základní počty závorky atd.\
Pokud v nastavení je zaplé ***Parenthesis corection*** i zde budou doplněny závorky na konci linky.\
Všechny funkce jsou též v nastavení v sekci ***Interface***


### Speciální simboly a příkazy

#### Odkaz na řádek
```
4*2                   | 8 
5*5                   | 25
@0                    | 8
@0 + @1               | 33
```
@[číslo] bude nahrazeno výsledkem daného řádku na který odkazuje.\
Řádky jsou číslované od 0\
V minulích verzích se používal znak € místo @. (jen aby nebyli zmatky)

#### Proměnné
```
n=5     | 📝5
m=3     | 📝3
5+n     |  10
n=4     | 📝4
n+m     |  7
```
No proměné s názvem zadaného před = se uloží hodnota výsledku linky.\
Tuto proměnou lze použít kdekoli v programu.\
Pokud se bude název proměné opakovat její hodnota se přepíš.

#### Zadávání čísel jiných než desítkových soustav
```
#FF      | 255
&101 + 4 |  9
```
Čísla před kterými je & jsou převedena z binárních na desítková.\
To stejné platí pro # jen zde jsou převáděna čísla 16tková.


#### Speciální příkazy
```
5       | 5
3       | 3
;sum    | 8
;randi  | 16827
;rand   | 0.119083
;randn  | -0.898984

;clear -> vymaže vstupní pole!
```
Speciální příkazy mají na začátku linky ; .\
Nelze u nich používat modifikátory linek.\
;rand je od 0 - 1\
;randn je od -1 - 1
> :information_source: Do budoucna bych rád přeďelal sum,rand,randi.randn jako proměné spíš než příkazy. Ale to je budoucnost ;)



### Modifikátory linek
#### Komentáře
```
4*2                   | 8 
c;Toto je komentář    |
c;6*2                 |
5+5                   | 10
;sum                  | 18
```
Vše co je před prvním ; je nastavení linky.\
Pokud nastavení linky obsahuje c výsledek je skrytý, nedá se na něj odkazovat a ani se nazapočítává při použití ;sum

#### Zobrazení desetiných míst
```
r3;5.5555555     | 5.556
r2; 5+ 2,255     | 7.26
@0+5  	         | 10.5555555
r1;@2            | 10.6
```
Dalším parametrem co lze před ; dát je r, to udává kolik desetiných míst bude zobrazeno.\
Čislo které linka reprezentuje bude nejpřesnější možné. Když se tedy budeme na linku odkazovat bude použito nejpřesnější číslo.
> :information_source: V nastavení musí být zapnuté ***(Dont round pointers)***.

Číslo po kliknutí zkopírované ovšem bude zkopírováno zaokrouhlené.
> :information_source: Plánováné je nastavení ***copy without rounding*** které umožní kopírování nezaokrouhlených čísel.


#### Zobrazení čísel v jiných soustavách
```
b2;10   | 1010
b8;8    |  10
b16;159 |  9F
```
Posledním parametrem co lze před ; dát je b, Které nastaví zobrazení čísla v dané číselné soustavě.\

#### Míchání parametrů
```
r3b2; 10.64  | 1010.101
cr2;10.5     |
```
Parametry lze použít v kombinaci i když kombinovat cokoli s c nedává moc smysl ;)



# Info k repozitáři
Repozitář je hlavně pro mě abych mohl sinchronizovat projekt mezi více počitači, obsahuje tudíž i poznámky a soubory spojené s vývojem.\
Aplikaci však chci držet open source a co v nejlepším stavu co zvládnu.\
**Pokud tudíž najdete nějaký bug nebo chybu popřípadě nejaký napdac co přidat do kódu, ocením když mi napíšete ;).**
> :warning: Projekt měl být uložený jako Vysual Studio Project bohužel jak je projekt na dvou počítáčích, nastavení projektu se nepřenáší tudíž nestačí jen stáhnout repozitář a upravovat. Do budnoucna snad budu řešit.
### License
Upravený ani neupravený repozitář žádným způsobem není dovoleno pordávat ani nijak jinak zpeňěžovat.






