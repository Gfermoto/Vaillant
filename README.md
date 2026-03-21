# Vaillant eloBLOCK & atmoTEC → ebusd

Интеграция котлов Vaillant с Home Assistant через eBUS: конфиги ebusd, MQTT, ограничение мощности, автоматизация.

**Подробная статья (под формат Habr):** [Vaillant eloBLOCK и atmoTEC в умном доме](HABR_ARTICLE.md) — архитектура, настройка ebusd, параметры котлов, ESPHome, автоматизация.

---

## Поддерживаемые котлы

| Модель | Тип | PROD | SW | HW |
|--------|-----|------|-----|-----|
| **Vaillant eloBLOCK VE 14/18** | Электрический | 0010023658 | SW0109 | HW7503 |
| **Vaillant atmoTEC plus VUW** | Газовый атмосферный | 0010015251 | SW0407 | HW0903 |

> В конфигурационных файлах оставлены только параметры, реально доступные на указанных прошивках. Неподдерживаемые возвращают `ERR: invalid position` и закомментированы с пояснениями.

---

## Что нужно для старта

### 1. Преобразователь eBUS

Для обмена по шине eBUS нужен адаптер. Рекомендуется **[eBUS Adapter v5-c6](https://adapter.ebusd.eu/v5-c6/index.en.html)**.

Опционально — **USR-ES1** (Ethernet-модуль): меньше задержек и джиттер, стабильнее синхронизация и аудит.

### 2. Демон ebusd

Запуск: **Docker** или **дополнение Home Assistant**.

| Вариант | Ссылка |
|--------|--------|
| Репозиторий ebusd | [john30/ebusd](https://github.com/john30/ebusd) |
| Пример Docker Compose | [docker-compose.yml](https://github.com/Gfermoto/Vaillant/blob/main/docker-compose.yml) |
| Дополнение HA | [LukasGrebe/ha-addons](https://github.com/LukasGrebe/ha-addons) |
| Пример настроек ebusd (классический формат addon) | [ebusd.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd.txt) |
| **Дополнение HA eBUSd 26.1+** (миграция, локальный configpath, MQTT) | [HA_ADDON_EBUSD_26.md](https://github.com/Gfermoto/Vaillant/blob/main/HA_ADDON_EBUSD_26.md) |
| Пример `commandline_options` для 26.x | [ebusd-addon-26.example.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt) |
| Конфиги котлов | [john30/ebusd-configuration](https://github.com/john30/ebusd-configuration) |

Для кастомных котлов — локальная копия конфигов и указание путей в настройках ebusd. После обновления дополнения на **26.1+** обязательно прочитайте [HA_ADDON_EBUSD_26.md](HA_ADDON_EBUSD_26.md): иначе возможны циклы перезапуска, `unknown` в HA и ошибки загрузки шаблонов.

### 3. MQTT-брокер

Docker или дополнение HA; чаще всего уже есть в системе.

### 4. MQTT Discovery

Дальнейшая настройка сущностей в Home Assistant идёт через MQTT Discovery.

---

## Изменения в конфигурации

### `mqtt-hassio.cfg`

- Закомментирован фильтр по имени сообщений: `(filter-name)`.
- Включена публикация записываемых сообщений: `filter-direction = r|u|^w` — в HA попадает больше параметров.
- В **`value_template`** для Discovery учтены и long JSON (`{"FlowTemp":{"value":42}}`), и short JSON (`{"FlowTemp":42}`) при `--mqttjson` — иначе сущности в HA могут быть **unknown** (ebusd 24+).

### `08.bai.csv`

Добавлены строки загрузки конфигов под Vaillant:

- **eloBLOCK** (0010023658): `[PROD='0010023658']!load,bai.0010023658.inc,,,`
- **atmoTEC** (0010015251): `[PROD='0010015251']!load,bai.0010015251.inc,,,`

### `bai.0010023658.inc` (eloBLOCK)

Собран на основе `bai.0010008045.inc` и `bai.308523.inc` под HW7503/SW0109:

- Параметры с `ERR: invalid position` закомментированы с пояснением.
- Убраны газовые параметры (DHW-storage, CO-сенсор и т.д.).
- Добавлены электрические (d.100–d.108): ступени нагрева, мощность, счётчик энергии.
- В `08.bai.csv` добавлен HW-fallback `[HW=7503]` на случай, если по PROD котел не определится.
- Закомментированы D.149, D.152–D.155 из мануала Vaillant (0020265768_01) — адреса eBUS пока не найдены.

Открытые вопросы: [ebusd_eloBLOCK.log](https://github.com/Gfermoto/Vaillant/blob/main/ebusd_eloBLOCK.log).  
Помощь по поиску адресов **D.152/D.153** (программное ограничение мощности без реле) приветствуется. Метод **`ebusctl grab result`** для смены параметров **на дисплее котла** обычно не подходит: сравнивайте **полный дамп регистров до и после** изменения D.152/D.153 (скрипты read-all, сервисные команды B509; см. [issue #3](https://github.com/Gfermoto/Vaillant/issues/3)).

### `bai.0010015251.inc` (atmoTEC)

Слияние `bai.0010004121.inc` и `bai.308523.inc`:

- Закомментированы `## d.xx` для функций, которых нет в atmoTEC.
- Закомментированы `# d.xx` из руководства без известной привязки.
- Часть функций взята из других конфигов.

Файл нуждается в проверке и дополнении. Ошибки: [ebusd_atmoTEC.log](https://github.com/Gfermoto/Vaillant/blob/main/ebusd_atmoTEC.log). Большинство — параметры CO-датчика (e.04–e.19), на SW0407 недоступны и в конфиге закомментированы.  
Помощь по предиктивным параметрам Pred\* и ServiceMode приветствуется.

### `bai.308523.inc`

Используется по умолчанию при неузнавании котла (общие сообщения). Второй конфиг выбирался по схожести **HW**; неподдерживаемые `d.xx` закомментированы с учётом **SW**.

---

## eloBLOCK: ограничение мощности

В мануале Vaillant (0020265768_01) описаны коды D.xxx. Важно: **D.152** (фаза ограничения) и **D.153** (уровень в кВт) — записываемые; eBUS-адреса пока не определены.

Ограничение по сухому контакту: на котле настраиваются фаза (D.152) и уровень (D.153). При замыкании контакта из настроенной мощности вычитается D.153. Для котла 18 кВт при ограничении по всем фазам шаг кратен 6 кВт (6 / 12 / 18).

**Пример:** мощность 10 кВт, D.153 = 6 кВт → при замкнутом контакте остаётся 4 кВт.

Управление контактом — реле **ESPHome** (NO+COM: замкнуто = ограничение):

- Устройство: [ESP-01S 1-channel relay](https://devices.esphome.io/devices/ESP-01S-1-channel-relay)
- Корпус: [Thingiverse](https://www.thingiverse.com/thing:4196595)
- Пример конфига ESPHome: [vaillant_power.yaml](https://github.com/Gfermoto/Vaillant/blob/main/vaillant_power.yaml)

В связке с датчиком мощности и автоматизацией в HA можно временно снижать мощность котла при пиковой нагрузке (стиральная машина, духовка и т.п.). Пример идеи: [Appliance notifications/actions](https://community.home-assistant.io/t/appliance-notifications-actions-washing-machine-clothes-dryer-dish-washer-etc/650166).

---

## Управление отоплением

**Термостаты и актуаторы.** Котёл имеет сухой контакт под нецифровой термостат. Для зонального отопления удобен контроллер тёплого пола **Beok CCT-10**: управляет насосом и котлом по схеме «ИЛИ», насос не крутится без запроса тепла.

- Интеграция термостатов в HA: [WThermostatBeca](https://github.com/fashberg/WThermostatBeca).
- Актуаторы лучше с нормально открытым (NO) состоянием.

**Автоматизация.** Пример: [Advanced Heating Control](https://github.com/panhans/HomeAssistant/blob/main/blueprints/automation/panhans/advanced_heating_control.yaml) — ночной нагрев по дешёвому тарифу, учёт присутствия.

- Радиаторы — разные температуры по комнатам.
- Тёплый пол — одна уставка по расписанию.

У котлов Vaillant нет байпаса: пока котёл работает, хотя бы один радиатор должен быть открыт. Достигается калибровкой термоголовок и расположением термостатов.

---

## Каскад котлов

Сухие контакты в каскаде **не соединять параллельно**. На одной шине eBUS два котла запустить не удалось — используются два адаптера и два контейнера ebusd.

---

Вопросы и предложения — через [Issues](https://github.com/Gfermoto/Vaillant/issues) или контакт в профиле.
