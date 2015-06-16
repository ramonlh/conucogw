/**  usando EEPROM 24LC512  **/   
/**  usando ESP8266 WIFI    **/

#define miversion 3
#define misubver 0
#define VERMEM 1
#define ARDUINO 103

#define ES        // idioma EN:english,  ES: español
#define LOG 0
#define WEBDUINO_SERIAL_DEBUGGING 0
#define WEBDUINO_FAIL_MESSAGE "<h1>FATAL ERROR</h1>"
#define PREFIX ""

// INCLUDE
#include "SPI.h"
#include "Ethernet.h"
#include "WebServer.h"
#include "EEPROM.h"
#include "EEPROMAnything.h"
#include "LiquidCrystal.h"
#include "Wire.h"
#include "DS1307RTC.h"
#include "Time.h"
#include "Base64.h"
#include "OneWire.h"
#include "Mudbus.h"
#include "DallasTemperature.h"
#include "commontexts.h"
#include "idiomas.h"
#include "SdFat.h"
#include <DHT.h>
#include <avr/wdt.h>
#include "esp8266.h"


// DEFINE
#define dserial Serial			// debug serial

// REPLICAR EN SATSERVER 
#define maxEDr 2     // número entradas digitales
#define maxSDr 6     // número salidas digitales+relés
#define maxEAr 4     // número entradas analógicas
//byte edPin[maxEDr]={12,13};      // entradas digitales
//byte sdPin[maxSDr]={2,3,4,5,10,11};  // salidas digitales+reles
//byte anPin[maxEAr]={0,1,2,3};    // entradas analógicas

volatile int ninter=0;

#define inact 255          // valor para programación, significa inactivo
#define despMail 251       // revalor para enviar mails, de 90 a 92
#define delayACKMail 2000   // espera a respuesta de servidor smtp despuÃ©s de cada comando
/* NO USAR LOS PINES: 52, 50, 10, 4   Ethernet shield  */
//#define spiMISO 50
//#define spiMOSI 51
//#define spiSCK 52
#define ethSEL 10
#define sdSEL 4
#define rstFab 42    //  Pin para reset fábrica

/* 49, 47, 45, 35, 33, 31, 29   LCD  */
#define lcdRS 49
#define lcdRW 47
#define lcdEN 45
#define lcdEN2 18    // sólo para Powertip PC4004, se quita una Ent. digital
#define lcdD0 35
#define lcdD1 33
#define lcdD2 31
#define lcdD3 29
// LiquidCrystal lcd(lcdRS,lcdRW,lcdEN,lcdD0,lcdD1,lcdD2,lcdD3); 

#define owPin1 43    // pin para 1-wire, puerto 1
//#define owPin2 46    // pin para 1-wire, puerto 2

#define espResetPin 66  // A12

#define lTAB 8       // en 8 bytes se guardan variables de 64 bits
#define nPINES 54    // varía según la placa, 54 para Arduino MEGA
#define nANALO 16    // número máximo de entradas analógicas
#define nDIG 34      // número máximo de entradas/salidas digitales
#define nPRG 32      // número de programaciones
#define nEsc 5       // número de escenas
#define nsalporesc 86  // número de salidas por escena
#define nPRGsem 5    // programas a los que se pueden asociar los programas semanales
#define nPRGeve 5    // programas a los que se pueden asociar los programas de eventos
#define LEN5 5
#define LEN20 20
#define LEN40 40
#define LEN80 80
#define maxTemp 20     // máximo de sondas 1-wire
#define baseTempr 20
#define baseAna 80
#define baseAnar 96

// dispositivos remotos esp8266
#define maxNodos 30
#define maxRem 120   // número de E/S remotas
#define nANALOr 20   // número máximo de entradas analogicas remotas
#define maxTempr 60  // número máximo de sondas remotas

#define lTABr 24     // en 24 bytes se guardan variables de 192 bits

// definición posiciones EEPROM  (hay que revisar tamaños y huecos)
// hueco desde 0 hasta 50
#define dirEEmodo 50       // 1 byte, modo de presentación web, 0:Avanzado, 1:Básico, 2:Local, 3:Domótica,4:Riego
#define dirEEevenTemp 51   // 40
#define dirEEprectemp 91   // 1 byte, precisión sondas ds18b20: 9,10,11,12
#define dirEEsdHay 99      // 1
#define dirEEmac 100       // 6 MAC
#define dirEEip 106        // 4 IP
#define dirEEmask 110      // 4 Mask
#define dirEEgw 114        // Gateway
#define dirEEdns 118       // DNS
#define dirEEwebport 122   // 2 Port web
#define dirEEpass 124      // 20 password
#define dirEEactprg 144    // 5 bytes guarda que programas están activos
#define dirEEuser 149      // 20 usuario
// hueco desde 173 hasta 183  // 15 bytes
#define dirEEusepass 184   // 1 activar password
#define dirEEshowTemp 185  // 3 bytes, 20 temperaturas (cabrían 24=8x3)
#define dirEEfecval 188     // 8
#define dirEEpmactVar 196    // 4 bytes
#define dirEEnumIA 200     // 1   
#define dirEEnumID 201     // 1
#define dirEEnumOD 202     // 1
#define dirEEshown 203     // 1 mostrar número de pin entre parentesis
#define dirEEperact 204    // 1 período de actualizacion en segundos de pantalla 
#define dirEEremPin 231    // 120 (MaxRem) pines de la E/S remotas (20 ANA, 20 EDIG, 20 SDIG, 60 RelÃ©s)
#define dirEEestado 351     // 32 bytes, se guarda el estado de 64 pines locales y 192 remotos
#define dirEEbshowLink 383  // 1 byte
// hueco desde 384 hasta 385 (2 bytes)
#define dirEEdescpinD 410   // descripción de pines locales 34=680
#define dirEEdescpinA 1090   // descripción de pines locales 16x20=320
#define dirEEremTpin 1410  // orden sonda remota en un nodo remoto 60
#define dirEEfecdia 1470     // 32
// hueco desde 1502 hasta 1505  4 bytes
#define dirEEremNodo 1506   // 120 tabla de nodos remotos, maxRem nodos (120)
//hueco desde 1746 hasta 1809  // 
#define dirEEevenvalD 1810   // 64 bytes
#define dirEEbprgtipo 1882   // 4 bytes, tipo señal salida en prg semana
#define dirEEbeveacttipo 1886   // 4 bytes, tipo señal activadora en prg evento
#define dirEEbevesaltipo 1890   // 4 bytes, tipo señal salida en prg evento
#define dirEEtipoLCD 1894       // 1 byte
#define dirEEtipoED 1895        // 16 bytes tipo de entrada digital
// hueco desde 1911 hasta 1958, 49 bytes
#define dirEEfecmin 1959    // 32  fecha, minuto   
#define dirEEevenact 1991   // 32 señales activadoras del evento
#define dirEEevensal 2023   // 32 bytes, pin sobre el que se actÃºa en eventos
#define dirEEfecsal 2055    // 32
#define dirEEbfectipo 2087   // 4 bytes, tipo señal salida en prg fecha
// hueco desde 2091 hasta 2092, 2 bytes
#define dirEEevenniv 2093   // 8 bytes, se guarda bevenniv, nivel de activaciÃ³n del evento
// hueco desde 2105 hasta 2108, 4 bytes
#define dirEEredNodo 2109   // 120, red a la que pertenece cada nodo
#define dirEEdescTemp 2229  // 400, 20x20 descripción de las sondas 1-wire
#define dirEEbshowpin 2629  // 8 bytes, Panel Principal locales
// hueco desde 2637 hasta 2639
#define dirEEremshowpin 2640 // 24 bytes, Panel Principal, pines remotos
#define dirEEbactsem 2664      // 4 bytes programa semanal activo
#define dirEEbacteve 2668      // 4 bytes programa eventos activo
#define dirEEbactfec 2672      // 4 bytes programa fechas activo
#define dirEEbshowEsc 2676       // 1 bytes
// hueco desde 2677 hasta 2682 (6 bytes)
#define dirEEprgsal 2683     // 32   
#define dirEEfecano 2715     // 32
#define dirEEprgval 2747     // 64  
#define dirEEprgdia 2811     // 32 
#define dirEEprghor 2843     // 32
#define dirEEprgmin 2875     // 32
#define dirEEprgcon 2907     // 32
#define dirEEprgconv 2939    // 64 bytes
#define dirEEprgcomp 3003    // 32
#define dirEEremTnodo 3035  // nodos de las sondas remotas, 60x2=120
#define dirEEfechor 3355     // 32
#define dirEEmailACT 3387    // 1 activar envío de correo
#define dirEEmailSVR 3388    // 40 servidor de correo
#define dirEEmailPER 3428    // 1 período de envío de correo en minutos
#define dirEEmailPORT 3429   // 2 puerto del servidor de correo
#define dirEEmailUSER 3431   // 40 usuario correo
#define dirEEmailPASS 3471   // 40 password correo
#define dirEEmailSENDER 3511 // 40 remitente correo
#define dirEEmailDEST 3551    // 120: 3 x 40, destinatarios correo
#define dirEEevencomp 3671   // 32 comparador eventos analógicos
#define dirEEcontadores 3703  // 32 8 contadores de 4 bytes(long) entradas digitales
#define dirEEfecmes 3735     // 32
#define dirEEcode433Off 3767  // 4 x 30=120 byter

#define dirEEdescTempr 4096    // 60 x 20=1200 bytes, 19 caracteres +1
// hueco desde 3887 hasta 4066, 180 bytes.
// hueco desde 4067 hasta 4095, 28 bytes
#define dirEEdescReler 5296    // 60 x 20=1200 bytes, 19 caracteres +1
#define dirEEdescAnalr 6496    // 20 x 20=400 bytes, 19 caracteres +1
#define dirEEdescEDigr 6896    // 20 x 20=400 bytes, 19 caracteres +1
#define dirEEdescSDigr 7296    // 20 x 20=400 bytes, 19 caracteres +1
#define dirEEfactorArem 7696   // 20 x 4 = 80 bytes, factor conversion analógicas remotas
#define dirEEfactorA 7776      // 16 x 4 = 64 bytes, factor conversion analógicas locales
#define dirEEdescEsc 7840      // 5 x 20 = 100, descripciones escenas
#define dirEEesc 7940          // 5 escenas x 11 = 55 bytes, escenas 
#define dirEEPRGsem 7999    // 20 bytes
#define dirEEPRGeve 8019    // 20 bytes
#define dirEEdyndnsHost 8039   // 40 bytes
#define dirEEdyndnsUser 8079   // 40 bytes
#define dirEEdyndnsPass 8119   // 40 bytes
// hueco desdespe 8119 hasta 8191, 73 bytes
#define dirEEevenvalA 10000  // 128, 32x4, valor de activación para analógicas
// hueco desde 10128 hasta 10255, 128 bytes  // Reservados ampliación evenval 
#define dirEEevenhis 10256   // 128  32x4, valor de histéresis de activación del evento
// hueco desde 10384 hasta 10511, 128 bytes  // Reservados ampliación evenhis 
#define dirEEdescPrg 10512      // 5 x 20 = 100, descripciones escenas
// hueco desde 10612 hasta 10711, 128 bytes  // Reservados ampliación evenhis 
#define dirEEurlLink 11312   // 5x80=400 5 posibles enlaces de usuario
#define dirEEdescLink 11712  // 5x20=100 descripciones Links
#define dirEEoffsetArem 11812   // 20 x 4 = 80 bytes, offset conversion analógicas remotas
#define dirEEoffsetA 11892      // 16 x 4 = 64 bytes, offset conversion analógicas locales
#define dirEEoffsetA 11892      // 16 x 4 = 64 bytes, offset conversion analógicas locales
#define dirEEsumatA 12706      // 20 x 1 = 20 bytes, sumatorio conversion analógicas locales
#define dirEEremIDnodos 12726  // 32 bytes (reservamos hasta 100)
#define dirEEremIPnodos 12826  // 32 bytes (reservamos hasta 100)
#define dirEEsumatArem 12926   // 20 x 1 = 20 bytes, sumatorio conversion analógicas remotas
#define maxVALr 22

// hueco desde 12926  hasta final

// VARIABLES
byte modo=0;
/********* IMPORTANTE  *****************/
//const byte valorpin[2] = {0,1};    // directo 0=LOW,  1=HIGH    depende de laplaca de relÃ©s: placa estrecha
const byte valorpin[2] = {1,0};    // 2  inverso 1=LOW,  0=HIGH    depende de laplaca de relÃ©s: placa ancha
const byte reversepin[2] = {1,0};  // 2  inverso 1=LOW,  0=HIGH    
const int buffLEN = 128;   // 2 tamaño buffer para webserver.processConnection

int minRam = 8000;        // 2

byte showN   =  1;    // 1 indica si se muestra el número de pin en la lista de entradas/salidas
int peract  =  60;     // 2 período de actualización automática de la página principal 
byte debugserial = 0;

byte usepass =  0;    // 1, usar password:1,  sin password:0
char user1[LEN20];    // 20 bytes  usuario
char pass1[LEN20];    // 20 bytes  password
tmElements_t tm;       // estructura para RTC
tmElements_t tmInicio; // estructura para RTC

const byte freepines[]={        // 54  pin libre=0; pin reservado=1
          0,0,0,0,1, 0,0,0,0,0,  1,0,0,1,0, 0,0,0,0,1,  // pines  0 a  19
          1,1,0,0,0, 0,0,0,0,1,  0,1,0,1,0, 1,0,0,0,0,  // pines 20 a 39
          0,0,1,1,1, 1,1,1,1,1,  1,1,1,1};              // pines 40 a 53
const byte tabpin[34]={0,1,2,3,5,6,7,8,9,11,12,14,15,16,17,18,19,22,23,  // tabla de pines útiles
                       24,25,26,27,28,30,32,34,36,37,38,39,40,41,42};        
  byte EEmac[6];                // 6  100-105   dirección MAC
  byte EEip[4];                 // 4   106-109   dirección IP
  byte EEgw[4];                 // 4   110-113   puerta de enlace 
  byte EEmask[4];               // 4   114-117   máscara de subred
  byte EEdns[4];                // 4   118-121   servidor DNS
  unsigned int EEwebPort=82;    // 2   122-123   puerto servidor web 
  // variables SD
  byte sdOK = 0;                // 1
  byte sdHay = 0;               // 1   indica si existe tarjeta SD
  SdFat SD;
  SdFile file;
  
  byte numIA = 16;              // 1   200       número de entradas analógicas máximo
  byte numID = 16;              // 1    201      número de entradas digitales
  byte numOD = 22;              // 1    202      número de E/S utilizadas, la suma de las 3 anteriores mÃ¡ximo 54
  byte numPines;                // 1    número total de pines usados, se calcula y no se guarda
  byte bshowpin[lTAB];          // 8    bytes,   0:visible, 1:oculto, en Panel Principal
  byte bshowEsc;                // 1 byte, por bits uno para cadaescena 
  byte bshowTemp[3];            // 3    bytes,   0:visible, 1:oculto, en Panel Principal
  byte bshowLink;               // 1    bytes,   0:visible, 1:oculto, en Panel Principal
  byte bprgtipo[4];             // tipo de salida en prg semanal: local/remota
  byte beveacttipo[4];          // tipo de activadora en prg evento: local/remota
  byte bevesaltipo[4];          // tipo de salida en prg evento: local/remota
  byte bfectipo[4];             // tipo de salida en prg fecha: local/remota
  byte bactsem[4];              // programa semanal activo, 4 bytes = 32 programas
  byte bacteve[4];              // programa eventos activo, 4 bytes = 32 programas
  byte bactfec[4];              // programa fechas activo, 4 bytes = 32 programas
  byte evenact[nPRG];           // 32   1 por cada evento, señal que activa el evento
  byte bevenENABLE[2][lTAB];    // 16   2x8 bytes, define si el evento ha activado ya algo
  byte evencomp[nPRG];          // 32   32 bytes, comparador: =, <>, >=, .... 
  byte evenvalD[nPRG];          // 32   32 bytes, valor de activación del evento para digitales
  byte evensal[nPRG];           // 68   define la señal sobre la que se actua
  byte bevenniv[lTAB];          // 8    8 bytes, define nivel a activar. 

  byte evenTemp[nPRG];          // 32   define la señal sobre la que se actua
  long evenvalA[nPRG];          // 128  32x4, valor de activación del evento para analógicas
  long evenhis[nPRG];           // 128  32x4, valor de histéresis de activación del evento
  byte bescena[nEsc][11];       // 55   5 x 11 = 55, definición de escenas   
  byte bPRGsem[nPRGsem][4];     // 20 bytes, para guardar 5x32 bits de asociación PRG-PRGsem
  byte bPRGeve[nPRGeve][4];     // 20 bytes, para guardar 5x32 bits de asociación PRG-PRGeve
  byte actPrg[5];               // 5 bytes guarda que programas están activos: 0/1 
  byte tipoED[16];              // tipo de la entrada digital: 0=ON/OFF, 1=DHT11
  float vanaTED[16];            // valores anal. de entradas digitales para sensores DHT11-Temp
  float vanaHED[16];            // valores anal. de entradas digitales para sensores DHT11-Hum
  float factorArem[nANALOr];  // 20x4 factor conversión analógicas remotas
  float offsetArem[nANALOr];  // 20x4 offset conversión analógicas remotas
  byte bsumatArem[3];  

  // variables para programación SEMANAL
  byte bprgval[lTAB];         // 8 bytes, valor a poner en cada programación, on/off
  byte prgsal[nPRG];          // 32 bytes, salida a actuar en cada programación
  byte prgdia[nPRG];          // 32 bytes, días de la semana, bits 0 a 6
  byte prghor[nPRG];          // 32 bytes, hora
  byte prgmin[nPRG];          // 32 bytes, minuto
  byte prgcon[nPRG];          // 32 bytes, señal para activación condicional
  byte prgcomp[nPRG];         // 32 bytes, tipo de comparación

  // variables para programación por FECHAS
  byte bfecval[lTAB];         // 8 bytes, valor a poner en cada programación, on/off
  byte fecsal[nPRG];          // 32 bytes, salida a actuar en cada programación por fechas
  byte fecano[nPRG];          // 32 bytes, año menos 2000 para poder usar un byte
  byte fecmes[nPRG];          // 32 bytes, mes
  byte fecdia[nPRG];          // 32 bytes, día del mes 
  byte fechor[nPRG];          // 32 bytes, hora
  byte fecmin[nPRG];          // 32 bytes, minuto

  byte nTemp1=0;                 // número temperaturas 1-wire, puerto 1
  byte addr1Wire[maxTemp][8];      // ver de eliminar
  unsigned long contadores[8];
  byte prectemp=12;
  int contador=0; 
  int velocidad=0; 

  long mact1;
  long mact10;
  long mact60;
  long mactVar;
  long pmactVar=60;
  int opcode=99;
  
  float factorA[nANALO];  // 16x4 factor conversión analógicas locales
  float offsetA[nANALO];   // 16x4 offset conversión analógicas locales
  byte bsumatA[2];  
  byte tipoLCD=0;
  boolean tareason=false;
  
  //File milog;
//  char namelogEve[]="eventos.log";  // tipo 2  
  char namelogAcc[]="acciones.log";  // tipo 1, 5 y 6    
  char namelogAna[]="analog.log";  // tipo 3
  char namelogSys[]="system.log";  // tipo 4    
  char namelogIP[]="ipaccess.log";  // tipo 7
  char namelogTem[]="temperat.log";  // 
  char namefilebackup[]={"backup.bin"};   // file backup
  char namefile[13];
  byte resetfab=0;
  
  // esp8266 remotes
  byte tremID[maxNodos];     // de 1 a 255
  byte tremIP[maxNodos];     // último byte de la dirección IP NNN: xxx.xxx.xxx.NNN
  byte thour[maxNodos];      // hora último vista nodo
  byte tmin[maxNodos];      // minute último vista nodo
  byte remNodo[maxRem]; // nodo e/s remota
  byte remPin[maxRem]; // pin e/s remota
  byte remTnodo[maxTempr];   // 60  60 x 1
  byte remTpin[maxTempr];       // 60  Sondas 1-wire remotas
  byte rbshowpin[lTABr];        // 24   bytes, hasta 192 bits, en panel general

//////  tratamiento de bits /////////////////////
  const byte tab[8] = {1,2,4,8,16,32,64,128};    // 8
  
  byte mailACT = 0;          //  1
  byte mailPER = 5;          //  2
  char mailSVR[LEN40] = "";  //  40
  int mailPORT = 25;         //  2
  char mailUSER[LEN40] = ""; //  40
  char mailPASS[LEN40] = "";   //  40
  char mailSENDER[LEN40] = ""; //  40
  char mailDEST[3][LEN40] = {"","",""};   //  120
  byte dyndnsServ=0;
  char dyndnsHost[LEN40]="";
  char dyndnsUser[LEN40]="";
  char dyndnsPass[LEN40]="";

  boolean mododebug=false;
  boolean transparentmode=false;
  char gatewayPort[]="88";
  int mysocket=4;
  char myid[]="myid";       
  int maxAI=4;    // número máximo de entradas analógicas
  int maxDI=4;    // número máximo de entradas digitales
  int maxDO=4;    // número máximo de salidas digitales
  int maxTI=4;    // número máximo de sondas de temperatura

// CREATOR
  WIFI wifi(0); 
  Mudbus Mb;
  DHT dht(6);
  WebServer webserver(PREFIX, EEwebPort);
  LiquidCrystal lcd(lcdRS,lcdRW,lcdEN,lcdD0,lcdD1,lcdD2,lcdD3); 
  EthernetClient EthClient;
  OneWire ds1(owPin1);  // on pin 43 (a 4.7K resistor is necessary)
  DallasTemperature sensors1(&ds1);

void printS(const prog_uchar *str)
  {
  char c;
  while((c = pgm_read_byte(str++))) dserial.write(c);
  }

void printS(const prog_uchar *str1,const prog_uchar *str2)
  {  printS(str1); printS(str2);  }
void printS(const prog_uchar *str1,const prog_uchar *str2,const prog_uchar *str3)
  {  printS(str1,str2); printS(str3);  }
  
void printS(const prog_uchar *str1,const prog_uchar *str2,const prog_uchar *str3,const prog_uchar *str4)
  {  printS(str1,str2); printS(str3,str4);  }
  
void printS(const prog_uchar *str1,const prog_uchar *str2,const prog_uchar *str3,const prog_uchar *str4,const prog_uchar *str5)
  {  printS(str1,str2,str3);printS(str4,str5);  }

void strcatP(char *dest, const prog_uchar *orig)
  {
  char c;
  while((c = pgm_read_byte(orig++))) dest[strlen(dest)]=c;
  }

int buscaposRem(byte nodo, byte pin, int ini, int fin)
  {
  int i=ini;
  boolean encontrado=false;
  while ((i<fin) && (!encontrado))
    {
    encontrado=((remNodo[i]==nodo) && (remPin[i]==pin));
    if (!encontrado) i++; 
    }
  if (i<fin) return i; else return 999;
  }
  
int buscaposTRem(byte nodo, byte pin, int ini, int fin)
  {
  int i=ini;
  boolean encontrado=false;
  while ((i<fin) && (!encontrado))
    {
    encontrado=((remTnodo[i]==nodo) && (remTpin[i]==pin));
    if (!encontrado) i++; 
    }
  if (i<fin) return i; else return 999;
  }
  
void actualizavalor (byte id, char tipo, byte pin, int val)
  {
  if (tipo=='a')
    {
    int auxpos=buscaposRem(id, pin, 0, 20);
    if (auxpos!=999) Mb.R[auxpos+baseAnar]=val;  
    }
  if (tipo=='i') 
    {
    int auxpos=buscaposRem(id, pin,20,40);
    if (auxpos!=999) setbit8(Mb.C8,auxpos+44,val);
    }
  if (tipo=='o')
    {
    int auxpos=buscaposRem(id, pin,40,120);
    if (auxpos!=999) setbit8(Mb.C8,auxpos+44,val);
    }
  if (tipo=='s')
    {
    int auxpos=buscaposTRem(id, pin,0,60);
    if (auxpos!=999) Mb.R[auxpos+baseTempr] = val;
    }
  }
  
void procesaJsonData()    // procesa texto JSON...y actualiza valores recibidos en tablas
  {
  if (tm.Hour < 10) printS(cero); dserial.print(tm.Hour); printS(dospuntos);
  if (tm.Minute < 10) printS(cero); dserial.print(tm.Minute); printS(dospuntos);
  if (tm.Second < 10) printS(cero); dserial.print(tm.Second);printS(b);
  dserial.println(wifi.inputstr);
  char tipo;
  byte pin;
  char *auxchr=strtok(wifi.inputstr+4,":,}");
  byte auxid=atoi(auxchr);    // ID del remoto que envía datos
  actualizaHoraRem(auxid);
  
  while (auxchr!=NULL)
    {
    auxchr=strtok(NULL,",:}");    
    if (auxchr!=NULL)
      {
      tipo=auxchr[0];
      if (tipo=='d') 
        {
        tipo=auxchr[1];
        if (tipo=='s') tipo='o';
        pin=atoi(auxchr+2);
        }
      else
        pin=atoi(auxchr+1);
      auxchr=strtok(NULL,",:}");  
      if (auxchr!=NULL)
        {
        int valor=atoi(auxchr);
        actualizavalor(auxid,tipo,pin,valor);
        }
      }
    }
  }

void asignaipaid(byte id, byte ip)
  {
   int i=0;
   while ((i<maxNodos) && (tremIP[i]!=ip)) i++; // borra la IP si ya existe
   if (i<maxNodos) tremIP[i]=0;
   i=0;
   while ((i<maxNodos) && (tremID[i]!=id)) i++;
   if (i<maxNodos)  tremIP[i]=ip;   // se encontró la ID en la tabla: se asigna la IP
   else            // no se encontró la ID en la tabla: se asigna la ID y la IP en un espacio vacío
     {
     i=0;
     while ((i<maxNodos) && (tremID[i]!=0)) i++;
     if (i<maxNodos)    // se asignan la ID y la IP
       {
       tremID[i]=id;
       tremIP[i]=ip;
       }
     }
  guardarDatosNodos();
  }
  
void actualizaHoraRem(int nrem)  // actualiza hora y minuto de cuando se ha visto un nodo por última vez
  {
  byte auxpos=buscaNodoenTab(nrem);
  if (auxpos!=99)
    {
    thour[auxpos]=tm.Hour;
    tmin[auxpos]=tm.Minute;
    }
  }
  
int contadorinf=0;

void procesaInf()    // procesa comandos INF...
  {
  dserial.print(contadorinf++); printS(guion,procesainf,paren_i); 
  dserial.print(wifi._comando); printS(coma);
  dserial.print(wifi._parametros); printlnS(paren_f);
  if (strcmp(wifi._comando,myid)==0)    // se ha recibido la ID de un remoto
    {
     char *auxIP=strtok(wifi._parametros,".-");
     auxIP=strtok(NULL,".-");
     auxIP=strtok(NULL,".-");
     auxIP=strtok(NULL,".-");
     char *auxID=strtok(NULL,".-");
     byte ip=atoi(auxIP);
     byte id=atoi(auxID);
     if ((ip>0) && (id>0)) asignaipaid(id,ip);
     actualizaHoraRem(id);
     return;
     }
  }

byte auxID;
char auxIP[16];
byte auxnTemp;
char auxaddr[3][16];

void procesaJsonConf()    // procesa comandos HTTP
  {
    if (tm.Hour < 10) printS(cero); dserial.print(tm.Hour); printS(dospuntos);
    if (tm.Minute < 10) printS(cero); dserial.print(tm.Minute); printS(dospuntos);
    if (tm.Second < 10) printS(cero); dserial.print(tm.Second);printS(b);
    dserial.println(wifi.inputstr+4);
  char *auxchr=strtok(wifi.inputstr+5,":,");    // "ID"
  auxchr=strtok(NULL,",:");                     // myID
  byte auxID=atoi(auxchr);                      // myID del remoto que envía datos
  if (auxID!=0) actualizaHoraRem(auxID);
  
  dserial.print("auxchr:");dserial.println(auxchr);
  auxchr=strtok(NULL,",:"); 
  dserial.print("auxchr:");dserial.println(auxchr);
  auxchr=strtok(NULL,",:"); 
  dserial.print("auxchr:");dserial.println(auxchr);
  
  
  dserial.print("ID:");dserial.println(auxID);
  
//  while (auxchr!=NULL)
//    {
//    auxchr=strtok(NULL,",:}"); 
//    dserial.println(auxchr);   
//    if (auxchr!=NULL)
//      {
//      if (strcmp(auxchr,"IP")==0)  // ID
//        {
//        auxchr=strtok(NULL,",:}"); 
//        strcpy(auxIP,auxchr);        
//        }
//      if (strcmp(auxchr,"Sondas")==0)  // ID
//        {
//        auxchr=strtok(NULL,",:}"); 
//        auxnTemp=atoi(auxchr);
//        }
//      if (auxchr[0]=='S') 
//        {
//        byte auxN=atoi(auxchr+2);
//        auxchr=strtok(NULL,",:}"); 
//        strcpy(auxaddr[auxN],auxchr);
//        }
//      else
//        strcpy(auxaddr[0],"");
//      }
//    }
  }

void formaJson()    // compone el texto json y lo guarda en wifi._inputstr
  {
     memset(wifi.inputstr,0,sizeof(wifi.inputstr));
     strcatP(wifi.inputstr,llave_i);
     char buff[20]; memset(buff,0,sizeof(buff));
     int valor=0; 
     for (int i=0; i<maxDI; i++)
       {
       strcatP(wifi.inputstr,di);
       strcat(wifi.inputstr,itoa(i,buff,10));
       strcatP(wifi.inputstr,dospuntos);
       strcat(wifi.inputstr,itoa(valor,buff,10));
       if (i < maxDI-1) strcatP(wifi.inputstr,coma);
       }
     for (int i=0; i<maxDO; i++)
       {
       strcatP(wifi.inputstr,ds);
       strcat(wifi.inputstr,itoa(i,buff,10));
       strcatP(wifi.inputstr,dospuntos);
       strcat(wifi.inputstr,itoa(valor,buff,10));
       if (i < maxDO-1) strcatP(wifi.inputstr,coma);
       }
     for (int i=0; i<maxAI; i++)
       {
       strcatP(wifi.inputstr,a);
       strcat(wifi.inputstr,itoa(i,buff,10));
       strcatP(wifi.inputstr,dospuntos);
       strcat(wifi.inputstr,itoa(valor,buff,10));
       if (i < maxAI-1) strcatP(wifi.inputstr,coma);
       }
     for (int i=0; i<maxTI; i++)
       {
       strcatP(wifi.inputstr,s);
       strcat(wifi.inputstr,itoa(i,buff,10));
       strcatP(wifi.inputstr,dospuntos);
       strcat(wifi.inputstr,itoa(valor,buff,10));
       if (i < maxTI-1) strcatP(wifi.inputstr,coma);
       }
     strcatP(wifi.inputstr,llave_f);
  }
  
//
byte getbit(byte mibyte, byte pin)
 {if ((mibyte & tab[(pin % 8)]) > 0) 
    return 1; 
  else 
    return 0;  }

void setbit(byte *mibyte, byte pin, byte value)  { 
  if (value == 0)
    *mibyte = *mibyte & (255 ^ tab[(pin % 8)]);  
  else
    *mibyte = *mibyte | tab[(pin % 8)]; }
 
byte getbit8(byte tabla[], byte pin)
 {if ((tabla[pin/8] & tab[(pin % 8)]) > 0) 
    return 1; 
  else 
    return 0;  }

void setbit8(byte tabla[], byte pin, byte value)  { 
  if (value == 0)
    tabla[pin/8] = tabla[pin/8] & (255 ^ tab[(pin % 8)]);  
  else
    tabla[pin/8] = tabla[pin/8] | tab[(pin % 8)]; }
    
byte EEread(unsigned int dir)  {
  return readEEPROM(EEPROM2dir, dir); }   // está en EEPROMAnything  }

void EEwrite(int dir, byte valor)  {
   writeEEPROM(EEPROM2dir,dir,valor);  }  // está en EEPROMAnything  

void printPiP(WebServer &server, const prog_uchar *texto1,int num,const prog_uchar *texto2)
  { server.printP(texto1);   server.print(num);   server.printP(texto2); }

void printPlP(WebServer &server, const prog_uchar *texto1,long num,const prog_uchar *texto2)
  { server.printP(texto1);   server.print(num);   server.printP(texto2); }

void printPcP(WebServer &server, const prog_uchar *texto1,char car,const prog_uchar *texto2)
  { server.printP(texto1);   server.print(car);   server.printP(texto2); }

void printP(WebServer &server, const prog_uchar *texto1)
  {  server.printP(texto1);  }
  
void printP(WebServer &server, const prog_uchar *texto1, const prog_uchar *texto2)
  {  server.printP(texto1); server.printP(texto2); }

void printP(WebServer &server, const prog_uchar *texto1, const prog_uchar *texto2, const prog_uchar *texto3)
  { server.printP(texto1); server.printP(texto2); server.printP(texto3); }

void printP(WebServer &server, const prog_uchar *texto1, const prog_uchar *texto2, 
                                const prog_uchar *texto3, const prog_uchar *texto4)
  { printP(server, texto1, texto2); printP(server, texto3, texto4); }

void printP(WebServer &server, const prog_uchar *texto1,  const prog_uchar *texto2,const prog_uchar *texto3,  
                               const prog_uchar *texto4,  const prog_uchar *texto5)
  {printP(server, texto1, texto2, texto3);  printP(server, texto4, texto5); }

void printP(WebServer &server, const prog_uchar *texto1,  const prog_uchar *texto2, const prog_uchar *texto3,  
                               const prog_uchar *texto4,  const prog_uchar *texto5,  const prog_uchar *texto6)
  { printP(server, texto1, texto2, texto3); printP(server, texto4, texto5, texto6); }
  
void printP(WebServer &server, const prog_uchar *texto1,  const prog_uchar *texto2, const prog_uchar *texto3,
                                const prog_uchar *texto4, const prog_uchar *texto5,  const prog_uchar *texto6,
                                const prog_uchar *texto7)
  { printP(server, texto1, texto2, texto3); printP(server, texto4, texto5, texto6, texto7); }
  
void printP(WebServer &server, const prog_uchar *texto1, const prog_uchar *texto2, const prog_uchar *texto3,
                               const prog_uchar *texto4, const prog_uchar *texto5,  const prog_uchar *texto6,
                               const prog_uchar *texto7,  const prog_uchar *texto8)
  { printP(server, texto1, texto2, texto3, texto4); printP(server, texto5, texto6, texto7, texto8); }

void printlnS(const prog_uchar *str)
  {
  printS(str);
  dserial.println();
  }

void serialmem(char texto[])
  {
//  printS(libre); dserial.print(mifreeRam()); 
  }
    
void printTDTR(WebServer &server, byte posicion, boolean trb, boolean tdb)
  {
    if (posicion==0)    // inicio
      {
      if (trb) printP(server,tr);
      if (tdb) printP(server,td);
      }
    else
      {
      if (tdb) printP(server,td_f);
      if (trb) printP(server,tr_f);
      }
  }
  
void printTD(WebServer &server, char *texto, boolean tr, boolean td)
  {
  printTDTR(server, 0, tr, td);
  server.print(texto);
  printTDTR(server, 1, tr, td);
  }
 
void printTDP(WebServer &server, const prog_uchar *texto, boolean tr, boolean td)
  {
  printTDTR(server, 0, tr, td);
  server.printP(texto);
  printTDTR(server, 1, tr, td);
  }
 
void printIP(WebServer &server, int valor, const prog_uchar *texto)
  {
    server.print(valor,DEC);
    server.printP(texto);
  }
void printIPH(WebServer &server, int valor, const prog_uchar *texto)
  {
    server.print(valor,HEX);
    server.printP(texto);
  }
  
void printLP(WebServer &server, long valor, const prog_uchar *texto)
  {
    server.print(valor,DEC);
    server.printP(texto);
  }
  
void printcampoC(WebServer &server, int numpar, char *valactual, byte tam, boolean visible, boolean td, boolean pass)
  {
  printTDTR(server, 0, false, td);
  if (visible)
    if (pass)
      server.printP(Form_input_pass_start);
    else
      server.printP(Form_input_text_start);
  else
    server.printP(Form_input_text_start_invis);
  printIP(server, numpar, Form_input_value);
  server.print(valactual);
  server.printP(Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoL(WebServer &server, int numpar, long valactual, byte tam, boolean td)
  {
  printTDTR(server, 0, false, td);
  printP(server,Form_input_text_start);
  printIP(server, numpar, Form_input_value);
  printLP(server, valactual, Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoChr(WebServer &server, int numpar, int valactual, byte tam, boolean td)
  {
  printTDTR(server, 0, false, td);
  server.printP(Form_input_text_start);
  printIP(server, numpar, Form_input_value);
  server.write(valactual);
  printP(server, Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoI(WebServer &server, int numpar, int valactual, byte tam, boolean td)
  {
  printTDTR(server, 0, false, td);
  server.printP(Form_input_text_start);
  printIP(server, numpar, Form_input_value);
  printIP(server, valactual, Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampobyte(WebServer &server, int numpar, byte valactual, byte tam,boolean td)
  {
  printTDTR(server, 0, false, td);
  printP(server,Form_input_text_start);
  printIP(server, numpar, Form_input_value);
  printIP(server, valactual, Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoF(WebServer &server, int numpar, float valactual, byte tam, byte deci, boolean td)
  {
  printTDTR(server, 0, false, td);
  printP(server,Form_input_text_start);
  printIP(server, numpar, Form_input_value);
  server.print(valactual,deci);
  server.printP(Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoH(WebServer &server, byte numpar, int valactual, byte tam,boolean td)
  {
  printTDTR(server, 0, false, td);
  printP(server,Form_input_text_start);
  
  printIP(server, numpar, Form_input_value);
  printIPH(server, valactual, Max_length);
  printIP(server, tam, size_i);
  printIP(server, tam, Form_input_end);
  printTDTR(server, 1, false, td);
  }
  
void printcampoSiNo(WebServer &server, int numpar, int valactual, const prog_uchar *uno, const prog_uchar *cero, boolean visible,boolean td)
  {
  printTDTR(server, 0, false, td);
  printP(server,visible ? Select_name:Select_name_invis);
  server.print(numpar); 
  printP(server,barramayor,optionvalue1);
  if (valactual == 1) printP(server,selected);
  printP(server, cierre, uno, option_f,optionvalue0);
  if (valactual == 0) printP(server,selected); 
  printP(server, cierre, cero, option_f,Select_f);  
  printTDTR(server, 1, false, td);
  }         
  
void printcampoCB(WebServer &server, int numpar, int valactual, int pri, int ult, boolean visible, boolean td)
  {
  printTDTR(server, 0, false, td);
  printPiP(server,Select_name,numpar,barramayor);
  for (int j=pri; j<=ult; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (j == valactual) server.printP(selected);
    printPiP(server,cierre,j,option_f);  
    }
  printP(server,Select_f);  
  printTDTR(server, 1, false, td);
  }         
  
void printparC(WebServer &server, char *titulo, int numpar, char valactual[], byte tam, boolean pass)
  {
  printTDTR(server, 0, true, true);
  server.print(titulo);  
  server.printP(td_f);  
  printcampoC(server, numpar, valactual, tam, true, true, pass);
  printTDTR(server, 1, true, false);
  }
  
void printparCP(WebServer &server, const prog_uchar *titulo, int numpar, char valactual[], byte tam, boolean pass)
  {
  printTDTR(server, 0, true, true);
  printP(server, titulo, td_f);
  printcampoC(server, numpar, valactual, tam, true, true, pass);
  printTDTR(server, 1, true, false);
  }
  
void printparI(WebServer &server, char *titulo, int numpar, int valactual, byte tam)
  {
  printTDTR(server, 0, true, true);
  server.print(titulo);
  printP(server,td_f);
  printcampoI(server, numpar, valactual, tam, true);
  printTDTR(server, 1, true, false);
  }  
  
void printparIP(WebServer &server, const prog_uchar *titulo, int numpar, int valactual, byte tam)
  {
  printTDTR(server, 0, true, true);
  printP(server, titulo, td_f);
  printcampoI(server, numpar, valactual, tam, true);
  printTDTR(server, 1, true, false);
  }  
  
void printparLP(WebServer &server, const prog_uchar *titulo, int numpar, long valactual, byte tam)
  {
  printTDTR(server, 0, true, true);
  printP(server, titulo, td_f);
  printcampoL(server, numpar, valactual, tam, true);
  printTDTR(server, 1, true, false);
  }  
  
void printColspan(WebServer &server, int ancho, boolean cerrado)
  {
  printP(server,colspan);
  server.print(ancho); 
  if (cerrado) printP(server,barramayor);
  }

void lineaH(WebServer &server, byte ancho)
  {
  printTDTR(server, 0, true, false);
  printColspan(server,ancho,true);
  printP(server,hr);
  printTDTR(server, 1, true, true);
  }

void printEE(WebServer &server, int dir, byte len)
  {
  int i=0;
  char mitext[80]; mitext[0]=0;
  boolean repeat;
  do {
    mitext[i] = EEread(dir+i);
    repeat = (mitext[i] != 0);
    i = i++;
    } while (repeat);
  server.print(mitext);
  }
  
void printEEmail(EthernetClient &client, int dir, byte len)
  {
  int i=0;
  char mitext[20]; mitext[0]=0;
  boolean repeat;
  do {
    mitext[i] = EEread(dir+i);
    repeat = (mitext[i] != 0);
    i = i++;
    } while (repeat);
  client.print(mitext);
  }
  
//******************************************************************************************************

void printEth(const prog_uchar *str)
  {
  char c;
  while((c = pgm_read_byte(str++)))
    EthClient.write(c);
  }

//Función que envia el email
byte sendEmail(byte pinx, byte even, byte tipo)
{
  boolean debugmail=true;
   char user64[40];
   char pass64[40];
    if (mailACT == 0) return(0);
    if (!EthClient.connect(mailSVR,mailPORT)) return 0; 
    if(!eRcv()) return 0;
    
    printEth(mailHELO);  
    if(!eRcv()) return 0; 
    
    printEth(mailAUTH);    //Le decimos que queremos autenticarnos
    if(!eRcv()) return 0;
    byte largo=strlen(mailUSER);
    base64_encode(user64,mailUSER,largo);

    EthClient.println(user64);   //Enviamos el usuario en base64
    if(!eRcv()) return 0;
    
    largo=strlen(mailPASS);
    base64_encode(pass64,mailPASS,largo);
    EthClient.println(pass64);   //Enviamos el password en base64    
    if(!eRcv()) return 0;
    
    printEth(mailFROM); 
    printEth(menor); EthClient.print(mailSENDER); printEth(mayor);     //especificamos el sender
    if (!eRcv()) return 0;

    printEth(mailRCPT); printEth(menor); 
    if (tipo==0)
      {
      EthClient.print(mailDEST[evensal[pinx]-despMail]);      //especificamos el destinatario
      }
    else
      EthClient.print(mailDEST[evenTemp[pinx]-despMail]);     //especificamos el destinatario
    printEth(mayor); 
    if(!eRcv()) return 0;
    
    printEth(mailDATA);   //indicamos que vamos a empezar a escribir el correo

    printEth(mailFROM); EthClient.print(mailSENDER); printEth(mayor); 
    printEth(mailTO); 
    if (tipo==0)
      EthClient.println(mailDEST[evensal[pinx]-despMail]);
    else
      EthClient.println(mailDEST[evenTemp[pinx]-despMail]);
    printEth(mailSubject); printEth(mailALARMA);
    if (tm.Day < 10)  EthClient.print(0); EthClient.print (tm.Day);      printEth(barra);
    if (tm.Month < 9) EthClient.print(0); EthClient.print (tm.Month+1);  printEth(barra);
    EthClient.print (tmYearToCalendar(tm.Year)); printEth(b);
    if (tm.Hour < 10) EthClient.print(0);   EthClient.print (tm.Hour);   printEth(dospuntos);
    if (tm.Minute < 10) EthClient.print(0); EthClient.print (tm.Minute); printEth(dospuntos);
    if (tm.Second < 10) EthClient.print(0); EthClient.print (tm.Second); printEth(b);
    if (tipo==0)
      printEEmail(EthClient, dirEEdescpinD+(evenact[pinx]*LEN20), LEN20);
    else
      printEEmail(EthClient, dirEEdescTemp+(evenact[pinx]*LEN20), LEN20);
    printEth(b); 
    if (tipo==0) 
      if (evenvalD[pinx-numIA] == 1) printEth(ON); else printEth(OFF);
    else
      {
      EthClient.print(float(Mb.R[pinx])/100); 
      printEth(blancoC); 
      }
    EthClient.println("");   
    if (tm.Day < 10)  EthClient.print(0); EthClient.print (tm.Day);     printEth(barra);
    if (tm.Month < 9) EthClient.print(0); EthClient.print (tm.Month+1); printEth(barra);
    EthClient.print (tmYearToCalendar(tm.Year)); printEth(b);
    if (tm.Hour < 10)   EthClient.print(0); EthClient.print (tm.Hour);   
    printEth(dospuntos);
    if (tm.Minute < 10) EthClient.print(0); EthClient.print (tm.Minute); 
    printEth(dospuntos);
    if (tm.Second < 10) EthClient.print(0);  EthClient.println (tm.Second);
    if (tipo==0) 
      {
      printEEmail(EthClient, dirEEdescpinD+(evenact[pinx]*LEN20), LEN20);
      printEth(mailHaPasadoa); 
      if (evenvalD[pinx] == 1) printEth(ON); else printEth(OFF);
      }
    else
      {
      printEth(mailsonda); 
      printEEmail(EthClient, dirEEdescTemp+(evenact[pinx]*LEN20), LEN20);
      printEth(mailhasuperado); 
      EthClient.print(float(evenvalA[pinx])/100);
      printEth(mailvaloract); 
      EthClient.print(float(Mb.R[pinx])/100);
      printEth(blancoC);
      }
    EthClient.println(""); 
    printEth(mailelconuco);
    EthClient.println(); 
    EthClient.println(""); 
    printEth(punto);        //Cuando enviamos un punto indica el fin del mensaje
    if(!eRcv()) return 0;
    
    printEth(mailQUIT);
    EthClient.println();   // Cerramos la conexion telnet
    if(!eRcv()) return 0;
    EthClient.flush();
    EthClient.stop();      //Paramos el cliente
    return 1; 
}

//

byte eRcv()
{
  byte respCode;
  byte thisByte;
  while(!EthClient.available()) delay(delayACKMail);
  respCode = EthClient.peek();
  while(EthClient.available())
    thisByte = EthClient.read();    
  if(respCode >= '4')
    {
    efail();
    return 0;  
    }  
  return 1;
}
//

void efail()
{
  byte thisByte = 0;
  printEth(mailQuitrin);
  while(!EthClient.available()) delay(1);
  while(EthClient.available())
    thisByte = EthClient.read();    
  EthClient.stop();
}

void printparentesis(WebServer &server, char letra, int numero)
  {
  server.printP(paren_i);
  server.print(letra); 
  server.print(numero);
  server.printP(paren_f2b);
  }

void printcorchete(WebServer &server, int numero, byte base)
  {
  server.printP(corchete_i);
  if (base==10) server.print(numero);
  server.printP(corchete_f);
  }

void printDayName(WebServer &server, byte dia)
  {
    if (dia==0) server.printP(domingo);
    else if (dia==1) server.printP(lunes);
      else if (dia==2) server.printP(martes);
        else if (dia==3) server.printP(miercoles);
          else if (dia==4) server.printP(jueves);
            else if (dia==5) server.printP(viernes);
              else if (dia==6) server.printP(sabado);
  }

void printMes(WebServer &server, byte mes)
  {
    if (mes==0) server.printP(enero);
      else if (mes==1) server.printP(febrero);
        else if (mes==2) server.printP(marzo);
          else if (mes==3) server.printP(abril);
            else if (mes==4) server.printP(mayo);
              else if (mes==5) server.printP(junio);
                else if (mes==6) server.printP(julio);
                  else if (mes==7) server.printP(agosto);
                    else if (mes==8) server.printP(septiembre);
                      else if (mes==9) server.printP(octubre);
                        else if (mes==10) server.printP(noviembre);
                          else if (mes==11) server.printP(diciembre);
  }
  
boolean autOK(WebServer &server)
 {
    char codeori[41]="";
    char code64[41]="";
    strcpy(codeori,user1);
    strcat(codeori,":");
    strcat(codeori,pass1);
    int largo=strlen(codeori);  
    base64_encode(code64,codeori,largo);
    if ((usepass) && (!server.checkCredentials(code64)))
      return false;
    else
      return true;
 }
 
void logAcc(char *mensaje, int npin, char tipo, boolean activa)
 {
 if (!sdHay) return;
//  char *auxchr
  ofstream sdlog(namelogAcc, ios::out | ios::app);  
  if (sdlog)
    {
      char buf[3];
       sdlog << tmYearToCalendar(tm.Year);
       if (tm.Month < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Month+1,buf,10);
       if (tm.Day < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Day,buf,10);
       if (tm.Hour < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Hour,buf,10); 
       if (tm.Minute < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Minute,buf,10); 
       if (tm.Second < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Second,buf,10); 
       sdlog << tipo; 
       sdlog << itoa(npin,buf,10);
       printSD(sdlog, guion); 
       printSD(sdlog,activa?uno:cero);
       sdlog << char(10); 
       sdlog.close();
    }
 } 
 
void logEve(char *mensaje, int npin, char tipo, boolean activa)
 {
 if (!sdHay) return;
//  ofstream sdlog(namelogEve, ios::out | ios::app);  
//  if (sdlog)
//    {
//      char buf[3];
//       sdlog << tmYearToCalendar(tm.Year);
//       if (tm.Month < 10) printSD(sdlog,cero); 
//       sdlog << itoa(tm.Month+1,buf,10);
//       if (tm.Day < 10) printSD(sdlog,cero); 
//       sdlog << itoa(tm.Day,buf,10);
//       if (tm.Hour < 10) printSD(sdlog,cero); 
//       sdlog << itoa(tm.Hour,buf,10); 
//       if (tm.Minute < 10) printSD(sdlog,cero); 
//       sdlog << itoa(tm.Minute,buf,10); 
//       if (tm.Second < 10) printSD(sdlog,cero); 
//       sdlog << itoa(tm.Second,buf,10); 
//       sdlog << tipo; 
//       sdlog << itoa(npin,buf,10);
//       printSD(sdlog, guion);
//       sdlog << (activa?"ON":"OFF");
//       sdlog << char(10); 
//       sdlog.close();
//    }
 } 

void logAna(char *mensaje, int npin, char tipo, boolean activa)
 {
 if (!sdHay) return;
  ofstream sdlog(namelogAna, ios::out | ios::app);  
  if (sdlog)
    {
      char buf[3];
       sdlog << tmYearToCalendar(tm.Year);
       if (tm.Month < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Month+1,buf,10);
       if (tm.Day < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Day,buf,10);
       if (tm.Hour < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Hour,buf,10); 
       if (tm.Minute < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Minute,buf,10); 
       if (tm.Second < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Second,buf,10); 
       sdlog << tipo; 
       sdlog << itoa(npin,buf,10);
       printSD(sdlog, guion); 
       printSD(sdlog,activa?uno:cero);
       sdlog << itoa(npin,buf,10);
       printSD(sdlog, guion);
       sdlog << (activa?"ON":"OFF");
////////////// aqui va lo de ana
       sdlog << char(10); 
       sdlog.close();
    }
 } 

void logSys(char *mensaje, int npin, char tipo, boolean activa)
 {
 if (!sdHay) return;
  ofstream sdlog(namelogSys, ios::out | ios::app);  
  if (sdlog)
    {
      char buf[3];
       sdlog << tmYearToCalendar(tm.Year);
       if (tm.Month < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Month+1,buf,10);
       if (tm.Day < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Day,buf,10);
       if (tm.Hour < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Hour,buf,10); 
       if (tm.Minute < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Minute,buf,10); 
       if (tm.Second < 10) printSD(sdlog,cero); 
       sdlog << itoa(tm.Second,buf,10); 
       sdlog << tipo; 
       sdlog << mensaje; 
       sdlog << char(10); 
       sdlog.close();
    }
 } 

void logIP(char *mensaje, int npin, char tipo, boolean activa)
 {
 if (!sdHay) return;
  byte rip[] = {0,0,0,0 };
  EthernetClient client = webserver.available();
  client.getRemoteIP(rip); // where rip is defined as byte rip[] = {0,0,0,0 };
  if (rip[0] != 192)
    {
    ofstream sdlog(namelogIP, ios::out | ios::app);  
    if (sdlog)
      {
        char buf[3];
         sdlog << tmYearToCalendar(tm.Year);
         if (tm.Month < 10) printSD(sdlog,cero); 
         sdlog << itoa(tm.Month+1,buf,10);
         if (tm.Day < 10) printSD(sdlog,cero); 
         sdlog << itoa(tm.Day,buf,10);
         if (tm.Hour < 10) printSD(sdlog,cero); 
         sdlog << itoa(tm.Hour,buf,10); 
         if (tm.Minute < 10) printSD(sdlog,cero); 
         sdlog << itoa(tm.Minute,buf,10); 
         if (tm.Second < 10) printSD(sdlog,cero); 
         sdlog << itoa(tm.Second,buf,10); 
         sdlog << tipo; 
         printSD(sdlog, guion); 
         sdlog << mensaje;
         printSD(sdlog, guion); 
         for (int i= 0; i < 4; i++)
           { 
            sdlog << itoa(rip[i],buf,10);
            if (i<3) printSD(sdlog,punto);
           }  
         sdlog << char(10); 
         sdlog.close();
      }
    }
 } 
 
void logIPP(const prog_uchar *str, int npin, char tipo, boolean activa)
  {
    
  char mensaje[20]="";
  char c;
  byte longi=0;
  while((c = pgm_read_byte(str++)))
    {
      mensaje[longi]=c;
      longi++;
    }
  mensaje[longi]=0;
  logIP(mensaje, npin, tipo, activa);
  }


byte buscaNodoenTab(unsigned int nodo)
  {
    boolean encontrado=false;
    byte i=0;
    while ((!encontrado) && (i<maxNodos))
      {
        encontrado=(tremID[i]==nodo);
        if (encontrado)
          return i;
        else
          i++;
      }
    return 99;
  }

//******************************************************************************************************  
void outputPins(WebServer &server, WebServer::ConnectionType type)
{ 
//    
  if (LOG==1) logIPP(Panel, 0, 'I', false);
  
  if (!autOK(server))  {server.httpUnauthorized(); return; }
      
  if (type == WebServer::HEAD) return;
  server.httpSuccess();

  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1,seguimiento);
  if (peract != 0)   {
    server.printP(htmlRefresh_i);
    printIP(server, peract,comillascierre);
    }
  printP(server, head_f, body_i,tablamenu, trcolor1_i);
  printP(server, opc01,  opc13, tr_f, table_f,brn,tablapanel);

// ENLACES
    for (byte i = 0; i < 5; i++)
      if (getbit(bshowLink,i)==1)
       { 
        printP(server,tr,tdcentercolspan3,href_i);
        printEE (server, dirEEurlLink+(i*LEN80), LEN80);
        printP(server,href_cierre);
        printEE (server, dirEEdescLink+(i*LEN20), LEN20);
        printP(server,href_f,td_f,tr_f);
        }
// ESCENAS
    for (byte i = 0; i < nEsc; i++)
      if (getbit(bshowEsc,i)==1)
       { 
        printP(server,tr,td);
        printEE (server, dirEEdescEsc+(i*LEN20), LEN20);
        printP(server,td_f,td,hrefon);
        server.print(i+100); server.printP(ona_f);
        printP(server,td_f,td,td_f,tr_f);
        }
// TEMPERATURAS LOCALES
  byte val;
  if (nTemp1 > 0) {
    for (int i=0; i<nTemp1; ++i)
      if (getbit8(bshowTemp,i) == 1)
      {
        printTDTR(server, 0, true, true);
        if (showN == 1) 
         {
          printparentesis(server,'s',i);
          for (int j = 0; j < 8; j++) {
            server.printP(b);
            server.print(addr1Wire[i][j], HEX);
            }
         }
        server.printP(b);
        printEE (server, dirEEdescTemp+(i*LEN20), LEN20);
        printTDTR(server, 1, false, true);
        printColspan(server,2,true);
        server.print(float(Mb.R[i])/100);
        printP(server, celsius);     
        printTDTR(server, 1, true, true);
      }
  }
  
// ENTRADAS ANALÓGICAS
  for (byte i=0; i<16; ++i)    
    if (getbit8(bshowpin,i+41) == 1)
     {  
      printTDTR(server, 0, true, true);
//      if (showN == 1) printparentesis(server,'a',tabpin[i]);
      if (showN == 1) printparentesis(server,'a',i);
      printEE (server, dirEEdescpinA+(i*LEN20), LEN20);
      printTDTR(server, 1, false, true);
      printColspan(server,2,true);
      server.print(float(Mb.R[i+baseAna])*factorA[i]); 
      printP(server, b);
      printTDTR(server, 1, true, true);
     }

// ENTRADAS DIGITALES
    for (byte i = 0; i<16; ++i)
      if (getbit8(bshowpin,i) == 1)
        { 
        printP(server,tr,td);
        if (showN == 1) printparentesis(server,'d',tabpin[i]);
        printEE (server, dirEEdescpinD+(i*LEN20), LEN20);
        printP(server, td_f);

        if (tipoED[i]==0)      // entrada digital ON/OFF
          {
          val = digitalRead(tabpin[i]);
          printP(server,(val==0) ? td : th, (val==0) ? OFF : ON, (val==0) ? td_f : th_f);
          if (i<=7)    // contadores
            {
            printP(server,td);
            printPiP(server,resetcontp,i,barramayor);
            server.print(contadores[i]);
            printP(server,a_f,td_f);
            }
          }
        if (tipoED[i]>0)      // entrada digital DHT11, DHT21, DHT22
          {
          printP(server,td);
          server.print(vanaTED[i]); 
          printP(server, celsius,td_f,td);
          server.print(vanaHED[i]); 
          printP(server, porciento,td_f);
          }
       }

// SALIDAS DIGITALES
  if (numOD > 0)   {
    for (byte i = 17; i < nDIG-1; ++i)
      if (getbit8(bshowpin,i) == 1)
       { 
          char buf[3];
          itoa(tabpin[i],buf,10);
          val = valorpin[digitalRead(tabpin[i])];
          
          printP(server,tr, td);
          if (showN == 1) printparentesis(server,'d',tabpin[i]);
          printEE (server, dirEEdescpinD+(i*LEN20), LEN20);
          printP(server,td_f,(val==0) ? td : th,hrefon);  
          server.print(buf); 
          printP(server,ona_f,(val==0) ? td_f : th_f,td,hrefoff);  
          server.print(buf); 
          printP(server,offa_f,td_f,tr_f);
        }
      }


//  PUNTOS REMOTOS
    // TEMPERATURAS REMOTAS
    for (byte i =0; i < maxTempr; i++) 
      {
        if (getbit8(rbshowpin,i+120)==1) 
        {   // temperaturas remotas
          char buf[3];
          itoa(i,buf,10);
          printTDTR(server, 0, true, true);
          if (showN == 1) 
            {
             server.printP(paren_i);
             printPiP(server,rs,i,paren_f1b);
            }
          printEE (server, dirEEdescTempr+(i*LEN20), LEN20);
          if (showN == 1) {
            server.printP(b);
            printparentesis(server,'n',remTnodo[i]); 
            printparentesis(server,'p',remTpin[i]); 
            }
          byte n=buscaNodoenTab(remTnodo[i]);
          printP(server, td_f);
          printColspan(server,2,true);
//          if (tlastnodo[n]<60)
            {
            server.print(float(Mb.R[i+baseTempr])/100); server.printP(celsius);
            }
//          else
//            printP(server,nodisponible);
          printTDTR(server, 1, true, true);
          }
      }
     
    // SEÑALES REMOTAS 
    for (byte i =0; i < maxRem; i++) 
      {
      if (getbit8(rbshowpin,i) == 1)  
        { 
        char buf[3];
        itoa(i,buf,10);
        printTDTR(server, 0, true, false);
        
        if (i>=0 && i<20) 
          {    // entradas ANALÓGICAS remotas
          printP(server, td);
          if (showN == 1)   {
             server.printP(paren_i);
             printPiP(server,ra,i,paren_f1b);     }
            
          printEE (server, dirEEdescAnalr+(i*LEN20), LEN20);
            
          if (showN == 1) 
            {
              server.printP(b);
              printparentesis(server,'n',remNodo[i]);
              printparentesis(server,'p',remPin[i]);
            }
          byte n=buscaNodoenTab(remNodo[i]);
          printP(server,td_f);
          printColspan(server,2,true);
          server.print(float(Mb.R[i+baseAnar])*factorArem[i]); 
          printP(server, td_f);
          }
////          
        if (i>=20 && i<40) {    // entradas digitales remotas
          val = getbit8(Mb.C8,i+44);  // AQUÍ SE LEE EL VALOR DEL PIN REMOTO, a partir pos 64
          printP(server, td);
          if (showN == 1) 
            {
             server.printP(paren_i);
             printPiP(server,letrasred,i,paren_f1b);
            }
          printEE (server, dirEEdescEDigr+((i-20)*LEN20), LEN20);
          if (showN == 1) {
            server.printP(b);
            printparentesis(server,'n',remNodo[i]);
            printparentesis(server,'p',remPin[i]); }
          byte n=buscaNodoenTab(remNodo[i]);
          printP(server,td_f,(val==0) ? td : th);
          server.printP(val ? ON : OFF);
          printP(server,(val==0) ? td_f : th_f);
          }
//          
        if (i>=40 && i<60) {  // Salidas digitales remotas
          val = getbit8(Mb.C8,i+44);  // AQUÍ SE LEE EL VALOR DEL PIN REMOTO, a partir pos 84
          printP(server, td);
          if (showN == 1) 
            {
             server.printP(paren_i);
             printPiP(server,letrasrsd,i,paren_f1b);
            }
          printEE (server, dirEEdescSDigr+((i-40)*LEN20), LEN20);
          if (showN == 1) {
            server.printP(b);
            printparentesis(server,'n',remNodo[i]);
            printparentesis(server,'p',remPin[i]); }
          byte n=buscaNodoenTab(remNodo[i]);
          printP(server,td_f,(val==0) ? td : th);
          printPiP(server, hrefonr, i, ona_f);
          printP(server,(val==0) ? td_f : th_f,td);
          printPiP(server, hrefoffr, i, offa_f);
          printP(server,td_f);
          }
        if (i>=60 && i<maxRem) {  // Salidas relé remotos
          if ((getbit8(rbshowpin,i) ==1))
            {
            val = getbit8(Mb.C8,i+44);  // AQUÍ SE LEE EL VALOR DEL PIN REMOTO, a partir pos 104
            printP(server, td);
            if (showN == 1) 
              {
               printP(server, paren_i);
               printPiP(server,letrasrsd,i,paren_f1b);
              }
           printEE (server, dirEEdescReler+((i-60)*LEN20), LEN20);
           if (showN == 1) {
              server.printP(b);
              printparentesis(server,'n',remNodo[i]);
              printparentesis(server,'p',remPin[i]); 
              }
            byte n=buscaNodoenTab(remNodo[i]);
            printP(server,td_f,(val==0) ? td : th);
            printPiP(server, hrefonr, i, ona_f);  
            printP(server,(val==0) ? td_f : th_f,td);
            printPiP(server, hrefoffr, i, offa_f);  
            printP(server,td_f);
            }
          }
        printP(server,tr_f);
        }
      }
      
  printP(server, tr, tdcolspan3);   

  if (RTC.read(tm)) 
    { 
    printDayName(server, tm.Wday);
    server.printP(comablanco);      
    if (tm.Day < 10) server.printP(cero); server.print (tm.Day); server.printP(barra);
    printMes(server,tm.Month);
    server.printP(barra);
    server.print (tmYearToCalendar(tm.Year)); server.printP(blancos4);
    if (tm.Hour < 10) server.printP(cero); server.print (tm.Hour); server.printP(dospuntos);
    if (tm.Minute < 10) server.printP(cero); server.print (tm.Minute); server.printP(dospuntos);
    if (tm.Second < 10) server.printP(cero); server.print (tm.Second);
    }
  else    
    if (RTC.chipPresent())      
      server.printP(rtcparado);
    else 
      server.printP(rtcnodisponible);
  printP(server,td_f,tr_f,trcolor1_i,tdcolspan3, letrav, b);
  server.print(miversion);
  printP(server,punto);
  server.print(misubver);
  printP(server, b,elconuco,b);
//  server.print(mifreeRam());
  printP(server,barra);
  server.print(velocidad);
  printP(server, td_f, tr_f,table_f, body_f, html_f);
}

void writeMenu(WebServer &server, byte opcprin, byte opcsec)
  {
  printP(server,htmlHead_i0,htmlHead_ix,htmlHead_i1,head_f,body_i,tablamenu,trcolor1_i);    // formato menú
  if (opcprin==1) printP(server,opc01); else printP(server,opc11);  // PANEL    
  if (opcprin==3) printP(server,opc03); else printP(server,opc13);  // CONFIGURACIÓN
  if (opcprin==5) printP(server,opc05); else printP(server,opc15);  // REMOTOS
  if (opcprin==2) printP(server,opc02); else printP(server,opc12);  // PROGRAMACIÓN
  if (opcprin==4) printP(server,opc04); else printP(server,opc14);  // AVANZADO
  printP(server,tr_f,table_f,tablamenu,trcolor1_i);    // formato menú
  if (opcprin==1)    // PANEL
    {
    }
  if (opcprin==2)    // PROGRAMACIÓN
    {
    if (opcsec==5) printP(server,opc025); else printP(server,opc125);  // Semanal
    if (opcsec==1) printP(server,opc021); else printP(server,opc121);  // Semanal
    if (opcsec==2) 
      if (modo==5)    // modo clima
        printP(server,opc022clima); 
      else
        printP(server,opc022); 
    else 
       if (modo==5)    // modo clima
         printP(server,opc122clima); 
       else
          printP(server,opc122);  // Eventos
    if (opcsec==3) printP(server,opc023); else printP(server,opc123);  // Fechas
    if (opcsec==4) printP(server,opc024); else printP(server,opc124);  // Escenas
    }
  if (opcprin==3)    // CONFIGURACIÓN
    {
    if (opcsec==0) printP(server,opc030); else printP(server,opc130);  // Dispositivo
    if (opcsec==10) printP(server,opc03a); else printP(server,opc13a);  // Mis enlaces
    if (opcsec==1) printP(server,opc031); else printP(server,opc131);  // Sondas Temp.
    if (opcsec==7) printP(server,opc037); else printP(server,opc137);  // E/S Analog
    if (opcsec==8) if (modo==5) printP(server,opc038clima); else printP(server,opc038); 
    else  if (modo==5) printP(server,opc138clima); else printP(server,opc138); 
    if (opcsec==9) printP(server,opc039); else printP(server,opc139);  // Sal. Digitales
    if (opcsec==3) printP(server,opc033); else printP(server,opc133);  // Red  
    if (opcsec==5) printP(server,opc035); else printP(server,opc135);  // Seguridad  
    if (opcsec==6) printP(server,opc036); else printP(server,opc136);  // Fecha/Hora
    }
  if (modo==0)  // Avanzado
    if (opcprin==4)    // AVANZADO
      {
      if (opcsec==1) printP(server,opc041); else printP(server,opc141);  // logs
      if (opcsec==2) printP(server,opc042); else printP(server,opc142);  // files
      if (opcsec==5) printP(server,opc045); else printP(server,opc145);  // logs
      if (opcsec==6) printP(server,opc046); else printP(server,opc146);  // files
      if (opcsec==3) printP(server,opc043); else printP(server,opc143);  // show EEPROM
      if (opcsec==4) printP(server,opc044); else printP(server,opc144);  // reinicia EEPROM
      }
  if (opcprin==5)    // REMOTOS
    {
    if (opcsec==0) printP(server,opc050); else printP(server,opc150);  // General
    if (opcsec==9) printP(server,opc059); else printP(server,opc159);  // Nodos remotos
    if (opcsec==1) printP(server,opc051); else printP(server,opc151);  // Sondas Temp.
    if (opcsec==2) printP(server,opc052); else printP(server,opc152);  // E/S Analog
    if (opcsec==3) printP(server,opc053); else printP(server,opc153);  // Ent. Digitales
    if (opcsec==4) printP(server,opc054); else printP(server,opc154);  // Sal. digitales  
    if (opcsec==5) printP(server,opc055); else printP(server,opc155);  // Relés
    if (opcsec==6) printP(server,opc056); else printP(server,opc156);  // Conf. nodo remoto
    if (opcsec==7) printP(server,opc057); else printP(server,opc157);  // Explora nodo remoto
    if (opcsec==8) printP(server,opc058); else printP(server,opc158);  // Reset nodo remoto
    }
   printP(server,tr_f,table_f);
  }

void setupPrgHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sprghtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 2;    // número de parámetros por fila

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    int indice,resto,mival;
    memset(actPrg,0,sizeof(actPrg));
    do   
      {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      indice = param_number / mp;
      resto = param_number % mp;
      mival = atoi(value);
      if (resto==0) 
        for (int j=0; j<LEN20;j++) 
          EEwrite(dirEEdescPrg+(indice*LEN20)+j, value[j]);
      if (resto==1) actPrg[indice]=mival;
      } while (repeat);
    EEPROM_writeAnything(dirEEactprg, actPrg);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sprghtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server, 2, 5);
  printP(server,brn,Form_action,sprghtm,Form_post,Form_input_send);
  printP(server,tablaconf,trcolor1_i,td_if);
  printTDP(server, descripcion, false, true);
  printTDP(server, activo, false, true);
  printP(server,tr_f);
  char buf[3];
  for (int i=0;i<5;i++) 
    {
      int mpi=mp*i;
      printP(server,trcolor1_i);
      printP(server,td,programa,b);
      server.print(i+1);                   
      printP(server,td_f);
      
      if (actPrg[i]==1) printP(server,th); else printP(server,td);
      printP(server,Form_input_text_start);      
      server.print(mpi);    // número de parámetro                  
      server.printP(Form_input_value);        
      printEE (server, dirEEdescPrg+(i*LEN20), LEN20);
      printPiP(server,Max_length,LEN20-1,size_i);
      server.print(LEN20-1);
      printP(server,Form_input_end);
      if (actPrg[i]==1) printP(server,th_f); else printP(server,td_f);
      
      miCheckBox(server, "", (actPrg[i]==1), itoa(mpi+1,buf,10), "1", (actPrg[i]==1),true);
    }
  server.printP(tr_f);
  server.printP(table_f);
  
  //print the send button
  printP(server,Form_input_send,Form_f,body_f,html_f);  
}

void setupLinksHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN80];
  int param_number = 0;
  int mp = 3;    // número de parámetros por fila
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    int indice,resto,mival;
    bshowLink=0;
    do   
      {
      repeat = server.readPOSTparam(name, LEN5, value, LEN80);
      if (!repeat) break;
      param_number = atoi(name);  
      indice = param_number / mp;
      resto = param_number % mp;
      mival = atoi(value);
      if (resto==0) 
        for (int j=0; j<LEN20;j++) 
          EEwrite(dirEEdescLink+(indice*LEN20)+j, value[j]);
      if (resto==1) 
        for (int j=0; j<LEN80;j++) 
          EEwrite(dirEEurlLink+(indice*LEN80)+j, value[j]);
      if (resto==2) 
        setbit8(&bshowLink,indice,mival);
      } while (repeat);

    EEPROM_writeAnything(dirEEbshowLink, bshowLink);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, slkhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server, 3, 10);
  printP(server,brn,Form_action,slkhtm,Form_post,Form_input_send);
  printP(server,tablaconf,trcolor1_i,td_if);
  printTDP(server, descripcion, false, true);
  printTDP(server, urltext, false, true);
  printTDP(server, activo, false, true);
  printP(server,tr_f);
  char buf[3];
  for (int i=0;i<5;i++) 
    {
      int mpi=mp*i;
      printP(server,trcolor1_i);
      printP(server,td,linkusuario,b);
      server.print(i+1);                   
      printP(server,td_f);
      
      if (getbit(bshowLink,i)==1) printP(server,th); else printP(server,td);
      printP(server,Form_input_text_start);      
      server.print(mpi);    // número de parámetro                  
      server.printP(Form_input_value);        
      printEE (server, dirEEdescLink+(i*LEN20), LEN20);
      printPiP(server,Max_length,LEN20-1,size_i);
      server.print(LEN20-1);
      printP(server,Form_input_end);
      if (getbit(bshowLink,i)==1) printP(server,th_f); else printP(server,td_f);
      
      if (getbit(bshowLink,i)==1) printP(server,th); else printP(server,td);
      printP(server,Form_input_text_start);      
      server.print(mpi+1);    // número de parámetro                  
      server.printP(Form_input_value);        
      printEE (server, dirEEurlLink+(i*LEN80), LEN80);
      printPiP(server,Max_length,LEN80-1,size_i);
      server.print(LEN80-1);
      printP(server,Form_input_end);
      if (getbit(bshowLink,i)==1) printP(server,th_f); else printP(server,td_f);
      
      miCheckBox(server, "", (getbit(bshowLink,i)==1), itoa(mpi+2,buf,10), "1", (getbit(bshowLink,i)==1),true);
    }
  server.printP(tr_f);
  server.printP(table_f);
  
  //print the send button
  printP(server,Form_input_send,Form_f,body_f,html_f);  
}

void setupEscHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (seschtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 5;    // número de parámetros por fila

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    int indice,resto,mival;
    memset(bescena,0,sizeof(bescena));
    bshowEsc=0;
    do   
      {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      mival = atoi(value);
      if (param_number>=500)
        {
        if (param_number>=600)
          {
          setbit(&bshowEsc,param_number-600, mival);
          }  
        else
         {
          for (byte i=0; i<LEN20; i++)
            EEwrite(dirEEdescEsc+((param_number-500)*LEN20)+i, value[i]);
        }
        }
      else
        {
        param_number = atoi(name);     
        indice = param_number / mp;
        resto = param_number % mp;
        setbit8(bescena[resto],indice,mival);
        }
      } while (repeat);

    EEPROM_writeAnything(dirEEesc, bescena);
    EEPROM_writeAnything(dirEEbshowEsc, bshowEsc);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, seschtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server, 2, 4);
  printP(server,brn,Form_action,seschtm,Form_post,Form_input_send);
  printP(server,tablaconf,trcolor1_i,td_if);
  printTDP(server, escena1, false, true);
  printTDP(server, escena2, false, true);
  printTDP(server, escena3, false, true);
  printTDP(server, escena4, false, true);
  printTDP(server, escena5, false, true);
  printP(server,tr_f);

  printP(server,trcolor1_i);
  printTDP(server, descripcion, false, true);
  char buf[3];
  for (int i=0;i<nEsc;i++) 
    {
    printP(server,(getbit(bshowEsc,i)==1)?th:td);
    printP(server,Form_input_text_start);      
    server.print(500+i);                   
    server.printP(Form_input_value);        
    printEE (server, dirEEdescEsc+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    miCheckBox(server,"",getbit(bshowEsc,i)==1,itoa(600+i,buf,10), "1", (getbit(bshowEsc,i)==1),false);   
    printP(server,(getbit(bshowEsc,i)==1)?th_f:td_f);
    }
  server.printP(tr_f);
  for (int i=0;i<86;i++)      
   if (((modo==2) && (i<=15)) || (modo!=2))
    {  
    int mpi = mp * i;
    printP(server,tr,td);
    if (i<=15) printEE (server, dirEEdescpinD+((i+17)*LEN20), LEN20);
    printP(server,td_f);
    char *milabel;
    for (int j=0; j<nEsc; j++)
      {
      if (getbit8(bescena[j],i)==1) milabel="ON"; else milabel="Off";
      miCheckBox(server,milabel,getbit8(bescena[j],i)==1,itoa(mpi+j,buf,10), "1", (getbit8(bescena[j],i)==1),true);   
      }
    server.printP(tr_f);
    }
  server.printP(table_f);
  
  //print the send button
  printP(server,Form_input_send,Form_f,body_f,html_f);  
}

void setupEveHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (svhtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 15;    // número de parámetros por fila
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    boolean enANA;
    int indice; int resto;
    memset(bPRGeve,0,sizeof(bPRGeve));
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      indice =(param_number / mp);
      resto= (param_number % mp);
      int mival = atoi(value);
      if (param_number<999)    
          {      
//          if (resto==0) setbit8(bacteve,indice,mival);
          if (resto==1) setbit8(beveacttipo,indice,mival);  // Tipo activadora: local o remota
          if (resto==2) evenact[indice] = mival;            // señal activadora 
          if (resto==3) evenvalD[indice] = mival;           // valor activador si digital
          if (resto==4) evencomp[indice]=mival;             // tipo comparador: meno-igual o mayor-igual 
          if (resto==5) evenvalA[indice] = atof(value)*100; // valor consigna 
          if (resto==6) evenhis[indice] = atof(value)*100;  // histéresis  
          if (resto==7) setbit8(bevesaltipo,indice,mival);  // Tipo salida: local o remota
          if (resto==8) evensal[indice] = mival;            // señal a activar
          if (resto==9) setbit8(bevenniv,indice, mival);    // valor que debe tomar la señal de salida
          if (resto>=10) {setbit8(bPRGeve[resto-10],indice,mival);  }   // asociacion PRG
          }
      } while (repeat);
    guardarDatosEventos();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, svhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server, 2,2);
  printP(server, brn,Form_action, svhtm, Form_post, Form_input_send);
  
  printP(server,tablaconf);
  server.printP(trcolor1_i);
  printP(server,td,evento,td_f,tdcolspan5,asociaraprograma,tdcolspan2,disparador,td_f);

  printTDP(server, dig, false, true);
  server.printP(tdcolspan3);
  printTDP(server, ana, false, false);
  server.printP(td_f);
  server.printP(tdcolspan3);
  printTDP(server, b, false, false);
  server.printP(td_f);
  server.printP(tr_f);

  server.printP(trcolor1_i);
  printTDP(server, n, false, true);
  printTDP(server, uno, false, true);
  printTDP(server, dos, false, true);
  printTDP(server, tres, false, true);
  printTDP(server, cuatro, false, true);
  printTDP(server, cinco, false, true);
  printTDP(server, lor, false, true);
  printTDP(server, senal, false, true);
  printTDP(server, onoff, false, true);
  printTDP(server, compa, false, true);
  printTDP(server, valor, false, true);
  printTDP(server, hist, false, true);
  printTDP(server, lor, false, true);
  printTDP(server, senalsalida, false, true);
  printTDP(server, onoff, false, true);
  server.printP(tr_f);
// 
/////////////////////////////////////////////////////////
  char buf[3];
  for (byte i=0;i<nPRG;i++)     
    {
    boolean colorea = ((getbit8(bPRGeve[0],i)==1) || (getbit8(bPRGeve[1],i)==1) ||
                       (getbit8(bPRGeve[2],i)==1) || (getbit8(bPRGeve[3],i)==1) ||
                       (getbit8(bPRGeve[4],i)==1));
    boolean actlocal= (getbit8(beveacttipo,i)==0);
    boolean actdigital= (evenact[i] <= 32) || ((evenact[i] >= 71) && (evenact[i] <= 170));
    boolean sallocal= (getbit8(bevesaltipo,i)==0);

    int  mpi = mp * i;
    int indice = (i*12)+420;  // parámetros del 420/426 en adelante
    printP(server,tr);   

    if (colorea) printP(server,th); else printP(server,td);
    server.print(i+1);
    if (colorea) printP(server,th_f); else printP(server,td_f);
    
    for (byte j=0;j<5;j++)
      miCheckBox(server, "",(getbit8(bPRGeve[j],i)==1),itoa(mpi+10+j,buf,10),"1",getbit8(bPRGeve[j],i)==1,true);

    if (colorea)
      {
      if (colorea) printP(server,th); else printP(server,td);
      if (modo==2)
        printP(server,Local);
      else
        printcampoSiNo(server, mpi+1, getbit8(beveacttipo,i), Local,Local, true, false);    // valor local/remota
      if (colorea) printP(server,th_f); else printP(server,td_f);

      if (colorea) printP(server,th); else printP(server,td);
      server.printP(Select_name);
      server.print(mpi+2);   server.printP(barramayor);
      if (actlocal) // señal activadora es local
        {
        for (byte j=0; j<16; j++)  {  // añade entradas digitales locales
          printPiP(server, optionvalue, j, barraatras);
          if (j == evenact[i]) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          }
        for (byte j=17; j<33; j++)  {  // añade salidas digitales locales
          printPiP(server, optionvalue, j, barraatras);
          if (j == evenact[i]) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          }
        for (byte j=0; j<nANALO; j++)  {  // añade entradas analógicas locales
          printPiP(server, optionvalue, j+33, barraatras);
          if (j+33 == evenact[i]) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j+33, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinA+(j*LEN20), LEN20);
          }
        for (byte j=0; j<maxTemp; j++)   // añade temperaturas locales
          if (getbit8(bshowTemp,j) == 1)        {             
            printPiP(server, optionvalue, j+171, barraatras);
            if (j+171 == evenact[i]) server.printP(selected);
            if (showN == 1) printPiP(server, mayorparen, j+171, parenguion); else server.printP(cierre);
            printEE (server, dirEEdescTemp+(j*LEN20), LEN20);
            }
        for (byte j=0; j<1; j++)   // añade precio kwh
          if (getbit8(bshowTemp,j) == 1)        {             
            printPiP(server, optionvalue, j+254, barraatras);
            if (j+254 == evenact[i]) server.printP(selected);
            if (showN == 1) printPiP(server, mayorparen, j+254, parenguion); else server.printP(cierre);
            printP(server,preciokwh);
            }
          }
      printP(server,  Select_f, td_f);
      if (actdigital)    // la señal activadora es digital
        {
        printP(server,th);   // valor ON/OFF que dispara
        printcampoSiNo(server, mpi+3, evenvalD[i], on, off,true,false);
        server.printP(th_f); 
        printP(server,th,th_f,th,th_f,th,th_f);
        }
      else
        {
        printP(server,th,th_f,th);
        server.printP(Select_name);
        server.print(mpi+4);   server.printP(barramayor);
        for (byte j=0; j<2; j++)  {  // añade salidas posibles
          printPiP(server, optionvalue, j, barraatras);
          if (evencomp[i]==j) server.printP(selected);
          server.printP(cierre);
          server.printP((j==0)?mayoroigual:menoroigual);
          server.printP(option_f);  
          }
        printP(server,  Select_f,th_f,th);
        printcampoF(server,mpi+5,float(evenvalA[i])/100, 5,2, false);
        printP(server,th_f,th); 
        printcampoF(server,mpi+6,float(evenhis[i])/100, 5,2, false);
        printP(server,th_f); 
        }
      if (colorea) printP(server,th_f,th); else printP(server,td_f,th);
      if (modo==2)
        printP(server,Local);
      else
        printcampoSiNo(server, mpi+7, getbit8(bevesaltipo,i), Local,Local, true, false);    // valor local/remota
      printP(server,th_f,th,Select_name);
      server.print(mpi+8);   
      printP(server,barramayor);  // señal de salida
      if (sallocal)    // salida a activar es local
        {
        for (byte j=17; j<33; j++)   {
          printPiP(server,optionvalue,j,barraatras);
          if (evensal[i] == j) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          }  
        for (byte m=0; m<3; m++)  {  // añade eventos correo
          server.printP(optionvalue); server.print(m+despMail); server.printP(barraatras);
          if (m+despMail == evensal[i]) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, m+despMail, parenguion); else server.printP(cierre);
          server.print(mailDEST[m]); }
        }
      printP(server, option_f, Select_f,th_f,th);
      printcampoSiNo(server, mpi+9, getbit8(bevenniv,i), on,off, true, false);
      server.printP(th_f);    // Nivel de la salida
      }
    }
  printP(server, table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupClimaHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sclhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 15;    // número de parámetros por fila
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    boolean enANA;
    int indice; int resto;
    memset(bPRGeve,0,sizeof(bPRGeve));
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      indice =(param_number / mp);
      resto= (param_number % mp);
      int mival = atoi(value);
      if (param_number<999)    
          {      
//          if (resto==0) setbit8(bacteve,indice,mival);
          if (resto==1) setbit8(beveacttipo,indice,mival);  // Tipo activadora: local o remota
          if (resto==2) evenact[indice] = mival;            // señal activadora 
          if (resto==3) evenvalD[indice] = mival;           // valor activador si digital
          if (resto==4) evencomp[indice]=mival;             // tipo comparador: meno-igual o mayor-igual 
          if (resto==5) evenvalA[indice] = atof(value)*100; // valor consigna 
          if (resto==6) evenhis[indice] = atof(value)*100;  // histéresis  
          if (resto==7) setbit8(bevesaltipo,indice,mival);  // Tipo salida: local o remota
          if (resto==8) evensal[indice] = mival;            // señal a activar
          if (resto==9) setbit8(bevenniv,indice, mival);    // valor que debe tomar la señal de salida
          if (resto>=10) {setbit8(bPRGeve[resto-10],indice,mival);}   // asociacion PRG
          }
      } while (repeat);
    guardarDatosEventos();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sclhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server, 2,2);
  printP(server, brn,Form_action, sclhtm, Form_post, Form_input_send,tablaconf,trcolor1_i);
  printTDP(server, n, false, true);
  printTDP(server, letraa, false, true);
  printTDP(server, circuito, false, true);
  printTDP(server, onoff, false, true);
  printTDP(server, termostato, false, true);
  printTDP(server, onoff, false, true);
  server.printP(tr_f);
// 
/////////////////////////////////////////////////////////
  char buf[3];
  for (byte i=0;i<nPRG;i++)     
    {
    boolean colorea = ((getbit8(bPRGeve[0],i)==1));
    boolean actlocal= (getbit8(beveacttipo,i)==0);
    boolean actdigital= (evenact[i] <= 32) || ((evenact[i] >= 71) && (evenact[i] <= 170));
    boolean sallocal= (getbit8(bevesaltipo,i)==0);

    int  mpi = mp * i;
    int indice = (i*12)+420;  // parámetros del 420/426 en adelante

    if (colorea) printP(server,tr,th); else printP(server,tr,td);
    server.print(i+1);
    if (colorea) printP(server,th_f); else printP(server,td_f);
      
  //    for (byte j=0;j<5;j++)
    for (byte j=0;j<1;j++)
      miCheckBox(server, "",(getbit8(bPRGeve[j],i)==1),itoa(mpi+10+j,buf,10),"1",getbit8(bPRGeve[j],i)==1,true);

    if (colorea)
      {
        printP(server,th,Select_name);
        server.print(mpi+8);   server.printP(barramayor);  // señal de salida
        if (sallocal)    // salida a activar es local
          {
          for (byte j=17; j<33; j++)   {
            printPiP(server,optionvalue,j,barraatras);
            if (evensal[i] == j) server.printP(selected);
            if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
            printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
            }  
          }
        printP(server, option_f, Select_f,th_f,th);
        printcampoSiNo(server, mpi+9, getbit8(bevenniv,i), on,off, true, false);
        server.printP(th_f);    // Nivel de la salida
      }
  
    if (colorea)
      {
      printP(server,th,Select_name);
      server.print(mpi+2);   server.printP(barramayor);
      if (actlocal) // señal activadora es local
        {
        for (byte j=0; j<16; j++)  {  // añade entradas digitales locales
          printPiP(server, optionvalue, j, barraatras);
          if (j == evenact[i]) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          }
        }
      printP(server,  Select_f, td_f);
      if (actdigital)    // la señal activadora es digital
        {
        printP(server,th);   // valor ON/OFF que dispara
        printcampoSiNo(server, mpi+3, evenvalD[i], on, off,true,false);
        server.printP(th_f); 
//        printP(server,th,th_f,th,th_f,th,th_f);
        }
      printP(server,th_f);
      }
    }
  
  printP(server, table_f,Form_input_send,Form_f,body_f,html_f);
}

void printSD(ofstream &sdlog, const prog_uchar *str)
  {
  char c;
  while((c = pgm_read_byte(str++)))
    sdlog << c;  
  }

void logTemp()
 {
  ofstream sdlog(namelogTem, ios::out | ios::app);  
  if (sdlog)
    {
     char buf[3];
     sdlog << tmYearToCalendar(tm.Year);
     if (tm.Month < 10) printSD(sdlog,cero); 
     sdlog << itoa(tm.Month+1,buf,10);
     if (tm.Day < 10) printSD(sdlog,cero); 
     sdlog << itoa(tm.Day,buf,10); 
//     printSD(sdlog, guion); 
     if (tm.Hour < 10) printSD(sdlog,cero); 
     sdlog << itoa(tm.Hour,buf,10); 
     if (tm.Minute < 10) printSD(sdlog,cero); 
     sdlog << itoa(tm.Minute,buf,10); 
     if (tm.Second < 10) printSD(sdlog,cero); 
     sdlog << itoa(tm.Second,buf,10); 
     printSD(sdlog, guion); 
     char buff[10];
     for (int i=0; i<80;i++)
       {
        sdlog << itoa((Mb.R[i]/100),buff,10); 
        printSD(sdlog,punto);
        sdlog << itoa((Mb.R[i] % 100),buff,10); 
        printSD(sdlog,puntoycoma);
       }
     sdlog << char(10) << char(13); 
     sdlog.close();
    }
 }

void pinVAL(int n, byte value)
  { 
    if (valorpin[digitalRead(n)] != value)
      {
       digitalWrite(n, valorpin[value]);
       setbit8(Mb.C8, n, value);
       EEwrite(dirEEestado+(n/8), Mb.C8[n/8]);    // el byte modificado
      }
  }

int ipnodo(char *id)
  {
  byte auxid=atoi(id);
  int i=0;
  while ((i<maxNodos) && (tremID[i]!=auxid))
    i++;
  if (i<maxNodos)
    {
    return tremIP[i];
    }
  return 0;
  }

void pinVALcond(int n, byte value)
  { 
    if (digitalRead(n) != valorpin[value])
      {
       digitalWrite(n, valorpin[value]);
       setbit8(Mb.C8, n, value);
       EEwrite(dirEEestado+(n/8), Mb.C8[n/8]);    // el byte modificado
      }
  }
  
void onescena(int nesc)
  {
    for (int j=0; j<16; j++)    //  Salidas digitales locales
      pinVAL (tabpin[j+17],getbit8(bescena[nesc],j));
  }
  
void onCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  int auxi = atoi(url_tail);
  if (auxi < nPINES)
    {
    pinVAL(auxi,1);
    if (LOG==1) logAcc("",auxi,'L',true);
    }
  else
    {
    onescena(auxi-100);
    if (LOG==1) logAcc("",auxi-100,'E',true);
    }
  server.httpSeeOther(PREFIX "/");
}

void offCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  int auxi = atoi(url_tail);
  if (auxi < nPINES)
    {
    pinVAL(auxi,0);
    if (LOG==1) logAcc("",auxi,'L',false);
    }
  server.httpSeeOther(PREFIX "/");
}

void onrHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  int auxpos=atoi(url_tail);
  if ((auxpos >=0) && (auxpos<120))
    {
    int auxnod=remNodo[auxpos];
    char buff[4]; memset(buff,0,sizeof(buff));
    char ipaux[16]; memset(ipaux,0,sizeof(ipaux));
    strcatP(ipaux,raizgwipdefault);    
    strcat(ipaux,itoa(ipnodo(itoa(auxnod,buff,10)),buff,10));
    char auxchr[20]="";
    strcatP(auxchr,cmdonr);    strcatP(auxchr,igual);    
    strcat(auxchr,itoa(remPin[auxpos],buff,10));
    dserial.println(mysocket);
    dserial.println(ipaux);
    dserial.println(gatewayPort);
    dserial.println(auxchr);
    int auxerr=wifi.SendRequest(mysocket,ipaux,gatewayPort,auxchr,3000,5000);
//    int auxerr=wifi.SendMsg(mysocket,ipaux,gatewayPort,auxchr,3000);
    dserial.println(auxerr);
    dserial.println(wifi.inputstr);
    }
  server.httpSeeOther(PREFIX "/");
}

void offrHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  
  int auxpos=atoi(url_tail);
  if ((auxpos >=0) && (auxpos<120))
    {
    int auxnod=remNodo[auxpos];
    char buff[4]="";
    char ipaux[16]="";
    strcatP(ipaux,raizgwipdefault);    
    strcat(ipaux,itoa(ipnodo(itoa(auxnod,buff,10)),buff,10));
//    strcat(ipaux,ipnodo(itoa(auxnod,buff,10)));
    char auxchr[20]="";
    strcatP(auxchr,cmdoffr);    strcatP(auxchr,igual);    
    strcat(auxchr,itoa(remPin[auxpos],buff,10));
    dserial.println(auxchr);
    int auxerr=wifi.SendRequest(mysocket,ipaux,gatewayPort,auxchr,3000,5000);
    dserial.println(auxerr);
    }

  server.httpSeeOther(PREFIX "/");
}

boolean enviado=true;

void ShowFileCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIP (url_tail, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  ifstream sdlog(url_tail);
  ArduinoOutStream cout(Serial);
  int c;
  if (sdlog.is_open()) 
    {
    while ((c = sdlog.get()) >= 0) 
      server.print((char)c);
    }
  else
    printP(server,ficheronoencontrado);
}

void indexHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
  { 
  outputPins(server, type);  
  }

void leerDatosDevice()   {
  for (byte i=0; i<LEN20; i++) user1[i] = EEread(dirEEuser+i);
  for (byte i=0; i<LEN20; i++) pass1[i] = EEread(dirEEpass+i);
  
  EEPROM_readAnything (dirEEmodo, modo);
  EEPROM_readAnything (dirEEsdHay, sdHay);
  EEPROM_readAnything (dirEEprectemp, prectemp);
  EEPROM_readAnything (dirEEnumIA, numIA);
  EEPROM_readAnything (dirEEnumID, numID);
  EEPROM_readAnything (dirEEnumOD, numOD);
  EEPROM_readAnything (dirEEshown, showN);
  EEPROM_readAnything (dirEEperact, peract);
  EEPROM_readAnything (dirEEusepass, usepass);
  EEPROM_readAnything(dirEEbshowLink, bshowLink);
  for (int i=0; i<8; i++)
    Mb.C8[i]=EEread(dirEEestado+i);    // los 8 primeros bytes de Mb.C8=bestado
  EEPROM_readAnything (dirEEbshowpin, bshowpin);
  EEPROM_readAnything (dirEEtipoED, tipoED);
  EEPROM_readAnything (dirEEshowTemp, bshowTemp);
  EEPROM_readAnything (dirEEshowTemp, bshowTemp);
  EEPROM_readAnything (dirEEshowTemp, bshowTemp);
//  EEPROM_readAnything (dirEEfactorA, factorA);

  if (numIA > nANALO) numIA = nANALO;
  if (numIA+numID > nPINES) numID = nPINES - numIA;
  if (numIA+numID+numOD > nPINES) numOD = nPINES - numIA - numID;
  numPines = numIA+numID+numOD;
  EEPROM_readAnything(dirEEactprg, actPrg);
  EEPROM_readAnything(dirEEpmactVar, pmactVar);
  EEPROM_readAnything(dirEEtipoLCD, tipoLCD);
  EEPROM_readAnything(dirEEbshowEsc, bshowEsc);
  }
//
void leerDatosRed()    {
    EEPROM_readAnything (dirEEmac, EEmac);
    EEPROM_readAnything (dirEEip, EEip);
    EEPROM_readAnything (dirEEmask, EEmask);
    EEPROM_readAnything (dirEEgw, EEgw);
    EEPROM_readAnything (dirEEdns, EEdns);
    EEPROM_readAnything (dirEEwebport, EEwebPort);
    EEPROM_readAnything (dirEEmailPER, mailPER);  
    EEPROM_readAnything (dirEEmailACT, mailACT);  
    EEPROM_readAnything (dirEEmailSVR, mailSVR);  
    EEPROM_readAnything (dirEEmailPORT, mailPORT);  
    EEPROM_readAnything (dirEEmailUSER, mailUSER);  
    EEPROM_readAnything (dirEEmailPASS, mailPASS);  
    EEPROM_readAnything (dirEEmailSENDER, mailSENDER);  
    EEPROM_readAnything (dirEEmailDEST, mailDEST);  
    EEPROM_readAnything (dirEEdyndnsHost, dyndnsHost);  
    EEPROM_readAnything (dirEEdyndnsUser, dyndnsUser);  
    EEPROM_readAnything (dirEEdyndnsPass, dyndnsPass);  
  }
  
void guardarDatosRed()    {
    EEPROM_writeAnything (dirEEmac, EEmac);
    EEPROM_writeAnything (dirEEip, EEip);
    EEPROM_writeAnything (dirEEmask, EEmask);
    EEPROM_writeAnything (dirEEgw, EEgw);
    EEPROM_writeAnything (dirEEdns, EEdns);
    EEPROM_writeAnything (dirEEwebport, EEwebPort);  
    EEPROM_writeAnything (dirEEmailPER, mailPER);  
    EEPROM_writeAnything (dirEEmailACT, mailACT);  
    EEPROM_writeAnything (dirEEmailSVR, mailSVR);  
    EEPROM_writeAnything (dirEEmailPORT, mailPORT);  
    EEPROM_writeAnything (dirEEmailUSER, mailUSER);  
    EEPROM_writeAnything (dirEEmailPASS, mailPASS);  
    EEPROM_writeAnything (dirEEmailSENDER, mailSENDER);  
    EEPROM_writeAnything (dirEEmailDEST, mailDEST);  
    EEPROM_writeAnything (dirEEdyndnsHost, dyndnsHost);  
    EEPROM_writeAnything (dirEEdyndnsUser, dyndnsUser);  
    EEPROM_writeAnything (dirEEdyndnsPass, dyndnsPass);  
  }
    
void leerDatosEventos()  {
  EEPROM_readAnything (dirEEevenact, evenact);
  EEPROM_readAnything (dirEEevencomp, evencomp);
  EEPROM_readAnything (dirEEevenvalA, evenvalA);
  EEPROM_readAnything (dirEEevenvalD, evenvalD);
  EEPROM_readAnything (dirEEevensal, evensal);
  EEPROM_readAnything (dirEEevenniv, bevenniv);
  EEPROM_readAnything (dirEEevenhis, evenhis);   
  EEPROM_readAnything (dirEEevenTemp, evenTemp);  
  EEPROM_readAnything (dirEEbeveacttipo, beveacttipo);  
  EEPROM_readAnything (dirEEbevesaltipo, bevesaltipo);  
  EEPROM_readAnything (dirEEbacteve, bacteve);
  EEPROM_readAnything (dirEEPRGeve, bPRGeve);
  }

void guardarDatosEventos()  {
  EEPROM_writeAnything (dirEEevenact, evenact);
  EEPROM_writeAnything (dirEEevencomp, evencomp);
  EEPROM_writeAnything (dirEEevenvalA, evenvalA);
  EEPROM_writeAnything (dirEEevenvalD, evenvalD);
  EEPROM_writeAnything (dirEEevensal, evensal);
  EEPROM_writeAnything (dirEEevenhis, evenhis);
  EEPROM_writeAnything (dirEEevenniv, bevenniv);  
  EEPROM_writeAnything (dirEEevenTemp, evenTemp);  
  EEPROM_writeAnything (dirEEbeveacttipo, beveacttipo);  
  EEPROM_writeAnything (dirEEbevesaltipo, bevesaltipo);  
  EEPROM_writeAnything (dirEEbacteve, bacteve);
  EEPROM_writeAnything (dirEEPRGeve, bPRGeve);
  }
    
void guardarDatosFechas()  {
  EEPROM_writeAnything (dirEEfecsal, fecsal);
  EEPROM_writeAnything (dirEEfecval, bfecval);
  EEPROM_writeAnything (dirEEfecdia, fecdia);
  EEPROM_writeAnything (dirEEfecmes, fecmes);
  EEPROM_writeAnything (dirEEfecano, fecano);
  EEPROM_writeAnything (dirEEfechor, fechor);
  EEPROM_writeAnything (dirEEfecmin, fecmin);    
  EEPROM_writeAnything (dirEEbfectipo, bfectipo);    
  EEPROM_writeAnything (dirEEbactfec, bactfec);
}  
  
void leerDatosFechas()  {
  EEPROM_readAnything (dirEEfecsal, fecsal);
  EEPROM_readAnything (dirEEfecdia, fecdia);
  EEPROM_readAnything (dirEEfecmes, fecmes);
  EEPROM_readAnything (dirEEfecano, fecano);
  EEPROM_readAnything (dirEEfechor, fechor);
  EEPROM_readAnything (dirEEfecmin, fecmin);    
  EEPROM_readAnything (dirEEbfectipo, bfectipo);    
  EEPROM_readAnything (dirEEbactfec, bactfec);
}  

void leerDatosSemanal()  {
  EEPROM_readAnything (dirEEprgsal, prgsal);
  EEPROM_readAnything (dirEEprgval, bprgval);
  EEPROM_readAnything (dirEEprgdia, prgdia);
  EEPROM_readAnything (dirEEprghor, prghor);
  EEPROM_readAnything (dirEEprgmin, prgmin);
  EEPROM_readAnything (dirEEprgcon, prgcon);
//  EEPROM_readAnything (dirEEprgconv, prgconv);
  EEPROM_readAnything (dirEEprgcomp, prgcomp);
  EEPROM_readAnything (dirEEbprgtipo, bprgtipo);
  EEPROM_readAnything (dirEEbactsem, bactsem);
  EEPROM_readAnything (dirEEPRGsem, bPRGsem);
  }

void guardarDatosSemanal()  {
  EEPROM_writeAnything (dirEEprgsal, prgsal);
  EEPROM_writeAnything (dirEEprgval, bprgval);
  EEPROM_writeAnything (dirEEprgdia, prgdia);
  EEPROM_writeAnything (dirEEprghor, prghor);
  EEPROM_writeAnything (dirEEprgmin, prgmin);
  EEPROM_writeAnything (dirEEprgcon, prgcon);
//  EEPROM_writeAnything (dirEEprgconv, prgconv);
  EEPROM_writeAnything (dirEEprgcomp, prgcomp);
  EEPROM_writeAnything (dirEEbprgtipo, bprgtipo);
  EEPROM_writeAnything (dirEEbactsem, bactsem);
  EEPROM_writeAnything (dirEEPRGsem, bPRGsem);
  }
  
void leerDatosNodos()  {
  EEPROM_readAnything (dirEEremIDnodos, tremID);
  EEPROM_readAnything (dirEEremIPnodos, tremIP);
  }
  
void guardarDatosNodos()  {
  EEPROM_writeAnything (dirEEremIDnodos, tremID);
  EEPROM_writeAnything (dirEEremIPnodos, tremIP);
  }
void leerDatosRem()    {
    EEPROM_readAnything (dirEEremNodo, remNodo);
    EEPROM_readAnything (dirEEremPin, remPin);
    EEPROM_readAnything (dirEEremshowpin, rbshowpin); 
    EEPROM_readAnything (dirEEfactorArem, factorArem);  
    EEPROM_readAnything (dirEEoffsetArem, offsetArem);  
    EEPROM_readAnything (dirEEsumatA, bsumatA);  
    EEPROM_readAnything (dirEEsumatArem, bsumatArem);  
  }
  
void guardarDatosRem()    {
    EEPROM_writeAnything (dirEEremNodo, remNodo);
    EEPROM_writeAnything (dirEEremPin, remPin);
    EEPROM_writeAnything (dirEEremshowpin, rbshowpin);  
    EEPROM_writeAnything (dirEEfactorArem, factorArem);  
    EEPROM_writeAnything (dirEEoffsetArem, offsetArem);  
    EEPROM_writeAnything (dirEEsumatA, bsumatA);  
    EEPROM_writeAnything (dirEEsumatArem, bsumatArem);  
  }
  
void leerDatosrTemp()    {
    EEPROM_readAnything (dirEEremTnodo, remTnodo);
    EEPROM_readAnything (dirEEremTpin, remTpin);
    EEPROM_readAnything (dirEEremshowpin, rbshowpin);  
  }
  
void guardarDatosrTemp()    {
    EEPROM_writeAnything (dirEEremTnodo, remTnodo);
    EEPROM_writeAnything (dirEEremTpin, remTpin);
    EEPROM_writeAnything (dirEEremshowpin, rbshowpin);  
  }
  
void leerEstado() {
  for (int i=0; i<8; i++)
    Mb.C8[i+32]=EEread(dirEEestado+i);    // los 8 primeros bytes de Mb.C8=bestado
  EEPROM_readAnything (dirEEcontadores, contadores);
  }

void miCheckBox (WebServer &server, char *label, boolean colorea, char *name, char *valor, boolean checked, boolean contd)
  {
    if (contd) printP(server,(colorea?th:td));
    server.checkBox(name,valor,label,checked);
    if (contd) printP(server,(colorea?th_f:td_f));
  }

/*********************************************************************************************/
void setupNetHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (snehtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN40];
  int param_number = 0;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    mailACT=0;
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN40);
      if (!repeat) break;
      param_number = atoi(name);        
        if (param_number >=0 && param_number <=5) EEmac[param_number]=strtol(value,NULL,16);       
        if (param_number >=6 && param_number <=9) EEip[param_number-6]=atoi(value);       
        if (param_number >=10 && param_number <=13) EEmask[param_number-10]=atoi(value);
        if (param_number >=14 && param_number <=17) EEgw[param_number-14]=atoi(value); 
        if (param_number >=18 && param_number <=21) EEdns[param_number-18]=atoi(value);
        if (param_number == 22) EEwebPort=atoi(value); 

        if (param_number == 23) mailACT=atoi(value); 
        if (param_number == 24) mailPER=atoi(value); 
        if (param_number == 25) strcpy(mailSVR,value);    
        if (param_number == 26) mailPORT = atoi(value); 
        if (param_number == 27) strcpy(mailUSER,value);        
        if (param_number == 28) strcpy(mailPASS,value);
        if (param_number == 29) strcpy(mailSENDER,value);
        if (param_number == 30) strcpy(mailDEST[0],value);
        if (param_number == 31) strcpy(mailDEST[1],value);
        if (param_number == 32) strcpy(mailDEST[2],value);
        if (param_number == 34) strcpy(dyndnsHost,value);
        if (param_number == 35) strcpy(dyndnsUser,value);
        if (param_number == 36) strcpy(dyndnsPass,value);

      } while (repeat);
    guardarDatosRed();  //  Guardar datos red en EEPROM
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, snehtm);
    server.httpSeeOther(auxchr); return;
    }

  server.httpSuccess();
  writeMenu(server,3,3);
  printP(server,brn,Form_action,snehtm,Form_post,Form_input_send,tablanormal,tr); 
  printTDP(server, MAC, false,true);
  server.printP(td);
  for (byte i=0;i<6;i++) printcampoH(server, i, EEmac[i], 2,false);
  printTDTR(server, 1, true, true);

  // print the current IP
  server.printP(tr);  
  printTDP(server, DIRIP, false,true);
  server.printP(td);    
  for (byte i=0; i<4; i++) printcampoI(server, i+6, EEip[i], 3,false);
  printTDTR(server, 1, true, true);
  
  // print the current SUBNET
  server.printP(tr);  
  printTDP(server, SUBNET,false,true);
  server.printP(td); 
  for (byte i=0;i<4;i++) printcampoI(server, i+10, EEmask[i], 3,false);
  printTDTR(server, 1, true, true);
  
  // print the current GATEWAY
  server.printP(tr);  
  printTDP(server, GATEWAY,false,true);
  server.printP(td); 
  for (byte i=0;i<4;i++) printcampoI(server, i+14, EEgw[i], 3,false);
  printTDTR(server, 1, true, true);
  
  // print the current DNS-SERVER
  server.printP(tr);  
  printTDP(server, DNS_SERVER,false,true);
  server.printP(td); 
  for (byte i=0;i<4;i++) printcampoI(server, i+18, EEdns[i], 3,false);
  printTDTR(server, 1, true, true);

  printparIP(server,WEB_PORT, 22, EEwebPort, 5);
  lineaH(server, 2);
  server.printP(tr);  
  printTDP(server, actenviomail, false, true);
  miCheckBox(server,"",mailACT==1,"23","1",mailACT==1,true);
  server.printP(tr_f); 
  
  printparIP(server, smtpPER, 24, mailPER, 2);
  printparCP(server, smtpSVR, 25, mailSVR, 40,false);
  printparIP(server, smtpPORT, 26, mailPORT, 5);
  printparCP(server, smtpUSER, 27, mailUSER, 40,false);
  printparCP(server, smtpPASS, 28, mailPASS, 40,true);
  printparCP(server, smtpREMI, 29, mailSENDER, 40,false);
  printparCP(server, smtpDEST1, 30, mailDEST[0], 40,false);
  printparCP(server, smtpDEST2, 31, mailDEST[1], 40,false);
  printparCP(server, smtpDEST3, 32, mailDEST[2], 40,false);
  lineaH(server, 2);
 
  printP(server,tr,td,dyndnsserv,td_f,td);
  printPiP(server,Select_name,33,barramayor);  // número de parámetro
  for (byte j=0; j<2; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (dyndnsServ == j) server.printP(selected);
    server.printP(cierre);
    if (j==0) server.printP(noip);
    if (j==1) server.printP(dyndns);
    server.printP(option_f);
    }  
  printP(server,td_f,tr);             // modo de funcionamiento
//  printparCP(server, dyndnsserv, 33, dyndnsServ, 40,false);
  printparCP(server, dyndnshost, 34, dyndnsHost, 40,false);
  printparCP(server, dyndnsuser, 35, dyndnsUser, 40,false);
  printparCP(server, dyndnspass, 36, dyndnsPass, 40,false);

  printP(server, table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupDevHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sdhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    showN=0;
    sdHay=0;
    debugserial=0;
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte mival = atoi(value);
      if (param_number == 0) showN=mival;  
      if (param_number == 1) peract=mival;
      if (param_number == 2) debugserial=mival;
      if (param_number == 3) prectemp=mival;
      if (param_number == 4) sdHay = mival; 
      if (param_number == 5) modo = mival; 
      if (param_number == 6) pmactVar = atol(value); 
      if (param_number == 7) tipoLCD = mival; 
      } while (repeat);
      
    //// guardar
    if (numIA > nANALO) numIA = nANALO;
    if (numIA+numID > nPINES) numID = nPINES - numIA;
    if (numIA+numID+numOD > nPINES) numOD = nPINES - numIA - numID;
    numPines = numIA+numID+numOD;

    EEPROM_writeAnything (dirEEnumIA, numIA);        
    EEPROM_writeAnything (dirEEnumID, numID);
    EEPROM_writeAnything (dirEEnumOD, numOD);
    EEPROM_writeAnything (dirEEshown, showN);
    EEPROM_writeAnything (dirEEperact, peract);
    EEPROM_writeAnything (dirEEprectemp, prectemp);
    EEPROM_writeAnything (dirEEsdHay, sdHay);
    EEPROM_writeAnything (dirEEmodo, modo);
    EEPROM_writeAnything (dirEEpmactVar, pmactVar);
    EEPROM_writeAnything (dirEEtipoLCD, tipoLCD);
    if (modo==2)
      {
      for (int i=0; i<4; i++) {bfectipo[i]=0;  }
        guardarDatosFechas();
      for (int i=0; i<4; i++) {
        bactsem[i]=0;
        bacteve[i]=0;}
        guardarDatosEventos();
      for (int i=0; i<4; i++) {bprgtipo[i]=0;  }
        guardarDatosSemanal();
      }
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sdhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,3,0);
  printP(server,brn,Form_action,sdhtm,Form_post,tablanormal);

  printP(server,tr,td);             // modo de funcionamiento
  printP(server,Modo);
  printP(server,td_f,td);
  printPiP(server,Select_name,5,barramayor);  // número de parámetro
  for (byte j=0; j<6; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (modo == j) server.printP(selected);
    server.printP(cierre);
    if (j==0) server.printP(avanzado);
    if (j==1) server.printP(basico);
    if (j==2) server.printP(local);
    if (j==3) server.printP(domotica);
    if (j==4) server.printP(riego);
    if (j==5) server.printP(climatizacion);
    server.printP(option_f);
    }  
  printP(server,td_f,tr);             // modo de funcionamiento

  printP(server,tr,td,sN,td_f);
  if (showN==1) printP(server,th); else printP(server,td);
  server.checkBox("0","1","",(showN==1));
  if (showN==1) printP(server,th_f); else printP(server,td_f);
  printP(server,tr_f);
  
    
  printparIP(server, periodoact255, 1, peract, 5);
  
  printP(server,tr,td,debugserie,td_f);
  if (debugserial==1) printP(server,th); else printP(server,td);
  server.checkBox("2","1","",(debugserial==1));
  if (debugserial==1) printP(server,th_f); else printP(server,td_f);
  printP(server, tr_f);
  
  printP(server,tr);
  printTDP(server,precisionsondas,false,true);
  printcampoCB(server,3,prectemp,9,12,true,true); 
  printP(server,tr_f);

  server.printP(tr);
  printTDP(server,acttarjetaSD,false,true);   
  if (sdHay==1) printP(server,th); else printP(server,td);
  server.checkBox("4","1","",(sdHay==1));
  if (sdHay==1) printP(server,th_f); else printP(server,td_f);
  printP(server,tr_f);

  printparLP(server, tmactVar, 6, pmactVar, 5);
  
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);
  
}

void ShowLogCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (slghtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  server.httpSuccess();
  writeMenu(server,4,1);
  printP(server, brn, tablanormal);
  printP(server, tr, td,sistema,td_f, versistema, delsistema,tr_f);
  printP(server, tr, td,acciones,td_f, veracciones, delacciones,tr_f);
  printP(server, tr, td,Ent_temp,td_f, verEnt_temp, delEnt_temp,tr_f);
  printP(server, tr, td,Ent_ana, td_f, verEnt_ana, delEnt_ana,tr_f);
  printP(server, tr, td,eventos,td_f, vereventos, deleventos,tr_f);
  printP(server,table_f,body_f,html_f);
}

void ShowFilesSD(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sflhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  server.httpSuccess();
  writeMenu(server,4,2);
  
  printP(server,brn,tablanormal);
  if (!SD.begin(sdSEL, SPI_HALF_SPEED)) SD.initErrorHalt();
  while (file.openNext(SD.vwd(), O_READ)) 
    {
    char buffile[13];
    if (file.getFilename(buffile))
      if (!file.isDir())
        {
        printP(server,tr,td, letraf, td_f,td,showfile01);
        server.print(buffile);
        printP(server,showfile02);
        server.print(buffile);
        printP(server,showfile03,td_f,td);
        server.print(file.fileSize());
        printP(server,b,bytes,td_f,td,deletefile01);
        server.print(buffile);
        printP(server,deletefile02,borrar,deletefile03,td_f,tr_f);
        }
    file.close();
    }
  printP(server,table_f,body_f,html_f);
}

byte actdescr=0;

void (*resetFunc)(void)=0;

void resetHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (rsthtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int auxpar=0;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte mival = atoi(value);
      if (param_number == 0) auxpar=1; 
      if (param_number == 1) resetfab=1; 
      } while (repeat);
      
    //// reset nodo 
    if (auxpar==1)
      if (resetfab==1)
        iniciavalores();          // reset fabrica  
      else
        resetFunc();          // reset simple  
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, rsthtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,4,4);
  printP(server,brn,Form_action,rsthtm,Form_post,tablanormal,trcolor1_i);
  printColspan(server,3,true);
  printP(server,h3_i,resetsystem,h3_f);
  printTDTR(server, 1, true, true);

  printP(server,td,resetsystem,td_f,td);
  server.checkBox("0","1","",(auxpar==1));
  printP(server,td_f,td,td_f,tr);

  resetfab=0;
  printP(server,tr,td,resetfabrica,td_f,td);
  server.checkBox("1","1","",(resetfab==1));
  printP(server,td_f,td,avisoresetfab,td_f,tr_f,table_f,Form_input_exec,Form_f);
  printP(server,body_f,html_f);
}

void setupSondHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sthtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (byte i=0; i<3;i++) bshowTemp[i]=0;
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      int indice1 = ((param_number)/3); 
      int indice2 = ((param_number+1)/3); 
      int resto = param_number % 3;
      byte mival = atoi(value);
      if (param_number >= nDIG*3)        
        if (param_number<200)   // TEMPERATURAS 1-WIRE LOCALES
          {
          if (resto == 1)        // descripción
            for (byte i=0; i<LEN20; i++)  
              EEwrite(dirEEdescTemp+((indice2-nDIG)*LEN20)+i, value[i]);
          if (resto == 2) setbit8(bshowTemp, indice1-nDIG, mival);  // Mostrar si/no   
          }
      } while (repeat);
      
    //// guardar
    EEPROM_writeAnything (dirEEshowTemp, bshowTemp);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sthtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,3,1);
  printP(server,brn,Form_action,sthtm,Form_post,Form_input_send,tablaconf,trcolor1_i);
  printColspan(server,2,true);
  server.printP(sondastemperatura); 
  printP(server,td_f);
  printColspan(server,2,true);
  printP(server,b);
  printTDTR(server, 1, true, true);
  printP(server,trcolor1_i,td_if);
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printP(server,tr_f); 

  char buf[5];
// TEMPERATURAS 1-WIRE LOCALES
  for (byte i=0; i<maxTemp; i++) 
    {
    printP(server,tr);
    if ((getbit8(bshowTemp,i)==1)) printP(server,th); else printP(server,td);
    server.printP(paren_i);
    printPiP(server,letras,i,paren_f1b); 
    printP(server,b,sondatemp,b);
    server.print(i+1); 
    printP(server,td_f);
    
    if ((getbit8(bshowTemp,i)==1)) printP(server,th); else printP(server,td);
    printP(server,Form_input_text_start);      
    server.print(((i+nDIG)*3)+1);                   
    printP(server,Form_input_value);        
//    int dir=i;
    printEE (server, dirEEdescTemp+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end,td_f);
    
    if (getbit8(bshowTemp,i)==1) printP(server,th); else printP(server,td);
    server.checkBox(itoa(((i+nDIG)*3)+2,buf,10),"1","",(getbit8(bshowTemp,i)==1));
    if (getbit8(bshowTemp,i)==1) printP(server,th_f); else printP(server,td_f);

    printP(server,tr_f);
    }
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupEAnagHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sahtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=5;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=0; i<nANALO;i++) setbit8(bshowpin,i+41,0);
    for (int i=0; i<2;i++) setbit8(bsumatA,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      int indice = param_number / mp;
      int resto = param_number % mp;
      byte mival = atoi(value);
      if (resto == 0)        // descripción
        for (byte i=0; i<LEN20; i++)  
          EEwrite(dirEEdescpinA+(indice*LEN20)+i, value[i]);
      if (resto == 1) setbit8(bshowpin, indice+41, mival);   // Mostrar si/no panel   
      if (resto == 2) factorA[indice] = atof(value); 
      if (resto == 3) offsetA[indice] = atof(value); 
      if (resto == 4) setbit8(bsumatA, indice, mival); 
      } while (repeat);
      
    //// guardar
    EEPROM_writeAnything (dirEEbshowpin, bshowpin);
    EEPROM_writeAnything (dirEEfactorA, factorA);
    EEPROM_writeAnything (dirEEoffsetA, offsetA);
    EEPROM_writeAnything (dirEEsumatA, bsumatA);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sahtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,3,7);
  printP(server,brn,Form_action,sahtm,Form_post,Form_input_send,tablaconf);
  
  char buf[5];
    
// E/S LOCALES ANALÓGICAS
  printP(server,trcolor1_i);
  printColspan(server,8,true);
  printP(server,eanalogicas,td_f,tr_f,trcolor1_i,td_if); 
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, factor, false, true);
  printTDP(server, offset, false, true);
  printTDP(server, sumat, false, true);
  server.printP(tr_f);  
  for (byte i=0; i<nANALO; i++)  
    {
    int mpi=mp*i;
    printP(server, tr);
    if (getbit8(bshowpin,i+41)==1) printP(server,th); else printP(server,td);
    server.printP(paren_i);
    printPiP(server,letraa,i,paren_f1b); 
    printP(server,b,entana,b);
    server.print(i+1); 
    printP(server,b);
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f,th); else printP(server,td_f,td);
    printP(server,Form_input_text_start);      
    server.print(mpi);                   
    printP(server,Form_input_value);        
    int dir=i;
    printEE (server, dirEEdescpinA+(dir*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+1,buf,10),"1","",(getbit8(bshowpin,i+41)==1));
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoF(server,mpi+2, factorA[i], 10,5, false);
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoF(server,mpi+3, offsetA[i], 10,5, false);
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f,th); else printP(server,td_f,th);
    
    server.checkBox(itoa(mpi+4,buf,10),"1","",(getbit8(bsumatA,i)==1));
    if (getbit8(bshowpin,i+41)==1) printP(server,th_f); else printP(server,td_f);
    
    printTDTR(server, 1, true, false);
    }
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupEdigHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sedhtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=3;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=0;i<16;i++)
      setbit8(bshowpin,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      int indice1 = ((param_number)/mp); 
      int indice2 = ((param_number+1)/mp); 
      int resto = param_number % mp;
      byte mival = atoi(value);
      if (param_number < nDIG*mp)        
        {  
        if (resto == 0)        // descripción
          for (byte i=0; i<LEN20; i++) 
            EEwrite(dirEEdescpinD+(indice2*LEN20)+i, value[i]);
            
        if (resto == 1) tipoED[indice1]=mival;                 // Mostrar si/no panel 
        if (resto == 2) setbit8(bshowpin, indice1, mival);     // Mostrar si/no panel 
        }
      } while (repeat);
      
    //// guardar
//    EEPROM_writeAnything (dirEEshowTemp, bshowTemp);
    EEPROM_writeAnything (dirEEtipoED, tipoED);
    EEPROM_writeAnything (dirEEbshowpin, bshowpin);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sedhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,3,8);
  printP(server,brn,Form_action,sedhtm,Form_post,Form_input_send,tablaconf);

  char buf[5];
    
// E. LOCALES DIGITALES
  printP(server,trcolor1_i);
  printColspan(server,4,true);
  if (modo==5) printP(server,termostatos); else printP(server,edigitales);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if); 
  printTDP(server, descripcion, false, true);
  printTDP(server, tipo, false, true);
  printTDP(server, ver, false, true);
  printP(server,tr_f);  
  for (byte i=0; i<16; i++)  
    {
    int mpi=mp*i;
    printP(server,tr);
    if (getbit8(bshowpin,i)==1) printP(server,th); else printP(server,td);
    server.printP(paren_i);
    printPiP(server,letrad,tabpin[i],paren_f1b); 
    server.printP((i<17)?entdig:saldig);
    printPiP(server,b,(i<17)?i+1:i-17+1,b);
    if (getbit8(bshowpin,i)==1) printP(server,th_f,th); else printP(server,td_f,td);

    printP(server,Form_input_text_start);      
    server.print((mpi));  // número parámetro
    server.printP(Form_input_value);        
    int dir=i;
    printEE (server, dirEEdescpinD+(dir*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(bshowpin,i)==1) printP(server,th_f,th); else printP(server,td_f,td);

    printPiP(server,Select_name,mpi+1,barramayor);
    for (byte j=0; j<4; j++)  
      {
      printPiP(server,optionvalue,j,barraatras);
      if (tipoED[i]==j) server.printP(selected);
      if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
      if (j==0) server.printP(ONOFF);
      if (j==1) server.printP(dht11);
      if (j==2) server.printP(dht21);
      if (j==3) server.printP(dht22);
      server.printP(option_f);
      }  
    printP(server,Select_f);
    if (getbit8(bshowpin,i)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+2,buf,10),"1","",(getbit8(bshowpin,i)==1));
    if (getbit8(bshowpin,i)==1) printP(server,th_f); else printP(server,td_f);
    printTDTR(server, 1, true, false);
    }
  printP(server,table_f,Form_input_send ,Form_f,body_f,html_f);
}

void setupSdigHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (ssdhtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=17;i<33;i++)
      setbit8(bshowpin,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      int indice1 = ((param_number)/3); 
      int indice2 = ((param_number+1)/3); 
      int resto = param_number % 3;
      byte mival = atoi(value);
      if (param_number < nDIG*3)        {  // E/S LOCALES
        if (resto == 1)        // descripción
          for (byte i=0; i<LEN20; i++)  
            EEwrite(dirEEdescpinD+(indice2*LEN20)+i, value[i]);
        if (resto == 2) setbit8(bshowpin, indice1, mival);  // Mostrar si/no panel   
        }
      } while (repeat);
      
    //// guardar
//    EEPROM_writeAnything (dirEEshowTemp, bshowTemp);
    EEPROM_writeAnything (dirEEbshowpin, bshowpin);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, ssdhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,3,9);
  printP(server,brn,Form_action,ssdhtm,Form_post,Form_input_send,tablaconf);
  
  char buf[5];
  printP(server,trcolor1_i);
  printColspan(server,2,true);
  printP(server,sdigitales,td,b);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if); 
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  server.printP(tr_f);  
// S LOCALES DIGITALES
  for (byte i=17; i<33; i++)  
    {
    if (getbit8(bshowpin,i)==1) printP(server,th); else printP(server,td);
    server.printP(paren_i);
    printPiP(server,letrad,tabpin[i],paren_f1b); 
    server.printP((i<17)?entdig:saldig);
    printPiP(server,b,(i<17)?i+1:i-17+1,b);
    if (getbit8(bshowpin,i)==1) printP(server,th_f,th); else printP(server,td_f,td);

    printP(server,Form_input_text_start);      
    server.print((i*3)+1);  // nÃºmero parÃ¡metro
    server.printP(Form_input_value);        
    int dir=i;
    printEE (server, dirEEdescpinD+(dir*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(bshowpin,i)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa((i*3)+2,buf,10),"1","",(getbit8(bshowpin,i)==1));
    if (getbit8(bshowpin,i)==1) printP(server,th_f); else printP(server,td_f);
    
    printTDTR(server, 1, true, false);
    }
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupAvaHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (savhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  server.httpSuccess();
  writeMenu(server,4,0);
  printP(server,brn,body_f,html_f);
}

void setupSegHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sshtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  char pass1temp[LEN20]; memset(pass1temp,0,sizeof(pass1temp));    // 20 bytes
  char pass2temp[LEN20]; memset(pass2temp,0,sizeof(pass2temp));   // 20 bytes
  int param_number = 0;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    usepass=0;
    opcode=0;
    do   
     {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);        
      if (param_number == 0) usepass=atoi(value);  
      if (param_number == 1) strcpy(pass1temp,value); 
      if (param_number == 2) strcpy(pass2temp,value);;      
      if (param_number == 3) strcpy(user1,value);;      
//      if (param_number == 1) for (byte i=0; i<LEN20; i++) pass1temp[i] = value[i];      
//      if (param_number == 2) for (byte i=0; i<LEN20; i++) pass2temp[i] = value[i];      
//      if (param_number == 3) for (byte i=0; i<LEN20; i++) user1[i] = value[i];      
      } while (repeat);

    if (usepass)    // contraseña activa
      {
       if (strcmp(pass1temp,pass2temp) == 0)    // si coinciden ambas password se almacena
         {
          opcode=1;
          strcpy(pass1,pass1temp);
          EEPROM_writeAnything (dirEEuser,user1);
          EEPROM_writeAnything (dirEEpass,pass1temp);
//          for (byte i=0; i<LEN20; i++) 
//            {
//              opcode=1;
//              pass1[i]=pass1temp[i];
//              EEwrite(dirEEuser+i, user1[i]); 
//              EEwrite(dirEEpass+i, pass1temp[i]); 
//            }
         }
       else
        {
         opcode=2;
         usepass=0;  // no se guarda y se desactiva contraseña
         }
      }
    else// contraseña NO activa
      if (strcmp(pass1temp,pass1)!= 0)    // si no se da la contraseña correcta, no se desactiva
        {
        opcode=3;
        usepass=1;
        }
    EEPROM_writeAnything (dirEEusepass, usepass);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sshtm);
    server.httpSeeOther(auxchr); return;
    }

/////////////////////
  server.httpSuccess();
  writeMenu(server,3,5);
  if (opcode!=99)
    {
    printP(server,brn);
    if (opcode==0) printP(server, operacionrealizada,brn);
    if (opcode==1) printP(server, contrasenaguardada,brn);
    if (opcode==2) printP(server, contrasenanocoincide,brn);
    if (opcode==3) printP(server, contrasenaincorrecta,brn);
    opcode=99;
    }
  printP(server,brn,Form_action,sshtm,Form_post,tablaconf,tr); 
  printTDP(server, activarcontrasena, false,true);

  if (usepass==1) printP(server,th); else printP(server,td);
  server.checkBox("0","1","",(usepass==1));
  if (usepass==1) printP(server,th_f); else printP(server,td_f);
//  printparCP(server, contrasena, 1, pass1, 20,true);
  printparCP(server, usuario, 3, user1, 20,false);
  printparCP(server, contrasena, 1, "", 20,true);
  printparCP(server, confcontrasena, 2, "", 20,true);
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupRelHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srlhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
//
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)
    {
    bool repeat;
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
        param_number = atoi(name);   
        int mival = atoi(value);   
        if (param_number==0) tm.Wday = mival;  
        if (param_number==1) tm.Day = mival;   
        if (param_number==2) tm.Month = mival;   
        if (param_number==3) tm.Year = CalendarYrToTm(mival);  
        if (param_number==4) tm.Hour = mival;   
        if (param_number==5) tm.Minute = mival;  
        if (param_number==6) tm.Second = mival;      
      } while (repeat);
    if (RTC.write(tm)) delay(100);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srlhtm);
    server.httpSeeOther(auxchr); return;
    }
//
  server.httpSuccess();
  writeMenu(server,3,6);
  printP(server,brn,Form_action,srlhtm,Form_post,tablaconf,tr);
  printTDP(server,diasemana,false,true);
  printColspan(server,3,true);
  printPiP(server,Select_name,0,barramayor);
  for (byte j=0; j<7; j++)  {
    printPiP(server,optionvalue,j,barraatras);
    if (j == tm.Wday) server.printP(selected);
    server.printP(cierre); 
    printDayName(server, j); 
    server.printP(option_f);  
    }
  server.printP(Select_f);  
  printTDTR(server, 1, true, false);   

  server.printP(tr); 
  printTDP(server,diamesano,false,true);
  printcampoCB(server, 1, tm.Day, 1, 31,true,true);
  printP(server,td);
  printPiP(server,Select_name,2,barramayor);
  for (byte j=0; j<12; j++)  {
    printPiP(server,optionvalue,j,barraatras);
    if (j == tm.Month) server.printP(selected);
    server.printP(cierre); 
    printMes(server,j);
    server.printP(option_f);  }
  printP(server,Select_f,td_f);   
  printcampoCB(server, 3, tmYearToCalendar(tm.Year), 2010, 2025,true,true);
  printP(server,tr_f,tr);
  printTDP(server, horaminseg,false,true);
  printcampoCB(server, 4, tm.Hour, 0, 23,true,true);
  printcampoCB(server, 5, tm.Minute, 0, 59,true,true);
  printcampoCB(server, 6, tm.Second, 0, 59,true,true);
  printP(server,tr_f,table_f,Form_input_send,Form_f,body_f,html_f);
}

void printLCD(const prog_uchar *str)
  {
  char c;
  while((c = pgm_read_byte(str++))) lcd.write(c);
  }

void iniciavalores()
  {
  lcd.clear();  printLCD(inic);
  for (int i=0;i<16000;i++) 
    {
    EEwrite(i,0);  // borra EEPROM
    if (i%1000==0) 
      dserial.println(i);
    }
  
  printLCD(uno);
  modo=0;
    EEPROM_writeAnything (dirEEmodo, modo);  
  for (byte i=0; i<nPRG; i++) evenTemp[i]=inact;
    EEPROM_writeAnything (dirEEevenTemp, evenTemp);  
  sdHay=0;     // tarjeta SD no activada
    EEPROM_readAnything (dirEEsdHay, sdHay);
  EEmac[0]=170;EEmac[1]=171;EEmac[2]=172;EEmac[3]=173;EEmac[4]=174;EEmac[5]=133;
    EEPROM_writeAnything (dirEEmac, EEmac);
  EEip[0]=192;EEip[1]=168;EEip[2]=1;EEip[3]=178;
    EEPROM_writeAnything (dirEEip, EEip);
  EEmask[0]=255;EEmask[1]=255;EEmask[2]=255;EEmask[3]=0;
    EEPROM_writeAnything (dirEEmask, EEmask);
  EEgw[0]=192;EEgw[1]=168;EEgw[2]=1;EEgw[3]=1;
    EEPROM_writeAnything (dirEEgw, EEgw);
  EEdns[0]=8;EEdns[1]=8;EEdns[2]=8;EEdns[3]=8;
    EEPROM_writeAnything (dirEEdns, EEdns);
  EEwebPort=82; 
    EEPROM_writeAnything (dirEEwebport, EEwebPort);  
  memset(pass1,0,sizeof(pass1));  // pasword
    for (byte i=0; i<LEN20; i++) EEwrite(dirEEpass+i, pass1[i]); 
  usepass = 0;    // activar password NO
    EEPROM_writeAnything (dirEEusepass, usepass);
  memset(bshowTemp,0,sizeof(bshowTemp));
    EEPROM_writeAnything (dirEEshowTemp, bshowTemp);
  memset(bfecval,0,sizeof(bfecval));      // Valor a poner la salida en programación fechas
    EEPROM_writeAnything (dirEEfecval, bfecval);
  numIA = 16;     // número de entradas analÃ³gicas
    EEPROM_writeAnything (dirEEnumIA, numIA);
  numID = 16;     // número de entradas digitales
    EEPROM_writeAnything (dirEEnumID, numID);
  numOD = 16;     // número de salidas digitales
    EEPROM_writeAnything (dirEEnumOD, numOD);
  showN = 1;      // mostrar número de pin
    EEPROM_writeAnything (dirEEshown, showN);
  peract = 15;     // período de actualización automática de la página principal
    EEPROM_writeAnything (dirEEperact, peract);
  for (byte i=0; i<32; i++) Mb.C8[i]=0;          // estado de E/S locales y remotas
    EEPROM_writeAnything (dirEEestado, Mb.C8);
  printLCD(dos);
  
  for (int i=0; i<16; i++) {    // descripciones de E digitales
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
//    strcpy(auxstr,"ENT DIG ");
    strcpyP(auxstr,entdig);
    strcat(auxstr,itoa(i+1,buff,10));
    EEPROM_writeAnything(dirEEdescpinD+(i*LEN20),auxstr);
    }
  for (int i=17; i<34; i++) {    // descripciones de S digitales
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
    strcpyP(auxstr,saldig);
//    strcpy(auxstr,"SAL DIG ");
    strcat(auxstr,itoa(i-16,buff,10));
    EEPROM_writeAnything(dirEEdescpinD+(i*LEN20),auxstr);
    }
  printLCD(tres);
  
  for (int i=0; i<16; i++) {    // descripciones de E/S analógicas
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
    strcpy(auxstr,"ENT ANA ");
    strcat(auxstr,itoa(i+1,buff,10));
    EEPROM_writeAnything(dirEEdescpinA+(i*LEN20),auxstr);
    }
  memset(fecdia,0,sizeof(fecdia));       // Día en programación fechas
    EEPROM_writeAnything (dirEEfecdia, fecdia);  // Día en programación fechas
  memset(evenvalD,0,sizeof(evenvalD));       // Día en programación fechas
    EEPROM_writeAnything (dirEEevenvalD, evenvalD);
  memset(bprgtipo,0,sizeof(bprgtipo));
    guardarDatosSemanal();
  memset(beveacttipo,0,sizeof(beveacttipo));
  memset(bevesaltipo,0,sizeof(bevesaltipo));
  printLCD(cuatro);
  guardarDatosEventos();
  memset(fecmin,0,sizeof(fecmin));
    EEPROM_writeAnything (dirEEfecmin, fecmin);     
  for (int i=0; i<nPRG; i++) evenact[i]=inact;
    EEPROM_writeAnything (dirEEevenact, evenact);
  memset(evensal,0,sizeof(evensal));
    EEPROM_writeAnything (dirEEevensal, evensal);
  for (byte i=0; i<nPRG; i++) fecsal[i]=inact;       // Salida a activar en programaciÃ³n fechas
    EEPROM_writeAnything (dirEEfecsal, fecsal);
  memset(bfectipo,0,sizeof(bfectipo));
    guardarDatosFechas();
  for (byte i=0; i<lTAB; i++) bevenniv[i]=0;    // nivel al que se activa la seÃ±al sobre la que se actÃºa en eventos
    EEPROM_writeAnything (dirEEevenniv, bevenniv);  
  printLCD(cinco);
  for (int i=0; i<20; i++) {    // descripciones de sondas temperatura
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
    strcpyP(auxstr,sonda);
//    strcpy(auxstr,"Sonda ");
    strcat(auxstr,itoa(i+1,buff,10));
    EEPROM_writeAnything(dirEEdescTemp+(i*LEN20),auxstr);
    }
  memset(bshowpin,0,sizeof(bshowpin));
    EEPROM_writeAnything (dirEEbshowpin, bshowpin);
  memset(tipoED,0,sizeof(tipoED));
    EEPROM_writeAnything (dirEEtipoED, tipoED);

  memset(bactsem,0,sizeof(bactsem));
  memset(bacteve,0,sizeof(bacteve));
    guardarDatosEventos();
  memset(bactfec,0,sizeof(bactfec));
    guardarDatosFechas();
  for (byte i=0; i<nPRG; i++) prgsal[i] = inact;    // todos desactivadas
    EEPROM_writeAnything (dirEEprgsal, prgsal);
  for (byte i=0; i<nPRG; i++) fecano[i]=13;      // Año en programación fechas
    EEPROM_writeAnything (dirEEfecano, fecano);
  memset(bprgval,0,sizeof(bprgval));
    EEPROM_writeAnything (dirEEprgval, bprgval);
  memset(prgdia,0,sizeof(prgdia));
    EEPROM_writeAnything (dirEEprgdia, prgdia);
  memset(prghor,0,sizeof(prghor));
    EEPROM_writeAnything (dirEEprghor, prghor);
  memset(prgmin,0,sizeof(prgmin));
    EEPROM_writeAnything (dirEEprgmin, prgmin);
  for (byte i=0; i<nPRG; i++) prgcon[i] = inact;    // todos desactivadas  
    EEPROM_writeAnything (dirEEprgcon, prgcon);
  memset(prgcomp,0,sizeof(prgcomp));
    EEPROM_writeAnything (dirEEprgcomp, prgcomp);
  memset(fechor,0,sizeof(fechor));
    EEPROM_writeAnything (dirEEfechor, fechor);
   mailACT=0;                                    // mail desactivado
    EEPROM_writeAnything (dirEEmailACT, mailACT);  
  memset(mailSVR,0,sizeof(mailSVR));
    EEPROM_writeAnything (dirEEmailSVR, mailSVR);  
  mailPER=60;                                    // período entre mails
    EEPROM_writeAnything (dirEEmailPER, mailPER);  
  mailPORT=25;                                   // puerto servidor smtp
    EEPROM_writeAnything (dirEEmailPORT, mailPORT);  
  memset(mailUSER,0,sizeof(mailUSER));
    EEPROM_writeAnything (dirEEmailUSER, mailUSER);  
  memset(mailPASS,0,sizeof(mailPASS));
    EEPROM_writeAnything (dirEEmailPASS, mailPASS);  
  memset(mailSENDER,0,sizeof(mailSENDER));
    EEPROM_writeAnything (dirEEmailSENDER, mailSENDER); 
  memset(mailDEST,0,sizeof(mailDEST)); 
    EEPROM_writeAnything (dirEEmailDEST, mailDEST);  
  memset(evencomp,0,sizeof(evencomp));
    EEPROM_writeAnything (dirEEevencomp, evencomp);
  memset(contadores,0,sizeof(contadores));
    EEPROM_writeAnything (dirEEcontadores, contadores);
  memset(fecmes,0,sizeof(fecmes));
    EEPROM_writeAnything (dirEEfecmes, fecmes);
  printLCD(seis);
  memset(factorA,0,sizeof(factorA));
    EEPROM_writeAnything (dirEEfactorA, factorA);    
    
  for (int i=0; i<5; i++) {    // descripciones de escenas
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
//    strcpy(auxstr,"Escena ");
    strcpyP(auxstr,escena);
    strcat(auxstr,itoa(i+1,buff,10));
    EEPROM_writeAnything(dirEEdescEsc+(i*LEN20),auxstr);
    }
  for (int i=0; i<5; i++) {    // descripciones de programas
    char buff[6]; memset(buff,0,sizeof(buff));
    char auxstr[20]; memset(auxstr,0,sizeof(auxstr));
    strcpyP(auxstr,programa);
    strcpy(auxstr,"Programa ");
    strcat(auxstr,itoa(i+1,buff,10));
    EEPROM_writeAnything(dirEEdescPrg+(i*LEN20),auxstr);
    }
    
//xxxxxxxxxxxxxxxxxxxx    
  memset(evenvalA,0,sizeof(evenvalA));
    EEPROM_writeAnything (dirEEevenvalA, evenvalA);
  memset(evenhis,0,sizeof(evenhis));
    EEPROM_writeAnything (dirEEevenhis, evenhis);
  }
  
void iniciavaloresIP()
  {
  lcd.clear();  
  printLCD(iniciando);
  printLCD(blancoIP);
  EEmac[0]=170;EEmac[1]=171;EEmac[2]=172;EEmac[3]=173;EEmac[4]=174;EEmac[5]=175;
  EEip[0]=192;EEip[1]=168;EEip[2]=1;EEip[3]=178;
  EEmask[0]=255;EEmask[1]=255;EEmask[2]=255;EEmask[3]=0;
  EEgw[0]=192;EEgw[1]=168;EEgw[2]=1;EEgw[3]=1;
  EEdns[0]=8;EEdns[1]=8;EEdns[2]=8;EEdns[3]=8;
  EEwebPort=82; 

  printLCD(uno);
  guardarDatosRed();
  
  }
  
void resetcontador(WebServer &server, byte n, boolean panel)
{
  contadores[n]=0;
  EEPROM_writeAnything(dirEEcontadores+(4*n), contadores[n]);
  server.httpSeeOther(PREFIX "/"); 
}

void systemHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{ 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  URLPARAM_RESULT rc;
  char name[LEN5];
  char value[LEN20];
  boolean params_present = false;
  
////////////////////////////////////////  

  if (type == WebServer::HEAD) return;
  // check for parameters
  if (strlen(url_tail)) 
    {
    while (strlen(url_tail)) {
      rc = server.nextURLparam(&url_tail, name, LEN5, value, LEN20);
      if (rc != URLPARAM_EOS) {
        params_present=true;
        if (strcmp(name, "act") == 0)    // actuación a realizar
          {
          if (LOG==1) logIPP (act, 0, 'I', false); 
          if (strcmp (value, "r0") == 0) { resetcontador(server,0,false); return; }
          if (strcmp (value, "r1") == 0) { resetcontador(server,1,false); return; }
          if (strcmp (value, "r2") == 0) { resetcontador(server,2,false); return; }
          if (strcmp (value, "r3") == 0) { resetcontador(server,3,false); return; }
          if (strcmp (value, "r4") == 0) { resetcontador(server,4,false); return; }
          if (strcmp (value, "r5") == 0) { resetcontador(server,5,false); return; }
          if (strcmp (value, "r6") == 0) { resetcontador(server,6,false); return; }
          if (strcmp (value, "r7") == 0) { resetcontador(server,7,false); return; }
          if (strcmp (value, "rp0") == 0) { resetcontador(server,0,false); return; }
          if (strcmp (value, "rp1") == 0) { resetcontador(server,1,false); return; }
          if (strcmp (value, "rp2") == 0) { resetcontador(server,2,false); return; }
          if (strcmp (value, "rp3") == 0) { resetcontador(server,3,false); return; }
          if (strcmp (value, "rp4") == 0) { resetcontador(server,4,false); return; }
          if (strcmp (value, "rp5") == 0) { resetcontador(server,5,false); return; }
          if (strcmp (value, "rp6") == 0) { resetcontador(server,6,false); return; }
          if (strcmp (value, "rp7") == 0) { resetcontador(server,7,false); return; }
          if (strcmp (value, "rsteex") == 0)  // reinicia EEPROM
            { 
            if (LOG==1) logIPP (rsteex, 0, 'I', false); 
            server.httpSuccess();
            writeMenu(server,4,4);
            iniciavalores(); 
            printP(server,br,operacionrealizada);
            printP(server,body_f,html_f);  
            }
          if (strcmp (value, "shee") == 0)  // show EEPROM
            {
            if (LOG==1) logIPP (shee, 0, 'I', false); 
            server.httpSuccess();
            writeMenu(server,4,3);
            printP(server,brn,tablamenu,trcolor1_i);
            printColspan(server,17,true);
            printP(server,h3_i,volcadoeeprom,h3_f);
            printTDTR(server, 1, true, true); 
            printP(server,table_f,tablaconf,tralignright,th,Bytes,th_f); 
            for (byte i=0; i<=15; i++) printPiP(server,th,i,th_f);     
            server.printP(tr_f);
            for (int i=0; i<4096; i++)   {
              server.printP(tralignright);
              printPiP(server,td,i*16,guion); 
              unsigned int auxi=i*16+15;
              server.print (auxi); 
              printP(server,td_f); 
              for (int j=0; j<16; j++)     // DECIMAL
                printPiP(server,td,EEread((i*16)+j),td_f);  
              server.printP(tr_f);
              }
            printP(server,table_f,body_f,html_f);  
            }
          if (strcmp (value, "backup") == 0)  // guarda EEPROM en fichero SD
            { 
            if (LOG==1) logIPP (backup, 0, 'I', false); 
            server.httpSuccess();
            writeMenu(server,4,5);
            printP(server,espere,brn);
            if (backupEE()==0)
              {
              printP(server,br,ficheroguardado);
              server.print(namefilebackup);
              }
            else
              printP(server,br,erroralguardar);
            printP(server,body_f,html_f);  
            }
          if (strcmp (value, "restore") == 0)  // recupera EEPROM de fichero SD
            { 
            if (LOG==1) logIPP (restore, 0, 'I', false); 
            server.httpSuccess();
            writeMenu(server,4,6);
            printP(server,espere,brn);
            if (restoreEE()==0)
              {
              printP(server,br,ficherorecuperado);
              server.print(namefilebackup);
              }
            else
              printP(server,br,erroralguardar);
            printP(server,body_f,html_f);  
            }
         }
        }
      }
    }
}

void errorHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
  { 
  server.httpFail();
  if (type == WebServer::HEAD) return;
  printP(server,Http400,body_f,html_f);  
  }

void DeleFileCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (deletefile, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  server.httpSuccess();
  writeMenu(server,4,2);
  if (SD.exists(url_tail)) 
    {
    if (file.isDir())
      SD.rmdir(url_tail);
    else
      SD.remove(url_tail); 
    printP(server,fichborrado,body_f,html_f);  
    }
  else
    {
    printP(server,fichnoexiste,body_f,html_f);  
    }
  }  

void setupSemHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (ssehtml, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  URLPARAM_RESULT rc;
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 18;    // número de parámetros por fila

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    int indice,resto,mival;
    memset(bPRGsem,0,sizeof(bPRGsem));
    memset(prgdia,0,sizeof(prgdia));
    for (int i=0; i<nPRG; i++) setbit8(bactsem,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);     
      indice = param_number / mp;
      resto = param_number % mp;
      mival = atoi(value);
      if (resto==0) {setbit8(bactsem,indice,mival);  }    // Activo: sí/no
      if (resto==1) {setbit8(bprgtipo,indice,mival);  }    // Tipo: local/remoto
      if (resto==2) {prgsal[indice] = mival;  }            // Número de salida
      if (resto==3) {setbit8(bprgval, indice, mival);  }   // valor de la salida
      if (resto==4) {setbit(&prgdia[indice], 0, mival);  }  // domingo
      if (resto==5) {setbit(&prgdia[indice], 1, mival);  }  // lunes
      if (resto==6) {setbit(&prgdia[indice], 2, mival);  }
      if (resto==7) {setbit(&prgdia[indice], 3, mival);  }
      if (resto==8) {setbit(&prgdia[indice], 4, mival);  }
      if (resto==9) {setbit(&prgdia[indice], 5, mival);  }  // viernes
      if (resto==10) {setbit(&prgdia[indice], 6, mival);  }  // sábado
      if (resto==11) {prghor[indice] = mival;  }           // Hora
      if (resto==12) {prgmin[indice] = mival;  }          // Minuto
      if (resto>=13) {setbit8(bPRGsem[resto-13],indice,mival);  }   // asociacion PRG
      } 
    while (repeat);

    guardarDatosSemanal();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, ssehtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,2,1);
  printP(server,brn,Form_action,ssehtm,Form_post,Form_input_send,tablaconf,trcolor1_i);
  printP(server,td,tdcolspan5,asociaraprograma,tdcolspan3);
  printP(server,salida,td_f,tdcolspan7,dias,tdcolspan2,hora,tr_f,trcolor1_i);
  printTDP(server, programa, false, true);
  printTDP(server, uno, false, true);
  printTDP(server, dos, false, true);
  printTDP(server, tres, false, true);
  printTDP(server, cuatro, false, true);
  printTDP(server, cinco, false, true);
  printTDP(server, lor, false, true);
  printTDP(server, nombre, false, true);
  printTDP(server, valor, false, true);
  printTDP(server, D, false, true);
  printTDP(server, L, false, true);
  printTDP(server, M, false, true);
  printTDP(server, X, false, true);
  printTDP(server, J, false, true);
  printTDP(server, V, false, true);
  printTDP(server, S, false, true);
  printTDP(server, hora, false, true);
  printTDP(server, minuto, false, true);
  server.printP(tr_f);
  char buf[5];
  for (int i=0; i<nPRG; i++) 
    {
    int mpi = mp*i;
    printP(server,tr); 
    boolean colorea=((getbit8(bPRGsem[0],i)==1) || 
                     (getbit8(bPRGsem[1],i)==1) || (getbit8(bPRGsem[2],i)==1) || 
                     (getbit8(bPRGsem[3],i)==1) || (getbit8(bPRGsem[4],i)==1));
    
    printP(server,(colorea?th:td));
    server.print(i+1);
    printP(server,(colorea?th_f:td_f));
    
    for (byte j=0;j<5;j++)
      miCheckBox(server,"",getbit8(bPRGsem[j],i),itoa(mpi+13+j,buf,10),"1",(getbit8(bPRGsem[j],i)==1),true);
      
    if (colorea)
      {
      printP(server,(colorea?th:td));
      if (modo==2)
        printP(server,Local);
      else
        printcampoSiNo(server, mpi+1, getbit8(bprgtipo,i),Local,Local, true, false);    // valor local/remota
      printP(server,(colorea?th_f:td_f),(colorea?th:td));
      printPiP(server,Select_name,mpi+2,barramayor);
      if (getbit8(bprgtipo,i)==0)    // locales
        {
        for (byte j=17; j<33; j++)   {
          printPiP(server,optionvalue,j,barraatras);
          if (prgsal[i] == j) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          server.printP(option_f);  }  
        }
      printP(server,Select_f,td_f,(colorea?th_f:td_f),(colorea?th:td));
      printcampoSiNo(server, mpi+3, getbit8(bprgval,i), on,off, true, false);    // valor on/off
      printP(server,(colorea?th_f:td_f));
      for (byte j=0; j<7;j++) {    // días de la semana
        if (getbit(prgdia[i],j)==1) printP(server,th); else printP(server,td);
        printPiP(server,inputcheckbox,mpi+4+j,value1);
        if (getbit(prgdia[i],j) == 1) server.printP(checked); 
        printP(server,cierre,br);
        if (getbit(prgdia[i],j)==1) printP(server,th_f); else printP(server,td_f);
       }
      if (colorea) printP(server,th); else printP(server,td);
      printcampoCB(server, mpi+11, prghor[i], 0, 23,false,false);  // hora
      if (colorea) printP(server,th_f,th); else printP(server,td_f,td);
      
      printcampoCB(server, mpi+12, prgmin[i], 0, 59,false,false);  // minuto
      if (colorea) printP(server,th_f); else printP(server,td_f);
      }
    server.printP(tr_f);
    }
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);  
}

void setupFecHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (sfhtml, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp = 9;    // número de parámetros por fila

  if (type == WebServer::HEAD) return;
  
  if (type == WebServer::POST)   {
    bool repeat;
    int indice,resto,mival;
    memset(bactfec,0,sizeof(bactfec));
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
        param_number = atoi(name);     
        indice = param_number / mp;
        resto = param_number % mp;
        mival = atoi(value);
        if (resto==0) setbit8(bactfec,indice,mival);    // programa activo si/no
        if (resto==1) setbit8(bfectipo,indice,mival);   // tipo de salida local/remota
        if (resto==2) fecsal[indice] = mival;           // Número de salida
        if (resto==3) setbit8(bfecval, indice, mival);  // valor de la salida
        if (resto==4) fecdia[indice] = mival;           // día
        if (resto==5) fecmes[indice] = mival;           // mes
        if (resto==6) fecano[indice] = mival-2000;      // año
        if (resto==7) fechor[indice] = mival;           // Hora
        if (resto==8) fecmin[indice] = mival;           // minuto
      } while (repeat);

    guardarDatosFechas();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, sfhtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,2,3);
  printP(server,brn,Form_action,sfhtm,Form_post,Form_input_send,tablaconf,trcolor1_i,tdcolspan2);
  printP(server,tdcolspan3,salida,td_f,tdcolspan5,fechahora,td_f,tr_f,trcolor1_i);
  printTDP(server, programa, false, true);
  printTDP(server, Act, false, true);
  printTDP(server, lor, false, true);
  printTDP(server, nombre, false, true);
  printTDP(server, valor, false, true);
  printTDP(server, dia, false, true);
  printTDP(server, mes, false, true);
  printTDP(server, ano, false, true);
  printTDP(server, hora, false, true);
  printTDP(server, minuto, false, true);
  printP(server,tr_f);
  char buf[3];
  for (byte i=0;i<nPRG;i++)      
    {
    int mpi = mp * i;
    boolean colorea=(getbit8(bactfec,i)==1);
    printP(server,tr);
    
    if (colorea) printP(server,th); else printP(server,td);
    server.print(i+1);
    if (colorea) printP(server,th_f); else printP(server,td_f);

    miCheckBox(server,"",colorea, itoa(mpi,buf,10), "1", getbit8(bactfec,i)==1,true);
    if (colorea)
      {  
      printP(server,th);
      if (modo==2)
        printP(server,Local);
      else
        printcampoSiNo(server, mpi+1, getbit8(bfectipo,i), Local,Local, true, false);    // valor local/remota
      printP(server,th_f,th); 
      printPiP(server,Select_name,mpi+2,barramayor);
      if (getbit8(bfectipo,i)==0)    // locales
        {
        for (byte j=17; j<33; j++)   {
          printPiP(server,optionvalue,j,barraatras);
          if (fecsal[i] == j) server.printP(selected);
          if (showN == 1) printPiP(server, mayorparen, j, parenguion); else server.printP(cierre);
          printEE (server, dirEEdescpinD+(j*LEN20), LEN20);
          server.printP(option_f);  }  
        }
      printP(server,Select_f,th_f,th); 
      printcampoSiNo(server, mpi+3, getbit8(bfecval,i), on,off, true, false);
      printP(server,th_f,th); 
      printcampoCB(server, mpi+4, fecdia[i], 1, 31,false,false);
      printP(server,th_f,th); 
      printcampoCB(server, mpi+5, fecmes[i], 1, 12,false,false);
      printP(server,th_f,th); 
      printcampoCB(server, mpi+6, fecano[i]+2000, 2013, 2050,false,false);
      printP(server,th_f,th);
      printcampoCB(server, mpi+7, fechor[i], 0, 23,false,false);
      printP(server,th_f,th); 
      printcampoCB(server, mpi+8, fecmin[i], 0, 59,false,false);
      printP(server,th_f); 
      }
    server.printP(tr_f);
    }
  printP(server,table_f,Form_input_send,Form_f,body_f,html_f);  
}

void jsonCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess();
  formaJson();
  server.print(wifi.inputstr);
}

void youridCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess();
  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i,tr);
  char auxip[16]="";
  strcatP(auxip,prefixIP);
  strcat(auxip,url_tail);
  int auxerr=wifi.SendRequest(mysocket,auxip,gatewayPort,cmdgetyourid,3000,5000);
  if (auxerr>=0)
    {
    server.print(wifi._parametros); 
    }
  else
    {
    printP(server,error);
    server.println(auxerr);
    }
  printP(server,tr_f,body_f,html_f);  
}

void gwipCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess();
  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i,tr);

  char *auxchr=strtok(url_tail,"$");
  char auxip[16]="";
  char buff[4];

  strcatP(auxip,prefixIP);
  strcat(auxip,itoa(ipnodo(url_tail),buff,10));
  char auxchr2[30]="";
  strcatP(auxchr2,cmdsetgwip);
  strcat(auxchr2,"=");
  strcat(auxchr2,strtok(NULL,"$&"));
  int auxerr=wifi.SendMsg(mysocket,auxip,gatewayPort,auxchr2,3000);
  if (auxerr>=0)
    {
    printP(server,hecho);
    }
  else
    {
    printP(server,error);
    server.println(auxerr);
    }
  printP(server,tr_f,body_f,html_f);  
}

void idsetCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess();
  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i,tr);
  char buff[4];
  char auxip[16]="";
  char *auxchr=strtok(url_tail,"$&");
  dserial.print("auxchr:");dserial.println(auxchr);
  
  strcatP(auxip,prefixIP);
  strcat(auxip,auxchr);
  char auxchr2[30]="";
  strcatP(auxchr2,cmdsetidset);
  strcat(auxchr2,strtok(NULL,"$"));
  
  dserial.print("auxip:");dserial.println(auxip);
  dserial.print("auxchr2:");dserial.println(auxchr2);
  int auxerr=wifi.SendMsg(mysocket,auxip,gatewayPort,auxchr2,3000);

  if (auxerr>=0)
    {
    printP(server,hecho,br,comprobando,br); 
    int auxerr=wifi.SendRequest(mysocket,auxip,gatewayPort,cmdgetyourid,3000,5000);
    if (auxerr>=0)
      server.print(wifi._parametros); 
    else
      {
      printP(server,error);
      server.println(auxerr);
      }
    }
  else
    {
    printP(server,error);
    server.println(auxerr);
    }
  printP(server,tr_f,body_f,html_f);  
}

int firstip=2;
int lastip=254;

void explCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {server.httpFail(); return;  }
  server.httpSuccess();
  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i);
  for (int i=99; i<=120; i++)
    {
    printP(server,br);
    char auxchr[16]="192.168.4.";
    char buff[4]="";
    strcat(auxchr,itoa(i,buff,10));
    printP(server,comprobando); server.print(auxchr); printP(server,espera); 
    int auxerr=wifi.SendRequest(mysocket,auxchr,gatewayPort,cmdgetyourid,3000,5000);
    printP(server,(auxerr>=0)?encontrado:noencontrado);
    if (auxerr>=0)
      server.print(wifi._parametros); 
    delay(10000);
    }
  printP(server,body_f,html_f);  
}

void jsonrCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess();
  printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i,tr);
  char buff[10]="";
  char auxip[20]="";
  int auxfin=ipnodo(url_tail);
  strcatP(auxip,prefixIP);
  strcat(auxip,itoa(auxfin,buff,10));
  dserial.println(auxip);
  printlnS(cmdgetjson);
  int auxerr=wifi.SendRequest(mysocket,auxip,gatewayPort,cmdgetjson,3000,5000);
  dserial.println(auxerr);
  if (auxerr>=0)
    server.print(wifi.inputstr); 
  else
    {
    printP(server,error);
    server.println(auxerr);
    }
  printP(server,tr_f,body_f,html_f);  
}

void jsonnamesCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (tjson, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  if (type == WebServer::POST)  {
    server.httpFail();
    return;  }
  server.httpSuccess("application/json");
  if (type == WebServer::HEAD) return;

  server.printP(llave_i);
  for (byte i = 0; i < 16; i++)    // entradas digitales locales
    {
      byte val = digitalRead(tabpin[i]);
      printPiP(server,barradi,i,letran);
      printP(server,dospuntos,barraatras);
      printEE (server, dirEEdescpinD+(i*LEN20), LEN20);
      printP(server,barraatras,coma);          
    }
  for (byte i = 17; i < nDIG-1; i++)    // salidas digitales locales
    {
      printPiP(server,barrads,i-17,letran);
      printP(server,dospuntos,barraatras);
      printEE (server, dirEEdescpinD+(i*LEN20), LEN20);
      printP(server,barraatras,coma);          
    }
  for (byte i = 0; i < 16 ; i++)    // entradas analógicas locales
    {
      printPiP(server,barraa,i,dospuntos);
      server.print(float(Mb.R[i+baseAna])*factorA[i]);
      server.printP(coma);}
  for (byte i = 0; i < 20 ; i++)    // temperaturas locales
    {
      printPiP(server,barras,i,letran);
      printP(server,dospuntos,barraatras);
      printEE (server, dirEEdescTemp+(i*LEN20), LEN20);
      if (i<19)
        printP(server,barraatras,coma);  
      }        
  printP(server,llave_f);
}

int minutosentre(tmElements_t tm, tmElements_t tmI)
  {long actual;
   long inicio; 
   actual = (tmYearToCalendar(tm.Year)*525600) + (tm.Month*44640) + (tm.Hour*60) + tm.Minute;
   inicio = (tmYearToCalendar(tmI.Year)*525600) + (tmI.Month*44640) + (tmI.Hour*60) + tmI.Minute;
   return actual - inicio;
  }
 
byte inverso(byte valor)
  {if (valor==0) return(1); else return(0);}
  
void horalcd(boolean exacto)
  {
  lcd.setCursor(0,1);
  if (RTC.read(tm)) 
    { 
    if (tm.Day < 10) printLCD(cero); lcd.print (tm.Day); printLCD(barra);
    if (tm.Month < 9) lcd.print(0); lcd.print (tm.Month+1); printLCD(barra);
    lcd.print (tmYearToCalendar(tm.Year));  printLCD(b);
    if (tm.Hour < 10) lcd.print(0); lcd.print (tm.Hour); 
    printLCD(dospuntos);
    if (tm.Minute < 10) printLCD(cero); lcd.print (tm.Minute); 
    if ((!exacto) || (tm.Second == 0))
      {
      printLCD(dospuntos);
      if (tm.Second < 10) lcd.print(0); 
      lcd.print (tm.Second);
      }
    }
  else    
    {
    if (RTC.chipPresent())  
      printLCD(rtcparado);    
    else 
      printLCD(rtcnodisponible);
    }
  }

///////////////////////////////////////////////////////////////////////////////////////////////////

//int mifreeRam()
//  {int auxmem;
//  auxmem=freeRam();
//  if (auxmem < minRam) minRam = auxmem;
//  return auxmem;
//    
//  }
//
//int freeRam () {
//  extern int __heap_start, *__brkval; 
//  int v; 
//  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
//}

void leevaloresOW()
  { 
  sensors1.requestTemperatures();
  for (int i=0; i<nTemp1; i++)
    Mb.R[i]=sensors1.getTempC(addr1Wire[i])*100;
  }

void webclientHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
}

void testEEPROM()
  {
    boolean ok=true;
    printS(ttestEEPROM);
    for (int i=0; i<16;i++)  // escribe todo ceros
      {
        writeEEPROM(EEPROM2dir,i,0);
        ok=(ok && (readEEPROM(EEPROM2dir,i)==0));
      }
    for (int i=0; i<16;i++)  // escribe valores
       {
        writeEEPROM(EEPROM2dir,i,i);
        ok=(ok && (readEEPROM(EEPROM2dir,i)==i));
      }
    printlnS(ok?blancoOK:blancoERROR);
  }

void leervaloresAna()
  {
  for (int i=0; i<16;i++) Mb.R[i+baseAna]=analogRead(i+54);
  }

void actualizasegunModbus()    // actualiza salidas seg´`un estado Modbus
  {
  for (int i=17; i<41; i++) // Salidas digitales locales
    pinVAL(tabpin[i], getbit8(Mb.C8, tabpin[i]));
  }
  
void actualizaDDNS()
  {
//  delay(1000);
//  P(connecting)="Connecting...";
//  P(conectado)="Connected";
//  P(confailed)="Connection failed";
//  printS(connecting);
//  
//  char server[]="www.google.es";
////  char server[]="http://dynupdate.no-ip.com";
//  if (EthClient.connect(server, 80)) 
//    {
//    printlnS(conectado);
////    P(host)="Host: www.google.com";
////    EthClient.println("GET http://ramonlh:pac2ram@dynupdate.no-ip.com/nic/update?hostname=elconucom.zapto.org HTTP/1.1");
//    EthClient.println("GET dynupdate.no-ip.com/nic/update?hostname=elconucom.zapto.org HTTP/1.1");
//    EthClient.println("Host: dynupdate.no-ip.com");
////    EthClient.println("Connection: close");
////    EthClient.println();
//    }
//  else 
//    {
//    printlnS(confailed);
//    }
//    
//    delay(5000);
//    while (EthClient.available()) {
//      char c = EthClient.read();
//      dserial.print(c);
//    }
    
    
    
    
//  
//      char server[]="https://dynupdate.no-ip.com";
//    char dyndnsHost[]="elconucom.no-ip.org";
//    if (dyndnsServ==0)
//      {
//      if(EthClient.connect("https://dynupdate.no-ip.com", 80))
//        {
//        dserial.println("Conectado a NO-IP");
////        EthClient.print("GET /nic/update?hostname=");
////        EthClient.print(dyndnsHost);
////          EthClient.println(" HTTP/1.0");
//        EthClient.println("GET /nic/update?hostname=elconucom.no-ip.org HTTP/1.0");
//        EthClient.println("Host: dynupdate.no-ip.com");
//        EthClient.println("Authorization: user:pass");
//        EthClient.println("User-Agent: Conuco WebServer ramonlh@telefonica.net");
//        EthClient.println();
//
//        //Wait for response
////        delay(5000);
////        dserial.print("Characters available: ");
////        dserial.println(EthClient.available());
//      
//        while(!EthClient.available())
//          {    
////          dserial.println("NO available");
//          while(EthClient.available())
//            {
//            v.println("available");
//            char read_char = EthClient.read(); 
//            dserial.write(read_char);
//            }
//          }
//        EthClient.stop();     
//        }
//      }
  }
  
byte backupEE()
{
//  SdFile file(namelogSys, O_WRITE | O_CREAT);
  if (SD.exists(namefilebackup)) 
    SD.remove(namefilebackup); 
  ofstream sdlog(namefilebackup, ios::out | ios::app);  
  if (sdlog)
    {
    for (long i=0; i<65536; i++)
      {
       byte leido=EEread(i);
       sdlog << leido;
       }
     sdlog.close();
     return 0;
    }
  else
    return 1;
}

byte restoreEE()
{
  ifstream sdlog(namefilebackup);
  ArduinoOutStream cout(Serial);
  int c;
  long cont=0;
  if (sdlog.is_open()) 
    {
    while (((c = sdlog.get()) >= 0) && (cont<65536))
      {
      EEwrite(cont,c);
      cont++;
      }
    sdlog.close();
    return 0;
    }
  else
    return 1;
}

/*********************************************************************************************/
int ultcomando=0;

void setupremHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  byte nAPlist=0;
  int auxerr=0;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   
    {
    bool repeat;
    showN=0;
    sdHay=0;
    debugserial=0;
    char buff[10];
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte mival = atoi(value);
      if (param_number == 0) wifi._CWMode=mival;
      if (param_number == 1) wifi._CIPMux=1;
      if (param_number == 4) strcpy(wifi._APssid, value);
      if (param_number == 5) strcpy(wifi._APpass, value);
      if (param_number == 6) strcpy(wifi._APch, value);
      if (param_number == 7) wifi._APenc[0] = value[0];
      
      if (param_number == 10) 
        {
        ultcomando=mival;
        if (mival==0)     // GETConfig
          {
          wifi.getVer();  
          wifi.getCWMode();
          wifi.getMux();    
          wifi.getCWSAP();    // ssid, pwd, ch, enc
          }
        if (mival==1) 
          {  // SET Config, configurar esp8266
          wifi.setCWMode(wifi._CWMode);
          wifi.setMux(1);
          wifi.setCWSAP(wifi._APssid,wifi._APpass,wifi._APch,wifi._APenc);
          wifi.Reset(); 
          }
        if (mival==2) 
          {auxerr=wifi.Reset();}
        }
      } while (repeat);
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srhtm);
    server.httpSeeOther(auxchr); return;
    }  
  
///////////////////
  server.httpSuccess();
  writeMenu(server, 5, 0);
  printP(server, html_i,body_i,Form_action,srhtm,Form_post,tablanormal);
  
  printP(server,tr,td,version,td_f,td);             // Version
  server.print(wifi._ver);
  printP(server,td_f,tr_f);             // Version
  
  wifi.getIP();
  printP(server,tr,td,dirip,td_f,td);   // IP
  server.print(wifi._myIP);
  printP(server,td_f,tr_f);             // IP
  
  printP(server,tr,td,cwmode,td_f,td);        // CWMODE
  printPiP(server,Select_name,0,barramayor);  // número de parámetro
  for (byte j=1; j<=3; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (wifi._CWMode == j) server.printP(selected);    server.printP(cierre);
    if (j==1) server.printP(cwmode1); if (j==2) server.printP(cwmode2); if (j==3) server.printP(cwmode3);
    server.printP(option_f);
    }  
  printP(server,td_f,tr_f);
  printP(server,tr,td,mux,td_f,td);
  printPiP(server,Select_name,1,barramayor);  // número de parámetro
  for (byte j=1; j<=1; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (wifi._CIPMux == j) server.printP(selected);    server.printP(cierre);
    server.print(j);
    printP(server,option_f);
    }  
  printP(server,td_f,tr_f);             // MUX
  
  printparCP(server, ssid, 2, wifi._APssid, 20,false);
  printparCP(server, contrasena, 3, wifi._APpass, 20,false);
  
  printP(server,tr,td,radCanal,td_f,td);
  printPiP(server,Select_name,6,barramayor);  // número de parámetro
  for (byte j=1; j<=13; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (atoi(wifi._APch) == j) server.printP(selected);
    printPiP(server,cierre,j,option_f);
    }  
  printP(server,td_f,tr_f,tr,td,encryption,td_f,td);             // encryption
  printPiP(server,Select_name,7,barramayor);  // número de parámetro
  for (byte j=0; j<=4; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (atoi(wifi._APenc) == j) server.printP(selected);
    server.printP(cierre);
    if (j==0) printP(server,noencriptyon);
    if (j==1) printP(server,wep);
    if (j==2) printP(server,wpapsk);
    if (j==3) printP(server,wpa2psk);
    if (j==4) printP(server,wpawpa2psk);
    server.printP(option_f);
    }  
  printP(server,td_f,tr_f,br);             // encryption

  printP(server,br,tr,td,comando,td_f,td);
  printPiP(server,Select_name,10,barramayor);  // número de parámetro
  for (byte j=0; j<=2; j++)  
    {
    printPiP(server,optionvalue,j,barraatras);
    if (ultcomando == j) server.printP(selected);
    server.printP(cierre);
    if (j==0) server.printP(GetConfig);
    if (j==1) server.printP(SetConfig);
    if (j==2) server.printP(RESET);
    server.printP(option_f);
    }  
  if (auxerr<0) printP(server,error);
  printP(server,td_f,tr_f,table_f,Form_input_exec,Form_f,body_f,html_f);
}

void mainrCmd(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
  {
  char destIP[16]; memset(destIP,0,sizeof(destIP));
  strcpy(destIP, "192.168.4.");
  strcat(destIP,url_tail); 
  int auxerr=wifi.SendRequest(mysocket,destIP,gatewayPort,cmdgetmainr,3000,5000);
  if (auxerr>=0)
    {
    server.httpSuccess();
    printP(server,htmlHead_i0, htmlHead_ix, htmlHead_i1, head_f, body_i,tablanormal);
    server.print(wifi.inputstr+4); 
    printP(server, table_f,body_f,html_f);
    }
  else
    {
    printP(server,error);
    server.println(auxerr);
    }
  printP(server,tr_f,body_f,html_f);  

  }

void setupremnoHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srnohtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  byte md=1;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    dserial.println(url_tail);
//    for (byte i=0; i<maxTempr; i++)
//      setbit8(rbshowpin,i+120,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number/md;
      byte resto = param_number % md; 
      unsigned int mival = atoi(value);
      if (resto == 0) tremID[indice] = mival;  
      } while (repeat);
    guardarDatosNodos();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srnohtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,5,9);
  printP(server,brn,Form_action,srnohtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i); 
  printColspan(server,4,true);
  printP(server,nodosremotos,td_f);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, Nodo, false, true);
  printTDP(server, raizdirip, false, true);
  printTDP(server, hora, false, true);
  server.printP(tr_f);

  char buf[5];
  for (byte i=0; i<maxNodos; i++)
    {
    boolean colorea=(tremIP[i]!=0);
    int mdi=md * i;
    printP(server,tr,(colorea?th:td),nodoremoto,b);
    server.print(i+1);    
    printP(server,b,td_f);

    printcampoI(server, mdi, tremID[i], 3, true);      // parámetro Nodo
    printP(server, (colorea?th:td));
    
    printP(server,href_i);
    server.print("mainr.htm?");
    server.print(tremIP[i]);
    printP(server,href_cierre);
    server.print(tremIP[i]);
    printP(server,href_f);
    
    printP(server,td_f,td);
    if (thour[i]<10) server.print(0);
    server.print(thour[i]);
    printP(server,dospuntos);
    if (tmin[i]<10) server.print(0);
    server.print(tmin[i]);

    printP(server, td_f,tr_f);
    }
  printP(server, table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupremtHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srthtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  byte md=4;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (byte i=0; i<maxTempr; i++)
      setbit8(rbshowpin,i+120,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number/md;
      byte resto = param_number % md; 
      unsigned int mival = atoi(value);
      if (resto == 0) setbit8(rbshowpin, indice+120, mival);     
      if (resto == 1) remTnodo[indice]=strtol(value,NULL,10);        
      if (resto == 2) remTpin[indice] = mival;  
      if (resto == 3)        // descripción
        {
        for (byte i=0; i<LEN20; i++)  
          EEwrite(dirEEdescTempr+(indice*LEN20)+i, value[i]);
        }
      } while (repeat);
    guardarDatosrTemp();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srthtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,5,1);
  printP(server,brn,Form_action,srthtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i); 
  printColspan(server,2,true);
  printP(server,Temp_rem,td_f,td,b,td_f);
  printColspan(server,2,true);
  printP(server,b);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, Nodo, false, true);
  printTDP(server, n, false, true);
  server.printP(tr_f);

  char buf[5];
  for (byte i=0; i<maxTempr; i++)
    {
    int mdi=md * i;
    boolean esth=(getbit8(rbshowpin,i+120)==1);
    if (esth) printP(server,tr,th,paren_i); else printP(server,tr,td,paren_i);
    printPiP(server,rs,i,paren_f1b); 
    printP(server,sondaremota);
    server.print(i+1);    
    if (esth) printP(server,b,th_f,th,Form_input_text_start); 
    else printP(server,b,td_f,td,Form_input_text_start);
    
    server.print(mdi+3);    // número de parámetro +3                 
    printP(server,Form_input_value);        
    printEE (server, dirEEdescTempr+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end, esth?th_f:td_f, esth?th:td);

    server.checkBox(itoa(mdi,buf,10),"1","",esth);
    if (esth) printP(server,th_f,th); else printP(server,td_f,td);

    printcampobyte(server, mdi+1, remTnodo[i], 6,false);      // parámetro +1
    printP(server, esth?th_f:td_f, esth?th:td);
    
    printcampoCB(server, mdi+2, remTpin[i], 1, 3,true,false);  // parámetro +2
    printP(server, esth?th_f:td_f,tr_f);
    }
  printP(server, table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupremaHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srhtm, 0,'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=7;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=0; i<20; i++) setbit8(rbshowpin,i,0);
    for (int i=0; i<3;i++) setbit8(bsumatArem,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number/mp;    // posiciones en rbshowpin 0..19
      byte resto = param_number % mp; 
      unsigned int mival = atoi(value);
      if (resto == 0) setbit8(rbshowpin, indice, mival);     
      if (resto == 1) remNodo[indice]=strtol(value,NULL,10);        
      if (resto == 2) remPin[indice] = mival; 
      if (resto == 3)        // descripción
        for (byte j=0; j<LEN20; j++)  
          EEwrite(dirEEdescAnalr+(indice*LEN20)+j, value[j]);
      if (resto == 4) factorArem[indice] = atof(value); 
      if (resto == 5) offsetArem[indice] = atof(value); 
      if (resto == 6) setbit8(bsumatArem, indice, mival);     
      } while (repeat);
    guardarDatosRem();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srahtm);
    server.httpSeeOther(auxchr); return;
    }
  
  server.httpSuccess();
  writeMenu(server,5,2);
  printP(server,brn,Form_action,srahtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i);
  printColspan(server,8,true);
  printP(server,Ent_ana,td_f); 
  printTDTR(server, 1, true, false);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, Nodo, false, true);
  printTDP(server, pin, false, true);
  printTDP(server, factor, false, true);
  printTDP(server, offset, false, true);
  printTDP(server, sumat, false, true);
  server.printP(tr_f);
  
  char buf[5];
  for (byte i=0; i<20; i++)
    {
    int mpi=mp*i;
    boolean mostrar=(getbit8(rbshowpin,i)==1);
    if (mostrar) printP(server,trcolor2_i,th); else printP(server,tr,td);
    
    printP(server,paren_i,letrar,letraa);
    server.print(i); 
    printP(server,paren_f1b,entana,remota,b); 
    server.print(i+1);
    if (mostrar) printP(server,b,th_f,th,Form_input_text_start); 
    else printP(server,b,td_f,td,Form_input_text_start);
    
    server.print(mpi+3);                  // número de parámetro +3                 
    server.printP(Form_input_value);        
    printEE (server, dirEEdescAnalr+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);        
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);
    
    server.checkBox(itoa(mpi,buf,10),"1","",(getbit8(rbshowpin,i)==1));
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampobyte(server, mpi+1, remNodo[i], 5,false);
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoCB(server, mpi+2, remPin[i], 1, maxEAr,true,false);
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoF(server,mpi+4, factorArem[i], 10,8, false);
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);

    printcampoF(server,mpi+5, offsetArem[i], 10,5, false);
    if (mostrar) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+6,buf,10),"1","",(getbit8(bsumatArem,i)==1));
    if (mostrar) printP(server,th_f,tr_f); else printP(server,td_f,tr_f);
    }
  printP(server ,table_f,Form_input_send,Form_f,body_f,html_f);
}

void setupremiHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srihtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=4;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=20; i<40; i++) setbit8(rbshowpin,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number / mp;
      byte resto = param_number % mp; 
      unsigned int mival = atoi(value);
      if (resto == 0)        // descripción
        {
        for (byte j=0; j<LEN20; j++)  
          EEwrite(dirEEdescEDigr+(indice*LEN20)+j, value[j]);
        }
      if (resto == 1) setbit8(rbshowpin, indice+20, mival);     
      if (resto == 2) remNodo[indice+20]=strtol(value,NULL,10);        
      if (resto == 3) remPin[indice+20] = mival; 
      } while (repeat);
    guardarDatosRem();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srihtm);
    server.httpSeeOther(auxchr); return;
    }
  server.httpSuccess();
  writeMenu(server,5,3);
  printP(server,brn,Form_action,srihtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i); 
  printColspan(server,2,true);
  server.printP(Ent_dig); 
  printP(server,td_f,td,b,td_f); 
  printColspan(server,2,true);
  printP(server,b);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, Nodo, false, true);
  printTDP(server, pin, false, true);
  
  server.printP(tr_f);
  char buf[5];
  for (byte i=0; i<20; i++)
    {
    int mpi=mp*i;
    int iaux=(i*mp)+20;
    server.printP(tr);
    if (getbit8(rbshowpin,i+20)==1) printP(server,th); else printP(server,td);
    printP(server,paren_i,letrasred);
    server.print(i+20); 
    printP(server,paren_f1b,entdig,remota); 
    printPiP(server,b,i+1,b);
    if (getbit8(rbshowpin,i+20)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printP(server,Form_input_text_start);      
    server.print(mpi);                  // número de parámetro +3                 
    server.printP(Form_input_value);        
    printEE (server, dirEEdescEDigr+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(rbshowpin,i+20)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+1,buf,10),"1","",(getbit8(rbshowpin,i+20)==1));
    if (getbit8(rbshowpin,i+20)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampobyte(server, mpi+2, remNodo[i+20], 5,false);
    if (getbit8(rbshowpin,i+20)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoCB(server, mpi+3, remPin[i+20], 1, maxEDr,true,false);
    if (getbit8(rbshowpin,i+20)==1) printP(server,th_f); else printP(server,td_f);
    server.printP(tr_f);
    }
  printP(server,table_f ,Form_input_send,Form_f,body_f,html_f);
}

void setupremoHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srohtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=4;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=40; i<60; i++) setbit8(rbshowpin,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number / mp;
      byte resto = param_number % mp; 
      unsigned int mival = atoi(value);
      if (resto == 0)        // descripción
        {
        for (byte j=0; j<LEN20; j++)  
          EEwrite(dirEEdescSDigr+(indice*LEN20)+j, value[j]);
        }
      if (resto == 1) setbit8(rbshowpin, indice+40, mival);     
      if (resto == 2) remNodo[indice+40]=strtol(value,NULL,10);        
      if (resto == 3) remPin[indice+40] = mival; 
      } while (repeat);
    guardarDatosRem();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srohtm);
    server.httpSeeOther(auxchr); return;
    }
  server.httpSuccess();
  writeMenu(server,5,4);
  printP(server,brn,Form_action,srohtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i); 
  printColspan(server,2,true);
  server.printP(Sal_dig); 
  printP(server,td_f,td,b,td_f); 
  printColspan(server,2,true);
  printP(server,b);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, Nodo, false, true);
  printTDP(server, pin, false, true);
  
  server.printP(tr_f);
  char buf[5];
  for (byte i=0; i<20; i++)
    {
    int mpi=mp*i;
    server.printP(tr);
    if (getbit8(rbshowpin,i+40)==1) printP(server,th); else printP(server,td);
    printP(server,paren_i,letrasrsd);
    server.print(i); 
    printP(server,paren_f1b,saldig,remota); 
    printPiP(server,b,i+1,b);
    if (getbit8(rbshowpin,i+40)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printP(server,Form_input_text_start);      
    server.print(mpi);                  // número de parámetro +3                 
    server.printP(Form_input_value);        
    printEE (server, dirEEdescSDigr+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(rbshowpin,i+40)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+1,buf,10),"1","",(getbit8(rbshowpin,i+40)==1));
    if (getbit8(rbshowpin,i+40)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampobyte(server, mpi+2, remNodo[i+40], 5,false);
    if (getbit8(rbshowpin,i+40)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoCB(server, mpi+3, remPin[i+40], 1, 2,true,false);
    if (getbit8(rbshowpin,i+40)==1) printP(server,th_f); else printP(server,td_f);
    server.printP(tr_f);
    }
  printP(server,table_f ,Form_input_send,Form_f,body_f,html_f);
}

void setupremrHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srrhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int mp=4;

  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    for (int i=60; i<120; i++) setbit8(rbshowpin,i,0);
    do   {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte indice = param_number / mp;
      byte resto = param_number % mp; 
      unsigned int mival = atoi(value);
      if (resto == 0)        // descripción
        {
        for (byte j=0; j<LEN20; j++)  
          EEwrite(dirEEdescReler+(indice*LEN20)+j, value[j]);
        }
      if (resto == 1) setbit8(rbshowpin, indice+60, mival);     
      if (resto == 2) remNodo[indice+60]=strtol(value,NULL,10);        
      if (resto == 3) remPin[indice+60] = mival; 
      } while (repeat);
    guardarDatosRem();
    char auxchr[20]="";
    strcatP(auxchr, barra);
    strcatP(auxchr, srrhtm);
    server.httpSeeOther(auxchr); return;
    }
  server.httpSuccess();
  writeMenu(server,5,5);
  printP(server,brn,Form_action,srrhtm,Form_post,Form_input_send,tablaconf);
  
  printP(server,trcolor1_i); 
  printColspan(server,2,true);
  server.printP(Sal_rel); 
  printP(server,td_f,td,b,td_f); 
  printColspan(server,2,true);
  printP(server,b);
  printTDTR(server, 1, true, true);
  
  printP(server,trcolor1_i,td_if);  
  printTDP(server, descripcion, false, true);
  printTDP(server, ver, false, true);
  printTDP(server, Nodo, false, true);
  printTDP(server, pin, false, true);
  
  server.printP(tr_f);
  char buf[5];
  for (byte i=0; i<60; i++)
    {
    int mpi=mp*i;
    server.printP(tr);
    if (getbit8(rbshowpin,i+60)==1) printP(server,th); else printP(server,td);
    printP(server,paren_i,letrasrsd);
    server.print(i); 
    printP(server,paren_f1b,releremoto); 
    printPiP(server,b,i+1,b);
    if (getbit8(rbshowpin,i+60)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printP(server,Form_input_text_start);      
    server.print(mpi);                  // número de parámetro +3                 
    server.printP(Form_input_value);        
    printEE (server, dirEEdescReler+(i*LEN20), LEN20);
    printPiP(server,Max_length,LEN20-1,size_i);
    server.print(LEN20-1);
    printP(server,Form_input_end);
    if (getbit8(rbshowpin,i+60)==1) printP(server,th_f,th); else printP(server,td_f,td);

    server.checkBox(itoa(mpi+1,buf,10),"1","",(getbit8(rbshowpin,i+60)==1));
    if (getbit8(rbshowpin,i+60)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampobyte(server, mpi+2, remNodo[i+60], 5,false);
    if (getbit8(rbshowpin,i+60)==1) printP(server,th_f,th); else printP(server,td_f,td);
    
    printcampoCB(server, mpi+3, remPin[i+60], 3, 6,true,false);
    if (getbit8(rbshowpin,i+60)==1) printP(server,th_f); else printP(server,td_f);
    server.printP(tr_f);
    }
  printP(server,table_f ,Form_input_send,Form_f,body_f,html_f);
}

char NodoAct[4]="";
char NodoNue[4]="";
char GWIP[16]="";
long peractr=60;

void setupremnHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  if (LOG==1) logIPP (srnhtm, 0, 'I', false); 
  if (!autOK(server))  {server.httpUnauthorized(); return; }
  char name[LEN5];
  char value[LEN20];
  int param_number = 0;
  int PerAct = 15;
  int auxerr=99;
  if (type == WebServer::HEAD) return;
  if (type == WebServer::POST)   {
    bool repeat;
    do   
      {
      repeat = server.readPOSTparam(name, LEN5, value, LEN20);
      if (!repeat) break;
      param_number = atoi(name);  
      byte mival = atoi(value);
      if (param_number == 0) strcpy(NodoAct,value); 
      if (param_number == 1) strcpy(NodoNue,value);  
      } while (repeat);
      
    //// enviar nueva config. al nodo
    int auxfin=ipnodo(NodoAct);
    char buff[4];
    if (auxfin==0) 
      auxerr=-99;
    else
      {
      char auxip[16]="";
      strcatP(auxip,prefixIP);
      strcat(auxip,itoa(ipnodo(NodoAct),buff,10));
      printlnS(cmdsetidset);
      char auxchr2[30]="";
      strcatP(auxchr2,cmdsetidset);
      strcat(auxchr2,NodoNue);
      dserial.println(auxchr2);
      auxerr=wifi.SendRequest(mysocket,auxip,gatewayPort,auxchr2,3000,5000);
      }
    printP(server,barra,srnhtm); return;
    }
  
  server.httpSuccess();
  writeMenu(server,5,6);
  printP(server,brn,Form_action,srnhtm,Form_post,tablanormal,trcolor1_i);
  printColspan(server,2,true);
  printP(server,h3_i,confnodos,h3_f);
  printTDTR(server, 1, true, true);
  printparCP(server, nodoact, 0, NodoAct, 3, false);
  printparCP(server, nodonue, 1, NodoNue, 3, false);
  strcatP(GWIP,raizgwipdefault);
  printparCP(server, gatewayip, 2, GWIP, 16, false);
  printparIP(server, periodoact86400, 3, peractr, 5);
  printP(server,table_f,Form_input_send,Form_f,body_f); 
  server.printP(html_f);
}

void setupremsHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
  
}

void setupremresetHTML(WebServer &server, WebServer::ConnectionType type, char *url_tail, bool tail_complete)
{
}

void initESP8266()
{
//  printS(treset,b,wifi.Reset()>=0?OK:ERROR);
//  dserial.println();
//  delay(2000);
  wifi.clearResults();
  printS(version,b,wifi.getVer()>=0?b:ERROR,b);  
  dserial.println(wifi._ver);
  printS(muxmode,b,((wifi.setMux(1)>=0)?b:ERROR),b); 
  dserial.println(wifi._CIPMux);
  printS(blancoscwmode,b,wifi.setCWMode(AP_STA)>=0?b:ERROR,b); 
  dserial.println(wifi._CWMode);
  strcpy(wifi._APssid,"conucogw");
  strcpy(wifi._APpass,"conucogw");
  strcpy(wifi._APch,"7");        // canal 7
  strcpy(wifi._APenc,"3");        // 
  printS(Cwsap,dospuntos,wifi.setCWSAP(wifi._APssid,wifi._APpass,wifi._APch,wifi._APenc)>=0?b:ERROR,b); 
  dserial.print(wifi._APssid);printS(coma);
  dserial.print(wifi._APpass);printS(coma);
  dserial.print(wifi._APch);printS(coma);
  dserial.println(wifi._APenc);
  printS(openserver,b,wifi.openServer(wifi._myPort)>=0?OK:ERROR);dserial.println();
  printS(blancoIP,b,wifi.getIP()>=0?b:ERROR); dserial.print(wifi._myIP);printS(dospuntos);
  dserial.println(wifi._myPort);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
//  wdt_disable();   // DESACTIVAR WDT
  SPI.begin(); 
  if (tipoLCD==1) lcd.begin (20,4); 
  if (tipoLCD==2) lcd.begin (40,4); 
  Wire.begin(); 
  
  leerDatosRed();
  leerDatosDevice(); 
  numPines = numIA+numID+numOD;
//  for (byte i = 0; i < 18; ++i)
  for (byte i = 0; i < 17; ++i)
    if (freepines[i] == 0) {
      pinMode(i, INPUT);  
      digitalWrite(i, INPUT_PULLUP);  
      }
  for (byte i = 22; i < 54; ++i)
    if (freepines[i] == 0) {
      pinMode(i, OUTPUT);  
      digitalWrite(i, valorpin[getbit8(Mb.C8,i)]);  
      }

  for (byte i=66;i<=69;i++)
    {  
    pinMode(i,INPUT);    
    digitalWrite(i,HIGH); 
    pinMode(i,OUTPUT);    
    digitalWrite(i,HIGH); 
    }
  
  pinMode(rstFab,INPUT);  digitalWrite(rstFab,HIGH);    // reset fábrica
  dserial.begin(115200);
  wifi.init(57600);  // 57600 baud

  if (digitalRead(rstFab)==0)
    {
    printS(espere);
    logSys("Fab+Reset",0,'S',false);
    iniciavalores();
    }
  //////////////////////////////////////////////////////////
//
  leerDatosEventos();
  leerDatosSemanal(); 
  leerDatosFechas(); 
  leerEstado();
  
  webserver.m_server.chport(EEwebPort);
  Ethernet.begin(EEmac, EEip);   
  webserver.begin(); 

  memset(bevenENABLE,255,sizeof(bevenENABLE));
  
  char auxchr[20]="";
  webserver.setDefaultCommand(&indexHTML);
  // Máximo 45 comandos. Para ampliar modificar variable m_commands[] en webserver.h
  webserver.addCommand("index.html", &indexHTML);
  webserver.addCommand("sd.htm", &setupDevHTML);
  webserver.addCommand("st.htm", &setupSondHTML);
  webserver.addCommand("sa.htm", &setupEAnagHTML);
  webserver.addCommand("sed.htm", &setupEdigHTML);
  webserver.addCommand("ssd.htm", &setupSdigHTML);
  webserver.addCommand("sse.htm", &setupSemHTML);
  webserver.addCommand("sav.htm", &setupAvaHTML);
  webserver.addCommand("sv.htm", &setupEveHTML);
  webserver.addCommand("scl.htm", &setupClimaHTML);
  webserver.addCommand("sne.htm", &setupNetHTML);
  webserver.addCommand("ss.htm", &setupSegHTML);
  webserver.addCommand("srl.htm", &setupRelHTML);
  webserver.addCommand("rst.htm", &resetHTML);
  webserver.addCommand("sf.htm", &setupFecHTML);
  webserver.addCommand("sesc.htm", &setupEscHTML);
  webserver.addCommand("sprg.htm", &setupPrgHTML);
  webserver.addCommand("slk.htm", &setupLinksHTML);
  webserver.addCommand("sy.htm", &systemHTML);
  webserver.addCommand("wc.htm", &webclientHTML);
  webserver.addCommand("slg.htm",&ShowLogCmd );   // muestra ficheros log
  webserver.addCommand("sfl.htm",&ShowFilesSD );   // muestra ficheros en SD
  webserver.addCommand("on",&onCmd);         // pone a ON salida digital  sintaxis: /on?nn
  webserver.addCommand("off",&offCmd);       // pone a OFF salida digital  sintaxis: /off?nn   
  webserver.addCommand("sf",&ShowFileCmd);   // borra fichero de la SD     sintaxis: /sf?namefile.txt
  webserver.addCommand("df",&DeleFileCmd);   // borra fichero de la SD     sintaxis: /df?namefile.txt
  webserver.addCommand("onrh", &onrHTML);           // pone ON salida remota
  webserver.addCommand("offrh", &offrHTML);        // pone OFF salida remota
  webserver.addCommand("json",&jsonCmd);
  webserver.addCommand("jsonn",&jsonnamesCmd);
  webserver.addCommand("yid",&youridCmd);        // pide ID de un remoto
  webserver.addCommand("jsonr",&jsonrCmd);          // pide json de un remoto
  webserver.addCommand("idset",&idsetCmd);          // asigna ID a un remoto
  webserver.addCommand("gwip",&gwipCmd);            // asigna GWIP a un remoto
  webserver.addCommand("expl",&explCmd);            // explora remotos
  webserver.addCommand("sr.htm", &setupremHTML);
  webserver.addCommand("srno.htm", &setupremnoHTML);  // nodos remotos
  webserver.addCommand("srt.htm", &setupremtHTML);  // sondas remotas
  webserver.addCommand("sra.htm", &setupremaHTML);  // entradas analógicas remotas
  webserver.addCommand("sri.htm", &setupremiHTML);  // entradas digitales remotas
  webserver.addCommand("sro.htm", &setupremoHTML);  // salidas digitales remotas
  webserver.addCommand("srr.htm", &setupremrHTML);  // relés remotos
  webserver.addCommand("srn.htm", &setupremnHTML);  // configurar nodo
  webserver.addCommand("srs.htm", &setupremsHTML);  // busca nodos
  webserver.addCommand("srst.htm", &setupremresetHTML);  // reset nodo
  webserver.addCommand("mainr.htm", &mainrCmd);    // acceso remotos

// 
  lcd.clear();
  printLCD(OK);
  for (byte i=0; i<3;i++) {lcd.print(EEip[i]); printLCD(punto);} lcd.print (EEip[3]);
  printLCD(dospuntos); lcd.print(EEwebPort); 
  lcd.setCursor(0,2); 
  RTC.read(tmInicio);
  horalcd(false);

//  iniciavaloresIP();

//
  sensors1.begin();
  nTemp1=sensors1.getDeviceCount();
//
    printS(conucoweb); dserial.print(miversion);
    printS(punto); dserial.println(misubver);
    serialmem(" "); dserial.println();
    
    printS(lanip);
    for (byte i=0; i<4;i++) { dserial.print(EEip[i]); if (i<3) printS(punto); }
    printS(dospuntos);
    dserial.print(EEwebPort);
    printS(blancogw);
    for (byte i=0; i<4;i++) { dserial.print(EEgw[i]); if (i<3) printS(punto); }
    dserial.println();

    printS(blanco1wp1); dserial.print(nTemp1, DEC);
    printS(blancoSondas,blancomodo);
    if (sensors1.isParasitePowerMode()) printS(parasite); else printS(power);
    dserial.println();
    for (int i=0; i<nTemp1; i++)   
      {
      if (sensors1.getAddress(addr1Wire[i], i))     
        {
        sensors1.setResolution(prectemp);
        printS(blancos4);
        for (uint8_t j = 0; j < 8; j++)     
          {
          if (addr1Wire[i][j] < 16) printS(cero);
          dserial.print(addr1Wire[i][j], HEX);   
          }
        dserial.println();
        }
      }
    dserial.println();
    leevaloresOW();

// inicia tarjeta SD
//     pinMode(ethSEL,OUTPUT);
//     sdHay=SD.begin(sdSEL,SPI_HALF_SPEED);
//     printS(tSD);
//     printlnS(sdHay?Ok:NO);
//     if (!SD.begin(sdSEL, SPI_HALF_SPEED)) SD.initErrorHalt();

  testEEPROM();

  EEPROM_readAnything (dirEEfactorA, factorA);
  EEPROM_readAnything (dirEEoffsetA, offsetA);
  EEPROM_readAnything (dirEEoffsetA, bsumatA);
  EEPROM_readAnything (dirEEesc, bescena);
  mact1 =millis();
  mact10 =millis();
  mact60 =millis();
  mactVar =millis();
////////////////////////////

  initESP8266();
//  wifi.setMux(1);
   
  memset(tremID,0,sizeof(tremID));
  memset(tremIP,0,sizeof(tremIP));
  leerDatosNodos();
  leerDatosRem();
  leerDatosrTemp();
  //////////////////////////////////////////////////////////
  printlnS(ready); 
  memset(vanaTED,0,sizeof(vanaTED));
  memset(vanaHED,0,sizeof(vanaHED));
// actualizaDDNS(); 
}

void espReset()
  {
  dserial.println("Reset ESP8266"); 
  wifi.HardReset(espResetPin,1000);  
  delay(3000);
  initESP8266();
  }
  
void buscatecla()
  {
  while (dserial.available()) 
    {
    int inByte = dserial.read();
    if ((inByte==49)) if (wifi.setMux(1)==0) {printS(muxmode); dserial.println(wifi._CIPMux);} // 1
    if (inByte==68) {printlnS(mododebugont); mododebug=true;}  // D
    if (inByte==72) {espReset();}    // H
    if (inByte==82) { iniciavalores();  }  // R    Reset fabrica
    if (inByte==84) {printlnS(modotransont); transparentmode=true;}  // T
    if (inByte==114) { printS(treset); printlnS((wifi.Reset()==0?OK:ERROR));}  // r
    if (inByte==119)   // w
      {  
      int auxerr= wifi.Reset();      // Reset esp8266
      printS(treset,paren_i); printS(auxerr>=0?OK:ERROR); printS(paren_f,b); dserial.println(wifi._ver);
      delay(3000);
      }
    }
  }

void loop()    // el bucle se ejecuta unas 75 veces/segundo
  {
  if (transparentmode)
    {
    if (dserial.available()) wserial.write(dserial.read());
    if (wserial.available()) dserial.write(wserial.read());
    return;
    }
  if (mododebug)
    {
    if (dserial.available()) 
      {
      char a=dserial.read();
      if (byte(a)==4)  // CTRL+D
        {
        mododebug=false; printlnS(mododebugofft);}
      else  
        wserial.write(a);
      }
    if (wserial.available()) dserial.write(wserial.read());
    return;
    }
  buscatecla();

  contador++;
  
  if (!wifi.receiving)
    {
    if (wifi.msgReceived)    // se ha localizado la cadena final de búsqueda
      {
      if (wifi._comtype==0) {printS(noesperado); dserial.println(wifi.inputstr);}
      if (wifi._comtype==1) { /*procesaGet()*/};
      if (wifi._comtype==2) procesaInf();
      if (wifi._comtype==3) { /*procesaSet()*/ };
      if (wifi._comtype==4) procesaJsonData();
      if (wifi._comtype==5) procesaJsonConf();
      wifi.indexinput=0;
      memset(wifi.inputstr,0,sizeof(wifi.inputstr));
      wifi.msgReceived=false;
      }
//  
    if (dserial.available())
      {
      char c=dserial.read();
      if (c=='D') {printlnS(mododebugont); mododebug=true;}
      return;
      }
    if ((millis() > (mact1 + 1000)))    // tareas que se hacen cada segundo
      {
      mact1=millis();
      RTC.read(tm);
      velocidad=contador;
      contador=0;
      leervaloresAna();
      }
    if ((millis() > (mact10 + 10000)))    // tareas que se hacen cada 10 segundos
      {
//        if (wifi.sendAT() <0) espReset();
        leevaloresOW();  
        mact10=millis();
        velocidad=contador/10;
        contador=0;
//        actualizasegunModbus();
        for (int i=0; i<16; i++)       // leer valores de sondas DHTxx
          {
          if (tipoED[i]==1)            // DHT11
            {
            dht.begin(tabpin[i],DHT11);
            vanaTED[i]=dht.readTemperature(false); 
            vanaHED[i]=dht.readHumidity(); 
            }
          if (tipoED[i]==2)            // DHT21
            {
            dht.begin(tabpin[i],DHT21);
            vanaTED[i]=dht.readTemperature(false); 
            vanaHED[i]=dht.readHumidity(); 
            }
          if (tipoED[i]==3)            // DHT22
            {
            dht.begin(tabpin[i],DHT22);
            vanaTED[i]=dht.readTemperature(false); 
            vanaHED[i]=dht.readHumidity(); 
            }
          }
      }

    if ((millis() > (mact60 + 60000)))    // tareas que se hacen cada 60 segundos
      {
       mact60=millis();
      }
    if ((millis() > (mactVar + (pmactVar*1000))))    // tareas que se hacen cada pmactVar segundos
      {
      mactVar=millis();
//      if (mactVar>0) logTemp();
      }
    for (byte i=0; i<8;i++) //  actualiza contadores
      {     
       if (Mb.C8[i+32]!=digitalRead(tabpin[i]))
         {
         if (digitalRead(tabpin[i])==1)
           {
           contadores[i]++;
//           EEPROM_writeAnything(dirEEcontadores+(4*i), contadores[i]);
           }
         Mb.C8[i+32]=digitalRead(tabpin[i]);
         }
      }
//
    Mb.Run();    // actualizar Modbus
    char buff[buffLEN];
    int len = buffLEN;  
    webserver.processConnection(buff, &len);
    }
  else
    if (millis()-wifi._tini>wifi._timeout)  // se ha superado timeout
      {
      printlnS(timeout);
      wifi.msgReceived=false;
      wifi.receiving=false;
      wifi.indexinput=0;
      memset(wifi.inputstr,0,sizeof(wifi.inputstr));
      wifi.clearResults();
      }   
  }
/////////////// end loop ////////////////////////////


void serialEvent1() 
  {
  if ((!mododebug) && (!transparentmode))
    wifi.procSerialEvent1();
  }  
