#pragma once

// ==============================================================================
// POLISH TRANSLATIONS
// ==============================================================================

// Slate watchface weather conditions (Polish without diacritics)
#define WF_S_WEATHER_CLEAR_SKY "Bezchmurnie"
#define WF_S_WEATHER_MOSTLY_CLEAR "Prawie czysto"
#define WF_S_WEATHER_PARTLY_CLOUDY "Lekkie chmury"
#define WF_S_WEATHER_OVERCAST "Pochmurno"
#define WF_S_WEATHER_FOG "Mgla"
#define WF_S_WEATHER_HEAVY_FOG "Gesta mgla"
#define WF_S_WEATHER_LIGHT_DRIZZLE "Lekka mzawka"
#define WF_S_WEATHER_DRIZZLE "Mzawka"
#define WF_S_WEATHER_HEAVY_DRIZZLE "Mzawka"
#define WF_S_WEATHER_LIGHT_FREEZING "Lekkie zamrz."
#define WF_S_WEATHER_HEAVY_FREEZING "Lodowato"
#define WF_S_WEATHER_LIGHT_RAIN "Lekki deszcz"
#define WF_S_WEATHER_RAIN "Deszcz"
#define WF_S_WEATHER_HEAVY_RAIN "Ulewny deszcz"
#define WF_S_WEATHER_FREEZING_RAIN "Marznacy"
#define WF_S_WEATHER_FREEZING_HARD "Zamiez"
#define WF_S_WEATHER_LIGHT_SNOW "Lekki snieg"
#define WF_S_WEATHER_SNOW "Snieg"
#define WF_S_WEATHER_HEAVY_SNOW "Snieznica"
#define WF_S_WEATHER_SNOW_GRAINS "Ziarna"
#define WF_S_WEATHER_LIGHT_SHOWERS "Przelotnie"
#define WF_S_WEATHER_SHOWERS "Deszczowo"
#define WF_S_WEATHER_HEAVY_SHOWERS "Opady"
#define WF_S_WEATHER_SNOW_SHOWERS "Sniezne"
#define WF_S_WEATHER_THUNDERSTORM "Burza"
#define WF_S_WEATHER_LIGHT_HAIL "Lekki grad"
#define WF_S_WEATHER_HEAVY_HAIL "Gradowo"
#define WF_S_WEATHER_UNKNOWN "Nieznane"

// Day names (abbreviated)
#define WF_S_DAY_MON "PON"
#define WF_S_DAY_TUE "WTO"
#define WF_S_DAY_WED "SRO"
#define WF_S_DAY_THU "CZW"
#define WF_S_DAY_FRI "PIA"
#define WF_S_DAY_SAT "SOB"
#define WF_S_DAY_SUN "NIE"

// Error messages
#define WF_T_ERROR "BLAD"        // Taychron
#define WF_I_ERROR "BLAD"        // InkField
#define WF_I_ERROR_SHORT "BL"    // InkField short

// Time format
#define WF_TIME_AM "AM"
#define WF_TIME_PM "PM"

// Weather submenu items
#define WEATHER_MENU_TEMPERATURE "Temperatura"
#define WEATHER_MENU_PRESSURE "Cisnienie"
#define WEATHER_MENU_HUMIDITY "Wilgotnosc"
#define WEATHER_MENU_CONDITIONS "Warunki pogodowe"
#define WEATHER_MENU_CLOUDINESS "Zachmurzenie"
#define WEATHER_MENU_WIND_SPEED "Predkosc wiatru"
#define WEATHER_MENU_WIND_GUSTS "Porywy wiatru"
#define WEATHER_MENU_VISIBILITY "Widocznosc"
#define WEATHER_MENU_PRECIPITATION "% opadow"

// Weather menu titles and messages
#define WEATHER_SELECT_DATE "Wybierz date"
#define WEATHER_STAT_TITLE "Statystyki pogody"
#define WEATHER_NOT_AVAILABLE "Pogoda niedostepna"
#define WEATHER_DATE_WRONG "Nieprawid. data?"
#define WEATHER_CORRUPTED "Pogoda uszkodzona"

// Chart titles with time notation
#define WEATHER_CHART_TEMP "Temp / C (24h)"
#define WEATHER_CHART_PRESSURE "Cisnienie / hPa (24h)"
#define WEATHER_CHART_HUMIDITY "Wilgotnosc / % (24h)"
#define WEATHER_CHART_CLOUDS "Chmury / % (24h)"
#define WEATHER_CHART_WIND_SPEED "Pred. wiatru / km/h (24h)"
#define WEATHER_CHART_WIND_GUSTS "Porywy / km/h (24h)"
#define WEATHER_CHART_VISIBILITY "Widoczn. / m (24h)"
#define WEATHER_CHART_PRECIPITATION "Opady / % (24h)"
#define WEATHER_CONDITIONS_TITLE "Warunki pogodowe (24h)"

// Month names
#define LANGUAGE_MONTH_NAMES {"STY", "LUT", "MAR", "KWI", "MAJ", "CZE", "LIP", "SIE", "WRZ", "PAZ", "LIS", "GRU"}

// Menu translations
#define MENU_MAIN "Menu glowne"
#define MENU_CALENDAR "Kalendarz"
#define MENU_SELECT_BOOK "Wybierz ksiazke"
#define MENU_ALARMS "Alarmy"
#define MENU_WEATHER "Pogoda"
#define MENU_SETTINGS "Ustawienia"
#define MENU_HEART_MONITOR "Monitor serca"
#define MENU_VAULT "Sejf"
#define MENU_WIFI_TOOL "Narzedzie WiFi"
#define MENU_EATING_APPLES "Jedzenie jablek"
#define MENU_SMASHING_APPLES "Rozbijanie jablek"
#define MENU_GAMES "Gry"
#define MENU_CREDITS "Autorzy"
#define MENU_WIFI "WiFi"
#define MENU_DEBUG "Debugowanie"
#define MENU_POWER_SETTINGS "Ustawienia zasilania"
#define MENU_CHANGE_WATCHFACE "Zmien tarcze"
#define MENU_TETRIS "Blockchy"
#define MENU_PONG "Pong"
#define MENU_VIDEO_PLAYER "Odtwarzacz video"
#define MENU_CONWAY "Conway"
#define MENU_PARTY "Impreza"
#define MENU_VIBRATIONS_DIS "Wibracje wylaczone"
#define MENU_WAKE_UP_DIS "Wybudzenie wylaczone"
#define MENU_POWER "Menu zasilania"

// ==============================================================================
// APP AND BOOK TRANSLATIONS
// ==============================================================================

// Book reader messages
#define BOOK_NO_SELECTED "Nie wybrano ksiazki"

// Calendar messages  
#define CALENDAR_NO_DATA "Brak danych kalendarza"

// ==============================================================================
// HEART MONITOR TRANSLATIONS
// ==============================================================================

#define HEART_MONITOR_TITLE "Monitor serca"
#define HEART_MONITOR_NOT_MEDICAL "Nie uzywac medycznie"
#define HEART_MONITOR_PLACE_DEVICE "Przyloz do serca"
#define HEART_MONITOR_CLICK_MENU "Nacisnij menu"
#define HEART_MONITOR_START_3S "Start za 3 sekundy"
#define HEART_MONITOR_TAKES_15S "Trwa 15 sekund"
#define HEART_MONITOR_BREATHE_SLOWLY "Oddychaj spokojnie"
#define HEART_MONITOR_TIME_LEFT "Pozostalo: "
#define HEART_MONITOR_BPM "bpm"

// ==============================================================================
// VIDEO PLAYER TRANSLATIONS
// ==============================================================================

#define VIDEO_PLAYER_CHOOSE "Wybierz video"

// ==============================================================================
// WATCHFACE SELECTION TRANSLATIONS
// ==============================================================================

#define WATCHFACE_SELECT_TITLE "Wybierz tarcze"

// ==============================================================================
// TETRIS GAME TRANSLATIONS
// ==============================================================================

// Tetris UI labels
#define TETRIS_LINE_CLEARS "Usuniete linie:"
#define TETRIS_SCORE "Wynik:"
#define TETRIS_LEVEL "Poziom:"
#define TETRIS_CONTROLS_UP "Gora:Prawo"
#define TETRIS_CONTROLS_DOWN "Dol:Lewo"
#define TETRIS_GAME_OVER "KONIEC GRY"

// ==============================================================================
// DEBUG MENU TRANSLATIONS
// ==============================================================================

// Debug menu titles
#define DEBUG_MENU_ACC "Debug Menu: Acc"
#define DEBUG_MENU_BATTERY "Debug Menu: Bateria"
#define DEBUG_MENU_CLOCK "Debug Menu: Zegar"
#define DEBUG_MENU_GIT "Debug Menu: Git"
#define DEBUG_MENU_HARDWARE "Debug Menu: Sprzet"
#define DEBUG_MENU_WIFI "Debug Menu: WiFi"

// Debug menu items (short names for main debug menu)
#define DEBUG_ITEM_CLOCK "Zegar"
#define DEBUG_ITEM_HARDWARE "Sprzet"
#define DEBUG_ITEM_BATTERY "Bateria"
#define DEBUG_ITEM_GIT "Git"
#define DEBUG_ITEM_ACC "Acc"
#define DEBUG_ITEM_FONT_PREVIEW "Podglad czcionki"

// Accelerometer debug
#define DEBUG_ACC_DISABLED "wylaczony"
#define DEBUG_ACC_FAILED_INIT "Blad init Acc"
#define DEBUG_ACC_DAMAGED "Acc uszkodzony"
#define DEBUG_ACC_WORKING "Acc dziala"
#define DEBUG_ACC_CLICK_3D "Menu dla 3D"
#define DEBUG_ACC_X "Acc X: "
#define DEBUG_ACC_Y "Acc Y: "
#define DEBUG_ACC_Z "Acc Z: "
#define DEBUG_ACC_PURE_X "Acc czyste X: "
#define DEBUG_ACC_PURE_Y "Acc czyste Y: "
#define DEBUG_ACC_PURE_Z "Acc czyste Z: "
#define DEBUG_ACC_STEPS "Kroki: "
#define DEBUG_ACC_IC "IC: "

// Battery debug
#define DEBUG_BATTERY_CURRENT_V "Obecne V: "
#define DEBUG_BATTERY_MINIMUM_V "Minimum V: "
#define DEBUG_BATTERY_MAXIMUM_V "Maksimum V: "
#define DEBUG_BATTERY_CRITICAL_V "Krytyczne V: "
#define DEBUG_BATTERY_LEVEL "Poziom %: "
#define DEBUG_BATTERY_CHARGING "Ladowanie: "
#define DEBUG_BATTERY_FULLY "Pelne: "

// Clock debug
#define DEBUG_CLOCK_DRIFT_SYNCS "Dryft synch.:"
#define DEBUG_CLOCK_NOT_AVAILABLE "Niedostepne"
#define DEBUG_CLOCK_LAST_SYNC "Ostatnia synch.:"
#define DEBUG_CLOCK_PREVIOUS_SYNC "Poprz. synch.:"
#define DEBUG_CLOCK_LAST_CHARGE "Ostatnie lad.:"

// Git debug
#define DEBUG_GIT_COMMIT_HASH "Hash commita:"
#define DEBUG_GIT_BRANCH "Galaz:"
#define DEBUG_GIT_BUILD_TIME "Czas budowy:"

// Hardware debug
#define DEBUG_HW_CHIP_MODEL "Model chipu:"
#define DEBUG_HW_RTC_TYPE "Typ RTC: "
#define DEBUG_HW_USED_HEAP "Pamiec KB: "
#define DEBUG_HW_CPU_TEMP "Temp CPU: "
#define DEBUG_HW_INIT_TEMP "Temp init: "

// RTC Types
#define DEBUG_RTC_UNKNOWN "Nieznany"
#define DEBUG_RTC_DS3231 "DS3231"
#define DEBUG_RTC_PCF8563 "PCF8563"
#define DEBUG_RTC_INTERNAL "WEWNETRZNY"
#define DEBUG_RTC_INVALID "Nieprawidlowy typ"

// WiFi debug
#define DEBUG_WIFI_MAC_ADDRESS "Adres MAC:"
#define DEBUG_WIFI_STATUS "Status WiFi: "
#define DEBUG_WIFI_IP "IP: "
#define DEBUG_WIFI_SSID "SSID: "
#define DEBUG_WIFI_SIGNAL "Sygnal WiFi: "
#define DEBUG_WIFI_CONNECTING "Laczenie: "
#define DEBUG_WIFI_ON "WL"
#define DEBUG_WIFI_OFF "WYL"

// Common debug terms
#define DEBUG_COMMON_PERCENT "%"
#define DEBUG_COMMON_COLON ": "

// ==============================================================================
// GAME AND APP TRANSLATIONS
// ==============================================================================

// Pong game
#define PONG_YOU_LOST "Przegrales!"

// Party app
#define PARTY_MESSAGE "Impreza impreza"

// Vault app
#define VAULT_KEY_INCORRECT "Klucz jest niepoprawny"
#define VAULT_EMPTY "Sejf jest pusty?"