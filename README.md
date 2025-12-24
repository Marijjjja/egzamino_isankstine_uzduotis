# Projekto aprašymas
Šio projekto tikslas – praktiškai ištirti asociatyvių konteinerių (map, unordered_map, set, unordered_set) taikymą teksto analizės užduotims. Projektas realizuotas C++ kalba ir skirtas parodyti, kodėl tokio tipo konteineriai yra efektyvesni už sekos konteinerius (pvz., vector) situacijose, kai dažnai atliekama paieška, skaičiavimas ir grupavimas.

Programa analizuoja tekstinį failą ir atlieka šias užduotis: <br>
- suskaičiuoja, kiek kartų kiekvienas skirtingas žodis pasikartoja tekste;
- išveda tik tuos žodžius, kurie pasikartojo daugiau nei vieną kartą;
- sukuria cross-reference tipo lentelę, kurioje nurodomos teksto eilutės, kuriose kiekvienas žodis buvo paminėtas;
- suranda ir išveda tekste esančius URL adresus.

## Funkcionalumas
**Žodžių skaičiavimas** <br>

- Tekstas skaitomas eilutėmis.
- Žodžiai normalizuojami: ignoruojami skyrybos ženklai, raidės verčiamos į mažąsias.
- Naudojamas asociatyvus konteineris map<string, int>, kuris leidžia efektyviai skaičiuoti pasikartojimus.

**Cross-reference lentelė** <br>

- Kiekvienam žodžiui saugomi teksto eilučių numeriai, kuriose jis pasirodo.
- Naudojama struktūra map<string, set<int>>, užtikrinanti, kad eilučių numeriai nesikartotų.

**URL paieška** <br>

- URL adresai randami naudojant reguliariąsias išraiškas (regex).
- Atpažįstami tiek pilni (http://, https://), tiek sutrumpinti (www., domenas + TLD) URL formatai.
- Papildomai naudojamas unordered_set galimų TLD patikrinimui.

## Kodėl naudoti asociatyvūs konteineriai

Užduotyje sąmoningai atsisakyta strategijų, paremtų: <br>
- visų elementų perėjimu ir tiesine paieška;
- rikiavimu po kiekvieno įterpimo.

Asociatyvūs konteineriai leidžia: <br>
- automatiškai tvarkyti duomenis įterpimo metu;
- greitai rasti ir atnaujinti reikšmes;
- aiškiai ir natūraliai realizuoti žodžių dažnio bei cross-reference užduotis.

## Projekto struktūra
- main.cpp – programos vykdymo pradžia
- functions.cpp – pagrindinė logika (teksto analizė, URL paieška, išvedimas)
- header.h – funkcijų ir struktūrų deklaracijos
- get_text.sh – pagalbinis skriptas teksto gavimui
- get_domain.sh – pagalbinis skriptas domenų apdorojimui

## Išvados
Projektas parodo, kad asociatyvūs konteineriai yra tinkamas ir efektyvus pasirinkimas teksto analizės užduotims, kai svarbu greita paieška, 
skaičiavimas ir duomenų grupavimas. Sprendimas atitinka užduoties reikalavimus ir demonstruoja praktinį C++ standartinės bibliotekos konteinerių pritaikymą.
