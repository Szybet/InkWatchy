#pragma once

// ITALIAN TRANSLATIONS

// Slate watchface weather conditions
#define WF_S_WEATHER_CLEAR_SKY "Cielo sereno"
#define WF_S_WEATHER_MOSTLY_CLEAR "Prevalentemente sereno"
#define WF_S_WEATHER_PARTLY_CLOUDY "Parzialmente nuvoloso"
#define WF_S_WEATHER_OVERCAST "Coperto"
#define WF_S_WEATHER_FOG "Nebbia"
#define WF_S_WEATHER_HEAVY_FOG "Nebbia fitta"
#define WF_S_WEATHER_LIGHT_DRIZZLE "Pioviggine leggera"
#define WF_S_WEATHER_DRIZZLE "Pioviggine"
#define WF_S_WEATHER_HEAVY_DRIZZLE "Pioviggine intensa"
#define WF_S_WEATHER_LIGHT_FREEZING "Gelo leggero"
#define WF_S_WEATHER_HEAVY_FREEZING "Gelo intenso"
#define WF_S_WEATHER_LIGHT_RAIN "Pioggia leggera"
#define WF_S_WEATHER_RAIN "Pioggia"
#define WF_S_WEATHER_HEAVY_RAIN "Pioggia intensa"
#define WF_S_WEATHER_FREEZING_RAIN "Pioggia gelata"
#define WF_S_WEATHER_FREEZING_HARD "Gelo forte"
#define WF_S_WEATHER_LIGHT_SNOW "Neve debole"
#define WF_S_WEATHER_SNOW "Neve"
#define WF_S_WEATHER_HEAVY_SNOW "Neve intensa"
#define WF_S_WEATHER_SNOW_GRAINS "Granuli di neve"
#define WF_S_WEATHER_LIGHT_SHOWERS "Rovesci deboli"
#define WF_S_WEATHER_SHOWERS "Rovesci"
#define WF_S_WEATHER_HEAVY_SHOWERS "Rovesci intensi"
#define WF_S_WEATHER_SNOW_SHOWERS "Rovesci di neve"
#define WF_S_WEATHER_THUNDERSTORM "Temporale"
#define WF_S_WEATHER_LIGHT_HAIL "Grandine debole"
#define WF_S_WEATHER_HEAVY_HAIL "Grandine intensa"
#define WF_S_WEATHER_UNKNOWN "Sconosciuto"

// Day names (abbreviated)
#define WF_S_DAY_MON "LUN"
#define WF_S_DAY_TUE "MAR"
#define WF_S_DAY_WED "MER"
#define WF_S_DAY_THU "GIO"
#define WF_S_DAY_FRI "VEN"
#define WF_S_DAY_SAT "SAB"
#define WF_S_DAY_SUN "DOM"

// Error messages
#define WF_T_ERROR "ERR"      // Taychron
#define WF_I_ERROR "ERR"      // InkField (max 3 characters)
#define WF_I_ERROR_SHORT "ER" // InkField short

// Time format
#define WF_TIME_AM "AM"
#define WF_TIME_PM "PM"

// Weather/Air Quality Selection items
#define WEATHER_AQ_MENU_TITLE "Tipo di dati"
#define WEATHER_AQ_MENU_WEATHER "Meteo"
#define WEATHER_AQ_MENU_AIR_QUALITY "Qualita dell'aria"
#define WEATHER_AQ_NOT_AVAILABLE "Meteo/Aria non disp."

// Weather submenu items
#define WEATHER_MENU_TEMPERATURE "Temperatura"
#define WEATHER_MENU_PRESSURE "Pressione"
#define WEATHER_MENU_HUMIDITY "Umidita"
#define WEATHER_MENU_CONDITIONS "Condizioni meteo"
#define WEATHER_MENU_CLOUDINESS "Copertura nuvolosa"
#define WEATHER_MENU_WIND_SPEED "Velocita del vento"
#define WEATHER_MENU_WIND_GUSTS "Raffiche di vento"
#define WEATHER_MENU_VISIBILITY "Visibilita"
#define WEATHER_MENU_PRECIPITATION "% di precipitazioni"
#define WEATHER_MENU_UV_INDEX "Indice UV"
#define WEATHER_MENU_UV_INDEX_CLEAR_SKY "UV Cielo sereno"
#define WEATHER_MENU_WET_BULB "Temp bulbo umido"
#define WEATHER_MENU_CAPE "CAPE"
#define WEATHER_MENU_DEW_POINT "Punto di rugiada"
#define WEATHER_MENU_PRECIPITATION_AMOUNT "Precipitazioni"
#define WEATHER_MENU_SNOW_DEPTH "Spessore neve"
#define WEATHER_MENU_LIFTED_INDEX "Lifted Index"
#define WEATHER_MENU_CONVECTIVE_INHIBITION "Inib. convettiva"
#define WEATHER_MENU_TERRESTRIAL_RADIATION "Rad. terrestre"
#define WEATHER_MENU_DIFFUSE_RADIATION "Rad. diffusa"
#define WEATHER_MENU_DIRECT_RADIATION "Rad. diretta"
#define WEATHER_MENU_TOTAL_WATER_VAPOUR "Vapore acqueo tot."

// Weather menu titles and messages
#define WEATHER_SELECT_DATE "Seleziona data"
#define WEATHER_STAT_TITLE "Statistiche meteo"
#define WEATHER_NOT_AVAILABLE "Meteo non disponibile"
#define WEATHER_DATE_WRONG "Data errata?"
#define WEATHER_CORRUPTED "Dati meteo corrotti"

// Chart titles with time notation
#define WEATHER_CHART_TEMP "Temp/C"
#define WEATHER_CHART_PRESSURE "Pressione/hPa"
#define WEATHER_CHART_HUMIDITY "Umidita/%"
#define WEATHER_CHART_CLOUDS "Nuvole/%"
#define WEATHER_CHART_WIND_SPEED "Vel. vento/km/h"
#define WEATHER_CHART_WIND_GUSTS "Raffiche/km/h"
#define WEATHER_CHART_VISIBILITY "Visib./m"
#define WEATHER_CHART_PRECIPITATION "Prec./%"
#define WEATHER_CONDITIONS_TITLE "Condizioni meteo"
#define WEATHER_CHART_UV_INDEX "Indice UV"
#define WEATHER_CHART_UV_INDEX_CLEAR_SKY "UV Cielo ser."
#define WEATHER_CHART_WET_BULB "Bulbo um./C"
#define WEATHER_CHART_CAPE "CAPE/J/kg"
#define WEATHER_CHART_DEW_POINT "P. rugiada/C"
#define WEATHER_CHART_PRECIPITATION_AMOUNT "Prec. tot."
#define WEATHER_CHART_SNOW_DEPTH "Altezza neve"
#define WEATHER_CHART_LIFTED_INDEX "Lifted Index"
#define WEATHER_CHART_CONVECTIVE_INHIBITION "Inib. conv."
#define WEATHER_CHART_TERRESTRIAL_RADIATION "Rad. terr."
#define WEATHER_CHART_DIFFUSE_RADIATION "Rad. diff."
#define WEATHER_CHART_DIRECT_RADIATION "Rad. dir."
#define WEATHER_CHART_TOTAL_WATER_VAPOUR "Vapor acq. tot."

// Air quality submenu items
#define AIR_MENU_EUAQI "AQI Europeo"
#define AIR_MENU_USAQI "AQI USA"
#define AIR_MENU_EU_AQI_PM2_5 "EU AQI PM2,5"
#define AIR_MENU_EU_AQI_PM10 "EU AQI PM10"
#define AIR_MENU_EU_AQI_NO2 "EU AQI NO2"
#define AIR_MENU_EU_AQI_O3 "EU AQI O3"
#define AIR_MENU_EU_AQI_SO2 "EU AQI SO2"
#define AIR_MENU_PM2_5 "Particolato PM2,5"
#define AIR_MENU_PM10 "Particolato PM10"
#define AIR_MENU_CARBON_MONOXIDE "Monossido carbonio"
#define AIR_MENU_CARBON_DIOXIDE "Anidride carbonica"
#define AIR_MENU_NITROGEN_DIOXIDE "Biossido azoto"
#define AIR_MENU_SULPHUR_DIOXIDE "Biossido zolfo"
#define AIR_MENU_OZONE "Ozono"
#define AIR_MENU_AEROSOL_OPTICAL_DEPTH "Spessore aerosol"
#define AIR_MENU_DUST "Polvere"
#define AIR_MENU_METHANE "Metano"
#define AIR_MENU_FORMALDEHYDE "Formaldeide"
#define AIR_MENU_GLYOXAL "Gliossale"
#define AIR_MENU_SEA_SALT_AEROSOL "Aerosol sale marino"
#define AIR_MENU_NITROGEN_MONOXIDE "Monossido azoto"
#define AIR_MENU_PEROXYACYL_NITRATES "Nitrati perossiacilici"

// Air quality menu titles and messages
#define AIR_QUALITY_SELECT_DATE "Seleziona data"
#define AIR_QUALITY_STAT_TITLE "Stat qualita aria"
#define AIR_QUALITY_NOT_AVAILABLE "Qualita aria non disp."
#define AIR_QUALITY_DATE_WRONG "Data errata?"
#define AIR_QUALITY_CORRUPTED "Dati qualita aria corrotti"

// Air quality chart titles with time notation
#define AIR_CHART_EUAQI "Indice QA UE"
#define AIR_CHART_USAQI "Indice QA USA"
#define AIR_CHART_EU_AQI_PM2_5 "PM2.5/ug/m3"
#define AIR_CHART_EU_AQI_PM10 "PM10/ug/m3"
#define AIR_CHART_EU_AQI_NO2 "NO2/ug/m3"
#define AIR_CHART_EU_AQI_O3 "Ossido/ug/m3"
#define AIR_CHART_EU_AQI_SO2 "SO2/ug/m3"
#define AIR_CHART_PM2_5 "PM2.5/ug/m3"
#define AIR_CHART_PM10 "PM10/ug/m3"
#define AIR_CHART_CARBON_MONOXIDE "CO/ug/m3"
#define AIR_CHART_CARBON_DIOXIDE "CO2/ppm"
#define AIR_CHART_NITROGEN_DIOXIDE "NO2/ug/m3"
#define AIR_CHART_SULPHUR_DIOXIDE "SO2/ug/m3"
#define AIR_CHART_OZONE "O3/ug/m3"
#define AIR_CHART_AEROSOL_OPTICAL_DEPTH "AOD"
#define AIR_CHART_DUST "Polvere/ug/m3"
#define AIR_CHART_METHANE "Metano/ug/m3"
#define AIR_CHART_FORMALDEHYDE "CH2O/ug/m3"
#define AIR_CHART_GLYOXAL "Gliossale/ug/m3"
#define AIR_CHART_SEA_SALT_AEROSOL "Sale marino/ug/m3"
#define AIR_CHART_NITROGEN_MONOXIDE "NO/ug/m3"
#define AIR_CHART_PEROXYACYL_NITRATES "PAN/ug/m3"

// Month names
#define LANGUAGE_MONTH_NAMES {"GEN", "FEB", "MAR", "APR", "MAG", "GIU", "LUG", "AGO", "SET", "OTT", "NOV", "DIC"}

// Menu translations
#define MENU_MAIN "Menu principale"
#define MENU_CALENDAR "Calendario"
#define MENU_SELECT_BOOK "Seleziona libro"
#define MENU_ALARMS "Sveglie"
#define MENU_WEATHER "Meteo"
#define MENU_SETTINGS "Impostazioni"
#define MENU_HEART_MONITOR "Monitor cardiaco"
#define MENU_HEALTH "Salute"
#define MENU_STEPS_PER_DAY "Passi al giorno"
#define MENU_VAULT "Vault"
#define MENU_WIFI_TOOL "Strumenti WiFi"
#define MENU_EATING_APPLES "Mangia mele"
#define MENU_SMASHING_APPLES "Schiaccia mele"
#define MENU_GAMES "Giochi"
#define MENU_CREDITS "Crediti"
#define MENU_WIFI "WiFi"
#define MENU_DEBUG "Debug"
#define MENU_POWER_SETTINGS "Imp. alimentazione"
#define MENU_CHANGE_WATCHFACE "Cambia quadrante"
#define MENU_UI "UI"
#define MENU_INVERT_COLORS "Inverti colori"
#define MENU_INVERT_SCREEN "Inverti schermo"
#define MENU_INVERT_WATCHFACE "Inverti quadrante"
#define MENU_TETRIS "Blockchy"
#define MENU_PONG "Pong"
#define MENU_SNAKE "Serpente"
#define MENU_JUMPER "Saltatore"
#define MENU_DICE "Dadi"
#define MENU_VIDEO_PLAYER "Lettore video"
#define MENU_CONWAY "Conway"
#define MENU_PARTY "Party"
#define MENU_VIBRATIONS_DIS "Vibrazioni disabilitate"
#define MENU_WAKE_UP_DIS "Risveglio disabilitato"
#define MENU_POWER "Menu alimentazione"

// APP AND BOOK TRANSLATIONS

// Book reader messages
#define BOOK_NO_SELECTED "Nessun libro selezionato"

// Calendar messages
#define CALENDAR_NO_DATA "Nessun dato calendario"

// BLE Scanner
#define BLE_SCANNER_TITLE "Scanner BLE"
#define BLE_SCANNER_SCANNING "Scansione..."
#define BLE_SCANNER_NONE "Nessun dispositivo"
#define BLE_SCANNER_CHOOSE "Scegli dispositivo"
#define BLE_SCANNER_CONNECT "Connessione a "
#define BLE_SCANNER_CONNECTED "Connesso a "
#define BLE_SCANNER_FAILED "Connessione fallita con "

// Notes
#define NOTES_TITLE "Note"
#define NOTES_NEW "Nuova nota"
#define NOTES_DELETE "Elimina nota"
#define NOTES_CONNECT_KBD "Connetti tastiera"
#define NOTES_CONNECTED "Connesso: "

// HEART MONITOR TRANSLATIONS

#define HEART_MONITOR_TITLE "Frequenza cardiaca"
#define HEART_MONITOR_NOT_MEDICAL "Non per uso medico"
#define HEART_MONITOR_PLACE_DEVICE "Posiziona sul cuore"
#define HEART_MONITOR_CLICK_MENU "Poi premi menu"
#define HEART_MONITOR_START_3S "Inizia tra 3 s"
#define HEART_MONITOR_TAKES_15S "Dura 15 s"
#define HEART_MONITOR_BREATHE_SLOWLY "Respira lentamente"
#define HEART_MONITOR_TIME_LEFT "Tempo rimanente: "
#define HEART_MONITOR_BPM "bpm"

// Steps translations
#define STEPS_NOT_AVAILABLE "Passi non disponibili"
#define STEPS_DATE_WRONG "Data errata?"
#define STEPS_CORRUPTED "Passi corrotti"
#define DAILY_STEPS_TITLE "Passi giornalieri"
#define STEPS_CHART_TITLE "Passi/(24h)"

// VIDEO PLAYER TRANSLATIONS
#define VIDEO_PLAYER_CHOOSE "Scegli video"

// WATCHFACE SELECTION TRANSLATIONS
#define WATCHFACE_SELECT_TITLE "Seleziona quadrante"

// TETRIS GAME TRANSLATIONS
#define TETRIS_LINE_CLEARS "Linee:"
#define TETRIS_SCORE "Punti:"
#define TETRIS_LEVEL "Livello:"
#define TETRIS_CONTROLS_UP "Su:Dx"
#define TETRIS_CONTROLS_DOWN "Giu:Sx"
#define TETRIS_GAME_OVER "FINE GIOCO"

// DEBUG MENU TRANSLATIONS

// Debug menu titles
#define DEBUG_MENU_ACC "Menu debug: Acc"
#define DEBUG_MENU_BATTERY "Menu debug: Batt"
#define DEBUG_MENU_CLOCK "Menu debug: Orologio"
#define DEBUG_MENU_GIT "Menu debug: Git"
#define DEBUG_MENU_HARDWARE "Menu debug: Hardware"
#define DEBUG_MENU_WIFI "Menu debug: WiFi"
#define DEBUG_MENU_MOTOR "Menu debug: Motore"

// Debug menu items (short names for main debug menu)
#define DEBUG_ITEM_CLOCK "Orologio"
#define DEBUG_ITEM_HARDWARE "Hardware"
#define DEBUG_ITEM_BATTERY "Batteria"
#define DEBUG_ITEM_GIT "Git"
#define DEBUG_ITEM_ACC "Acc"
#define DEBUG_ITEM_MOTOR "Motore"
#define DEBUG_ITEM_FONT_PREVIEW "Anteprima font"

// Accelerometer debug
#define DEBUG_ACC_DISABLED "disabilitato"
#define DEBUG_ACC_FAILED_INIT "Impossibile inizializzare Acc"
#define DEBUG_ACC_DAMAGED "Acc danneggiato"
#define DEBUG_ACC_WORKING "Acc funzionante"
#define DEBUG_ACC_CLICK_3D "Anteprima 3D"
#define DEBUG_ACC_X "Accel X: "
#define DEBUG_ACC_Y "Accel Y: "
#define DEBUG_ACC_Z "Accel Z: "
#define DEBUG_ACC_PURE_X "Accel puro X: "
#define DEBUG_ACC_PURE_Y "Accel puro Y: "
#define DEBUG_ACC_PURE_Z "Accel puro Z: "
#define DEBUG_ACC_STEPS "Passi: "
#define DEBUG_ACC_IC "IC: "
#define DEBUG_ACC_ERROR_CODE "Codice errore: "
#define DEBUG_ACC_STATUS_CODE "Codice stato: "

// Battery debug
#define DEBUG_BATTERY_CURRENT_V "Tensione att.: "
#define DEBUG_BATTERY_MINIMUM_V "Tensione min.: "
#define DEBUG_BATTERY_MAXIMUM_V "Tensione max.: "
#define DEBUG_BATTERY_CRITICAL_V "Tensione crit.: "
#define DEBUG_BATTERY_LEVEL "Livello %: "
#define DEBUG_BATTERY_CHARGING "In carica: "
#define DEBUG_BATTERY_FULLY "Carica completa: "

// Clock debug
#define DEBUG_CLOCK_DRIFT_SYNCS "Divergenza tra sincr.:"
#define DEBUG_CLOCK_NOT_AVAILABLE "Non disponibile"
#define DEBUG_CLOCK_LAST_SYNC "Ultima sincr.:"
#define DEBUG_CLOCK_PREVIOUS_SYNC "Sincr. precedente:"
#define DEBUG_CLOCK_LAST_CHARGE "Ultima carica:"
#define DEBUG_CLOCK_REMOVE_FILES "Rimuovi file orologio"
#define DEBUG_CLOCK_TIMEZONE "Fuso orario:"
#define DEBUG_CLOCK_OLSON "Fuso Olson: "
#define DEBUG_CLOCK_DRIFT_VALUES "Valori deriva:"

// Git debug
#define DEBUG_GIT_COMMIT_HASH "Hash commit:"
#define DEBUG_GIT_BRANCH "Branch:"
#define DEBUG_GIT_BUILD_TIME "Ora build:"

// Hardware debug
#define DEBUG_HW_CHIP_MODEL "Mod. chip:"
#define DEBUG_HW_RTC_TYPE "Tipo RTC: "
#define DEBUG_HW_USED_HEAP "Heap usata KB: "
#define DEBUG_HW_CPU_TEMP "Temp CPU: "
#define DEBUG_HW_INIT_TEMP "Temp iniziale: "

// Motor debug
#define DEBUG_MOTOR_VIB_DIS "Vibrazioni disabilitate: "
#define DEBUG_MOTOR_VIB_POWER "Potenza vibrazione: "
#define DEBUG_MOTOR_VIB_TIME "Durata vibrazione: "
#define DEBUG_MOTOR_BATT_V "Tensione batteria: "
#define DEBUG_MOTOR_SET_POWER "Cambia potenza"
#define DEBUG_MOTOR_SET_TIME "Cambia durata"

// RTC Types
#define DEBUG_RTC_UNKNOWN "Sconosciuto"
#define DEBUG_RTC_DS3231 "DS3231"
#define DEBUG_RTC_PCF8563 "PCF8563"
#define DEBUG_RTC_INTERNAL "INT"
#define DEBUG_RTC_INVALID "Tipo non valido"

// WiFi debug
#define DEBUG_WIFI_MAC_ADDRESS "Indirizzo MAC:"
#define DEBUG_WIFI_STATUS "Stato WiFi: "
#define DEBUG_WIFI_IP "IP: "
#define DEBUG_WIFI_SSID "SSID: "
#define DEBUG_WIFI_SIGNAL "Segnale WiFi: "
#define DEBUG_WIFI_CONNECTING "Connessione: "
#define DEBUG_WIFI_ON "ON"
#define DEBUG_WIFI_OFF "OFF"

// Common debug terms
#define DEBUG_COMMON_PERCENT "%"
#define DEBUG_COMMON_COLON ": "

// GAME AND APP TRANSLATIONS

// Pong game
#define PONG_YOU_LOST "Hai perso!"

// Jumper game
#define JUMPER_LOST "Hai perso! Punti: "

// Party app
#define PARTY_MESSAGE "Festa festa"

// Vault app
#define VAULT_TITLE "Vault"
#define VAULT_KEY_INCORRECT "Chiave errata"
#define VAULT_EMPTY "Vault vuoto?"

// ALARM TRANSLATIONS

// Alarm menu titles
#define ALARM_MENU_EDIT_DAYS "Modifica giorni"
#define ALARM_MENU_EDIT_ALARM "Modifica sveglia"
#define ALARM_MENU_ALARMS "Sveglie"
#define ALARM_MENU_BROWSE_ALARMS "Sfoglia sveglie"
#define ALARM_MENU_QUICK_ALARMS_SET "Imposta sveglie rapide"
#define ALARM_MENU_QUICK_ALARM "Sveglia rapida"
#define ALARM_MENU_POMODORO "Pomodoro"

// Alarm status and settings
#define ALARM_STATUS_ENABLED "Abilitata"
#define ALARM_STATUS_DISABLED "Disabilitata"
#define ALARM_LABEL_TIME "Ora: "
#define ALARM_LABEL_ONE_TIME_ALARM "Sveglia singola"
#define ALARM_LABEL_DAYS "Giorni: "
#define ALARM_LABEL_REQUIRE_WIFI "Richiede WiFi"

// Quick alarm messages
#define ALARM_QUICK_ALERT_TITLE "Avviso sveglia rapida"
#define ALARM_QUICK_ALARM_SET_FOR "Sveglia impostata per:"
#define ALARM_QUICK_WILL_RING_AT "Suonera alle:"

// Day names (full)
#define ALARM_DAY_MONDAY "Lunedi"
#define ALARM_DAY_TUESDAY "Martedi"
#define ALARM_DAY_WEDNESDAY "Mercoledi"
#define ALARM_DAY_THURSDAY "Giovedi"
#define ALARM_DAY_FRIDAY "Venerdi"
#define ALARM_DAY_SATURDAY "Sabato"
#define ALARM_DAY_SUNDAY "Domenica"

// Alarm info strings
#define ALARM_INFO_ALL_DAYS "Tutti i giorni"
#define ALARM_INFO_NEVER "Mai"
#define ALARM_INFO_ONCE ", una volta"
#define ALARM_INFO_WIFI ", WiFi"
#define ALARM_INFO_QUICK ", rapida"
#define ALARM_INFO_POMODORO ", pomodoro"

// Pomodoro strings
#define POMODORO_STATUS "Stato: "
#define POMODORO_RUNNING "in esecuzione, "
#define POMODORO_WORKING "lavoro, "
#define POMODORO_PAUSE "pausa, "
#define POMODORO_ITER "iterazione: "
#define POMODORO_DISABLED "disabilitato"
#define POMODORO_RESET_TURN_OFF "Reset e spegni"
#define POMODORO_START "Avvia"

// Set Clock GUI
#define SETCLOCK_SET_TIME "Imposta ora"
#define SETCLOCK_SET_DATE "Imposta data"
#define SETCLOCK_TIMEZONE_MENU "Menu fuso orario"
#define SETCLOCK_CLOCK_SETTINGS "Impostazioni orologio"
#define SETCLOCK_CURRENT_TIMEZONE "Fuso orario attuale: "
#define SETCLOCK_CLEAR_TIMEZONE "Cancella fuso orario"
#define SETCLOCK_SET_LOCATION_TIMEZONE "Imposta fuso orario per posizione"
#define SETCLOCK_SET_UTC_TIMEZONE "Imposta fuso orario UTC"
#define SETCLOCK_TIMEZONE_SETTINGS "Impostazioni fuso orario"
#define SETCLOCK_TIMEZONE_WARNING "Questo menu ignora l'ora legale/solare e sovrascrivera il fuso orario, che potrebbe gia includerla.\nFare clic sul pulsante Indietro per confermare"
#define SETCLOCK_WARNING_TITLE "Avviso"
#define SETCLOCK_SELECT_TIMEZONE "Seleziona fuso orario"

// Time unit translations (for automatic formatting)
#define TIME_UNIT_MINUTE "minuto"
#define TIME_UNIT_MINUTES "minuti"
#define TIME_UNIT_HOUR "ora"
#define TIME_UNIT_HOURS "ore"
#define TIME_UNIT_DAY "giorno"
#define TIME_UNIT_DAYS "giorni"
#define TIME_UNIT_AND "e"
