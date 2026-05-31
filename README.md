NAUDOJIMOSI INSTRUKCIJA
1) Atidarykite terminala projekto kataloge.
2) Sugeneruokite build failus i build aplanka:
```
cmake -S . -B build
```
3) Sukompiliuokite programa:
```
cmake --build build
```
4) Ieikite i aplanka kuriame yra .exe failai:
```
cd build
cd debug
```
5) Paleiskite norima programa:
```
./main
```
arba
```
./testai
```

TEKSTAS

Tekstas paimtas iš vikipedijos apie žemę https://lt.wikipedia.org/wiki/%C5%BDem%C4%97

VEIKIMAS

Programa atidaro `tekstas.md` ir skaito failą eilutė po eilutės.
Kiekvienoje eilutėje:


Pirmiausia randami URL'ai.
Išgauti URL'ai pašalinami.
Eilutė valoma nuo Markdown konstrukcijų su `stripMarkdown`.
Tada eilutė skaidoma į žodžius. Kiekvienas žodis apdorojamas su `punctuation()` (šalinami brūkšniai ir specialūs simboliai) ir ASCII raidės mažinamos su `toLower()`.
Programa skaičiuoja kiek kartų žodis pasikartoja ir išsaugo eilutės numerius, kuriuose žodis pasirodė.


Išvedama:


Pasikartojantys žodžiai su kiekiais ir eilutėmis į `zodziai.txt`.
Visi rasti URL'ai jų aptikimo tvarka į `url.txt`.



