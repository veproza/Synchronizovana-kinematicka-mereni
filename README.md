# Diplomová práce „Synchronizované měření pro dynamické zkoušky systémů integrované bezpečnosti“

Práce popisující zejména dosažitelnou přesnost časové synchronizace s low-cost mikrokontroléry typu Arduino a low- až mid-cost synchronizačními metodami – fotodioda (a integrovaný ADC), 433MHz rádiová spoušť, Nordic NRF24l01, WiFi ESP8266 a GPS modul.

Dále probírá možnosti zpracování relativně velkého toku dat (měření v alespoň jedné ose s 3B velikost9 záznamu a 1KHz snímkovací frekvencí), a to buď uložením na SD karty (včetně benchmarků levné NTLM a kvalitní SanDisk karty), nebo jejich odesíláním přes NRF24l01 nebo WiFi.

Cílové synchronizační přesnosti <2 ms je možné spolehlivě dosáhnout se všemi low-level senzory, tedy s výjímkou ESP8266. Cílový datový tok 3KBps je možné ukládat na SD karty nebo přenášet přes NRF. WiFi se opět na stálý tok příliš neosvědčila, malé vyrovnávací buffery Arduina (<1 KB) nejsou schopny obsloužit příležitostné několikasekundové výpadky.

Zajímavou částí je také měření přesnosti public NTP serverů, které se od etalonu získaného z GPS lišily jen o desetiny milisekund. Řádově milisekundová přesnost synchronizace se ukázala dosažitelná i s připojením přes mobilní 4G + WiFi hotspot.
