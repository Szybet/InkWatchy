#pragma once

// ==============================================================================
// ITALIAN TRANSLATIONS
// ==============================================================================

// Slate watchface weather conditions
#define WF_S_WEATHER_CLEAR_SKY "Cielo sereno"
#define WF_S_WEATHER_MOSTLY_CLEAR "Per lo più sereno"
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
#define WEATHER_AQ_NOT_AVAILABLE "Meteo e qualita dell'aria non disponibili"

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
#define WEATHER_MENU_UV_INDEX_CLEAR_SKY "UV Cielo Sereno"
#define WEATHER_MENU_WET_BULB "Temp Bulbo Umido"
#define WEATHER_MENU_CAPE "CAPE"
#define WEATHER_MENU_DEW_POINT "Punto Rugiada"
#define WEATHER_MENU_PRECIPITATION_AMOUNT "Precipitazioni"
#define WEATHER_MENU_SNOW_DEPTH "Spessore Neve"
#define WEATHER_MENU_LIFTED_INDEX "Indice Sollevato"
#define WEATHER_MENU_CONVECTIVE_INHIBITION "Inib Convettiva"
#define WEATHER_MENU_TERRESTRIAL_RADIATION "Rad Terrestre"
#define WEATHER_MENU_DIFFUSE_RADIATION "Rad Diffusa"
#define WEATHER_MENU_DIRECT_RADIATION "Rad Diretta"
#define WEATHER_MENU_TOTAL_WATER_VAPOUR "Vapore Acqueo Tot"

// Weather menu titles and messages
#define WEATHER_SELECT_DATE "Seleziona data"
#define WEATHER_STAT_TITLE "Statistiche meteo"
#define WEATHER_NOT_AVAILABLE "Meteo non disponibile"
#define WEATHER_DATE_WRONG "Data errata?"
#define WEATHER_CORRUPTED "Dati meteo corrotti"

// Chart titles with time notation
#define WEATHER_CHART_TEMP "Temp/°C"
#define WEATHER_CHART_PRESSURE "Pressione/hPa"
#define WEATHER_CHART_HUMIDITY "Umidita/%"
#define WEATHER_CHART_CLOUDS "Nuvole/%"
#define WEATHER_CHART_WIND_SPEED "Velocità del vento/km/h"
#define WEATHER_CHART_WIND_GUSTS "Raffiche di vento/km/h"
#define WEATHER_CHART_VISIBILITY "Visib./m"
#define WEATHER_CHART_PRECIPITATION "Prec./%"
#define WEATHER_CONDITIONS_TITLE "Condizioni meteo"
#define WEATHER_CHART_UV_INDEX "UV Index"
#define WEATHER_CHART_UV_INDEX_CLEAR_SKY "UV CieloSer"
#define WEATHER_CHART_WET_BULB "Bulbo Um C"
#define WEATHER_CHART_CAPE "CAPE J/kg"
#define WEATHER_CHART_DEW_POINT "P Rugiada C"
#define WEATHER_CHART_PRECIPITATION_AMOUNT "Prec. Tot"
#define WEATHER_CHART_SNOW_DEPTH "Altezza Neve"
#define WEATHER_CHART_LIFTED_INDEX "Ind Sollev"
#define WEATHER_CHART_CONVECTIVE_INHIBITION "Inib Conv"
#define WEATHER_CHART_TERRESTRIAL_RADIATION "Rad Terr"
#define WEATHER_CHART_DIFFUSE_RADIATION "Rad Diff"
#define WEATHER_CHART_DIRECT_RADIATION "Rad Dir"
#define WEATHER_CHART_TOTAL_WATER_VAPOUR "Vapor Acq Tot"

// Air quality submenu items
#define AIR_MENU_EUAQI "AQI Europeo"
#define AIR_MENU_USAQI "US AQI"
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

// Air quality meni titles and messages
#define AIR_QUALITY_SELECT_DATE "Seleziona data"
#define AIR_QUALITY_STAT_TITLE "Stat qualita aria"
#define AIR_QUALITY_NOT_AVAILABLE "Qualita aria non disp"
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
#define AIR_CHART_SEA_SALT_AEROSOL "Sale Marino/ug/m3"
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
#define MENU_HEART_MONITOR "ECG"
#define MENU_HEALTH "Salute"
#define MENU_STEPS_PER_DAY "Passi al giorno"
#define MENU_VAULT "Vault"
#define MENU_WIFI_TOOL "Tool Wi‑Fi"
#define MENU_EATING_APPLES "Mangia mele"
#define MENU_SMASHING_APPLES "Schiaccia mele"
#define MENU_GAMES "Giochi"
#define MENU_CREDITS "Crediti"
#define MENU_WIFI "Wi‑Fi"
#define MENU_DEBUG "Debug"
#define MENU_POWER_SETTINGS "Imp. alimentazione"
#define MENU_CHANGE_WATCHFACE "Cambia quadrante"
#define MENU_TETRIS "Blockchy"
#define MENU_PONG "Pong"
#define MENU_SNAKE "Serpente"
#define MENU_JUMPER "Saltatore"
#define MENU_VIDEO_PLAYER "Lettore video"
#define MENU_CONWAY "Conway"
#define MENU_PARTY "Party"
#define MENU_VIBRATIONS_DIS "Vibrazioni disabilitate"
#define MENU_WAKE_UP_DIS "Risveglio disabilitato"
#define MENU_POWER "Menu alimentazione"

// ==============================================================================
// APP AND BOOK TRANSLATIONS
// ==============================================================================

// Book reader messages
#define BOOK_NO_SELECTED "Nessun libro selezionato"

// Calendar messages
#define CALENDAR_NO_DATA "Nessun dato calendario"

// ==============================================================================
// HEART MONITOR TRANSLATIONS
// ==============================================================================

#define HEART_MONITOR_TITLE "Misurazione frequenza cardiaca"
#define HEART_MONITOR_NOT_MEDICAL "Non per uso medico"
#define HEART_MONITOR_PLACE_DEVICE "Posiziona il dispositivo sul cuore"
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

// ==============================================================================
// VIDEO PLAYER TRANSLATIONS
// ==============================================================================

#define VIDEO_PLAYER_CHOOSE "Scegli video"

// ==============================================================================
// WATCHFACE SELECTION TRANSLATIONS
// ==============================================================================

#define WATCHFACE_SELECT_TITLE "Seleziona quadrante"

// ==============================================================================
// TETRIS GAME TRANSLATIONS
// ==============================================================================

// Tetris UI labels
#define TETRIS_LINE_CLEARS "Lines clr:"
#define TETRIS_SCORE "Score:"
#define TETRIS_LEVEL "Level:"
#define TETRIS_CONTROLS_UP "Up:Right"
#define TETRIS_CONTROLS_DOWN "Down:Left"
#define TETRIS_GAME_OVER "GAMEOVER"

// ==============================================================================
// DEBUG MENU TRANSLATIONS
// ==============================================================================

// Debug menu titles
#define DEBUG_MENU_ACC "Menu debug: Acc"
#define DEBUG_MENU_BATTERY "Menu debug: Batt"
#define DEBUG_MENU_CLOCK "Menu debug: Orolog"
#define DEBUG_MENU_GIT "Menu debug: Git"
#define DEBUG_MENU_HARDWARE "Menu debug: Hardware"
#define DEBUG_MENU_WIFI "Menu debug: Wi‑Fi"
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
#define DEBUG_CLOCK_DRIFT_SYNCS "Divergenza tra le sincr.:"
#define DEBUG_CLOCK_NOT_AVAILABLE "Non disponibile"
#define DEBUG_CLOCK_LAST_SYNC "Ultima sincronizzazione:"
#define DEBUG_CLOCK_PREVIOUS_SYNC "Sincronizzazione precedente:"
#define DEBUG_CLOCK_LAST_CHARGE "Ultima carica:"
#define DEBUG_CLOCK_REMOVE_FILES "Rimuovi file orologio"

// Git debug
#define DEBUG_GIT_COMMIT_HASH "Commit hash:"
#define DEBUG_GIT_BRANCH "Branch:"
#define DEBUG_GIT_BUILD_TIME "Ora build:"

// Hardware debug
#define DEBUG_HW_CHIP_MODEL "Mod chip:"
#define DEBUG_HW_RTC_TYPE "Tipo RTC: "
#define DEBUG_HW_USED_HEAP "Heap usata KB: "
#define DEBUG_HW_CPU_TEMP "Temp CPU: "
#define DEBUG_HW_INIT_TEMP "Temp iniziale: "

// Motor debug
#define DEBUG_MOTOR_VIB_DIS "Vibrazioni disabilitate: "
#define DEBUG_MOTOR_VIB_POWER "Potenza vibrazione selezionata: "
#define DEBUG_MOTOR_VIB_TIME "Tempo vibrazione selezionato: "
#define DEBUG_MOTOR_BATT_V "Tensione batteria: "
#define DEBUG_MOTOR_SET_POWER "Cambia potenza"
#define DEBUG_MOTOR_SET_TIME "Cambia ora"

// RTC Types
#define DEBUG_RTC_UNKNOWN "Sconosciuto"
#define DEBUG_RTC_DS3231 "DS3231"
#define DEBUG_RTC_PCF8563 "PCF8563"
#define DEBUG_RTC_INTERNAL "INT"
#define DEBUG_RTC_INVALID "Tipo non valido"

// WiFi debug
#define DEBUG_WIFI_MAC_ADDRESS "Indirizzo MAC:"
#define DEBUG_WIFI_STATUS "Stato Wi‑Fi: "
#define DEBUG_WIFI_IP "IP: "
#define DEBUG_WIFI_SSID "SSID: "
#define DEBUG_WIFI_SIGNAL "Segnale Wi‑Fi: "
#define DEBUG_WIFI_CONNECTING "Connessione: "
#define DEBUG_WIFI_ON "ON"
#define DEBUG_WIFI_OFF "OFF"

// Common debug terms
#define DEBUG_COMMON_PERCENT "%"
#define DEBUG_COMMON_COLON ": "

// ==============================================================================
// GAME AND APP TRANSLATIONS
// ==============================================================================

// Pong game
#define PONG_YOU_LOST "You lost!"

// Party app
#define PARTY_MESSAGE "Party party"

// Vault app
#define VAULT_KEY_INCORRECT "Key is incorrect"
#define VAULT_EMPTY "Vault is empty?"

// ==============================================================================
// ALARM TRANSLATIONS
// ==============================================================================

// Alarm menu titles
#define ALARM_MENU_EDIT_DAYS "Modifica giorni"
#define ALARM_MENU_EDIT_ALARM "Modifica sveglia"
#define ALARM_MENU_ALARMS "Sveglie"
#define ALARM_MENU_BROWSE_ALARMS "Sfoglia sveglie"
#define ALARM_MENU_QUICK_ALARMS_SET "Sveglie rapidi impostati"
#define ALARM_MENU_QUICK_ALARM "Sveglie rapido"
#define ALARM_MENU_POMODORO "Pomodoro"

// Alarm status and settings
#define ALARM_STATUS_ENABLED "Abilitato"
#define ALARM_STATUS_DISABLED "Disabilitato"
#define ALARM_LABEL_TIME "Ora: "
#define ALARM_LABEL_ONE_TIME_ALARM "Sveglia singolo"
#define ALARM_LABEL_DAYS "Giorni: "
#define ALARM_LABEL_REQUIRE_WIFI "Richiede Wi‑Fi"

// Quick alarm messages
#define ALARM_QUICK_ALERT_TITLE "Avviso sveglia rapido"
#define ALARM_QUICK_ALARM_SET_FOR "Sveglia impostato per:"
#define ALARM_QUICK_WILL_RING_AT "Suonerà alle:"

// Day names (full)
#define ALARM_DAY_MONDAY "Lunedì"
#define ALARM_DAY_TUESDAY "Martedì"
#define ALARM_DAY_WEDNESDAY "Mercoledì"
#define ALARM_DAY_THURSDAY "Giovedì"
#define ALARM_DAY_FRIDAY "Venerdì"
#define ALARM_DAY_SATURDAY "Sabato"
#define ALARM_DAY_SUNDAY "Domenica"

// Alarm info strings
#define ALARM_INFO_ALL_DAYS "Tutti i giorni"
#define ALARM_INFO_NEVER "Mai"
#define ALARM_INFO_ONCE ", una volta"
#define ALARM_INFO_WIFI ", WiFi"
#define ALARM_INFO_QUICK ", rapido"
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
