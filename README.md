# Překladač imperativního jazyka IFJ18

## Řešitelé

Tomáš Zálešák, Tomáš Smädo, Vojtěch Novotný, Robin Skaličan

## Použití

make - překlad

make clean - vyčistí

make test - spustí testy /tests/our_tests/*.src
 - vypíše spoustu informací:
    - jméno souboru
    - zdroják
    - ukázkový výstup pravého ruby
    - ukázkovou return hodnotu pravého ruby
    - náš ifjcode output
    - náš output
    - zvlášť naši return hodnotu a naši return hodnotu z generování
 - přidávejte nové, upravujte, experimentujte..

make exampletest - stejný jako make test, ale jen pro jeden soubor /tests/our_tests/example.src
 - vhodné pro rychlou kontrolu a úpravu example.src

make schooltest - zatím není pro nás ://, je třeba generovat vestavěné funkce
