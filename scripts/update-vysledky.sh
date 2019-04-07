#!/bin/bash

while true; do cp tabulka.csv vysledky.csv; ../scripts/zpracuj.py -w > vysledky-static.html; scp vysledky-static.html bach@wdsss:/var/www/html/jbk.k8o.eu/index.html; sleep 60; done
