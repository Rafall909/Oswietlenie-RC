# Jak załadować 67 bajtów HEX z pliku XML przez stronę HTML

## Opis problemu
Potrzebujesz załadować wartość 67 bajtów w formacie hex z pliku XML na stronie HTML.

## Rozwiązanie

### 1. Struktura pliku XML

Stwórz plik XML (np. `dane.xml`) z następującą strukturą:

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

### 2. Kod HTML/JavaScript

```html
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>Ładowanie HEX z XML</title>
</head>
<body>
    <h1>Ładowanie 67 bajtów HEX z XML</h1>
    
    <button onclick="zaladujXML()">Załaduj dane</button>
    
    <div id="wynik"></div>
    
    <script>
        function zaladujXML() {
            // Metoda 1: XMLHttpRequest
            var xhr = new XMLHttpRequest();
            xhr.open('GET', 'dane.xml', true);
            xhr.onload = function() {
                if (xhr.status === 200) {
                    przetworzXML(xhr.responseText);
                }
            };
            xhr.send();
        }
        
        // Alternatywnie - Metoda 2: Fetch API (nowoczesne)
        function zaladujXMLFetch() {
            fetch('dane.xml')
                .then(response => response.text())
                .then(xml => przetworzXML(xml))
                .catch(error => console.error('Błąd:', error));
        }
        
        function przetworzXML(xmlText) {
            // Parsowanie XML
            var parser = new DOMParser();
            var xmlDoc = parser.parseFromString(xmlText, "text/xml");
            
            // Pobranie danych hex
            var daneHex = xmlDoc.getElementsByTagName("dane_hex")[0].textContent;
            
            // Przetworzenie na tablicę bajtów
            var bajty = daneHex.trim().split(/\s+/);
            
            // Walidacja - sprawdzenie czy jest 67 bajtów
            if (bajty.length === 67) {
                console.log("Załadowano 67 bajtów:", bajty);
                wyswietlDane(bajty);
            } else {
                console.error("Błąd: znaleziono " + bajty.length + " bajtów, oczekiwano 67");
            }
        }
        
        function wyswietlDane(bajty) {
            var wynikDiv = document.getElementById('wynik');
            wynikDiv.innerHTML = '<h2>Załadowane dane (' + bajty.length + ' bajtów):</h2>';
            wynikDiv.innerHTML += '<pre>' + bajty.join(' ') + '</pre>';
            
            // Konwersja do różnych formatów
            wynikDiv.innerHTML += '<h3>Formaty:</h3>';
            
            // Jako tablica liczb dziesiętnych
            var liczby = bajty.map(b => parseInt(b, 16));
            wynikDiv.innerHTML += '<p><b>Dziesiętnie:</b> ' + liczby.join(', ') + '</p>';
            
            // Jako ciąg binarny
            var binarnie = bajty.map(b => parseInt(b, 16).toString(2).padStart(8, '0'));
            wynikDiv.innerHTML += '<p><b>Binarnie (pierwsze 8 bajtów):</b> ' + binarnie.slice(0, 8).join(' ') + '</p>';
        }
    </script>
</body>
</html>
```

### 3. Użycie danych

Po załadowaniu możesz wykorzystać dane na różne sposoby:

```javascript
// Konwersja do tablicy liczb
var liczby = bajty.map(b => parseInt(b, 16));

// Wysłanie do serwera
function wyslijNaSerwer(bajty) {
    fetch('/api/dane', {
        method: 'POST',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify({dane: bajty})
    });
}

// Zapisanie do localStorage
localStorage.setItem('dane_hex', JSON.stringify(bajty));

// Konwersja do Uint8Array (dla operacji binarnych)
var uint8Array = new Uint8Array(bajty.map(b => parseInt(b, 16)));
```

### 4. Obsługa wyboru pliku przez użytkownika

Jeśli chcesz, aby użytkownik mógł wybrać plik XML:

```html
<input type="file" id="plikInput" accept=".xml" onchange="wczytajPlik(event)">

<script>
function wczytajPlik(event) {
    var plik = event.target.files[0];
    var reader = new FileReader();
    
    reader.onload = function(e) {
        przetworzXML(e.target.result);
    };
    
    reader.readAsText(plik);
}
</script>
```

### 5. CORS - ważne uwagi

Jeśli ładujesz XML z tego samego serwera - działa bez problemu.

Jeśli plik jest lokalny (file://), nowoczesne przeglądarki mogą blokować żądania. Rozwiązania:
- Użyj lokalnego serwera HTTP (np. `python -m http.server`)
- Użyj inputa file (metoda 4)
- Skonfiguruj serwer z odpowiednimi nagłówkami CORS

### 6. Walidacja danych

```javascript
function walidujHex(bajty) {
    // Sprawdź liczbę bajtów
    if (bajty.length !== 67) {
        return false;
    }
    
    // Sprawdź format każdego bajtu
    var hexPattern = /^[0-9A-Fa-f]{2}$/;
    for (var i = 0; i < bajty.length; i++) {
        if (!hexPattern.test(bajty[i])) {
            console.error("Nieprawidłowy bajt na pozycji " + i + ": " + bajty[i]);
            return false;
        }
    }
    
    return true;
}
```

## Przykładowe zastosowania

1. **Konfiguracja urządzenia** - wysłanie danych do mikrokontrolera przez WebSerial API
2. **Edytor konfiguracji** - modyfikacja i zapis z powrotem do XML
3. **Wizualizacja** - wyświetlenie danych w formie graficznej
4. **Konwersja formatów** - eksport do innych formatów (bin, hex, C array)

## Pełny przykład z obsługą błędów

```javascript
async function zaladujIWaliduj() {
    try {
        const response = await fetch('dane.xml');
        if (!response.ok) throw new Error('Błąd ładowania pliku');
        
        const xmlText = await response.text();
        const parser = new DOMParser();
        const xmlDoc = parser.parseFromString(xmlText, "text/xml");
        
        // Sprawdź błędy parsowania
        if (xmlDoc.querySelector('parsererror')) {
            throw new Error('Błędny format XML');
        }
        
        const element = xmlDoc.querySelector('dane_hex');
        if (!element) throw new Error('Nie znaleziono elementu dane_hex');
        
        const bajty = element.textContent.trim().split(/\s+/);
        
        if (!walidujHex(bajty)) {
            throw new Error('Nieprawidłowe dane hex');
        }
        
        console.log('✓ Pomyślnie załadowano 67 bajtów');
        return bajty;
        
    } catch (error) {
        console.error('✗ Błąd:', error.message);
        return null;
    }
}
```

## Podsumowanie

To rozwiązanie pozwala:
- ✓ Załadować dane z pliku XML
- ✓ Sprawdzić poprawność (67 bajtów hex)
- ✓ Przetworzyć dane do różnych formatów
- ✓ Obsłużyć błędy
- ✓ Działa w każdej nowoczesnej przeglądarce

Możesz to zintegrować z istniejącym serwerem HTML i dostosować do swoich potrzeb.
