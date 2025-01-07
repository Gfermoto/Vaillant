# Vaillant
Vaillant eloBLOCK &amp; atmoTEC **(ebusd)**

__*(Что бы общаться с котлом требуются некоторые приготовления)*__  
- Для соединения с котлом и обмена через EBUS нужен преобразователь. Лучший вариант: https://adapter.ebusd.eu/v5-c6/index.en.html  
Так же рекомендую сразу приобрести преобразователь USR-ES1, это снизит задежки и джиттер, синхронизация и аудит будут стабильней. 
- Нужно поднять демон EBUSD *(в контейнере Docker или как дополнение в HASSio)*  
https://github.com/john30/ebusd  
https://github.com/LukasGrebe/ha-addons *(файл настроек https://github.com/Gfermoto/Vaillant/blob/main/ebusd.txt )*  
Для работы с кастомными файлами настроек, нужно сделать локальную копию конфигурационных фалов и прописать пути к ним.  
https://github.com/john30/ebusd-configuration
- Нужно поднять сервер MQTT *(в контейнере Docker или как дополнение в HASSio)*  
Вероятно он у вас уже есть.
- Дальнейшую работу выполнит MQTT-Discovery

**В файле mqtt-hassio.cfg содержатся два изменения:**
1) Закоментирован фильтр сообщений  
`(filter-name)`
2) Добавлено отслеживание записываемых сообщений  
`(filter-direction = r|u|^w)`  
*(Это позволяет получать больше информации)*  
  
**В файле 08.bai.csv добавлены строки:**  
`[PROD='0010023658']!load,bai.0010023658.inc,,,` *(Vaillant eloBLOCK)*  
`[PROD='0010015251']!load,bai.0010015251.inc,,,` *(Vaillant atmoTEC)* 
  
**Файл bai.0010023658.inc создан благодаря слиянию файлов bai.0010008045.inc и bai.308523.inc:**  
В полученом файле закоментированы строки d.xx относящиеся к функциям не доступным в Vaillant eloBLOCK.  
  
**Файл bai.0010015251.inc создан благодаря слиянию файлов bai.0010004121.inc и bai.308523.inc:**  
В полученом файле закоментированы строки d.xx относящиеся к функциям не доступным в Vaillant atmoTEC.  

Кофиг bai.308523.inc является файлом по умолчанию, если котел не распознан *(содержит общие сообщения, но не все)*.    
Второй конфигурационный файл выбирался на основе схожести аппаратных версий плат управления **(HW)**.  
Закоментированные не поддерживаемые функции d.xx сопряжены с версией програмного обеспечения платы **(SW)**.  

Актуально для электрических котлов, в частности eloBLOCK. Котел имеет сухой контакт для ограничения мощьности, если это настроено. Необходимо настроить ограниение по всем фазам или на какой то конкретной, а так же уровень ограничения. Это число будет вычитатья из максимального настроенного уровня. Однако при настроке ограничения по всем фазам для котла с максимальной мощьностью 18kW, можно настроит ограничение только кратное 6, тоесть 6kW, 12kW и 18kW. Пример: При настроке мощьности на 10kW и ограничения на 6kW, при размыкании контакта, мощность будет выставлена на 10kW-6kW=4kW. Для размыкания контакта удобно использовать реле ESPHome:  
https://devices.esphome.io/devices/ESP-01S-1-channel-relay  
Корпус: https://www.thingiverse.com/thing:4196595   
Мой файл настроек: https://github.com/Gfermoto/Vaillant/blob/main/vaillant_power.yaml  
Если у вас есть HASSio и датчик потребляемой энергии, то можно сформировать автоматизацию, которая при привышении нагрузки в сети временно понизит лимит потребления энергии котлом. Это полезно в момент включения мощьных потребителей, таких как стиральная машина или духовка.  

Любой котел имеет сухой контакт к которому подключается не цифровой термостат. В качестве такового рекомендую контроллер теплых полов **Beok CCT-10** для зонального отопления. Это устройство применяется при позонном управлении теплыми полами и является логическим реле по схеме "ИЛИ", управляющим рециркуляционным насосом и котлом. Это экономично так как насос отключается при отсутствии запроса тепла.  
В качестве термостатов интегрируемых в HASSio я остановился на проекте: https://github.com/fashberg/WThermostatBeca  
Так же вам понадобятся актуаторы. Остановите свой выбор на контроллере NO (нормально открытом) и соответственно NO актуаторах.  

Автоматизация управляющая моим отопление:  
https://github.com/panhans/HomeAssistant/blob/main/blueprints/automation/panhans/advanced_heating_control.yaml  
Отопление форсируется в ночные часы по дешовому тарифу, а также по присутствию людей.  
В доме также есть радиаторы отопления с термостатическими головками ZigBee. Они настроены постоянно на разные температуры для разных помещений как тонкая настройка комфорта. Полы же включаются на одинаковую температуру в соответствии с автоматизацией.  
Важно что бы запрос тепла от полов отклюяался раньше радиаторов, так как у котла нет **bypass** и хотя бы один радиатор дожен быть открыт пока работает котел! Этого легко добиться програмной калибровкой головок радиаторов и положением термостатов по высоте от пола.

При каскадном соединении котлов сухие контакты нельза соединять параллельно. Так же у меня не завелись котлы на одной шине EBUS, по этому я использовал два преобразователя и два демона ebusd в разных контейнерах.
