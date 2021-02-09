Silnik gry został napisany na wzór architektury ECS. Stan gry znajduje się w strukturze `GameState`,
zdefiniowanej w `gamestate.h`

Gra podzielona jest na funkcje, nazywane w kodzie systemami, które mają dostęp do i mogą modyfikować stan gry. 
Lista systemów znajduje się w `main.c`. Są podzielone na systemy inicjalizacyjne, eventowe, klatkowe i deinicjalizacyjne.
- systemy inicjalizacyjne - uruchamiane na początku programu
- systemy eventowe - jedyne systemy mające dostęp do wydarzeń generowanych przez Allegro. 
  Tutaj przechwytywane jest wejście od użytkownika i zapisywane dla wykorzystania przez resztę systemów.
- systemy klatkowe - systemy uruchamiane w każdej klatce. Tutaj dzieje się cała logika gry oraz renderowanie.
- systemy deinicjalizacyjne - odpowiedzialne za zwalnianie zasobów.

## Encje

Informacje o encjach na ekranie przechowywane są w wektorze `entities` i wektorach o nazwach `*_components`,
znajdujących się w strukturze `GameState`. Wszystkie wektory w każdym momencie działania gry są tej samej długości.  
Same encje utożsamiamy z typem `int` - indeksem w wektorach komponentów. Na przykład, by sprawdzić, czy encja `x` 
żyje, sprawdzamy czy na `x`-tym miejscu w wektorze `entities` znajduje się wartość `true`. Jeżeli chcemy sprawdzić, 
ile ma życia (oraz czy w ogóle posiada licznik życia), sprawdzamy `x`-ty indeks w wektorze `health_components`.  
Struktura ta pozwala na łatwe ponowne wykorzystanie kodu takiego, jak sprawdzanie kolizji czy renderowanie sprite'ów.

### Poszczególne pliki
Krótki opis działania każdego pliku znajduje się w j. angielskim w odpowiadającym mu pliku nagłówkowym.
