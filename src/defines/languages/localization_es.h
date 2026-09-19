#pragma once

// SPANISH TRANSLATIONS

// Slate watchface weather conditions
#define WF_S_WEATHER_CLEAR_SKY "Cielo despejado"
#define WF_S_WEATHER_MOSTLY_CLEAR "Mayormente despejado"
#define WF_S_WEATHER_PARTLY_CLOUDY "Parcialmente nublado"
#define WF_S_WEATHER_OVERCAST "Nublado"
#define WF_S_WEATHER_FOG "Niebla"
#define WF_S_WEATHER_HEAVY_FOG "Niebla densa"
#define WF_S_WEATHER_LIGHT_DRIZZLE "Llovizna ligera"
#define WF_S_WEATHER_DRIZZLE "Llovizna"
#define WF_S_WEATHER_HEAVY_DRIZZLE "Llovizna fuerte"
#define WF_S_WEATHER_LIGHT_FREEZING "Helada ligera"
#define WF_S_WEATHER_HEAVY_FREEZING "Helada fuerte"
#define WF_S_WEATHER_LIGHT_RAIN "Lluvia ligera"
#define WF_S_WEATHER_RAIN "Lluvia"
#define WF_S_WEATHER_HEAVY_RAIN "Lluvia fuerte"
#define WF_S_WEATHER_FREEZING_RAIN "Lluvia helada"
#define WF_S_WEATHER_FREEZING_HARD "Helada intensa"
#define WF_S_WEATHER_LIGHT_SNOW "Nieve ligera"
#define WF_S_WEATHER_SNOW "Nieve"
#define WF_S_WEATHER_HEAVY_SNOW "Nieve fuerte"
#define WF_S_WEATHER_SNOW_GRAINS "Granizo menudo"
#define WF_S_WEATHER_LIGHT_SHOWERS "Chubascos ligeros"
#define WF_S_WEATHER_SHOWERS "Chubascos"
#define WF_S_WEATHER_HEAVY_SHOWERS "Chubascos fuertes"
#define WF_S_WEATHER_SNOW_SHOWERS "Chubascos de nieve"
#define WF_S_WEATHER_THUNDERSTORM "Tormenta"
#define WF_S_WEATHER_LIGHT_HAIL "Granizo ligero"
#define WF_S_WEATHER_HEAVY_HAIL "Granizo fuerte"
#define WF_S_WEATHER_UNKNOWN "Desconocido"

// Day names (abbreviated)
#define WF_S_DAY_MON "LUN"
#define WF_S_DAY_TUE "MAR"
#define WF_S_DAY_WED "MIE"
#define WF_S_DAY_THU "JUE"
#define WF_S_DAY_FRI "VIE"
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
#define WEATHER_AQ_MENU_TITLE "Tipo de datos"
#define WEATHER_AQ_MENU_WEATHER "Clima"
#define WEATHER_AQ_MENU_AIR_QUALITY "Calidad de aire"
#define WEATHER_AQ_NOT_AVAILABLE "Clima/Calidad no disp."

// Weather submenu items
#define WEATHER_MENU_TEMPERATURE "Temperatura"
#define WEATHER_MENU_PRESSURE "Presion"
#define WEATHER_MENU_HUMIDITY "Humedad"
#define WEATHER_MENU_CONDITIONS "Condiciones clima"
#define WEATHER_MENU_CLOUDINESS "Nubosidad"
#define WEATHER_MENU_WIND_SPEED "Velocidad viento"
#define WEATHER_MENU_WIND_GUSTS "Rafagas viento"
#define WEATHER_MENU_VISIBILITY "Visibilidad"
#define WEATHER_MENU_PRECIPITATION "% precipitacion"
#define WEATHER_MENU_UV_INDEX "Indice UV"
#define WEATHER_MENU_UV_INDEX_CLEAR_SKY "UV Cielo desp."
#define WEATHER_MENU_WET_BULB "Temp. bulbo hum."
#define WEATHER_MENU_CAPE "CAPE"
#define WEATHER_MENU_DEW_POINT "Punto de rocio"
#define WEATHER_MENU_PRECIPITATION_AMOUNT "Precipitacion"
#define WEATHER_MENU_SNOW_DEPTH "Espesor de nieve"
#define WEATHER_MENU_LIFTED_INDEX "Indice Lifted"
#define WEATHER_MENU_CONVECTIVE_INHIBITION "Inhib. convectiva"
#define WEATHER_MENU_TERRESTRIAL_RADIATION "Rad. terrestre"
#define WEATHER_MENU_DIFFUSE_RADIATION "Rad. difusa"
#define WEATHER_MENU_DIRECT_RADIATION "Rad. directa"
#define WEATHER_MENU_TOTAL_WATER_VAPOUR "Vapor agua total"

// Weather menu titles and messages
#define WEATHER_SELECT_DATE "Seleccionar fecha"
#define WEATHER_STAT_TITLE "Estad. clima"
#define WEATHER_NOT_AVAILABLE "Clima no disponible"
#define WEATHER_DATE_WRONG "Fecha incorrecta?"
#define WEATHER_CORRUPTED "Clima corrupto"

// Chart titles with time notation
#define WEATHER_CHART_TEMP "Temp/C"
#define WEATHER_CHART_PRESSURE "Presion/hPa"
#define WEATHER_CHART_HUMIDITY "Humedad/%"
#define WEATHER_CHART_CLOUDS "Nubes/%"
#define WEATHER_CHART_WIND_SPEED "Vel. viento/km/h"
#define WEATHER_CHART_WIND_GUSTS "Rafagas/km/h"
#define WEATHER_CHART_VISIBILITY "Visib./m"
#define WEATHER_CHART_PRECIPITATION "Precip./%"
#define WEATHER_CONDITIONS_TITLE "Condiciones clima"
#define WEATHER_CHART_UV_INDEX "Indice UV"
#define WEATHER_CHART_UV_INDEX_CLEAR_SKY "UV Cielo desp."
#define WEATHER_CHART_WET_BULB "Bulbo hum./C"
#define WEATHER_CHART_CAPE "CAPE/J/kg"
#define WEATHER_CHART_DEW_POINT "Pto. rocio/C"
#define WEATHER_CHART_PRECIPITATION_AMOUNT "Precip. total"
#define WEATHER_CHART_SNOW_DEPTH "Espesor nieve"
#define WEATHER_CHART_LIFTED_INDEX "Indice Lifted"
#define WEATHER_CHART_CONVECTIVE_INHIBITION "Inhib. conv."
#define WEATHER_CHART_TERRESTRIAL_RADIATION "Rad. terr."
#define WEATHER_CHART_DIFFUSE_RADIATION "Rad. difusa"
#define WEATHER_CHART_DIRECT_RADIATION "Rad. directa"
#define WEATHER_CHART_TOTAL_WATER_VAPOUR "Vapor agua tot."

// Air quality submenu items
#define AIR_MENU_EUAQI "AQI Europeo"
#define AIR_MENU_USAQI "AQI EE.UU."
#define AIR_MENU_EU_AQI_PM2_5 "EU AQI PM 2,5"
#define AIR_MENU_EU_AQI_PM10 "EU AQI PM 10"
#define AIR_MENU_EU_AQI_NO2 "EU AQI NO2"
#define AIR_MENU_EU_AQI_O3 "EU AQI O3"
#define AIR_MENU_EU_AQI_SO2 "EU AQI SO2"
#define AIR_MENU_PM2_5 "Particulas PM 2,5"
#define AIR_MENU_PM10 "Particulas PM 10"
#define AIR_MENU_CARBON_MONOXIDE "Monoxido carbono"
#define AIR_MENU_CARBON_DIOXIDE "Dioxido carbono"
#define AIR_MENU_NITROGEN_DIOXIDE "Dioxido nitrogeno"
#define AIR_MENU_SULPHUR_DIOXIDE "Dioxido azufre"
#define AIR_MENU_OZONE "Ozono"
#define AIR_MENU_AEROSOL_OPTICAL_DEPTH "Espesor aerosol"
#define AIR_MENU_DUST "Polvo"
#define AIR_MENU_METHANE "Metano"
#define AIR_MENU_FORMALDEHYDE "Formaldehido"
#define AIR_MENU_GLYOXAL "Glioxal"
#define AIR_MENU_SEA_SALT_AEROSOL "Aerosol sal marina"
#define AIR_MENU_NITROGEN_MONOXIDE "Monoxido nitrogeno"
#define AIR_MENU_PEROXYACYL_NITRATES "Nitratos peroxiacilo"

// Air quality menu titles and messages
#define AIR_QUALITY_SELECT_DATE "Seleccionar fecha"
#define AIR_QUALITY_STAT_TITLE "Estad. calidad aire"
#define AIR_QUALITY_NOT_AVAILABLE "Calidad no disponible"
#define AIR_QUALITY_DATE_WRONG "Fecha incorrecta?"
#define AIR_QUALITY_CORRUPTED "Calidad corrupta"

// Air quality chart titles with time notation
#define AIR_CHART_EUAQI "Indice Calidad Aire EU"
#define AIR_CHART_USAQI "Indice Calidad Aire US"
#define AIR_CHART_EU_AQI_PM2_5 "PM2.5/ug/m3"
#define AIR_CHART_EU_AQI_PM10 "PM10/ug/m3"
#define AIR_CHART_EU_AQI_NO2 "NO2/ug/m3"
#define AIR_CHART_EU_AQI_O3 "Oxido/ug/m3"
#define AIR_CHART_EU_AQI_SO2 "SO2/ug/m3"
#define AIR_CHART_PM2_5 "PM2.5/ug/m3"
#define AIR_CHART_PM10 "PM10/ug/m3"
#define AIR_CHART_CARBON_MONOXIDE "CO/ug/m3"
#define AIR_CHART_CARBON_DIOXIDE "CO2/ppm"
#define AIR_CHART_NITROGEN_DIOXIDE "NO2/ug/m3"
#define AIR_CHART_SULPHUR_DIOXIDE "SO2/ug/m3"
#define AIR_CHART_OZONE "O3/ug/m3"
#define AIR_CHART_AEROSOL_OPTICAL_DEPTH "AOD /"
#define AIR_CHART_DUST "Polvo/ug/m3"
#define AIR_CHART_METHANE "Metano/ug/m3"
#define AIR_CHART_FORMALDEHYDE "CH2O/ug/m3"
#define AIR_CHART_GLYOXAL "Glioxal/ug/m3"
#define AIR_CHART_SEA_SALT_AEROSOL "Sal marina/ug/m3"
#define AIR_CHART_NITROGEN_MONOXIDE "NO/ug/m3"
#define AIR_CHART_PEROXYACYL_NITRATES "PAN/ug/m3"

// Month names
#define LANGUAGE_MONTH_NAMES {"ENE", "FEB", "MAR", "ABR", "MAY", "JUN", "JUL", "AGO", "SEP", "OCT", "NOV", "DIC"}

// Menu translations
#define MENU_MAIN "Menu principal"
#define MENU_CALENDAR "Calendario"
#define MENU_SELECT_BOOK "Seleccionar libro"
#define MENU_ALARMS "Alarmas"
#define MENU_WEATHER "Clima"
#define MENU_SETTINGS "Ajustes"
#define MENU_HEART_MONITOR "Monitor cardiaco"
#define MENU_HEALTH "Salud"
#define MENU_STEPS_PER_DAY "Pasos por dia"
#define MENU_VAULT "Boveda"
#define MENU_WIFI_TOOL "Herramienta WiFi"
#define MENU_EATING_APPLES "Comiendo manzanas"
#define MENU_SMASHING_APPLES "Aplastando manzanas"
#define MENU_GAMES "Juegos"
#define MENU_CREDITS "Creditos"
#define MENU_WIFI "WiFi"
#define MENU_DEBUG "Depuracion"
#define MENU_POWER_SETTINGS "Energia"
#define MENU_CHANGE_WATCHFACE "Cambiar esfera"
#define MENU_INVERT_SCREEN "Invertir pantalla"
#define MENU_INVERT_WATCHFACE "Invertir esfera"
#define MENU_TETRIS "Blockchy"
#define MENU_PONG "Pong"
#define MENU_SNAKE "Serpiente"
#define MENU_JUMPER "Saltador"
#define MENU_DICE "Dados"
#define MENU_VIDEO_PLAYER "Reproductor video"
#define MENU_CONWAY "Conway"
#define MENU_PARTY "Fiesta"
#define MENU_VIBRATIONS_DIS "Vibracion desactivada"
#define MENU_WAKE_UP_DIS "Encendido desactivado"
#define MENU_POWER "Menu de energia"

// APP AND BOOK TRANSLATIONS

// Book reader messages
#define BOOK_NO_SELECTED "Ningun libro sel."

// Calendar messages
#define CALENDAR_NO_DATA "Sin datos calendario"

// BLE Scanner
#define BLE_SCANNER_TITLE "Escaner BLE"
#define BLE_SCANNER_SCANNING "Escaneando..."
#define BLE_SCANNER_NONE "Dispositivos no enc."
#define BLE_SCANNER_CHOOSE "Elegir dispositivo"
#define BLE_SCANNER_CONNECT "Conectando a "
#define BLE_SCANNER_CONNECTED "Conectado a "
#define BLE_SCANNER_FAILED "Error al conectar a "

// Notes
#define NOTES_TITLE "Notas"
#define NOTES_NEW "Nueva nota"
#define NOTES_DELETE "Eliminar nota"
#define NOTES_CONNECT_KBD "Conectar teclado"
#define NOTES_CONNECTED "Conectado: "

// HEART MONITOR TRANSLATIONS

#define HEART_MONITOR_TITLE "Monitor cardiaco"
#define HEART_MONITOR_NOT_MEDICAL "No para uso medico"
#define HEART_MONITOR_PLACE_DEVICE "Colocar en corazon"
#define HEART_MONITOR_CLICK_MENU "Luego pulsar menu"
#define HEART_MONITOR_START_3S "Iniciara en 3s"
#define HEART_MONITOR_TAKES_15S "Tarda 15s"
#define HEART_MONITOR_BREATHE_SLOWLY "Respira lentamente"
#define HEART_MONITOR_TIME_LEFT "Tiempo restante: "
#define HEART_MONITOR_BPM "ppm"

// Steps translations
#define STEPS_NOT_AVAILABLE "Pasos no disponibles"
#define STEPS_DATE_WRONG "Fecha incorrecta?"
#define STEPS_CORRUPTED "Pasos corruptos"
#define DAILY_STEPS_TITLE "Pasos diarios"
#define STEPS_CHART_TITLE "Pasos/(24h)"

// VIDEO PLAYER TRANSLATIONS
#define VIDEO_PLAYER_CHOOSE "Elegir video"

// WATCHFACE SELECTION TRANSLATIONS
#define WATCHFACE_SELECT_TITLE "Seleccionar esfera"

// TETRIS GAME TRANSLATIONS
#define TETRIS_LINE_CLEARS "Lineas:"
#define TETRIS_SCORE "Puntos:"
#define TETRIS_LEVEL "Nivel:"
#define TETRIS_CONTROLS_UP "Arr:Der"
#define TETRIS_CONTROLS_DOWN "Abj:Izq"
#define TETRIS_GAME_OVER "FIN DEL JUEGO"

// DEBUG MENU TRANSLATIONS

// Debug menu titles
#define DEBUG_MENU_ACC "Menu Depur.: Acc"
#define DEBUG_MENU_BATTERY "Menu Depur.: Bat"
#define DEBUG_MENU_CLOCK "Menu Depur.: Reloj"
#define DEBUG_MENU_GIT "Menu Depur.: Git"
#define DEBUG_MENU_HARDWARE "Menu Depur.: Hard"
#define DEBUG_MENU_WIFI "Menu Depur.: Wifi"
#define DEBUG_MENU_MOTOR "Menu Depur.: Motor"

// Debug menu items (short names for main debug menu)
#define DEBUG_ITEM_CLOCK "Reloj"
#define DEBUG_ITEM_HARDWARE "Hardware"
#define DEBUG_ITEM_BATTERY "Bateria"
#define DEBUG_ITEM_GIT "Git"
#define DEBUG_ITEM_ACC "Acc"
#define DEBUG_ITEM_MOTOR "Motor"
#define DEBUG_ITEM_FONT_PREVIEW "Vista fuentes"

// Accelerometer debug
#define DEBUG_ACC_DISABLED "desactivado"
#define DEBUG_ACC_FAILED_INIT "Fallo al iniciar Acc"
#define DEBUG_ACC_DAMAGED "Acc danado"
#define DEBUG_ACC_WORKING "Acc funcionando"
#define DEBUG_ACC_CLICK_3D "Vista previa 3D"
#define DEBUG_ACC_X "Acel. X: "
#define DEBUG_ACC_Y "Acel. Y: "
#define DEBUG_ACC_Z "Acel. Z: "
#define DEBUG_ACC_PURE_X "Acel. pura X: "
#define DEBUG_ACC_PURE_Y "Acel. pura Y: "
#define DEBUG_ACC_PURE_Z "Acel. pura Z: "
#define DEBUG_ACC_STEPS "Pasos: "
#define DEBUG_ACC_IC "IC: "
#define DEBUG_ACC_ERROR_CODE "Codigo error: "
#define DEBUG_ACC_STATUS_CODE "Codigo estado: "

// Battery debug
#define DEBUG_BATTERY_CURRENT_V "V actual: "
#define DEBUG_BATTERY_MINIMUM_V "V minimo: "
#define DEBUG_BATTERY_MAXIMUM_V "V maximo: "
#define DEBUG_BATTERY_CRITICAL_V "V critico: "
#define DEBUG_BATTERY_LEVEL "Nivel %: "
#define DEBUG_BATTERY_CHARGING "Cargando: "
#define DEBUG_BATTERY_FULLY "Completa: "

// Clock debug
#define DEBUG_CLOCK_DRIFT_SYNCS "Desvio entre sincr.:"
#define DEBUG_CLOCK_NOT_AVAILABLE "No disponible"
#define DEBUG_CLOCK_LAST_SYNC "Ultima sincr.:"
#define DEBUG_CLOCK_PREVIOUS_SYNC "Sincr. anterior:"
#define DEBUG_CLOCK_LAST_CHARGE "Ultima carga:"
#define DEBUG_CLOCK_REMOVE_FILES "Borrar arch. reloj"
#define DEBUG_CLOCK_TIMEZONE "Zona horaria:"
#define DEBUG_CLOCK_OLSON "Zona Olson: "
#define DEBUG_CLOCK_DRIFT_VALUES "Valores desvio:"

// Git debug
#define DEBUG_GIT_COMMIT_HASH "Hash commit:"
#define DEBUG_GIT_BRANCH "Rama:"
#define DEBUG_GIT_BUILD_TIME "Fecha comp.:"

// Hardware debug
#define DEBUG_HW_CHIP_MODEL "Modelo Chip:"
#define DEBUG_HW_RTC_TYPE "Tipo RTC: "
#define DEBUG_HW_USED_HEAP "Heap usada KB: "
#define DEBUG_HW_CPU_TEMP "Temp CPU: "
#define DEBUG_HW_INIT_TEMP "Temp inic: "

// Motor debug
#define DEBUG_MOTOR_VIB_DIS "Vibracion desact: "
#define DEBUG_MOTOR_VIB_POWER "Potencia vibracion: "
#define DEBUG_MOTOR_VIB_TIME "Tiempo vibracion: "
#define DEBUG_MOTOR_BATT_V "Voltaje bateria: "
#define DEBUG_MOTOR_SET_POWER "Cambiar potencia"
#define DEBUG_MOTOR_SET_TIME "Cambiar tiempo"

// RTC Types
#define DEBUG_RTC_UNKNOWN "Desconocido"
#define DEBUG_RTC_DS3231 "DS3231"
#define DEBUG_RTC_PCF8563 "PCF8563"
#define DEBUG_RTC_INTERNAL "INT"
#define DEBUG_RTC_INVALID "Tipo invalido"

// WiFi debug
#define DEBUG_WIFI_MAC_ADDRESS "Direccion MAC:"
#define DEBUG_WIFI_STATUS "Estado WiFi: "
#define DEBUG_WIFI_IP "IP: "
#define DEBUG_WIFI_SSID "SSID: "
#define DEBUG_WIFI_SIGNAL "Senal WiFi: "
#define DEBUG_WIFI_CONNECTING "Conectando: "
#define DEBUG_WIFI_ON "ENCENDIDO"
#define DEBUG_WIFI_OFF "APAGADO"

// Common debug terms
#define DEBUG_COMMON_PERCENT "%"
#define DEBUG_COMMON_COLON ": "

// GAME AND APP TRANSLATIONS

// Pong game
#define PONG_YOU_LOST "Perdiste!"

// Jumper game
#define JUMPER_LOST "Perdiste! Puntos: "

// Party app
#define PARTY_MESSAGE "Fiesta fiesta"

// Vault app
#define VAULT_TITLE "Boveda"
#define VAULT_KEY_INCORRECT "Clave incorrecta"
#define VAULT_EMPTY "Boveda vacia?"

// ALARM TRANSLATIONS

// Alarm menu titles
#define ALARM_MENU_EDIT_DAYS "Editar dias"
#define ALARM_MENU_EDIT_ALARM "Editar alarma"
#define ALARM_MENU_ALARMS "Alarmas"
#define ALARM_MENU_BROWSE_ALARMS "Explorar alarmas"
#define ALARM_MENU_QUICK_ALARMS_SET "Ajuste rapido alarmas"
#define ALARM_MENU_QUICK_ALARM "Alarma rapida"
#define ALARM_MENU_POMODORO "Pomodoro"

// Alarm status and settings
#define ALARM_STATUS_ENABLED "Activada"
#define ALARM_STATUS_DISABLED "Desactivada"
#define ALARM_LABEL_TIME "Hora: "
#define ALARM_LABEL_ONE_TIME_ALARM "Alarma de una vez"
#define ALARM_LABEL_DAYS "Dias: "
#define ALARM_LABEL_REQUIRE_WIFI "Requiere WiFi"

// Quick alarm messages
#define ALARM_QUICK_ALERT_TITLE "Alerta alarma rapida"
#define ALARM_QUICK_ALARM_SET_FOR "Alarma para:"
#define ALARM_QUICK_WILL_RING_AT "Sonara a las:"

// Day names (full)
#define ALARM_DAY_MONDAY "Lunes"
#define ALARM_DAY_TUESDAY "Martes"
#define ALARM_DAY_WEDNESDAY "Miercoles"
#define ALARM_DAY_THURSDAY "Jueves"
#define ALARM_DAY_FRIDAY "Viernes"
#define ALARM_DAY_SATURDAY "Sabado"
#define ALARM_DAY_SUNDAY "Domingo"

// Alarm info strings
#define ALARM_INFO_ALL_DAYS "Todos los dias"
#define ALARM_INFO_NEVER "Nunca"
#define ALARM_INFO_ONCE ", una vez"
#define ALARM_INFO_WIFI ", wifi"
#define ALARM_INFO_QUICK ", rapida"
#define ALARM_INFO_POMODORO ", pomodoro"

// Pomodoro strings
#define POMODORO_STATUS "Estado: "
#define POMODORO_RUNNING "en curso, "
#define POMODORO_WORKING "trabajando, "
#define POMODORO_PAUSE "pausa, "
#define POMODORO_ITER "iter: "
#define POMODORO_DISABLED "desactivado"
#define POMODORO_RESET_TURN_OFF "Reiniciar y apagar"
#define POMODORO_START "Iniciar"

// Set Clock GUI
#define SETCLOCK_SET_TIME "Ajustar hora"
#define SETCLOCK_SET_DATE "Ajustar fecha"
#define SETCLOCK_TIMEZONE_MENU "Menu zona horaria"
#define SETCLOCK_CLOCK_SETTINGS "Ajustes de reloj"
#define SETCLOCK_CURRENT_TIMEZONE "Zona horaria actual: "
#define SETCLOCK_CLEAR_TIMEZONE "Borrar zona horaria"
#define SETCLOCK_SET_UTC_TIMEZONE "Establecer zona UTC"
#define SETCLOCK_SET_LOCATION_TIMEZONE "Zona horaria por ubicacion"
#define SETCLOCK_TIMEZONE_SETTINGS "Ajustes zona horaria"
#define SETCLOCK_TIMEZONE_WARNING "Este menu ignora el horario de verano/estandar y sobreescribira su zona horaria.\nPulse el boton atras para confirmar"
#define SETCLOCK_WARNING_TITLE "Aviso"
#define SETCLOCK_SELECT_TIMEZONE "Seleccionar zona"

// Time unit translations (for automatic formatting)
#define TIME_UNIT_MINUTE "minuto"
#define TIME_UNIT_MINUTES "minutos"
#define TIME_UNIT_HOUR "hora"
#define TIME_UNIT_HOURS "horas"
#define TIME_UNIT_DAY "dia"
#define TIME_UNIT_DAYS "dias"
#define TIME_UNIT_AND "y"
