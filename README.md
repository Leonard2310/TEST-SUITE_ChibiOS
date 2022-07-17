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

### RGB LED
Uno dei componenti utilizzati durante il campus è stato il led RGB, il quale è formato da 3 differenti colori a led. Il led può essere collegato ai pin d’ingresso, in serie con delle resistenze, e fornendo un segnale alto o basso (di tensione) si può rispettivamente accendere e spegnere il led.
![Senza tito2lo](https://user-images.githubusercontent.com/71086591/179380009-5ee7dafe-1c7c-4f2e-9451-a0145f3cacb1.png)


###	CHIBIOS PAL DRIVER
Il PAL Driver, Port Abstraction Layer è un driver di ChibiOS che usa il GPIO. Come suggerisce il nome, il PAL driver fornisce un’astrazione della struttura hardware della scheda per facilitare la progettazione del codice.
Sono presenti diversi metodi di identificazione ed il rispettivo insieme di funzioni:
*	Relativi ai Pad che agiscono su in singolo I/O (port,pad)
*	Relativi ai Group che agiscono su un gruppo di I/O (port, mask, offset)
*	Relativi alle Port che agiscono su un’intera Port (port)
*	Relativi alle Line che sono un’alternativa al Pad (X).
Configurazioni per cambiare i Pin:
*	palSetPadMode(port, pad, mode)
*	palSetLineMode(line, mode)
*	palSetGroupMode(port, mask, offset, mode)
*	palSetPortMode(port, mode)

 ![Senza titolo](https://user-images.githubusercontent.com/71086591/179380012-096c766b-7156-4e55-9ec4-0cf3720565b3.png)

Ogni pad deve essere opportunamente configurato prima di essere utilizzato. La configurazione per ogni pad deve seguire le specifiche e le semantiche della board.
L’inizializzazione iniziale è effettuata dalla funzione halInit().

###	STM32 USART
Lo UART o Universal Asynchronous Receiver-Transmitter è un dispositivo hardware, di uso generale o dedicato, che converte flussi di bit di dati da un formato parallelo a un formato seriale asincrono o viceversa.
Comunicazione parallela: ogni bit viene trasmesso attraverso una linea dedicata. Richiede una linea per ogni bit più una linea per la sincronizzazione.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179380170-a0e331e3-4b2e-404b-bde2-9094241c2b42.png)
Comunicazione seriale: i bit vengono trasmessi attraverso una singola linea, chiamata bus. Richiede dei meccanismi di sincronizzazione più complessi.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179380182-2f79cee4-a6e4-4961-8648-53e7f5e42597.jpg)
La sincronizzazione seriale può essere: sincrona o ascincrona.
Mentre la comunicazione può essere semplice, half-duplex o full-duplex.
L’USART è una variante dell’UART che dispone di un clock addizionale. È una periferica progettata per implementare vari protocolli seriali.

###	CHIBIOS SERIAL DRIVER
Un Seriale è un driver di ChibiOS/HAL che usa l’USART. Il driver seriale memorizza nel buffer i flussi di input ed output utilizzando delle code di I/O. Ed Ogni API (Application Programming Interface) del driver seriale generalmente inizia con il prefisso "sd". 

Prima di utilizzarli, ogni driver seriale deve essere inizializzato e configurato. L’inizializzazione è effettuata automaticamente chiamando la funzione hallinit() nel main. La configurazione invece viene effettuata dall’utente utilizzato la funzione sdStart(). Tale funzione riceve in ingresso due parametri: un puntatore al driver che vogliamo avviare ed un puntatore alla struttura di configurazione.

### SHELL
Una shell è una interfaccia testuale che si basa su un flusso di I/O, in questo caso, offerto dal driver seriale. È uno strumento potente, che permette di creare eventi asincroni, ovvero che si verificano indipendentemente dal flusso del programma principale, inserendo dei comandi da tastiera.
Per utilizzare una Shell in ChibiStudio è necessario aggiungere il path della libreria “shell.h” all’interno
del Makefile.
Per utilizzarla in maniera corretta, bisogna:
*	Allocare memoria: bisogna definire la memoria heap in cui la Shell verrà allocata.
*	Definire una lista dei comandi: è un vettore che contiene i nomi dei comandi che la Shell si aspetta di ricevere dalla tastiera e i nomi delle relative funzioni.
* Configurare una struttura contenete il driver seriale e il vettore dei comandi.
*	Creare un nuovo thread, con parametri:

&emsp; &emsp; • Puntatore all’heap;

&emsp; &emsp; • Dimensione della Working Area;

&emsp; &emsp; • Identificatore del Thread;

&emsp; &emsp; • Priorità;

&emsp; &emsp; • Funzione del Thread;

&emsp; &emsp; • Argomenti (passati con cast a void).
*	Definire comandi personalizzati, i parametri sono:
&emsp; &emsp; • Puntatore allo stream seriale;
&emsp; &emsp; • Massimo numero di stringhe che la shell può ricevere ;
&emsp; &emsp; • Vettore che contiene le varie stringhe che sono state inserite da tastiera.
 
###	STM32 ADC
Un Convertitore Analogico-Digitale converte un segnale continuo nel tempo in una sequenza di valori discreti. È necessario un convertitore perché i segnali discreti sono direttamente interpretabili dal microcontrollore.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179380210-f1d7265c-3a29-4d5f-ab43-92f2df9eaa3a.jpg)
Specifiche di un convertitore ADC:
* FSR (Full Scale Voltage Range), massimo range di valori analogici che può essere dato in ingresso: 𝑉𝐹𝑆𝘙 = 𝑉i𝑛𝑝𝑢𝑡𝑀𝑎𝗑 − 𝑉i𝑛𝑝𝑢𝑡𝑀i𝑛
* Risoluzione, numero di valori discreti che l’ADC può produrre nell’intervallo consentito di valori d’ingresso analogico. La risoluzione in termini di tensione: Δ𝑉 =	𝑉𝐹𝑆𝘙/𝑟i𝑠𝑜𝑙𝑢zi𝑜𝑛𝑒
*	Sampling rate, cadenza di campionamento, misurato in S/s (samples per second) e i suoi multipli (kS/s, MS/s or GS/s).
In particolare, la scheda è equipaggiata con un “Successive Approximation ADC” a 12-bit, composto da:
*	Sample & Hold, campionatore e interfaccia tra segnale analogico e ADC;
*	Digital to Analog Converter;
*	Comparatore, riceve in input due segnali e li compara;
*	SAR (Successive Approximation Register).
 
La SMT32 fornisce due modalità di conversioni:
*	Singola conversione: l’ADC fa una sola conversione e poi si ferma.
*	Continua conversione: l’ADC inizia una nuova conversione quando l’ultima è terminata.
Inoltre, ogni SMT32 fornisce molte sorgenti di input da utilizziare come canali per effettuare la conversione. In particolare, ogni canale è identificato con un numero progressivo che parte da 0. I primi 16 canali sono generalmente assegnati a fonti esterne.

####	JOYSTICK
Il joystick è un altro dispositivo utilizzato durante il campus e che è stato fornito nello starter kit.
Esso è un semplice esempio di periferica analogica: usa due potenziometri, essi variano la resistenza nel circuito e indicano la posizione del joystick rispetto agli assi X ed Y.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179380109-0a1d56e6-37a3-437c-9fa6-48fc44a613a6.png)

###	COMUNICAZIONE I2C
L’I2C (Inter-Integrated Circuit) è un bus di comunicazione seriale, half-duplex e sincrono. Utilizza un’architettura di tipo master-slave, con un unico master. Il segnale di clock è generato da uno degli endpoint ed è fornito agli altri tramite una linea specifica (SCL).
La comunicazione avviene tramite un’unica linea (SDA), spesso chiamata seriale a due fili. Una transazione è composta da uno o più messaggi, in cui ogni messaggio è composto da una parola di un byte più un bit (ACK/NACK) aggiuntivo.
L’intero sottosistema del driver I2C può essere abilitato mediante l’apposito campo in “halconf.h”
(HAL_USE_I2C C TRUE).
Si può assegnare il driver ad una specifica periferica, modificando “mcuconf.h”.
Ogni operazione del driver può essere eseguita solo se il driver è stato configurato correttamente.

####	OLED DISPLAY
Un esempio di dispositivo che comunica con l’I2C è un Display OLED. Al Campus in particolare ci è stato fornito un Display OLED SSD1306.
Successivamente alla struttura di configurazione dell’I2C, implementiamo la struttura di configurazione dell’OLED Display e la definizione del proprio Driver.
Per utilizzarlo si sfruttano dei thread ed una serie di funzioni messe a disposizione dalla libreria ssd1306, che permettono di impostare la dimensione e colori del font, figure geometriche, ecc…

###	PWM
PWM (Pulse Width Modulation) è una modulazione digitale in cui la forma d'onda cambia tra due stati
con tempo di salita trascurabile e un periodo costante. In particolare, viene utilizzato per la gestione in hardware di periferiche esterne grazie a dei timer (TIM) in grado di interfacciarsi ai sensori.
Importante è il concetto di Duty Cycle, ovvero, il rapporto tra la durata dell’impulso positivo e l’intero periodo preso in considerazione.

![Senza titolo](https://user-images.githubusercontent.com/71086591/179380154-c53b758d-2b2a-44ec-a489-8554f97b342b.jpg)


####	BUZZER
Un dispositivo che usa il PWM è il Buzzer, un dispositivo di segnalazione audio, che può essere meccanico o elettromeccanico. I tipici utilizzi del buzzer includono dispositivi di allarme, timer, ecc…
La PWM gestisce in hardware il buzzer, essa tramite timer interni è in grado di prendere in ingresso un
certo periodo dato dall’utente e tramite impulsi far suonare o meno il buzzer.
La PWM viene utilizzata perché in grado di cambiare il periodo in runtime, quindi, utile in caso di variazione del periodo durante l’esecuzione del programma (utile anche per i LED in modalità DYNAMIC). Per la configurazione del Buzzer:
 
###	ENCODER
L’Encoder è un apparato elettronico che converte la posizione angolare del suo asse rotante in un segnale elettrico digitale collegato da opportuni circuiti elettronici e con appropriate connessioni meccaniche. È in grado di misurare spostamenti angolari, movimenti rettilinei e circolari nonché velocità di rotazione e accelerazioni.
 
L’Encoder può essere utilizzato per vari scopi. Nel nostro progetto ha avuto il ruolo di Dimmer, un regolatore elettronico utilizzato per controllare la potenza assorbita da un carico. È quindi in grado di controllare l’intensità luminosa del LED.

### PROGETTO:
I requisiti funzionali del progeto sono:
*	TS-REQ01-1: LED [RED|GREEN|BLUE] [STATIC|DYNAMIC]

&emsp; &emsp; Se STATIC C [ON|OFF] altrimenti DYNAMIC C [100÷1000] (ms).
*	TS-REQ01-2: JOY [XY|POLAR]

&emsp; &emsp; Una volta attivato vengono visualizzati:

&emsp; &emsp; &emsp; • se in XY: le coordinate cartesiane relative alla posizione del joypad.

&emsp; &emsp; &emsp; • se in POLAR: le coordinate polari relative alla posizione del joypad.
*	TS-REQ01-3: OLED [LED|JOY]

&emsp; &emsp; Una volta attivato, lo stato del LED o del JOYPAD viene stampato sul display OLED.
*	TS-REQ01-4: BUZZ [ON|OFF] [PERIOD]

&emsp; &emsp; Viene riprodotto un segnale acustico, con periodo espresso in multipli interi del secondo.
*	TS-REQ01-5: DIMMER [ON|OFF] [RED|GREEN|BLUE]

&emsp; &emsp; Una volta attivato, il led indicato deve avere l'intensità controllata dall'encoder.
*	TS-REQ02-1: DEMO

&emsp; &emsp; Implementa una ruota dei colori con display OLED e il BUZZER.

![IMG_1603](https://user-images.githubusercontent.com/71086591/179379953-c6e6047a-6d97-4c85-95be-1590b0e73fb4.jpeg)
