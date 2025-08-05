# 📌 Schéma zapojení – Chytrý stojan na mobil (Arduino + HC-05 + Servo)

Tento soubor popisuje kompletní zapojení komponent použité v projektu chytrého stojanu na mobil, ovládaného přes Bluetooth.

---

## 🔌 Použité komponenty

| Komponenta          | Popis                            |
|---------------------|----------------------------------|
| Arduino Uno / Nano  | Hlavní mikrokontrolér            |
| HC-05               | Bluetooth modul                  |
| Servo motor         | Ovládání pohybu stojanu          |
| PIR senzor          | Detekce pohybu                   |
| RGB LED (3 piny)    | Indikace režimů a světla         |
| Tlačítko (s pull-up)| Přepínání režimů                |
| Potenciometr        | Ovládání jasu / směru            |
| Rezistory (220–330 Ω) | Pro LED                         |
| Externí napájení    | Pro servo (doporučeno)           |

---

## 🔧 Zapojení do Arduina

### 🔵 HC-05 Bluetooth modul

| HC-05 Pin | Arduino Pin | Poznámka                           |
|-----------|-------------|------------------------------------|
| VCC       | 5V          | Napájení                           |
| GND       | GND         | Zem                                |
| TXD       | D11         | Připojeno přes `SoftwareSerial`   |
| RXD       | D10         | Pozor: RX by měl mít dělič napětí |

> ⚠️ **Poznámka:** HC-05 RX je 3.3V tolerantní, doporučuje se dělič z dvou rezistorů (např. 1kΩ + 2kΩ).

---

### 🟠 Servo motor

| Servo Pin | Arduino Pin |
|-----------|-------------|
| Signál    | D9          |
| VCC       | 5V (nebo externí 5V) |
| GND       | GND         |

> Doporučuje se **oddělené napájení** serva, pokud máš silnější servo.

---

### 🟢 RGB LED

| Barva | Arduino Pin | Barva v kódu        |
|-------|-------------|---------------------|
| Červená (R) | D3     | `redPin`           |
| Zelená (G)  | D5     | `greenPin`         |
| Modrá (B)   | D6     | `bluePin`          |

> Použij vhodné **rezistory** pro každý pin (220–330 Ω).

---

### 🔘 Tlačítko (s vnitřním pull-up)

| Pin tlačítka | Arduino Pin |
|--------------|-------------|
| Jeden kontakt | D2         |
| Druhý kontakt | GND        |

> Pull-up je zapnutý v kódu pomocí `INPUT_PULLUP`.

---

### 🧭 Potenciometr

| Pin potenciometru | Arduino Pin |
|-------------------|-------------|
| Střední vývod     | A0          |
| Krajní vývody     | 5V a GND    |

---

### 👀 PIR senzor

| PIR Pin | Arduino Pin |
|---------|-------------|
| OUT     | D7          |
| VCC     | 5V          |
| GND     | GND         |

---

## 📋 Shrnutí pinů

| Komponenta     | Arduino Pin |
|----------------|-------------|
| HC-05 TX → RX  | D11 (SoftSerial) |
| HC-05 RX ← TX  | D10 (SoftSerial) |
| Servo          | D9          |
| RGB LED (R)    | D3          |
| RGB LED (G)    | D5          |
| RGB LED (B)    | D6          |
| Tlačítko       | D2          |
| PIR senzor     | D7          |
| Potenciometr   | A0          |

---

## 📌 Doporučení

- Servo motor může brát hodně proudu – napájej jej **z externího zdroje** (5V) přes **GND propojený s Arduinem**.
- Pro HC-05 RX použij **dělič napětí** (např. 1kΩ a 2kΩ).
- Pokud ti RGB LED problikává při změnách, zkus dát **kondenzátor mezi 5V a GND**.

---

## 🧠 Autor: Sebastian Fišer – 2025