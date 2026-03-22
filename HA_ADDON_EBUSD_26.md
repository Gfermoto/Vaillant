# Home Assistant: дополнение eBUSd 26.1+ и локальные конфиги

Краткая инструкция по миграции с [LukasGrebe/ha-addons](https://github.com/LukasGrebe/ha-addons) **≤25.1** на **≥26.1** и работе с **ebusd 24+** (загрузка определений сообщений по URL, локальный `--configpath`).

Официальное описание изменений: [ebusd/DOCS.md](https://github.com/LukasGrebe/ha-addons/blob/main/ebusd/DOCS.md).

---

## Что изменилось

- Отдельные поля (`scanconfig`, `mqtttopic`, `configpath`, `network_device` + `mode` и т.д.) **убраны** из схемы; вместо них — **`commandline_options`** (по одному флагу на строку) и строка **`network_device`** целиком, например `ens:192.168.1.142:9999`.
- Каталог конфигурации дополнения на хосте: **`/addon_configs/<id>_ebusd/`** (в контейнере **`/config/`**). Старые файлы из общего `/config/` нужно **перенести** ([миграция](https://github.com/LukasGrebe/ha-addons/blob/main/ebusd/DOCS.md#migrating-config-folder-from-version-251-or-older)).
- MQTT к Mosquitto часто подставляет само дополнение; **`--mqttjson`** и **`--mqttint=/config/mqtt-hassio.cfg`** могут уже задаваться — **не дублировать** в `commandline_options`.

---

## Типичные ошибки

| Симптом | Причина | Что сделать |
|--------|---------|-------------|
| `invalid configPath URL (connect)` | Нет доступа к интернету из контейнера, ebusd тянет конфиги по HTTP | Локальный `--configpath=.../en` + полная копия [ebusd-configuration](https://github.com/john30/ebusd-configuration) |
| `unable to load scan config 08: list files in vaillant ERR: element not found` | Указан путь `.../en/vaillant` вместо родителя | **`--configpath`** должен указывать на **`.../en`**, не на `vaillant` — подкаталог откроет сам ebusd |
| `field type POWER` / `TEMP` при загрузке `.inc` | Нет `_templates.csv`, `broadcast.csv`, при необходимости `tem/` | См. структуру каталога ниже; типы задаются в шаблонах, не только в `.inc` |
| Все сущности MQTT в HA — **unknown** | Short JSON без `.value` в шаблоне Discovery | Взять `https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg` (обновлённый `value_template`) |
| `invalid arguments` | Устаревшие флаги в 26.x | Убрать подозрительные опции, свериться с [wiki Run](https://github.com/john30/ebusd/wiki/2.-Run) |

---

## Рекомендуемая структура на хосте HA

```
/addon_configs/<id>_ebusd/
  mqtt-hassio.cfg
  ebusd-configuration/
    en/
      _templates.csv
      tem/
        _templates.csv
        15.csv
      vaillant/
        _templates.csv    # при ошибке парсера на ~102-й строке — усечь до ~101 строки (см. issue #4)
        broadcast.csv
        08.bai.csv
        bai.0010023658.inc
        bai.0010015251.inc
        bai.308523.inc      # копия из репозитория Vaillant или raw archived/en/vaillant
        errors.inc
        hcmode.inc
```

**Дополнение (UI):**

- **Network / enhanced:** `ens:<IP_адаптера>:9999`
- **Additional options** — см. `https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt`

---

## Файлы в репозитории Vaillant

Ниже — **полные URL** (можно копировать как есть на Habr или в браузер):

| Файл | Назначение |
|------|------------|
| `https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg` | Discovery для HA; шаблон значения для long/short JSON |
| `https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt` | Пример строк для **commandline_options** (addon ≥26.1) |
| `https://github.com/Gfermoto/Vaillant/blob/main/ebusd.txt` | Пример для **старого** формата дополнения (до 26.1) / Docker |
| `https://github.com/Gfermoto/Vaillant/blob/main/bai.308523.inc` | Fallback **ecoTEC** / generic BAI (из `archived/en/vaillant` + патчи issue #5) |

Кликабельные варианты (Markdown): [mqtt-hassio.cfg](https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg) · [ebusd-addon-26.example.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt) · [ebusd.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd.txt) · [bai.308523.inc](https://github.com/Gfermoto/Vaillant/blob/main/bai.308523.inc)

---

## john30/ebusd-configuration: ветка `archived/en`

Актуальные CSV для локального `--configpath` лежат в **`archived/en/`**, не в корне репозитория. Пример прямой ссылки на тот же fallback, что в нашем `bai.308523.inc`:

`https://raw.githubusercontent.com/john30/ebusd-configuration/master/archived/en/vaillant/bai.308523.inc`

Старые пути без `archived/en` часто дают **404**.

---

## Опрос шины: `--pollinterval`

По [wiki ebusd](https://github.com/john30/ebusd/wiki/4.-Configuration) и [issue #458](https://github.com/john30/ebusd/issues/458): префиксы **`r1`…`r9`** задают **приоритет** сообщений в очереди poll (не умножают интервал). При большом числе сообщений и **`--pollinterval=30`** полный проход очереди может занимать **десятки минут**.

- В примере [ebusd-addon-26.example.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt) стоит **`--pollinterval=5`** (близко к умолчанию ebusd).
- **`--pollinterval=30`** имеет смысл на очень шумной шине; иначе в HA долго **`unknown`** на редко опрашиваемых полях.

В **`bai.0010023658.inc`** / **`bai.0010015251.inc`** / **`bai.308523.inc`** для насоса, спроса и переключателей выставлен приоритет **`r1`** ([issue #5](https://github.com/Gfermoto/Vaillant/issues/5)).

---

## MQTT Discovery: retain

В [mqtt-hassio.cfg](https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg) включено **`definition-retain = 1`**: после перезапуска Home Assistant брокер отдаёт последние **config**-сообщения Discovery — сущности не «пропадают» из‑за порядка старта.

Опционально для **значений** топиков: флаг ebusd **`--mqttretain`** (компромисс: при обрыве шины в MQTT могут остаться устаревшие значения).

Эксперименты с **`type_map-string`** и отключением **`filter-non-field`** в `mqtt-hassio.cfg` могут снова дать **`unknown`** у части сущностей; «тихий» профиль — как в текущем файле репозитория.

---

## Минимальный `08.bai.csv` (опционально)

При большом upstream-`08.bai.csv` в логе возможен шум `condition scan id: message not found`. Для своих котлов можно оставить только строки `[PROD=…]`, `[HW=…]` и fallback — отдельная ветка в [репозитории](https://github.com/Gfermoto/Vaillant).

---

*Обсуждение: [issue #4](https://github.com/Gfermoto/Vaillant/issues/4), [issue #5](https://github.com/Gfermoto/Vaillant/issues/5) (r1, HcPumpMode, retain, pollinterval, `bai.308523.inc`).*
