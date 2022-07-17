# TEST-SUITE_ChibiOS
Progetto NISC2021 TEST-SUITE NUCLEO64 (2021)

### NISC2021:
Il NeaPolis Innovation Summer Campus 2021 è stato svolto interamente online, attraverso la piattaforma di videoconferenze Microsoft Teams.
Il campus si è svolto dal 26 agosto 2021 al 5 settembre 2021. Le giornate prevedevano dei seminari intensivi sui microcontrollori a 32 bit e sui Sistemi Operativi Real-Time.
Dopo una prima fase preliminare di seminari e lavoro individuale, i tutor ci hanno raggruppati in team per sviliuppare un progetto. A tale scopo, ogni partecipante ha ricevuto una scheda di sviluppo e delle componenti elettroniche.
I team sono stati scelti in maniera casuale dai tutor, permettendoci di lavorare con studenti italiani e stranieri con un background completamente diverso e con maggior esperienza.

### AMBIENTE DI SVILUPPO:
L’ambiente di sviluppo che abbiamo utilizzato per programmare e controllare la nostra scheda è “ChibiStudio”: un insieme di programmi basati su Eclipse per il supporto di ChibiOS, un sistema operativo real-time molto compatto ed efficiente per progettare applicazioni embedded. Il principale linguaggio di programmazione utilizzato è stato il C.
ChibiStudio è composto da:
*	Eclipse IDE;
*	GNU GCC ARM Compiler & Tools;
*	Open OCD Tool & scripts: software open-source per effettuare il debugging di device embedded;
*	ChibiOS: fornisce una serie di librerie per sfruttare al massimo il potenziale della scheda Nucleo e per semplificare e rendere più veloce la scrittura del codice di controllo.

ChibiStudio inoltre presenta due tipologie di prospettive:
*	C/C++ development: per la creazione e la gestione dei progetti, per la scrittura e la compilazione del codice e per la correzzione di errori di sintassi.
*	Debug: per testare le funzionalità ed individuare comportamenti non attesi e per correggere errori funzionali.

L'anatomia del progetto si suddivide in:
Insieme di cartelle:
*	Debug	folder:	contiene	la	configurazione
d’esecuzione.
*	Headers di configurazione:

&emsp; &emsp; • chconf.h: principali impostazioni relative al kernel;

&emsp; &emsp; • halconf.h: configurazioni relative ai driver di ChibiOS ed HAL;

&emsp; &emsp; • mcuconf.h: configurazioni strettamente legate al MCU
*	Makefile: contiene uno script che fornisce una serie di direttive usate dal compilatore per effettuare il build iniziale del codice.
*	Main: il codice sorgente, contenente l’entry point dell’applicazione.

### SCHEDA DI SVILUPPO:
La scheda che ci è stata fornita è una STM32 Nucleo-64 (NUCLEO-F401RE). Si tratta di una scheda appartenente alla famiglia di microcontrollori a 32 bit basati su architettura ARM Cortex-M.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179379789-9ab43abd-b920-42ec-8091-00f09c9c4434.jpg)

### GENERAL PURPOSE INPUT OUTPUT:
GPIO, “General Purpose Input Output”, è un'interfaccia informatica hardware che consente ai dispositivi
come i microprocessori di interagire con un'altra periferica.
I pin di I/O della STM Nucleo sono organizzati in gruppi di 16 elementi (da 0 a 15), e ciascuno di questi pin possono essere configurati come Input o Output, in maniera completamente indipendente.
Ogni gruppo è chiamato “Port” ed è identificato da una lettera (GPIOA, GPIOB, GPIOC…). I pin invece sono identificati dalla combinazione della lettera P, dall’indentificatore della porta (A,B,C,…) e dall’identificatore del pin (0,1,2,…15):.

È possibile programmare ciascun pin in 4 differenti modalità:
*	Input Mode, consente di campionare il livello logico di un pin.

&emsp; &emsp; • Il buffer di output viene disattivato;

&emsp; &emsp; • Il trigger TTL Schmitt viene acceso;

&emsp; &emsp; • I pin sono continuamente campionati e conservati in una memoria che può essere acceduta e letta per ottenere lo stato del pin.
*	Output Mode, consente di impostare il livello logico di un pin.

&emsp; &emsp; • Il buffer di output viene attivato;

&emsp; &emsp; • Il trigger TTL Schmitt viene acceso;

&emsp; &emsp; • Può essere cambiato lo stato del pin;

&emsp; &emsp; • Si può controllare lo stato elettrico del pin.
*	Analoge Mode, consente di usare ADC o DAC.
*	Modalità alternata, consente di assegnare un pin a una periferica di STM32.

&emsp; &emsp; • STM32 è dotata di un grande numero di periferiche, connesse al GPIO attraverso dei multiplexler;

&emsp; &emsp; • Ogni periferica è mappata su più di un pin per garantire una maggiore flessibilità.

Nella STM32 al microcontrollore viene fornita una tensione di 3,3V.
Lo stato GPIO alto è di 3,3V mentre quello basso è di 0V. I GPIO tollerano anche tensioni che arrivano fino a 5V e la corrente massima che un singolo pin è in grado di generare/assorbire è di circa 25 mA → la corrente totale assorbita/generata da tutti i pin non deve superare i 120 mA.

### PROGETTO:
I requisiti funzionali del progeto sono:
*	TS-REQ01-1: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC]

&emsp; &emsp; Se STATIC C [ON|OFF] altrimenti DYNAMIC C [100÷1000] (ms).
*	TS-REQ01-2: JOY [XY|POLAR]

&emsp; &emsp; Una volta attivato vengono visualizzati:

&emsp; &emsp; &emsp; • se in XY: le coordinate cartesiane relative alla posizione del joypad.

&emsp; &emsp; &emsp; • se in POLAR: le coordinate polari relative alla posizione del joypad.
•	TS-REQ01-3: OLED [LED|JOY]

&emsp; &emsp; Una volta attivato, lo stato del LED o del JOYPAD viene stampato sul display OLED.
•	TS-REQ01-4: BUZZ [ON|OFF] [PERIOD]

&emsp; &emsp; Viene riprodotto un segnale acustico, con periodo espresso in multipli interi del secondo.
•	TS-REQ01-5: DIMMER [ON|OFF] [RED|GREEN|BLUE]

&emsp; &emsp; Una volta attivato, il led indicato deve avere l'intensità controllata dall'encoder.
•	TS-REQ02-1: DEMO

&emsp; &emsp; Implementa una ruota dei colori con display OLED e il BUZZER. Il buzzer emette un tono quando viene raggiunto uno dei tre colori primari e la percentuale dei singoli color dominanti (contemporaneamente) viene visualizzato sul display.
