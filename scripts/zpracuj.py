#!/usr/bin/python3

import getopt
import sys

registrace = open("registrace.csv", 'r')
vysledky = open("vysledky.csv", 'r')

celkove = []
casy = []
chybi = []

# radek_vysledky[0] = "Startovni cislo"
# radek_vysledky[1] = "Jmeno"
# radek_vysledky[2] = "Prijmeni"
# radek_vysledky[3] = "Rocnik"
# radek_vysledky[4] = "Typ"
# radek_vysledky[5] = "Pohlavi"
# radek_vysledky[6] = "Kategorie"

# celkove = radek_vysledky + [ poradi, cas]

# celkove[7] = "Poradi"
# celkove[8] = "Cas"


for radek_registrace in registrace:
    ucastnik = radek_registrace.strip().split(';')[0:7]
    celkove.append(ucastnik + [0, ""])


for radek_vysledky in vysledky:
    vysledek = radek_vysledky.strip().split(';')

    for i in range(0, len(celkove)):
        if celkove[i][0] == vysledek[2]:
            celkove[i][7] = int(vysledek[0])
            celkove[i][8] = vysledek[1]
#             print(celkove[i])
            # break

    registrace.seek(0)
    for radek_registrace in registrace:
        ucastnik = radek_registrace.strip().split(';')
        if ucastnik[0] == vysledek[2]:
            vysledek_ucastnik = ucastnik[0:7] + [vysledek[0], vysledek[1]]
#             print(vysledek_ucastnik)
            casy.append(vysledek_ucastnik)
    

registrace.close()
vysledky.close()

# print(celkove)

# sys.exit(1)

def _poradi(vysledek):
    return vysledek[7]

celkove_registrace = list(celkove)
celkove.sort(key=_poradi)
# print(celkove)

# for radek_vysledky in vysledky:
#     vysledek = radek_vysledky.strip().split(';')
# 
#     for radek_registrace in registrace:
#         ucastnik = radek_registrace.strip().split(';')
#         if ucastnik[0] == vysledek[2]:
#             vysledek_ucastnik = vysledek + ucastnik
#             print(vysledek_ucastnik)
#             celkove.append(vysledek_ucastnik)
#     registrace.seek(0)

# print(celkove)

vysledky_kategorie = {}
# 
# vysledky_kategorie["pes"] = []
for radek in celkove:
    vysledky_kategorie[radek[6]] = []

# 
# for radek in celkove:
#     if radek[11] == "pes":
#         vysledky_kategorie["pes"].append(radek)
#     else:
#         vysledky_kategorie[radek[8]+radek[10]+radek[11]].append(radek)
# 

# radek_vysledky[0] = "Startovni cislo"
# radek_vysledky[1] = "Jmeno"
# radek_vysledky[2] = "Prijmeni"
# radek_vysledky[3] = "Rocnik"
# radek_vysledky[4] = "Typ"
# radek_vysledky[5] = "Pohlavi"
# radek_vysledky[6] = "Kategorie"

# celkove = radek_vysledky + [ poradi, cas]

# celkove[7] = "Poradi"
# celkove[8] = "Cas"

opts, args = getopt.getopt(sys.argv[1:], "akwcr", ["help", "output="])

for o, a in opts:
    if o == "-a":

        print("Poradi;Startovni cislo;Cas;Jmeno;Prijmeni;Rocnik;Kategorie")
        for vysledek in celkove:
            print("%s;%s;%s;%s;%s;%s;%s" %
                (vysledek[7],
                vysledek[0],
                vysledek[8],
                vysledek[1],
                vysledek[2],
                vysledek[3],
                vysledek[6]
                )
            )

        print()
        print()

# Start. Číslo;Jméno;Příjmení;Ročník;Typ trasy;Muž/ženy;Kategorie;Cas;Poradi Celkem;Poradi Kategorie
    if o == "-r":

        print("Start. Číslo;Jméno;Příjmení;Ročník;Typ trasy;Muž/ženy;Kategorie;Cas;Poradi Celkem;Poradi Kategorie")
        for vysledek in celkove_registrace:
            print("%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;" %
                (vysledek[0],
                 vysledek[1],
                 vysledek[2],
                 vysledek[3],
                 vysledek[4],
                 vysledek[5],
                 vysledek[6],
                 vysledek[8],
                 vysledek[7],
                 ""
                )
            )

        print()
        print()

    if o == "-c":

        print("Poradi;Startovni cislo;Cas;Jmeno;Prijmeni;Rocnik;Kategorie")
        for vysledek in casy:
            print("%s;%s;%s;%s;%s;%s;%s" %
                (vysledek[7],
                vysledek[0],
                vysledek[8],
                vysledek[1],
                vysledek[2],
                vysledek[3],
                vysledek[6]
                )
            )

        print()
        print()

    if o == "-k":
        for kategorie in vysledky_kategorie.keys():
            print("Kategorie: %s" % kategorie)
            print("Poradi;Startovni cislo;Cas;Jmeno;Prijmeni;Rocnik;Kategorie")
            poradi = 1
            # for vysledek_kategorie in vysledky_kategorie[kategorie]:
            for vysledek_kategorie in celkove:
                if vysledek_kategorie[6] == kategorie:
                    print("%d;%s;%s;%s;%s;%s;%s" %
                          (poradi,
                           vysledek_kategorie[0],
                           vysledek_kategorie[8],
                           vysledek_kategorie[1],
                           vysledek_kategorie[2],
                           vysledek_kategorie[3],
                           kategorie
                          )
                    )
                    poradi = poradi + 1

            print()

    if o == "-w":
        print("""<html><meta charset="utf-8" http-equiv="refresh" content="120"><style>
* {
  box-sizing: border-box;
}

/* Create two equal columns that floats next to each other */
.column {
  float: left;
  width: 50%;
  padding: 10px;
  height: 300px; /* Should be removed. Only for demonstration */
}

/* Clear floats after the columns */
.row:after {
  content: "";
  display: table;
  clear: both;
}
</style><body><h1>PREDBEZNE Vysledky 06. 04. 2019   BEZ ZARUKY</h1>

        | """)

        for kategorie in vysledky_kategorie.keys():
            print("<a href='#%s'>%s</a> | " % (kategorie, kategorie))

        print('<div class="row"><div class="column">')
        print("<h2>Casy</h2>")
        print('<table border="1" cellpadding="5"><tr><td>Poradi</td><td>Startovni Cislo</td><td>Cas</td><td>Jmeno</td></tr>')
        poradi = 1
        for vysledek in casy:
            if vysledek[7] == 0:
                continue
            print("<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td></tr>" %
                  (poradi, vysledek[0],vysledek[8], vysledek[1] + " " + vysledek[2])
            )
            poradi += 1
        print("</table>")
        print("""
</div><div class="column">
        <h2>Kategorie</h2>""")
        for kategorie in vysledky_kategorie.keys():
            print("<h2 id='%s'>%s</h2>" % (kategorie, kategorie))
            # print("Poradi;Startovni cislo;Cas;Jmeno;Prijmeni;Rocnik;Kategorie")
            print('<table border="1" cellpadding="5"><tr><td>Poradi</td><td>Startovni Cislo</td><td>Cas</td><td>Jmeno</td></tr>')
            poradi = 1
            # for vysledek_kategorie in vysledky_kategorie[kategorie]:
            for vysledek in celkove:
                if vysledek[6] == kategorie and vysledek[8] != "":
                    # print("%d;%s;%s;%s;%s;%s;%s" %
                    #       (poradi,
                    #        vysledek_kategorie[0],
                    #        vysledek_kategorie[8],
                    #        vysledek_kategorie[1],
                    #        vysledek_kategorie[2],
                    #        vysledek_kategorie[3],
                    #        kategorie
                    #       )
                    # )
                    print("<tr><td>%d</td><td>%s</td><td>%s</td><td>%s</td></tr>" %
                          (poradi, vysledek[0],vysledek[8], vysledek[1] + " " + vysledek[2])
                    )

                    poradi = poradi + 1
            print("</table>")
        print('</div></div> ')
        print("</body></html>")
