#!/usr/bin/python3

registrace = open("registrace.csv", 'r')
vysledky = open("vysledky.csv", 'r')

celkove = []
for radek_vysledky in vysledky:
    vysledek = radek_vysledky.strip().split(';')

    for radek_registrace in registrace:
        ucastnik = radek_registrace.strip().split(';')
        if ucastnik[0] == vysledek[2]:
            vysledek_ucastnik = vysledek + ucastnik
            # print(vysledek_ucastnik)
            celkove.append(vysledek_ucastnik)
            registrace.seek(0)
            break

registrace.close()
vysledky.close()

# print(celkove)

vysledky_kategorie = {}

for radek in celkove:
    vysledky_kategorie[radek[8]] = []

for radek in celkove:
    vysledky_kategorie[radek[8]].append(radek)

for kategorie in vysledky_kategorie.keys():
    print("Kategorie: %s" % kategorie)
    print("Poradi;Startovni cislo;Cas;Jmeno;Prijmeni;Rocnik;Kategorie")
    for vysledek_kategorie in vysledky_kategorie[kategorie]:
        print("%s;%s;%s;%s;%s;%s;%s" % 
            (vysledek_kategorie[0],
            vysledek_kategorie[2],
            vysledek_kategorie[1],
            vysledek_kategorie[5],
            vysledek_kategorie[6],
            vysledek_kategorie[7],
            vysledek_kategorie[8]
            )
        )

    print()
