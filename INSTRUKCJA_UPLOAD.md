# Instrukcja użycia - Upload pliku XML z danymi HEX

## Opis
Strona `upload-hex.html` umożliwia wczytywanie plików XML zawierających 67 bajtów danych w formacie hex.

## Jak użyć

### Metoda 1: Drag & Drop
1. Otwórz plik `upload-hex.html` w przeglądarce
2. Przeciągnij plik XML na obszar z przerywaną ramką
3. Dane zostaną automatycznie wczytane i wyświetlone

### Metoda 2: Wybór pliku
1. Otwórz plik `upload-hex.html` w przeglądarce
2. Kliknij przycisk "Wybierz plik XML"
3. Wybierz plik XML z dysku
4. Dane zostaną automatycznie wczytane i wyświetlone

## Format pliku XML

Plik XML powinien zawierać element z 67 bajtami danych hex. Przykład (`przyklad.xml`):

```xml
<?xml version="1.0" encoding="UTF-8"?>
<konfiguracja>
    <dane_hex>
        00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F
        10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F
        20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F
        30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F
        40 41 42
    </dane_hex>
</konfiguracja>
```

### Obsługiwane nazwy elementów
Strona automatycznie wykrywa element zawierający dane hex. Obsługiwane nazwy:
- `hex_data`
- `dane_hex`
- `data`
- `hexdata`
- lub dowolny inny element zawierający prawidłowe dane hex

## Funkcje strony

### Wyświetlanie danych
- **Interaktywne bajty**: Każdy bajt jest wyświetlany osobno z tooltipem pokazującym wartość hex i dziesiętną
- **Statystyki**: Liczba bajtów, format, status walidacji

### Konwersje formatów
Dane są automatycznie konwertowane do:
- **HEX**: Format szesnastkowy (np. 00 01 02...)
- **Decimal**: Wartości dziesiętne w tablicy (np. [0, 1, 2...])
- **Binary**: Wartości binarne pierwszych 8 bajtów (np. 0b00000000...)
- **C Array**: Format tablicy C (np. {0x00, 0x01, 0x02...})

### Akcje
- **📋 Kopiuj HEX**: Kopiuje dane hex do schowka
- **💾 Pobierz HEX**: Pobiera dane jako plik tekstowy
- **💾 Pobierz jako Array**: Pobiera dane jako tablicę C (plik .c)
- **🔄 Resetuj**: Resetuje stronę do stanu początkowego

## Walidacja

Strona automatycznie waliduje:
1. **Format XML**: Sprawdza poprawność struktury XML
2. **Format HEX**: Każdy bajt musi być w formacie XX (00-FF)
3. **Liczba bajtów**: Ostrzeżenie jeśli liczba bajtów != 67 (ale dane są wczytywane)

## Wymagania techniczne

- Nowoczesna przeglądarka z obsługą:
  - FileReader API
  - DOMParser
  - Clipboard API
  - Drag & Drop API
- JavaScript musi być włączony
- Brak potrzeby serwera - strona działa lokalnie (file://)

## Testowanie

Użyj dołączonego pliku `przyklad.xml` do przetestowania funkcjonalności:
1. Otwórz `upload-hex.html`
2. Wybierz lub przeciągnij `przyklad.xml`
3. Sprawdź czy wyświetla się 67 bajtów
4. Przetestuj przyciski akcji

## Uwagi
- Strona działa w trybie offline (nie wymaga połączenia internetowego)
- Wszystkie operacje są wykonywane po stronie klienta (brak wysyłania danych)
- Obsługuje pliki XML w kodowaniu UTF-8
