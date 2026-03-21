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
| Все сущности MQTT в HA — **unknown** | Short JSON без `.value` в шаблоне Discovery | Использовать [mqtt-hassio.cfg](https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg) из репозитория [Vaillant](https://github.com/Gfermoto/Vaillant) (обновлённый `value_template`) |
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
        bai.308523.inc      # из upstream, если нужен fallback в 08.bai
        errors.inc
        hcmode.inc
```

**Дополнение (UI):**

- **Network / enhanced:** `ens:<IP_адаптера>:9999`
- **Additional options** — см. [ebusd-addon-26.example.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt)

---

## Файлы в репозитории Vaillant

| Файл | Назначение |
|------|------------|
| [mqtt-hassio.cfg](https://github.com/Gfermoto/Vaillant/blob/main/mqtt-hassio.cfg) | Discovery для HA; шаблон значения для long/short JSON |
| [ebusd-addon-26.example.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd-addon-26.example.txt) | Пример строк для **commandline_options** (addon ≥26.1) |
| [ebusd.txt](https://github.com/Gfermoto/Vaillant/blob/main/ebusd.txt) | Пример для **старого** формата дополнения (до 26.1) / Docker |

---

## Минимальный `08.bai.csv` (опционально)

При большом upstream-`08.bai.csv` в логе возможен шум `condition scan id: message not found`. Для своих котлов можно оставить только строки `[PROD=…]`, `[HW=…]` и fallback — отдельная ветка в [репозитории](https://github.com/Gfermoto/Vaillant).

---

*Обсуждение и дополнения: [issue #4](https://github.com/Gfermoto/Vaillant/issues/4).*
